/*
 * Name  : KDTreeTest
 * Path  :
 * Use   : Unit tests around KDTree class and behaviour
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#include "pch.h"

#include "../KDTree/ChurchillHeader.h"
#include "../KDTree/KDTreeNode.h"
#include "../KDTree/KDTree.h"


using namespace KDTree_NS;


TEST(KDTree, CreateSimple) {
 
    // Arrange
    std::vector<Point> points{ Point{0, 1, 1.0f, 1.0f}};
    
    // Act
    KDTree kdtree(100, &points[0], &points[0] + 1);

    // Assert
}
