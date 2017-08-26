/*
 * Name  : Helper
 * Path  :
 * Use   : Unit tests around the KDTree helper class and behaviour
 * Author: Sven Schmidt
 * Date  : 08/26/2017
 */
#include "pch.h"


using namespace KDTree_NS;


TEST(Helper, Split_X) {

    // Arrange

    std::vector<Point> const points{
        Point{ 0, 1, -0.5,  0.5f },
        Point{ 0, 1,  0.5,  0.5f },
        Point{ 0, 1,  0.5, -0.5f }, 
        Point{ 0, 1, -0.5, -0.5f }, };

    // Act
    auto splits = Helper::split(points, 0);

    // Assert
    //auto splitting_value = std::get<0>(splits);
    //auto split_a = std::get<1>(splits);
    //auto split_b = std::get<2>(splits);

    //ASSERT_EQ(-0.5f, splitting_value);
    //ASSERT_EQ(2, split_a.size());
}
