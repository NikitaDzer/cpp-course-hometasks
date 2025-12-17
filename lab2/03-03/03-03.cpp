#include <iostream>
#include <utility>
#include <stdexcept>
#include <gtest/gtest.h>


namespace list {

class List {
   private:
    struct Node {
        int value = 0;
        Node* next = nullptr;
    };

    Node* m_head = nullptr;
    Node* m_tail = nullptr;

   public:
    List() = default;
    List(const List&) = delete;
    List& operator=(const List&) = delete;

    List(List&& rhs) : m_head(rhs.m_head), m_tail(rhs.m_tail) {
        rhs.m_head = nullptr;
        rhs.m_tail = nullptr;
    }

    List& operator=(List&& rhs) {
        if (this != &rhs) {
            clear();
            m_head = std::exchange(rhs.m_head, nullptr);
            m_tail = std::exchange(rhs.m_tail, nullptr);
        }
        return *this;
    }

    ~List() { clear(); }

    void clear() noexcept {
        while (m_head != nullptr) {
            Node* temp = m_head;
            m_head = m_head->next;
            delete temp;
        }
        m_tail = nullptr;
    }

    [[nodiscard]] bool empty() const noexcept { return m_head == nullptr; }

    void show() const noexcept {
        Node* current = m_head;
        while (current != nullptr) {
            std::cout << current->value << ' ';
            current = current->next;
        }
        std::cout << '\n';
    }

    void push_front(int value) {
        Node* newNode = new Node{value};

        if (empty()) {
            m_head = m_tail = newNode;
        } else {
            newNode->next = m_head;
            m_head = newNode;
        }
    }

    void push_back(int value) {
        Node* newNode = new Node{value};

        if (empty()) {
            m_head = m_tail = newNode;
        } else {
            m_tail->next = newNode;
            m_tail = newNode;
        }
    }

    void pop_front() {
        if (empty()) {
            return;
        }

        Node* temp = m_head;
        m_head = m_head->next;

        if (m_head == nullptr) {
            m_tail = nullptr;
        }

        delete temp;
    }

    void pop_back() {
        if (empty()) {
            return;
        }

        if (m_head == m_tail) {
            delete m_head;
            m_head = m_tail = nullptr;
            return;
        }

        Node* current = m_head;
        while (current->next != m_tail) {
            current = current->next;
        }

        delete m_tail;
        m_tail = current;
        m_tail->next = nullptr;
    }

    int get() const {
        if (empty()) {
            throw std::runtime_error("Cannot get middle element from empty list");
        }

        Node* slow = m_head;
        Node* fast = m_head;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow->value;
    }
};

}  // namespace list

using namespace list;

TEST(ListTest, DefaultConstructor) {
    List list;
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, PushFront) {
    List list;

    list.push_front(3);
    EXPECT_FALSE(list.empty());

    list.push_front(2);
    list.push_front(1);
}

TEST(ListTest, PushBack) {
    List list;

    list.push_back(1);
    EXPECT_FALSE(list.empty());

    list.push_back(2);
    list.push_back(3);
}

// Test pop_front functionality
TEST(ListTest, PopFront) {
    List list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_front();
    // List should be: 2 -> 3

    list.pop_front();
    list.pop_front();
    EXPECT_TRUE(list.empty());
}

// Test pop_back functionality
TEST(ListTest, PopBack) {
    List list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_back();
    // List should be: 1 -> 2

    list.pop_back();
    list.pop_back();
    EXPECT_TRUE(list.empty());
}

// Test pop_front on empty list (should not crash)
TEST(ListTest, PopFrontEmpty) {
    List list;
    EXPECT_NO_THROW(list.pop_front());
    EXPECT_TRUE(list.empty());
}

// Test pop_back on empty list (should not crash)
TEST(ListTest, PopBackEmpty) {
    List list;
    EXPECT_NO_THROW(list.pop_back());
    EXPECT_TRUE(list.empty());
}

// Test get middle element functionality
TEST(ListTest, GetMiddleElement) {
    List list;

    // Single element
    list.push_back(1);
    EXPECT_EQ(list.get(), 1);

    // Two elements
    list.push_back(2);
    EXPECT_EQ(list.get(), 2);

    // Three elements
    list.push_back(3);
    EXPECT_EQ(list.get(), 2);

    // Four elements
    list.push_back(4);
    EXPECT_EQ(list.get(), 3);

    // Five elements
    list.push_back(5);
    EXPECT_EQ(list.get(), 3);
}

// Test get on empty list (should throw)
TEST(ListTest, GetEmptyList) {
    List list;
    EXPECT_THROW(list.get(), std::runtime_error);
}

// Test mixed operations
TEST(ListTest, MixedOperations) {
    List list;

    // Push front and back
    list.push_front(2);
    list.push_back(3);
    list.push_front(1);
    list.push_back(4);

    // List should be: 1 -> 2 -> 3 -> 4
    // Middle should be 3 (for even length, second middle)

    // Remove from both ends
    list.pop_front();
    list.pop_back();

    // List should be: 2 -> 3
    EXPECT_EQ(list.get(), 3);
}

// Test single element list operations
TEST(ListTest, SingleElementList) {
    List list;
    list.push_front(42);

    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.get(), 42);

    list.pop_front();
    EXPECT_TRUE(list.empty());

    // Test with push_back
    list.push_back(100);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.get(), 100);

    list.pop_back();
    EXPECT_TRUE(list.empty());
}

// Test two elements list
TEST(ListTest, TwoElementsList) {
    List list;
    list.push_back(10);
    list.push_back(20);

    EXPECT_EQ(list.get(), 20);  // Second element for even length

    list.pop_front();
    EXPECT_EQ(list.get(), 20);

    list.pop_back();
    EXPECT_TRUE(list.empty());
}

// Test destructor cleans up properly
TEST(ListTest, DestructorCleansUp) {
    {
        List list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.push_back(4);
        list.push_back(5);
    }  // Destructor should be called here

    // If we get here without memory leaks, destructor works correctly
    SUCCEED();
}

// Test move semantics (since they are defaulted)
TEST(ListTest, MoveSemantics) {
    List source;
    source.push_back(1);
    source.push_back(2);
    source.push_back(3);

    // Move constructor
    List destination(std::move(source));
    EXPECT_FALSE(destination.empty());
    EXPECT_EQ(destination.get(), 2);

    // Move assignment
    List another;
    another = std::move(destination);
    EXPECT_FALSE(another.empty());
    EXPECT_EQ(another.get(), 2);
}


// Test edge case: alternating push and pop
TEST(ListTest, AlternatingPushPop) {
    List list;

    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            list.push_front(i);
        } else {
            list.push_back(i);
        }
    }

    EXPECT_FALSE(list.empty());

    // Remove all elements
    while (!list.empty()) {
        if (list.get() % 2 == 0) {
            list.pop_front();
        } else {
            list.pop_back();
        }
    }

    EXPECT_TRUE(list.empty());
}

// Test get with large list
TEST(ListTest, LargeListGet) {
    List list;

    // Create list with 100 elements
    for (int i = 1; i <= 100; ++i) {
        list.push_back(i);
    }

    // For 100 elements, middle should be 51
    EXPECT_EQ(list.get(), 51);

    // Add one more element
    list.push_back(101);

    // For 101 elements, middle should be 51
    EXPECT_EQ(list.get(), 51);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}