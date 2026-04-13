#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <type_traits>

std::int32_t test(std::int32_t x) {
    return x * 2;
}

using FuncType = std::int32_t(*)(std::int32_t);

class Wrapper {
public:
    Wrapper(FuncType func = nullptr) : m_func(func) {}
    
    operator FuncType() const {
        return m_func;
    }
    
    FuncType get() const {
        return m_func;
    }

private:
    FuncType m_func;
};

int main() {
    Wrapper function = test;
    
    assert(function != nullptr);
    assert((*function)(21) == 42);
    
    std::cout << "Test: (*function)(21) = " << (*function)(21) << std::endl;
    
    static_assert(std::is_same_v<decltype(function), Wrapper>);
    static_assert(std::is_convertible_v<Wrapper, FuncType>);
    static_assert(std::is_same_v<decltype(static_cast<FuncType>(function)), FuncType>);
    
    std::cout << "\n=== All tests passed ===" << std::endl;
    
    return 0;
}
