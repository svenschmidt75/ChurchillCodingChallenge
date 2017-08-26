/*
 * Name  : KDTreeTest
 * Path  :
 * Use   : Unit tests around KDTree class and behaviour
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#include "pch.h"


using namespace KDTree_NS;


TEST(KDTreeTest, Create_Simple) {
 
    // Arrange
    std::vector<Point> points{ Point{0, 1, 1.0f, 1.0f}};
    
    // Act
    KDTree kdtree(100, &points[0], &points[0] + 1);

    // Assert
}

TEST(KDTreeTest, Create_Empty) {

    // Arrange
    std::vector<Point> const points;

    // Act
    KDTree kdtree(1, 0, 0);

    // Assert
    ASSERT_EQ(1, kdtree.depth());
}

TEST(KDTreeTest, Create_Partition_4Leafes) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  1.0f,  0.5f },
        Point{ 0, 1,  2.0f,  0.5f },
        Point{ 0, 1,  3.0f, -0.5f },
        Point{ 0, 1,  4.0f, -0.5f }, };

    // Act
    KDTree kdtree(1, &points[0], &points[points.size() - 1] + 1);

    // Assert
    ASSERT_EQ(3, kdtree.depth());
}

TEST(KDTreeTest, Intersect_Leaf1) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  2.0f, 2.0f },
        Point{ 0, 1,  9.0f, 18.0f },
        Point{ 0, 1,  3.0f, 19.0f },
        Point{ 0, 1,  7.0f, 5.0f },
        Point{ 0, 1,  5.0f, 9.0f },
        Point{ 0, 1,  7.0f, 14.0f },
        Point{ 0, 1,  9.0f, 4.0f },
        Point{ 0, 1,  0.0f, 14.0f },
        Point{ 0, 1,  6.0f, 17.0f },
        Point{ 0, 1,  8.0f, 9.0f }, };
    KDTree kdtree(1, &points[0], &points[points.size() - 1] + 1);

    auto rect = Rect{ 4, 2, 5, 10 };

    // Act
    auto leafs = kdtree.intersect_with_rect(rect);

    // Assert
    ASSERT_EQ(1, leafs.size());
    ASSERT_EQ(1, leafs[0]->points_.size());
    //auto p = Point{ 1, 1, 5.0f, 0.9f };
    //ASSERT_EQ(p, leafs[0]->points_[0]);
    EXPECT_FLOAT_EQ(5.0f, leafs[0]->points_[0].x);
    EXPECT_FLOAT_EQ(9.0f, leafs[0]->points_[0].y);
}

TEST(KDTreeTest, Intersect_With_Rect1) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  2.0f, 2.0f },
        Point{ 0, 1,  9.0f, 18.0f },
        Point{ 0, 1,  3.0f, 19.0f },
        Point{ 0, 1,  7.0f, 5.0f },
        Point{ 0, 1,  5.0f, 9.0f },
        Point{ 0, 1,  7.0f, 14.0f },
        Point{ 0, 1,  9.0f, 4.0f },
        Point{ 0, 1,  0.0f, 14.0f },
        Point{ 0, 1,  6.0f, 17.0f },
        Point{ 0, 1,  8.0f, 9.0f }, };
    KDTree kdtree(1, &points[0], &points[points.size() - 1] + 1);

    auto rect = Rect{ 4, 2, 5, 10 };

    // Act
    auto leafs = kdtree.intersect_with_rect(rect);

    // Assert
    ASSERT_EQ(1, leafs.size());
}
