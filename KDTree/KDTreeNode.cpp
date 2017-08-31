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


KDTreeNode::KDTreeNode(std::vector<Point> const & points_x, std::vector<Point> const & points_y, uint8_t axis, uint64_t x_min, uint64_t x_max, uint64_t y_min, uint64_t y_max)
    :
    splitting_value_{0},
    axis_{axis},
    points_x_{points_x},
    points_y_{points_y},
    x_min_{x_min},
    x_max_{x_max},
    y_max_{y_max},
    y_min_{y_min} {}

size_t
KDTreeNode::num_points() const {
    if (points_x_.empty())
        return 0;
    return 1 + (axis_ == 0 ? x_max_ - x_min_ : y_max_ - y_min_);
}

int
KDTreeNode::depth() const {
    return 1 + std::max(left_ == nullptr ? 0 : left_->depth(), right_ == nullptr ? 0 : right_->depth());
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

uint64_t KDTreeNode::xmin() const {
    return x_min_;
}

uint64_t KDTreeNode::xmax() const {
    return x_max_;
}

uint64_t KDTreeNode::ymin() const {
    return y_min_;
}

uint64_t KDTreeNode::ymax() const {
    return y_max_;
}

bool
KDTreeNode::is_leaf() const {
    return left_ == nullptr && right_ == nullptr;
}
