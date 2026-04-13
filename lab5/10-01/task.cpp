#include <vector>
#include <deque>
#include <print>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <algorithm>

template<typename T>
void test_vector_growth()
{
    std::print("\n=== std::vector<{}> capacity growth analysis ===\n", typeid(T).name());
    
    std::vector<T> vec;
    std::size_t prev_capacity = 0;
    std::vector<std::pair<std::size_t, std::size_t>> growth_points;
    
    for (std::size_t i = 1; i <= 10000; ++i)
    {
        vec.push_back(T{});
        
        if (vec.capacity() != prev_capacity)
        {
            if (prev_capacity > 0)
            {
                double ratio = static_cast<double>(vec.capacity()) / prev_capacity;
                growth_points.emplace_back(vec.size(), vec.capacity());
                std::print("size={:6} capacity={:6} growth_ratio={:.3f}\n", 
                          vec.size(), vec.capacity(), ratio);
            }
            else
            {
                growth_points.emplace_back(vec.size(), vec.capacity());
                std::print("size={:6} capacity={:6} (initial)\n", 
                          vec.size(), vec.capacity());
            }
            prev_capacity = vec.capacity();
        }
    }
    
    if (!growth_points.empty())
    {
        std::print("\nEstimated growth factor: ");
        std::vector<double> ratios;
        for (std::size_t i = 1; i < growth_points.size(); ++i)
        {
            double r = static_cast<double>(growth_points[i].second) / growth_points[i-1].second;
            if (r > 1.0) ratios.push_back(r);
        }
        if (!ratios.empty())
        {
            double avg = 0;
            for (double r : ratios) avg += r;
            avg /= ratios.size();
            std::print("{:.3f} (average of {} transitions)\n", avg, ratios.size());
        }
    }
}

template<typename T>
void test_deque_memory_layout()
{
    std::print("\n=== std::deque<{}> memory page analysis ===\n", typeid(T).name());
    
    std::deque<T> deq;
    std::vector<std::uintptr_t> addresses;
    std::vector<std::pair<std::size_t, std::uintptr_t>> first_addresses;
    
    constexpr std::size_t test_count = 1000;
    
    for (std::size_t i = 0; i < test_count; ++i)
    {
        deq.push_back(T{static_cast<T>(i)});
        addresses.push_back(reinterpret_cast<std::uintptr_t>(&deq[i]));
        
        if (i == 0 || i == 15 || i == 31 || i == 63 || i == 127 || 
            i == 255 || i == 511 || i == 999)
        {
            first_addresses.emplace_back(i, addresses.back());
        }
    }
    
    std::print("\nSample element addresses:\n");
    std::print("{:<8} {:<20} {:<20}\n", "index", "address", "hex");
    for (const auto& [idx, addr] : first_addresses)
    {
        std::print("{:<8} {:<20} 0x{:<18x}\n", idx, addr, addr);
    }
    
    std::print("\nDetecting contiguous memory blocks (pages):\n");
    std::size_t page_start = 0;
    std::size_t page_count = 0;
    constexpr std::size_t max_gap = 100;
    
    for (std::size_t i = 1; i < addresses.size(); ++i)
    {
        std::size_t expected = addresses[i-1] + sizeof(T);
        std::size_t actual = addresses[i];
        
        if (actual != expected && std::abs(static_cast<std::ptrdiff_t>(actual - expected)) > max_gap)
        {
            std::size_t page_size = i - page_start;
            std::print("Page {}: indices [{}, {}] size={} elements, addr_range=[0x{:x}, 0x{:x}]\n",
                      ++page_count, page_start, i-1, page_size,
                      addresses[page_start], addresses[i-1]);
            page_start = i;
        }
    }
    if (page_start < addresses.size())
    {
        std::size_t page_size = addresses.size() - page_start;
        std::print("Page {}: indices [{}, {}] size={} elements, addr_range=[0x{:x}, 0x{:x}]\n",
                  ++page_count, page_start, addresses.size()-1, page_size,
                  addresses[page_start], addresses.back());
    }
    
    std::print("\nTypical page size for deque<{}>: ~{} elements (sizeof(T)={} bytes)\n", 
              typeid(T).name(), 
              page_count > 0 ? addresses.size() / page_count : 0,
              sizeof(T));
}

void test_address_continuity_vector()
{
    std::print("\n=== std::vector<int> address continuity test ===\n");
    
    std::vector<int> vec;
    
    for (std::size_t target : {10, 100, 1000, 5000})
    {
        vec.clear();
        vec.reserve(target);
        
        for (int i = 0; i < static_cast<int>(target); ++i)
        {
            vec.push_back(i);
        }
        
        bool contiguous = true;
        for (std::size_t i = 1; i < vec.size(); ++i)
        {
            if (&vec[i] != &vec[i-1] + 1)
            {
                contiguous = false;
                break;
            }
        }
        
        std::print("size={:5} contiguous={} first_addr=0x{:x} last_addr=0x{:x} stride={} bytes\n",
                  vec.size(), contiguous,
                  reinterpret_cast<std::uintptr_t>(&vec[0]),
                  reinterpret_cast<std::uintptr_t>(&vec.back()),
                  contiguous ? sizeof(int) : 0);
    }
}

void test_address_continuity_deque()
{
    std::print("\n=== std::deque<int> address continuity test ===\n");
    
    for (std::size_t target : {10, 100, 1000, 5000})
    {
        std::deque<int> deq;
        
        for (int i = 0; i < static_cast<int>(target); ++i)
        {
            deq.push_back(i);
        }
        
        std::size_t breaks = 0;
        std::vector<std::size_t> break_points;
        
        for (std::size_t i = 1; i < deq.size(); ++i)
        {
            if (&deq[i] != &deq[i-1] + 1)
            {
                ++breaks;
                if (break_points.size() < 10)
                    break_points.push_back(i);
            }
        }
        
        std::print("size={:5} breaks={} page_boundaries:", deq.size(), breaks);
        for (std::size_t bp : break_points)
        {
            std::print(" {}", bp);
        }
        std::print("\n");
    }
}

int main()
{
    test_vector_growth<int>();
    test_vector_growth<double>();
    
    test_deque_memory_layout<int>();
    test_deque_memory_layout<double>();
    
    test_address_continuity_vector();
    test_address_continuity_deque();
    
    return 0;
}