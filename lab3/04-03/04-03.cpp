#include <gtest/gtest.h>
#include <vector>
#include <list>

namespace {

template <typename Container>
void handle(Container& container, int value) {
    container.push_back(value);
}

template <typename Container, typename T>
void handle(Container& container, T&& value) {
    // Ignore
}

template <typename Container, typename... Args>
void insert_integers(Container& container, Args&&... args) {
    (handle(container, std::forward<Args>(args)), ...);
}

} // namespace

TEST(VariadicTemplateInsert, BasicIntInsert) {
    std::vector<int> vec;
    insert_integers(vec, 1, 2, 3, 4, 5);
    
    ASSERT_EQ(5, vec.size());
    EXPECT_EQ(1, vec[0]);
    EXPECT_EQ(2, vec[1]);
    EXPECT_EQ(3, vec[2]);
    EXPECT_EQ(4, vec[3]);
    EXPECT_EQ(5, vec[4]);
}

TEST(VariadicTemplateInsert, MixedTypes) {
    std::vector<int> vec;
    insert_integers(vec, 1, 3.14, 'a', 2, "hello", 3, 4.5f, 4);
    
    ASSERT_EQ(4, vec.size());
    EXPECT_EQ(1, vec[0]);
    EXPECT_EQ(2, vec[1]);
    EXPECT_EQ(3, vec[2]);
    EXPECT_EQ(4, vec[3]);
}

TEST(VariadicTemplateInsert, NoInts) {
    std::vector<int> vec;
    insert_integers(vec, 3.14, 'b', "test", 4.2f);
    
    EXPECT_TRUE(vec.empty());
}

TEST(VariadicTemplateInsert, SingleInt) {
    std::vector<int> vec;
    insert_integers(vec, 42);
    
    ASSERT_EQ(1, vec.size());
    EXPECT_EQ(42, vec[0]);
}

TEST(VariadicTemplateInsert, EmptyPack) {
    std::vector<int> vec;
    insert_integers(vec);
    
    EXPECT_TRUE(vec.empty());
}

TEST(VariadicTemplateInsert, WithListContainer) {
    std::list<int> lst;
    insert_integers(lst, 10, 3.14, 20, 'a', 30);
    
    ASSERT_EQ(3, lst.size());
    auto it = lst.begin();
    EXPECT_EQ(10, *it++);
    EXPECT_EQ(20, *it++);
    EXPECT_EQ(30, *it++);
}

