#include "avz.h"
using namespace AvZ;
bool IsGGExist(int hp)
{
    for (auto&& zombie : alive_zombie_filter) {
        if (zombie.type() == GIGA_GARGANTUAR && zombie.hp() > hp) {
            return true;
        }
    }
    return false;
}
void Script()
{
    SetGameSpeed(10);
    SkipTick([=]() { return true; });
    OpenMultipleEffective('X', MAIN_UI_OR_FIGHT_UI);
    SelectCards({COFFEE_BEAN, ICE_SHROOM, CHERRY_BOMB, BLOVER, PUMPKIN, GATLING_PEA, TWIN_SUNFLOWER, GLOOM_SHROOM, CATTAIL, WINTER_MELON});
    pao_operator.autoGetPaoList();
    plant_fixer.start(PUMPKIN, {{3, 9}, {4, 9}}, 1000);
    auto zombie_type_list = GetMainObject()->zombieTypeList();
    if (zombie_type_list[BALLOON_ZOMBIE]) {
        SetTime(400, 10);
        Card(BLOVER, 1, 1);
    }
    for (int wave = 1; wave <= 20; wave++) {
        SetTime(-38, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
    }
    for (auto wave : {9, 19, 20}) {
        InsertTimeOperation(435, wave, [=]() {
            if (GetMainObject()->refreshCountdown() > 200 || IsGGExist(600)) {
                SetTime(435, wave);
                pao_operator.pao({{2, 9}, {5, 9}});
            }
        });
        InsertTimeOperation(1036, wave, [=]() {
            if (IsGGExist(600)) {
                SetTime(1036, wave);
                pao_operator.pao({{2, 9}, {5, 9}});
            }
        });
    }
    SetTime(335, 10);
    Card(CHERRY_BOMB, 2, 9);
    SetTime(-350, 20);
    Card({{ICE_SHROOM, 2, 1}, {COFFEE_BEAN, 2, 1}});
}