// others
#include "pch.h"
#include "util/vec.h"

int main()
{
    std::print("hello wurl\n");

    ember::embVec2 hehe1 = ember::embVec2();
    ember::embVec2 hehe2 = ember::embVec2();
    hehe1.m_Data[0] = 1;
    hehe1.m_Data[1] = 2;
    hehe2.m_Data[0] = 3;
    hehe2.m_Data[1] = 4;

    hehe1 += hehe2;

    std::print("hehe1 is {},{}", hehe1.m_Data[0], hehe1.m_Data[1]);

    return 0;
}
