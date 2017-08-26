/*
 * Name  : KDTreeNode.h
 * Path  :
 * Use   : defines a node in a kd tree
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#include "pch.h"

#include "ChurchillHeader.h"
#include "KDTreeNode.h"


using namespace KDTree_NS;


KDTreeNode::KDTreeNode(std::vector<Point> points, int axis)
    :
    points_{points},
    axis_{axis} {}
