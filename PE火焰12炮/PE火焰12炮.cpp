#include "avz.h"
using namespace AvZ;
void SmartCardNotInQueue(int type, const std::vector<Position>& lst, bool IsImitater = false)
{
    auto seed = GetMainObject()->seedArray() + GetSeedIndex(type, IsImitater);
    if (seed->isUsable()) {
        for (const auto& each : lst) {
            if (Asm::getPlantRejectType(type, each.row - 1, each.col - 1) == Asm::NIL) {
                CardNotInQueue(GetSeedIndex(type, IsImitater) + 1, each.row, each.col);
                break;
            }
        }
    }
}
void Script()
{
    OpenMultipleEffective('X', MAIN_UI_OR_FIGHT_UI);
    SelectCards({COFFEE_BEAN, ICE_SHROOM, M_ICE_SHROOM, DOOM_SHROOM, LILY_PAD, PUMPKIN, GATLING_PEA, TWIN_SUNFLOWER, GLOOM_SHROOM, CATTAIL});
    pao_operator.autoGetPaoList();
    plant_fixer.start(PUMPKIN, {{1, 1}, {2, 1}, {5, 1}, {6, 1}}, 1000);
    auto zombie_type_list = GetMainObject()->zombieTypeList();
    int checktime = 602;
    if (zombie_type_list[BUNGEE_ZOMBIE]) {
        checktime = 848;
        InsertTimeOperation(97, 10, [=]() {
            if (IsZombieExist(BUNGEE_ZOMBIE)) {
                SetTime(97, 10);
                Card({{ICE_SHROOM, 2, 7}, {COFFEE_BEAN, 2, 7}});
            }
        });
        InsertTimeOperation(354, 20, [=]() {
            if (IsZombieExist(BUNGEE_ZOMBIE)) {
                SetTime(354, 20);
                Card({{ICE_SHROOM, 2, 7}, {COFFEE_BEAN, 2, 7}});
            }
        });
    }
    for (int wave = 1; wave <= 20; wave++) {
        SetTime(-32, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
    }
    for (auto wave : {9, 19, 20}) {
        InsertTimeOperation(438, wave, [=]() {
            if (GetMainObject()->refreshCountdown() > 200 || IsZombieExist(32)) {
                SetTime(438, wave);
                pao_operator.pao({{2, 9}, {5, 9}});
            }
        });
        InsertTimeOperation(1039, wave, [=]() {
            if (IsZombieExist(32)) {
                SetTime(1039, wave);
                pao_operator.pao({{2, 9}, {5, 9}});
            }
        });
    }
    SetTime(-717, 20);
    Card(M_ICE_SHROOM, 5, 7);
    SetTime(-397, 20);
    Card(COFFEE_BEAN, 5, 7);
    InsertTimeOperation(1700, 20, [=]() {
        if (IsZombieExist()) {
            SetTime(1700, 20);
            pao_operator.pao({{2, 9}, {5, 9}});
        }
    });
    InsertTimeOperation(checktime, 10, [=]() {
        if (GetMainObject()->wave() == 10) {
            SmartCardNotInQueue(LILY_PAD, {{3, 9}, {4, 9}});
            SmartCardNotInQueue(DOOM_SHROOM, {{3, 9}, {4, 9}});
            SmartCardNotInQueue(COFFEE_BEAN, {{3, 9}, {4, 9}});
        }
    });
}