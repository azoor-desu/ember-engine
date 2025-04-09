// others
#include "pch.h"
#include "util/vec_base.h"
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
    hehe2[1] = 4.92;

    hehe1 += hehe2;
    hehe2 = -hehe2;

    std::print("hehe1 is {},{} and hehe2 is {}\n", hehe1[0], hehe1[1], hehe2);

    return 0;
}
