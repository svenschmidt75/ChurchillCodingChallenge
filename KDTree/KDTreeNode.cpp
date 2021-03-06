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


KDTreeNode::KDTreeNode(std::vector<Point> points, uint8_t axis)
    :
    points_{points},
    axis_{axis},
    splitting_value_{0} {}

size_t
KDTreeNode::num_points() const {
    return points_.size();
}

int
KDTreeNode::depth() const {
    return 1 + std::max(left_ == nullptr ? 0 : left_->depth(), right_ == nullptr ? 0 : right_->depth());
}

std::vector<Point> const &
KDTreeNode::points() const {
    return points_;
}

uint8_t
KDTreeNode::splitting_axis() const {
    return axis_;
}

bool
KDTreeNode::rect_intersects_left_subtree(Rect const & rect) const {
    if (axis_ == 0) {
        return rect.lx <= splitting_value_;
    }
    return rect.ly <= splitting_value_;
}

bool
KDTreeNode::rect_intersects_right_subtree(Rect const & rect) const {
    if (axis_ == 0) {
        return rect.hx >= splitting_value_;
    }
    return rect.hy >= splitting_value_;
}

bool
KDTreeNode::is_leaf() const {
    return points_.size() > 0;
}
