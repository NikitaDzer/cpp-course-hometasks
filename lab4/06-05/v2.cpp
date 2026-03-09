#include <print>
#include <boost/dll.hpp>

#define API extern "C" BOOST_SYMBOL_EXPORT

API void test() { std::print("v2::test\n"); }
