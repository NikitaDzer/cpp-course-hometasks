#include <algorithm>
#include <limits>
#include <vector>

#include <gtest/gtest.h>

namespace rectangle {

struct Point final {
    int x, y;
};

class Rectangle final {
   private:
    Point m_left_top, m_right_bottom;

   public:
    Rectangle(Point left_top, Point right_bottom)
        : m_left_top(left_top), m_right_bottom(right_bottom) {}
    Rectangle(int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y)
        : m_left_top{left_top_x, left_top_y}, m_right_bottom{right_bottom_x, right_bottom_y} {}

    [[nodiscard]] const Point &left_top() const noexcept { return m_left_top; }
    [[nodiscard]] const Point &right_bottom() const noexcept { return m_right_bottom; }

    [[nodiscard]] unsigned width() const noexcept { return m_right_bottom.x - m_left_top.x; }
    [[nodiscard]] unsigned height() const noexcept { return m_right_bottom.y - m_left_top.y; }

    [[nodiscard]] bool is_degenerate() const noexcept { return width() == 0 || height() == 0; }

    int square() const noexcept {
        return (m_right_bottom.x - m_left_top.x) * (m_right_bottom.y - m_left_top.y);
    }
};

Rectangle intersection(const Rectangle &lhs, const Rectangle &rhs) {
    if (lhs.right_bottom().y <= rhs.left_top().y || lhs.right_bottom().x <= rhs.left_top().x ||
        lhs.left_top().y >= rhs.right_bottom().y || lhs.left_top().x >= rhs.right_bottom().x) {
        return Rectangle{0, 0, 0, 0};
    }

    int inter_left = std::max(lhs.left_top().x, rhs.left_top().x);
    int inter_top = std::max(lhs.left_top().y, rhs.left_top().y);
    int inter_right = std::min(lhs.right_bottom().x, rhs.right_bottom().x);
    int inter_bottom = std::min(lhs.right_bottom().y, rhs.right_bottom().y);

    return Rectangle(inter_left, inter_top, inter_right, inter_bottom);
}

Rectangle intersection(const std::vector<Rectangle> &rectangles) {
    if (rectangles.empty()) {
        return Rectangle(0, 0, 0, 0);
    }

    auto result = rectangles[0];

    for (std::size_t i = 1; i < rectangles.size(); ++i) {
        result = intersection(result, rectangles[i]);
        if (result.is_degenerate()) {
            break;
        }
    }

    return result;
}

Rectangle bounding_box(const std::vector<Rectangle> &rectangles) {
    if (rectangles.empty()) {
        return Rectangle(0, 0, 0, 0);
    }

    int min_x = std::numeric_limits<int>::max();
    int min_y = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int max_y = std::numeric_limits<int>::min();

    for (const auto &rect : rectangles) {
        min_x = std::min(min_x, rect.left_top().x);
        min_y = std::min(min_y, rect.left_top().y);
        max_x = std::max(max_x, rect.right_bottom().x);
        max_y = std::max(max_y, rect.right_bottom().y);
    }

    if (min_x > max_x || min_y > max_y) {
        return Rectangle(0, 0, 0, 0);
    }

    return Rectangle(min_x, min_y, max_x, max_y);
}

}  // namespace rectangle


using namespace rectangle;

// Tests for intersection of multiple rectangles
TEST(RectangleIntersectionTest, EmptyVector) {
    std::vector<Rectangle> empty;
    Rectangle result = intersection(empty);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

TEST(RectangleIntersectionTest, SingleRectangle) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10)};
    Rectangle result = intersection(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.square(), 100);
    EXPECT_EQ(result.left_top().x, 0);
    EXPECT_EQ(result.left_top().y, 0);
    EXPECT_EQ(result.right_bottom().x, 10);
    EXPECT_EQ(result.right_bottom().y, 10);
}

TEST(RectangleIntersectionTest, TwoRectanglesIntersection) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10), Rectangle(5, 5, 15, 15)};
    Rectangle result = intersection(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 5);
    EXPECT_EQ(result.left_top().y, 5);
    EXPECT_EQ(result.right_bottom().x, 10);
    EXPECT_EQ(result.right_bottom().y, 10);
    EXPECT_EQ(result.square(), 25);
}

TEST(RectangleIntersectionTest, ThreeRectanglesIntersection) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10), Rectangle(5, 5, 15, 15),
                                    Rectangle(8, 8, 12, 12)};
    Rectangle result = intersection(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 8);
    EXPECT_EQ(result.left_top().y, 8);
    EXPECT_EQ(result.right_bottom().x, 10);
    EXPECT_EQ(result.right_bottom().y, 10);
    EXPECT_EQ(result.square(), 4);
}

TEST(RectangleIntersectionTest, NoIntersection) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 5, 5), Rectangle(10, 10, 15, 15),
                                    Rectangle(20, 20, 25, 25)};
    Rectangle result = intersection(rects);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

TEST(RectangleIntersectionTest, PartialChainIntersection) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 0, 10, 10),   // intersects with next
        Rectangle(5, 5, 15, 15),   // intersects with both
        Rectangle(20, 20, 25, 25)  // no intersection with first
    };
    Rectangle result = intersection(rects);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

TEST(RectangleIntersectionTest, TouchingEdges) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 0, 5, 5), Rectangle(5, 0, 10, 5)  // touches right edge
    };
    Rectangle result = intersection(rects);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

// Tests for bounding box
TEST(BoundingBoxTest, EmptyVector) {
    std::vector<Rectangle> empty;
    Rectangle result = bounding_box(empty);
    EXPECT_TRUE(result.is_degenerate());
    EXPECT_EQ(result.square(), 0);
}

TEST(BoundingBoxTest, SingleRectangle) {
    std::vector<Rectangle> rects = {Rectangle(5, 5, 15, 15)};
    Rectangle result = bounding_box(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 5);
    EXPECT_EQ(result.left_top().y, 5);
    EXPECT_EQ(result.right_bottom().x, 15);
    EXPECT_EQ(result.right_bottom().y, 15);
    EXPECT_EQ(result.square(), 100);
}

TEST(BoundingBoxTest, TwoRectanglesBounding) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10), Rectangle(15, 15, 25, 25)};
    Rectangle result = bounding_box(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 0);
    EXPECT_EQ(result.left_top().y, 0);
    EXPECT_EQ(result.right_bottom().x, 25);
    EXPECT_EQ(result.right_bottom().y, 25);
    EXPECT_EQ(result.square(), 625);
}

TEST(BoundingBoxTest, ThreeRectanglesBounding) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 10, 5, 15),  // left side
        Rectangle(10, 0, 15, 5),  // top right
        Rectangle(8, 20, 12, 25)  // bottom middle
    };
    Rectangle result = bounding_box(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 0);
    EXPECT_EQ(result.left_top().y, 0);
    EXPECT_EQ(result.right_bottom().x, 15);
    EXPECT_EQ(result.right_bottom().y, 25);
    EXPECT_EQ(result.square(), 375);
}

TEST(BoundingBoxTest, DegenerateRectangles) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 0, 0, 10),    // zero width
        Rectangle(5, 5, 15, 5),    // zero height
        Rectangle(10, 10, 20, 20)  // normal
    };
    Rectangle result = bounding_box(rects);
    EXPECT_FALSE(result.is_degenerate());
    EXPECT_EQ(result.left_top().x, 0);
    EXPECT_EQ(result.left_top().y, 0);
    EXPECT_EQ(result.right_bottom().x, 20);
    EXPECT_EQ(result.right_bottom().y, 20);
    EXPECT_EQ(result.square(), 400);
}

// Combined functionality tests
TEST(CombinedTest, IntersectionThenBoundingBox) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 10, 10), Rectangle(5, 5, 15, 15),
                                    Rectangle(8, 8, 12, 12)};

    Rectangle intersected = intersection(rects);
    Rectangle bbox = bounding_box(rects);

    EXPECT_FALSE(intersected.is_degenerate());
    EXPECT_EQ(intersected.square(), 4);

    EXPECT_FALSE(bbox.is_degenerate());
    EXPECT_EQ(bbox.left_top().x, 0);
    EXPECT_EQ(bbox.left_top().y, 0);
    EXPECT_EQ(bbox.right_bottom().x, 15);
    EXPECT_EQ(bbox.right_bottom().y, 15);
    EXPECT_EQ(bbox.square(), 225);
}

TEST(CombinedTest, AllDegenerate) {
    std::vector<Rectangle> rects = {
        Rectangle(0, 0, 0, 10),  // zero width
        Rectangle(5, 5, 15, 5),  // zero height
        Rectangle(8, 8, 8, 8)    // point
    };

    Rectangle intersected = intersection(rects);
    Rectangle bbox = bounding_box(rects);

    EXPECT_TRUE(intersected.is_degenerate());
    EXPECT_EQ(intersected.square(), 0);

    EXPECT_FALSE(bbox.is_degenerate());
    EXPECT_EQ(bbox.left_top().x, 0);
    EXPECT_EQ(bbox.left_top().y, 0);
    EXPECT_EQ(bbox.right_bottom().x, 15);
    EXPECT_EQ(bbox.right_bottom().y, 10);
    EXPECT_EQ(bbox.square(), 150);
}