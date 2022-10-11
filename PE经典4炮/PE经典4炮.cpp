#include "avz.h"
using namespace AvZ;
PlantFixer lily_pad_fixer;
TickRunner c_user;
TickRunner blover_user;
TickRunner gloom_shroom_fixer;
bool IsPlantExist(int type, int row, int col)
{
    for (auto&& plant : alive_plant_filter) {
        if (plant->type() == type && plant->row() == row - 1 && plant->col() == col - 1) {
            return true;
        }
    }
    return false;
}
void SmartCardNotInQueue(int type, int row, int col, bool IsImitater = false)
{
    auto seed = GetMainObject()->seedArray() + GetSeedIndex(type, IsImitater);
    if (seed->isUsable() && Asm::getPlantRejectType(type, row - 1, col - 1) == Asm::NIL) {
        CardNotInQueue(GetSeedIndex(type, IsImitater) + 1, row, col);
    }
}
void SmartC()
{
    for (auto&& zombie : alive_zombie_filter) {
        if (zombie->type() == 3 && zombie->abscissa() <= 600 && GetPlantIndex(zombie->row() + 1, 6) >= 0) {
            ShovelNotInQueue(zombie->row() + 1, 6);
            break;
        }
    }
    for (auto&& zombie : alive_zombie_filter) {
        if ((zombie->type() == GIGA_GARGANTUAR || zombie->type() == GARGANTUAR) && zombie->abscissa() <= 520 && !zombie->isHammering()) {
            if (GetPlantIndex(zombie->row() + 1, 6) == -1) {
                SmartCardNotInQueue(PUFF_SHROOM, zombie->row() + 1, 6);
                if (zombie->hp() >= 200) {
                    SmartCardNotInQueue(SCAREDY_SHROOM, zombie->row() + 1, 6);
                }
                break;
            }
        }
    }
}
void SmartBlover()
{
    for (auto&& zombie : alive_zombie_filter) {
        if (zombie->type() == BALLOON_ZOMBIE && zombie->abscissa() <= 200) {
            SmartCardNotInQueue(BLOVER, 1, 1);
            break;
        }
    }
}
void FixGloomShroom(const std::vector<Position>& lst)
{
    auto coffee = GetMainObject()->seedArray() + GetSeedIndex(COFFEE_BEAN);
    if (!coffee->isUsable()) {
        return;
    }
    for (const auto& each : lst) {
        if (!IsPlantExist(GLOOM_SHROOM, each.row, each.col)) {
            if (!IsPlantExist(FUME_SHROOM, each.row, each.col)) {
                SmartCardNotInQueue(FUME_SHROOM, each.row, each.col);
            }
            SmartCardNotInQueue(COFFEE_BEAN, each.row, each.col);
            SmartCardNotInQueue(GLOOM_SHROOM, each.row, each.col);
            break;
        }
    }
}
void Script()
{
    OpenMultipleEffective('X', MAIN_UI_OR_FIGHT_UI);
    plant_fixer.start(PUMPKIN, {{3, 5}, {3, 6}, {4, 5}, {4, 6}, {6, 4}, {1, 4}}, 2000);
    pao_operator.resetPaoList({{3, 1}, {4, 1}, {3, 3}, {4, 3}});
    ice_filler.start({{3, 5}, {6, 4}, {1, 4}, {1, 5}, {6, 5}});
    auto zombie_type_list = GetMainObject()->zombieTypeList();
    if (zombie_type_list[GIGA_GARGANTUAR] || zombie_type_list[GARGANTUAR]) {
        SelectCards({ICE_SHROOM, M_ICE_SHROOM, COFFEE_BEAN, DOOM_SHROOM, CHERRY_BOMB, LILY_PAD, PUMPKIN, SCAREDY_SHROOM, PUFF_SHROOM, BLOVER});
        c_user.pushFunc(SmartC);
    } else {
        SelectCards({ICE_SHROOM, M_ICE_SHROOM, COFFEE_BEAN, DOOM_SHROOM, CHERRY_BOMB, LILY_PAD, PUMPKIN, FUME_SHROOM, GLOOM_SHROOM, BLOVER});
        InsertTimeOperation(0, 9, [=]() {
            gloom_shroom_fixer.pushFunc([]() {
                FixGloomShroom({{2, 5}, {5, 5}, {3, 6}, {4, 6}});
            });
        });
        SetTime(-651, 10);
        gloom_shroom_fixer.pause();
    }
    if (zombie_type_list[BALLOON_ZOMBIE]) {
        blover_user.pushFunc(SmartBlover);
    }
    SetTime(43, 1);
    Card({{LILY_PAD, 4, 9}, {DOOM_SHROOM, 4, 9}, {COFFEE_BEAN, 4, 9}});
    lily_pad_fixer.start(LILY_PAD, {{3, 6}, {4, 6}}, 0);
    for (auto wave : {2, 11}) {
        SetTime(-32, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        SetTime(304, wave);
        ice_filler.coffee();
    }
    for (auto wave : {3, 12}) {
        InsertTimeOperation(602, wave - 1, [=]() {
            if (GetMainObject()->wave() == wave) {
                SetTime(1227, wave);
                pao_operator.pao({{2, 8.6}, {5, 8.6}});
                SetTime(1503, wave);
                ice_filler.coffee();
            } else {
                SetTime(1828, wave - 1);
                pao_operator.pao({{2, 8.6}, {5, 8.6}});
                SetTime(2104, wave - 1);
                ice_filler.coffee();
            }
        });
    }
    for (auto wave : {4, 5, 6, 7, 8, 13, 14, 15, 16, 17}) {
        InsertTimeOperation(1801, wave - 1, [=]() {
            if (GetMainObject()->wave() == wave) {
                SetTime(1227, wave);
                pao_operator.pao({{2, 8.6}, {5, 8.6}});
                SetTime(1503, wave);
                ice_filler.coffee();
            } else {
                SetTime(3027, wave - 1);
                pao_operator.pao({{2, 8.6}, {5, 8.6}});
                SetTime(3303, wave - 1);
                ice_filler.coffee();
            }
        });
    }
    for (auto wave : {9, 18}) {
        InsertTimeOperation(1801, wave - 1, [=]() {
            if (GetMainObject()->wave() == wave) {
                SetTime(1227, wave);
                pao_operator.pao({{2, 8.6}, {5, 8.6}});
            } else {
                SetTime(3027, wave - 1);
                pao_operator.pao({{2, 8.6}, {5, 8.6}});
            }
        });
    }
    SetTime(-651, 10);
    lily_pad_fixer.pause();
    SetTime(100, 10);
    Card({{LILY_PAD, 3, 8}, {DOOM_SHROOM, 3, 8}, {COFFEE_BEAN, 3, 8}});
    lily_pad_fixer.goOn();
    SetTime(298);
    Card(CHERRY_BOMB, 5, 9);
    SetTime(1100, 18);
    lily_pad_fixer.pause();
    SetTime(43, 19);
    Card({{LILY_PAD, 3, 9}, {DOOM_SHROOM, 3, 9}, {COFFEE_BEAN, 3, 9}});
    lily_pad_fixer.goOn();
    SetTime(-32, 20);
    pao_operator.pao({{2, 9}, {5, 9}});
    SetTime(97, 20);
    ice_filler.coffee();
    InsertTimeOperation(1000, 20, [=]() {
        if (IsZombieExist(32)) {
            SetNowTime();
            pao_operator.recoverPao({{2, 8.6}, {5, 8.6}});
            SetTime(3903, 20);
            pao_operator.pao({{2, 8}, {5, 8}});
        }
    });
    for (auto wave : {9, 19}) {
        SetTime(0, wave);
        ice_filler.resetFillList({{3, 5}, {6, 4}, {1, 4}});
        SetTime(1500, wave + 1);
        ice_filler.resetFillList({{3, 5}, {6, 4}, {1, 4}, {1, 5}, {6, 5}});
        InsertTimeOperation(1, wave, [=]() {
            for (auto&& zombie : alive_zombie_filter) {
                if ((zombie->type() == GIGA_GARGANTUAR || zombie->type() == GARGANTUAR) && zombie->abscissa() > 840) {
                    InsertTimeOperation(2920, wave, [=]() {
                        for (auto&& zombie : alive_zombie_filter) {
                            if (zombie->type() == GIGA_GARGANTUAR && zombie->hp() > 1200) {
                                SetNowTime();
                                pao_operator.pao({{2, 8}, {5, 8}});
                                break;
                            }
                        }
                    });
                    break;
                }
            }
        });
    }
}