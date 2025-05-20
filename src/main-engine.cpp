#include "pch-engine.h"

#include "util/bitset.h"
#include "util/hash.h"
#include "util/matrix.h"
#include "util/matrix_utils.h"
#include "util/types.h"
#include "util/vec.h"
#include "util/vec2_coord_container.h"
#include "util/vec_utils.h"


#include "engine/engine.h"
#include "engine/timer.h"

int main()
{
    using namespace ember;

    std::print("hello wurl\n");

    embVec2 hehe1 = embVec2();
    embVec2 hehe2 = embVec2();
    std::print("hehe1 is same? {} as hehe2\n", hehe1 == hehe2);
    hehe1.x = 1;
    hehe1[1] = 2.2;
    hehe2[0] = 3;
    hehe2[1] = 4.93;

    hehe1 += hehe2;
    hehe2 = -hehe2;

    std::print("hehe1 is {},{} and hehe2 {}\n", hehe1[0], hehe1[1], hehe2);

    hehe1 = embVec2(2.f, 3.f);
    hehe2 = embVec2(4.f, 5.f);
    std::print("normalized is {} and {} and mag is {}\n", math::normalize(hehe2),
               math::normalize(hehe2), math::length(math::normalize(hehe2)));

    embMtx4 hoho1;
    hoho1[0][0] = 5;
    hoho1[0][1] = 6;
    hoho1[1][0] = 7;
    hoho1[1][1] = 8;
    hoho1[3][3] = -12345678.67891;

    std::print("hoho is {}\n", hoho1);

    embCoordInt nnnn{3, 56};
    std::print("new boi is {}\n", nnnn);

    embBitset<64> bbbbb;

    std::print("I HAVE A HASH {} AND A guid {}\n", Hash::GenerateHash("ss"), Hash::GenerateGUID());

    Engine& engine = Engine::Instance();
    Timer& timer = Timer::Instance();

    engine.Init();
    engine.SetSimulationActive(true); // leave it on true to keep running
    timer.SetSimTimeScale(1.f);
    timer.SetTargetSimRate(20);
    timer.SetTargetFramerate(20);

    while (engine.IsEngineRunning())
    {

        if (timer.ShouldUpdate(engine.IsSimulationActive()))
        {
            engine.Update();
            engine.Render();
        }
    }

    engine.Destroy();

    return 0;
}
