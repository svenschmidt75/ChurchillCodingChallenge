/*
 * Name  : KDTreeNode.h
 * Path  :
 * Use   : declares a node in a kd tree
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#pragma once


 // forward declarations
struct Point;


namespace KDTree_NS {

    class KDTreeNode {

        friend class KDTree;

    public:
        explicit KDTreeNode(std::vector<Point> points, uint8_t axis);

        size_t numPoints() const;

    private:
        std::vector<Point>          points_;
        uint8_t                     axis_;
        float                       splitting_value_;
        std::unique_ptr<KDTreeNode> left_;
        std::unique_ptr<KDTreeNode> right_;
    };


} // namespace KDTree_NS

#pragma warning(default:4251)
