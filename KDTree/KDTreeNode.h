/*
 * Name  : KDTreeNode.h
 * Path  :
 * Use   : declares a node in a kd tree
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#pragma once


#pragma warning(disable:4251)


 // forward declarations
struct Point;


// forward declarations
GOOGLE_TEST(KDTreeTest, Intersect_Leaf_1);


namespace KDTree_NS {

    class KDTreeNode {

        friend class KDTree;
        FRIEND_GOOGLE_TEST(KDTreeTest, Intersect_Leaf_1);


    public:
        explicit KDTreeNode(std::vector<Point> const & points, uint8_t axis, uint64_t x_min, uint64_t x_max, uint64_t y_min, uint64_t y_max);

        size_t                        num_points() const;
        int                           depth() const;
        bool                          rect_intersects_left_subtree(Rect const & rect) const;
        bool                          rect_intersects_right_subtree(Rect const & rect) const;
        uint64_t                      xmin() const;
        uint64_t                      xmax() const;
        uint64_t                      ymin() const;
        uint64_t                      ymax() const;

    private:
        std::vector<Point>          points_;
        uint8_t                     axis_;
        float                       splitting_value_;
        uint64_t                    x_min_;
        uint64_t                    x_max_;
        uint64_t                    y_max_;
        uint64_t                    y_min_;
        std::unique_ptr<KDTreeNode> left_;
        std::unique_ptr<KDTreeNode> right_;
    };


} // namespace KDTree_NS

#pragma warning(default:4251)
