/*
 * Name  : KDTreeTest
 * Path  :
 * Use   : Unit tests around KDTree class and behaviour
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#include "pch.h"


using namespace KDTree_NS;



/* TODO SS
 * 
 * - test if only one point and partition
 * 
 * 
 * 
 */


TEST(KDTree, CreateSimple) {
 
    // Arrange
    std::vector<Point> points{ Point{0, 1, 1.0f, 1.0f}};
    
    // Act
    KDTree kdtree(100, &points[0], &points[0] + 1);

    // Assert
}

TEST(KDTree, Create_Partition_4Leafes) {

    // Arrange
    std::vector<Point> const points{
        Point{ 0, 1,  1.0f,  0.5f },
        Point{ 0, 1,  2.0f,  0.5f },
        Point{ 0, 1,  3.0f, -0.5f },
        Point{ 0, 1,  4.0f, -0.5f }, };

    // Act
    KDTree kdtree(1, &points[0], &points[points.size() - 1] + 1);

    // Assert
}
