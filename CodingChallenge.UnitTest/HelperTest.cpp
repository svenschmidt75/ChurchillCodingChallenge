/*
 * Name  : Helper
 * Path  :
 * Use   : Unit tests around the KDTree helper class and behaviour
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#include "pch.h"


using namespace KDTree_NS;


TEST(Helper, Split_X_NoPoints) {

    // Arrange
    std::vector<Point> const points;

    // Act
    auto splits = Helper::split(points, 0);

    // Assert
    auto split_a = std::get<1>(splits);
    auto split_b = std::get<2>(splits);

    ASSERT_TRUE(split_a.empty());
    ASSERT_TRUE(split_b.empty());
}

TEST(Helper, Split_X_OnePoint) {

    // Arrange
    std::vector<Point> const points{ Point{ 0, 1,  4.0f, -0.5f } };

    // Act
    auto splits = Helper::split(points, 0);

    // Assert
    auto splitting_value = std::get<0>(splits);
    auto split_a = std::get<1>(splits);
    auto split_b = std::get<2>(splits);

    ASSERT_FLOAT_EQ(4.0f, splitting_value);
    ASSERT_EQ(1, split_a.size());
    ASSERT_EQ(0, split_b.size());
}

TEST(Helper, Split_X_EvenPoints) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  1.0f,  0.5f },
        Point{ 0, 1,  2.0f,  0.5f },
        Point{ 0, 1,  3.0f, -0.5f },
        Point{ 0, 1,  4.0f, -0.5f }, };

    // Act
    auto splits = Helper::split(points, 0);

    // Assert
    auto splitting_value = std::get<0>(splits);
    auto split_a = std::get<1>(splits);
    auto split_b = std::get<2>(splits);

    ASSERT_FLOAT_EQ(2.5f, splitting_value);
    ASSERT_EQ(2, split_a.size());
    ASSERT_EQ(2, split_b.size());
}

TEST(Helper, Split_X_OddPoints) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  1.0f,  0.5f },
        Point{ 0, 1,  2.0f,  0.5f },
        Point{ 0, 1,  3.0f, -0.5f }, };

    // Act
    auto splits = Helper::split(points, 0);

    // Assert
    auto splitting_value = std::get<0>(splits);
    auto split_a = std::get<1>(splits);
    auto split_b = std::get<2>(splits);

    ASSERT_FLOAT_EQ(2.0f, splitting_value);
    ASSERT_EQ(2, split_a.size());
    ASSERT_EQ(1, split_b.size());
}

TEST(Helper, Point_In_Rect) {

    // Arrange
    Point const p{ 0, 1,  0.2f,  0.5f };
    Rect const r{ -1.0f, -1.0f,  1.0f,  1.0f };

    // Act
    auto inside = Helper::is_point_in_rect(p, r);

    // Assert
    ASSERT_TRUE(inside);
}

TEST(Helper, Point_In_ZeroWidthRect_X) {

    // Arrange
    Point const p{ 0, 1,  1.0f, 0.5f };
    Rect const r{ 1.0f, -1.0f, 1.0f, 1.0f };

    // Act
    auto inside = Helper::is_point_in_rect(p, r);

    // Assert
    ASSERT_TRUE(inside);
}

TEST(Helper, Point_In_ZeroWidthRect_Y) {

    // Arrange
    Point const p{ 0, 1,  1.0f,  0.5f };
    Rect const r{ 1.0f, 0.5f, 1.0f, 0.5f };

    // Act
    auto inside = Helper::is_point_in_rect(p, r);

    // Assert
    ASSERT_TRUE(inside);
}

TEST(Helper, Rect_Outside_Leaf_From_Left) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  0.0f, 14.0f },
        Point{ 0, 1,  2.0f, 2.0f },
        Point{ 0, 1,  3.0f, 19.0f },
        Point{ 0, 1,  9.0f, 18.0f },
        Point{ 0, 1,  5.0f, 9.0f },
        Point{ 0, 1,  6.0f, 17.0f },
        Point{ 0, 1,  7.0f, 5.0f },
        Point{ 0, 1,  7.0f, 14.0f },
        Point{ 0, 1,  8.0f, 9.0f },
        Point{ 0, 1,  9.0f, 4.0f }, };

    KDTreeNode const node{points, uint8_t(0)};
    Rect const r{-1.0f, 0.5f, -0.5f, 0.5f};

    // Act
    auto points_in_rect = Helper::intersect(std::vector<KDTreeNode const *> {&node}, r);

    // Assert
    ASSERT_TRUE(points_in_rect.empty());
}

TEST(Helper, Rect_Outside_Leaf_From_Right) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  0.0f, 14.0f },
        Point{ 0, 1,  2.0f, 2.0f },
        Point{ 0, 1,  3.0f, 19.0f },
        Point{ 0, 1,  9.0f, 18.0f },
        Point{ 0, 1,  5.0f, 9.0f },
        Point{ 0, 1,  6.0f, 17.0f },
        Point{ 0, 1,  7.0f, 5.0f },
        Point{ 0, 1,  7.0f, 14.0f },
        Point{ 0, 1,  8.0f, 9.0f },
        Point{ 0, 1,  9.0f, 4.0f }, };

    KDTreeNode const node{ points, uint8_t(0) };
    Rect const r{ 10.1f, 0.5f, 20.5f, 0.5f };

    // Act
    auto points_in_rect = Helper::intersect(std::vector<KDTreeNode const *> {&node}, r);

    // Assert
    ASSERT_TRUE(points_in_rect.empty());
}

TEST(Helper, Rect_Ends_Where_Leaf_Starts) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  0.0f, 14.0f },
        Point{ 0, 1,  2.0f, 2.0f },
        Point{ 0, 1,  3.0f, 19.0f },
        Point{ 0, 1,  9.0f, 18.0f },
        Point{ 0, 1,  5.0f, 9.0f },
        Point{ 0, 1,  6.0f, 17.0f },
        Point{ 0, 1,  7.0f, 5.0f },
        Point{ 0, 1,  7.0f, 14.0f },
        Point{ 0, 1,  8.0f, 9.0f },
        Point{ 0, 1,  9.0f, 4.0f }, };

    KDTreeNode const node{ points, uint8_t(0) };
    Rect const r{ -1.0f, 0.5f, 0.0f, 14.0f };

    // Act
    auto points_in_rect = Helper::intersect(std::vector<KDTreeNode const *> {&node}, r);

    // Assert
    ASSERT_EQ(1, points_in_rect.size());
    ASSERT_FLOAT_EQ(0.f, points_in_rect[0].x);
    ASSERT_FLOAT_EQ(14.f, points_in_rect[0].y);
}

TEST(Helper, Rect_Overlaps_From_Left) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  0.0f, 14.0f },
        Point{ 0, 1,  2.0f, 2.0f },
        Point{ 0, 1,  3.0f, 19.0f },
        Point{ 0, 1,  9.0f, 18.0f },
        Point{ 0, 1,  5.0f, 9.0f },
        Point{ 0, 1,  6.0f, 17.0f },
        Point{ 0, 1,  7.0f, 5.0f },
        Point{ 0, 1,  7.0f, 14.0f },
        Point{ 0, 1,  8.0f, 9.0f },
        Point{ 0, 1,  9.0f, 4.0f }, };

    KDTreeNode const node{ points, uint8_t(0) };
    Rect const r{ -1.0f, 0.5f, 2.0f, 14.0f };

    // Act
    auto points_in_rect = Helper::intersect(std::vector<KDTreeNode const *> {&node}, r);

    // Assert
    ASSERT_EQ(2, points_in_rect.size());
}

TEST(Helper, Rect_Overlaps_From_Right) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  0.0f, 14.0f },
        Point{ 0, 1,  2.0f, 2.0f },
        Point{ 0, 1,  3.0f, 19.0f },
        Point{ 0, 1,  9.0f, 18.0f },
        Point{ 0, 1,  5.0f, 9.0f },
        Point{ 0, 1,  6.0f, 17.0f },
        Point{ 0, 1,  7.0f, 5.0f },
        Point{ 0, 1,  7.0f, 14.0f },
        Point{ 0, 1,  8.0f, 9.0f },
        Point{ 0, 1,  9.0f, 4.0f }, };

    KDTreeNode const node{ points, uint8_t(0) };
    Rect const r{ 5.5f, 8.0f, 8.5f, 17.0 };

    // Act
    auto points_in_rect = Helper::intersect(std::vector<KDTreeNode const *> {&node}, r);

    // Assert
    ASSERT_EQ(3, points_in_rect.size());
}

TEST(Helper, Rect_Inside_Rect) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  0.0f, 14.0f },
        Point{ 0, 1,  2.0f, 2.0f },
        Point{ 0, 1,  3.0f, 19.0f },
        Point{ 0, 1,  9.0f, 18.0f },
        Point{ 0, 1,  5.0f, 9.0f },
        Point{ 0, 1,  6.0f, 17.0f },
        Point{ 0, 1,  7.0f, 5.0f },
        Point{ 0, 1,  7.0f, 14.0f },
        Point{ 0, 1,  8.0f, 9.0f },
        Point{ 0, 1,  9.0f, 4.0f }, };

    KDTreeNode const node{ points, uint8_t(0) };
    Rect const r{ -1.0f, -1.5f, 11.0f, 20.0f };

    // Act
    auto points_in_rect = Helper::intersect(std::vector<KDTreeNode const *> {&node}, r);

    // Assert
    ASSERT_EQ(10, points_in_rect.size());
}
