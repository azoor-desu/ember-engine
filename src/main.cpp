// others
#include "pch.h"
#include "util/vec.h"
#include "util/vec_utils.h"

int main()
{
    std::print("hello wurl\n");

    ember::embVec2 hehe1 = ember::embVec2();
    ember::embVec2 hehe2 = ember::embVec2();
    std::print("hehe1 is same? {} as hehe2\n", hehe1 == hehe2);
    hehe1.x = 1;
    hehe1[1] = 2.2;
    hehe2[0] = 3;
    hehe2[1] = 4.93;

    hehe1 += hehe2;
    hehe2 = -hehe2;

    std::print("hehe1 is {},{} and hehe2 {}\n", hehe1[0], hehe1[1], hehe2);

    hehe1 = ember::embVec2(2.f, 3.f);
    hehe2 = ember::embVec2(4.f, 5.f);
    // std::print("normalized is {} and {} and mag is {}\n", ember::VecNormalize(hehe2),
    //            ember::VecNormalize(hehe2), ember::VecMagnitude(ember::VecNormalize(hehe2)));


    return 0;
}
