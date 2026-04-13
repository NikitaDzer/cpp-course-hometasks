#include <iostream>
#include <source_location>

class Tracer {
public:
    Tracer(std::source_location loc = std::source_location::current()) : m_loc(loc) {
        std::cout << "Entering function: " << m_loc.function_name() 
                  << " (" << m_loc.file_name() << ":" << m_loc.line() << ")" << std::endl;
    }

    ~Tracer() {
        std::cout << "Exiting function: " << m_loc.function_name() << std::endl;
    }

    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;

private:
    std::source_location m_loc;
};

#ifndef NDEBUG
#define TRACE_CONCAT_IMPL(x, y) x##y
#define TRACE_CONCAT(x, y) TRACE_CONCAT_IMPL(x, y)
#define trace() Tracer TRACE_CONCAT(tracer_, __LINE__)(std::source_location::current())
#else
#define trace() ((void)0)
#endif

void helper_function() {
    trace();
    std::cout << "Helper logic executed" << std::endl;
}

void outer_function() {
    trace();
    std::cout << "Outer function started" << std::endl;
    helper_function();
    std::cout << "Outer function finished" << std::endl;
}

int main() {
    trace();
    
    std::cout << "--- Test 1: Basic Function Call ---" << std::endl;
    outer_function();
    
    std::cout << "\n--- Test 2: Local Scope ---" << std::endl;
    {
        trace();
        std::cout << "Inside local scope" << std::endl;
    }
    
    std::cout << "\n--- Test 3: Loop ---" << std::endl;
    for (int i = 0; i < 1; ++i) {
        trace();
        std::cout << "Loop iteration" << std::endl;
    }

    std::cout << "\n--- End of Main ---" << std::endl;
    
    return 0;
}