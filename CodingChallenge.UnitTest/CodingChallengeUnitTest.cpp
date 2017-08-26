/*
 * Name  :
 * Path  :
 * Use   : The sole purpose of this file is the creation of the precompiled
           header file pch.pch.
           To do this, all the project is set to "Use precompiled /Yu",
           whereas this file is the only one with "Create precompiled /Yc".
 * Author: Sven Schmidt
 * Date  : 08/25/2017
 */

#include "pch.h"


int main(int argc, char ** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
