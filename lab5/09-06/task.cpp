#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>
#include <iterator>
#include <gtest/gtest.h>
#include <boost/iterator/iterator_facade.hpp>

namespace fibonacci {

class Range {
public :

    class Iterator
    {
    public :

        using iterator_category = std::forward_iterator_tag;
        using value_type        = int;
        using difference_type   = std::ptrdiff_t;
        using pointer           = int*;
        using reference         = int;

    //  -------------------------------------------------------------------

        Iterator() : m_prev(0), m_curr(0) {}

        Iterator(int prev, int curr) : m_prev(prev), m_curr(curr) {}

    //  -------------------------------------------------------------------

        reference operator*() const
        {
            return m_curr;
        }

    //  -------------------------------------------------------------------

        Iterator & operator++()
        {
            int next = m_prev + m_curr;
            m_prev = m_curr;
            m_curr = next;

            return *this;
        }

    //  -------------------------------------------------------------------

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

    //  -------------------------------------------------------------------

        bool operator==(Iterator const & other) const
        {
            return m_prev == other.m_prev && m_curr == other.m_curr;
        }

        bool operator!=(Iterator const & other) const
        {
            return !(*this == other);
        }

    private :

        int m_prev;
        int m_curr;
    };

//  -----------------------------------------------------------------------

    Iterator begin() const { return Iterator(1, 0); }

    Iterator end  () const { return Iterator(      ); }
};

///////////////////////////////////////////////////////////////////////////////////

class BoostRange
{
public :

    class Iterator : public boost::iterator_facade <
        Iterator, int, std::forward_iterator_tag, int >
    {
    public :

        Iterator() : m_prev(0), m_curr(0) {}

        Iterator(int prev, int curr) : m_prev(prev), m_curr(curr) {}

    private :

        friend class boost::iterator_core_access;

    //  -------------------------------------------------------------------

        int dereference() const
        {
            return m_curr;
        }

    //  -------------------------------------------------------------------

        void increment()
        {
            int next = m_prev + m_curr;
            m_prev = m_curr;
            m_curr = next;
        }

    //  -------------------------------------------------------------------

        bool equal(Iterator const & other) const
        {
            return m_prev == other.m_prev && m_curr == other.m_curr;
        }

    //  -------------------------------------------------------------------

        int m_prev;
        int m_curr;
    };

//  -----------------------------------------------------------------------

    Iterator begin() const { return Iterator(1, 0); }

    Iterator end  () const { return Iterator(      ); }
};

}  // namespace fibonacci

TEST(Fibonacci, CustomIterator) {
    std::vector<int> expected{0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89};
    std::vector<int> actual;
    
    for (auto it = fibonacci::Range().begin(); it != fibonacci::Range().end() && *it <= 89; ++it) {
        actual.push_back(*it);
    }
    
    EXPECT_EQ(actual, expected);
}

TEST(Fibonacci, BoostIterator) {
    std::vector<int> expected{0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89};
    std::vector<int> actual;
    
    for (auto it = fibonacci::BoostRange().begin(); it != fibonacci::BoostRange().end() && *it <= 89; ++it) {
        actual.push_back(*it);
    }
    
    EXPECT_EQ(actual, expected);
}