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
        explicit KDTreeNode(std::vector<uint64_t> const & points, uint8_t axis);

        size_t                        num_points() const;
        int                           depth() const;
        std::vector<uint64_t> const & points() const;
        bool                          rect_intersects_left_subtree(Rect const & rect) const;
        bool                          rect_intersects_right_subtree(Rect const & rect) const;

    private:
        std::vector<uint64_t>       points_;
        uint8_t                     axis_;
        float                       splitting_value_;
        std::unique_ptr<KDTreeNode> left_;
        std::unique_ptr<KDTreeNode> right_;
    };


} // namespace KDTree_NS

#pragma warning(default:4251)
