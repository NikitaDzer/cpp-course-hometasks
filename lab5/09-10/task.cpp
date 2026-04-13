#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <new>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

class Allocator {
public:
    virtual ~Allocator() = default;
    
    virtual auto allocate(std::size_t size = 1, std::size_t alignment = alignof(std::max_align_t)) -> void* = 0;
    virtual void deallocate(void* x) = 0;
    virtual void show() const = 0;
    
protected:
    template<typename T>
    auto get_byte(T* x) const -> std::byte* {
        return static_cast<std::byte*>(static_cast<void*>(x));
    }
    
    static constexpr auto s_alignment = alignof(std::max_align_t);
};

class LinearAllocator : public Allocator {
public:
    explicit LinearAllocator(std::size_t size) : m_size(size) {
        m_begin = operator new(m_size, std::align_val_t(s_alignment));
    }
    
    ~LinearAllocator() override {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }
    
    auto allocate(std::size_t size, std::size_t alignment = s_alignment) -> void* override {
        void* begin = get_byte(m_begin) + m_offset;
        auto free = m_size - m_offset;
        
        if (begin = std::align(alignment, size, begin, free); begin) {
            m_offset = m_size - free + size;
            return begin;
        }
        return nullptr;
    }
    
    void deallocate(void*) override {}
    
    void show() const override {
        std::cout << "LinearAllocator: m_size=" << m_size 
                  << " m_begin=" << m_begin 
                  << " m_offset=" << m_offset << std::endl;
    }
    
private:
    std::size_t m_size = 0;
    std::size_t m_offset = 0;
    void* m_begin = nullptr;
};

class StackAllocator : public Allocator {
public:
    explicit StackAllocator(std::size_t size) : m_size(size) {
        m_begin = operator new(m_size, std::align_val_t(s_alignment));
    }
    
    ~StackAllocator() override {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }
    
    auto allocate(std::size_t size, std::size_t alignment = s_alignment) -> void* override {
        void* begin = get_byte(m_begin) + m_offset + sizeof(header_t);
        auto free = m_size - m_offset - sizeof(header_t);
        
        if (begin = std::align(alignment, size, begin, free); begin) {
            auto header = get_header(get_byte(begin) - sizeof(header_t));
            *header = static_cast<header_t>(get_byte(begin) - (get_byte(m_begin) + m_offset));
            m_offset = get_byte(begin) - get_byte(m_begin) + size;
            return begin;
        }
        return nullptr;
    }
    
    void deallocate(void* x) override {
        auto header = get_header(get_byte(x) - sizeof(header_t));
        m_offset = static_cast<std::size_t>(get_byte(x) - get_byte(m_begin) - *header);
    }
    
    void show() const override {
        std::cout << "StackAllocator: m_size=" << m_size 
                  << " m_begin=" << m_begin 
                  << " m_offset=" << m_offset << std::endl;
    }
    
private:
    using header_t = std::uint8_t;
    
    auto get_header(std::byte* x) const -> header_t* {
        return reinterpret_cast<header_t*>(x);
    }
    
    std::size_t m_size = 0;
    std::size_t m_offset = 0;
    void* m_begin = nullptr;
};

class ListAllocator : public Allocator {
public:
    ListAllocator(std::size_t size, std::size_t step) : m_size(size), m_step(step) {
        assert(m_size % m_step == 0 && m_step >= sizeof(Node));
        make_list();
        m_begin = m_head;
    }
    
    ~ListAllocator() override {
        for (auto list : m_lists) {
            operator delete(list, m_size, std::align_val_t(s_alignment));
        }
    }
    
    auto allocate(std::size_t = 1, std::size_t = s_alignment) -> void* override {
        if (!m_head) {
            if (m_offset == std::size(m_lists)) {
                make_list();
            } else {
                m_head = get_node(m_lists[++m_offset - 1]);
            }
        }
        
        auto node = m_head;
        
        if (!node->next) {
            auto next = get_byte(node) + m_step;
            if (next != get_byte(m_lists[m_offset - 1]) + m_size) {
                m_head = get_node(next);
                m_head->next = nullptr;
            } else {
                m_head = m_head->next;
            }
        } else {
            m_head = m_head->next;
        }
        
        return node;
    }
    
    void deallocate(void* x) override {
        auto node = get_node(x);
        node->next = m_head;
        m_head = node;
    }
    
    void show() const override {
        std::cout << "ListAllocator: m_size=" << m_size 
                  << " m_step=" << m_step 
                  << " m_begin=" << m_begin 
                  << " m_head=" << static_cast<void*>(m_head) 
                  << " m_offset=" << m_offset << std::endl;
    }
    
private:
    struct Node {
        Node* next = nullptr;
    };
    
    auto get_node(void* x) const -> Node* {
        return static_cast<Node*>(x);
    }
    
    void make_list() {
        m_head = get_node(operator new(m_size, std::align_val_t(s_alignment)));
        m_head->next = nullptr;
        ++m_offset;
        m_lists.push_back(m_head);
    }
    
    std::size_t m_size = 0;
    std::size_t m_step = 0;
    std::size_t m_offset = 0;
    void* m_begin = nullptr;
    Node* m_head = nullptr;
    std::vector<void*> m_lists;
};

class FreeListAllocator : public Allocator {
public:
    explicit FreeListAllocator(std::size_t size) : m_size(size) {
        assert(m_size >= sizeof(Node) + 1);
        m_begin = operator new(m_size, std::align_val_t(s_alignment));
        m_head = get_node(m_begin);
        m_head->size = m_size - sizeof(Header);
        m_head->next = nullptr;
    }
    
    ~FreeListAllocator() override {
        operator delete(m_begin, m_size, std::align_val_t(s_alignment));
    }
    
    auto allocate(std::size_t size, std::size_t alignment = s_alignment) -> void* override {
        void* end = get_byte(m_begin) + sizeof(Header) + size;
        void* next = end;
        auto free = 2 * alignof(Header);
        
        if (next = std::align(alignof(Header), sizeof(Header), next, free); next) {
            auto padding = get_byte(next) - get_byte(end);
            
            Node* current = m_head;
            Node* previous = nullptr;
            
            while (current && sizeof(Header) + size + padding > current->size) {
                previous = current;
                current = current->next;
            }
            
            if (current) {
                if (current->size >= size + padding + sizeof(Node) + 1) {
                    auto step = sizeof(Header) + size + padding;
                    auto node = get_node(get_byte(current) + step);
                    node->size = current->size - step;
                    node->next = current->next;
                    current->next = node;
                } else {
                    padding += current->size - size - padding;
                }
                
                if (!previous) {
                    m_head = current->next;
                } else {
                    previous->next = current->next;
                }
                
                return get_byte(current) + sizeof(Header);
            }
        }
        return nullptr;
    }
    
    void deallocate(void* x) override {
        auto node = get_node(get_byte(x) - sizeof(Header));
        Node* previous = nullptr;
        Node* current = m_head;
        
        while (current && node > current) {
            previous = current;
            current = current->next;
        }
        
        node->next = current;
        if (!previous) {
            m_head = node;
        } else {
            previous->next = node;
        }
        
        merge(previous, node);
    }
    
    void show() const override {
        std::cout << "FreeListAllocator: m_size=" << m_size 
                  << " m_begin=" << m_begin 
                  << " m_head=" << static_cast<void*>(m_head) << std::endl;
    }
    
private:
    struct Node {
        std::size_t size = 0;
        Node* next = nullptr;
    };
    
    struct alignas(std::max_align_t) Header {
        std::size_t size = 0;
    };
    
    auto get_node(void* x) const -> Node* {
        return static_cast<Node*>(x);
    }
    
    void merge(Node* previous, Node* node) const {
        if (node->next && get_byte(node) + sizeof(Header) + node->size == get_byte(node->next)) {
            node->size += sizeof(Header) + node->next->size;
            node->next = node->next->next;
        }
        if (previous && get_byte(previous) + sizeof(Header) + previous->size == get_byte(node)) {
            previous->size += sizeof(Header) + node->size;
            previous->next = node->next;
        }
    }
    
    std::size_t m_size = 0;
    void* m_begin = nullptr;
    Node* m_head = nullptr;
};

void test_allocator(Allocator& allocator, const char* name) {
    std::cout << "\n=== Testing " << name << " ===" << std::endl;
    
    allocator.show();
    auto x = allocator.allocate(16);
    allocator.show();
    auto y = allocator.allocate(16);
    allocator.show();
    auto z = allocator.allocate(16);
    allocator.show();
    
    allocator.deallocate(x);
    allocator.show();
    allocator.deallocate(y);
    allocator.show();
    allocator.deallocate(z);
    allocator.show();
    
    std::cout << name << " test passed!" << std::endl;
}

static void benchmark_allocator(benchmark::State& state, std::unique_ptr<Allocator> allocator) {
    auto kb = 1uz << 10;
    auto mb = 1uz << 20;
    auto gb = 1uz << 30;
    
    std::vector<void*> vector(kb, nullptr);
    
    for (auto _ : state) {
        for (std::size_t i = 0; i < kb; ++i) {
            vector[i] = allocator->allocate(mb);
        }
        for (std::size_t i = 0; i < kb; ++i) {
            allocator->deallocate(vector[i]);
        }
        benchmark::DoNotOptimize(vector);
    }
}

static void benchmark_linear(benchmark::State& state) {
    benchmark_allocator(state, std::make_unique<LinearAllocator>(16 * gb));
}

static void benchmark_stack(benchmark::State& state) {
    benchmark_allocator(state, std::make_unique<StackAllocator>(2 * gb));
}

static void benchmark_list(benchmark::State& state) {
    benchmark_allocator(state, std::make_unique<ListAllocator>(gb, mb));
}

static void benchmark_freelist(benchmark::State& state) {
    benchmark_allocator(state, std::make_unique<FreeListAllocator>(gb));
}

BENCHMARK(benchmark_linear)->Name("LinearAllocator");
BENCHMARK(benchmark_stack)->Name("StackAllocator");
BENCHMARK(benchmark_list)->Name("ListAllocator");
BENCHMARK(benchmark_freelist)->Name("FreeListAllocator");

int main() {
    {
        LinearAllocator linear(1 << 10);
        test_allocator(linear, "LinearAllocator");
    }
    
    {
        StackAllocator stack(1 << 10);
        test_allocator(stack, "StackAllocator");
    }
    
    {
        ListAllocator list(32, 8);
        test_allocator(list, "ListAllocator");
    }
    
    {
        FreeListAllocator freelist(1 << 10);
        test_allocator(freelist, "FreeListAllocator");
    }
    
    std::cout << "\n=== Polymorphic usage ===" << std::endl;
    std::vector<std::unique_ptr<Allocator>> allocators;
    allocators.push_back(std::make_unique<LinearAllocator>(1 << 10));
    allocators.push_back(std::make_unique<StackAllocator>(1 << 10));
    allocators.push_back(std::make_unique<ListAllocator>(32, 8));
    allocators.push_back(std::make_unique<FreeListAllocator>(1 << 10));
    
    for (auto& alloc : allocators) {
        alloc->show();
        alloc->allocate(16);
        alloc->show();
    }
    
    std::cout << "\n=== All tests passed ===" << std::endl;
    
    benchmark::RunSpecifiedBenchmarks();
    
    return 0;
}
