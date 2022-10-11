#include "avz.h"
using namespace AvZ;
#define C_row(abs1, abs6) (abs1 < abs6) ? 1 : 6
PlantFixer lily_pad_fixer;
PlantFixer fume_shroom_fixer;
TickRunner blover_user;
TickRunner C_user;
TickRunner gloom_shroom_user;
int ReadyCOB()
{
    int cob = 0;
    for (auto&& plant : alive_plant_filter) {
        if (plant->type() == 47 && plant->state() == 37) {
            cob = cob + 1;
        }
    }
    return cob;
}
int LeftmostGGabs(int row)
{
    int result = 854;
    for (auto&& zombie : alive_zombie_filter) {
        if (zombie->type() == 32 && zombie->row() == row - 1 && zombie->state() != 70) {
            result = (zombie->abscissa() < result) ? zombie->abscissa() : result;
        }
    }
    return result;
}
void UseI(int wave)
{
    InsertTimeOperation(2, wave, [=]() {
        if (IsZombieExist(32) || IsZombieExist(15, 2) || IsZombieExist(15, 5) || IsZombieExist(7, 2) || IsZombieExist(7, 5)) {
            auto coffee_bean = GetMainObject()->seedArray() + GetSeedIndex(COFFEE_BEAN);
            if (coffee_bean->isUsable()) {
                SetTime(2, wave);
                ice_filler.coffee();
            } else if (coffee_bean->cd() >= 611) {
                SetTime(753 - coffee_bean->cd(), wave);
                ice_filler.coffee();
            }
        }
    });
}
void SmartBlover()
{
    auto blover = GetMainObject()->seedArray() + GetSeedIndex(BLOVER);
    if (blover->isUsable()) {
        for (auto&& zombie : alive_zombie_filter) {
            if (zombie->type() == BALLOON_ZOMBIE && zombie->abscissa() < 10) {
                CardNotInQueue(5, 2, 7);
                break;
            }
        }
    }
}
void SmartC()
{
    auto puff_shroom = GetMainObject()->seedArray() + GetSeedIndex(PUFF_SHROOM);
    if (puff_shroom->isUsable()) {
        int abs_6 = LeftmostGGabs(6);
        int abs_1 = LeftmostGGabs(1);
        if (abs_6 < 500 || abs_1 < 500) {
            CardNotInQueue(7, C_row(abs_1, abs_6), 6);
        }
    }
}
void PlantGloomShroom(const std::vector<Position>& lst)
{
    auto gloom_shroom = GetMainObject()->seedArray() + GetSeedIndex(GLOOM_SHROOM);
    for (const auto& each : lst) {
        if (gloom_shroom->isUsable() && GetPlantIndex(each.row, each.col, 10) > -1) {
            CardNotInQueue(9, each.row, each.col);
        }
    }
}
void P_I(int wave)
{
    InsertTimeOperation(-32, wave, [=]() {
        if (ReadyCOB() >= 2) {
            SetNowTime();
            pao_operator.pao({{2, 8.7}, {5, 8.7}});
        } else {
            UseI(wave);
        }
    });
}
void RemoveGloomShroom(int wave, int row, int col)
{
    InsertTimeOperation(0, wave, [=]() {
        auto gloom_shroom = GetMainObject()->seedArray() + GetSeedIndex(GLOOM_SHROOM);
        if (gloom_shroom->isUsable()) {
            for (auto&& plant : alive_plant_filter) {
                if (plant->type() == 42 && plant->row() == row - 1 && plant->col() == col - 1 && plant->hp() <= 100) {
                    ShovelNotInQueue(row, col);
                }
            }
        }
    });
}
void Script()
{
    // SetGameSpeed(2);
    //  SkipTick([=]() { return true; });
    SetErrorMode(NONE);
    OpenMultipleEffective('X', MAIN_UI_OR_FIGHT_UI);
    SelectCards({ICE_SHROOM, M_ICE_SHROOM, COFFEE_BEAN, DOOM_SHROOM, BLOVER, PUMPKIN, PUFF_SHROOM, LILY_PAD, GLOOM_SHROOM, FUME_SHROOM});
    plant_fixer.start(PUMPKIN, {{1, 1}, {2, 1}, {5, 1}, {6, 1}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {4, 6}, {4, 7}, {4, 8}, {4, 9}}, 2000);
    lily_pad_fixer.start(LILY_PAD, {{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}}, 0);
    ice_filler.start({{4, 6}, {3, 6}, {4, 5}});
    gloom_shroom_user.pushFunc([]() {
        PlantGloomShroom({{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 6}, {5, 6}});
    });
    pao_operator.autoGetPaoList();
    auto zombie_type_list = GetMainObject()->zombieTypeList();
    if (zombie_type_list[BALLOON_ZOMBIE]) {
        blover_user.pushFunc(SmartBlover);
    }
    if (zombie_type_list[GIGA_GARGANTUAR]) {
        C_user.pushFunc(SmartC);
        for (auto wave : {9, 19, 20}) {
            InsertTimeOperation(1, wave, [=]() {
                for (auto&& zombie : alive_zombie_filter) {
                    if (zombie->type() == 32 && zombie->hp() > 2400 && (zombie->row() == 0 || zombie->row() == 5)) {
                        SetNowTime();
                        pao_operator.recoverPao({{2, 8.7}, {5, 8.7}});
                        break;
                    }
                }
                for (auto&& zombie : alive_zombie_filter) {
                    if (zombie->type() == 32 && zombie->hp() > 4200 && (zombie->row() == 0 || zombie->row() == 5)) {
                        SetNowTime();
                        pao_operator.recoverPao({{2, 8.7}, {5, 8.7}});
                        break;
                    }
                }
            });
        }
    }
    SetTime(0, 1);
    Card({{DOOM_SHROOM, 2, 9}, {COFFEE_BEAN, 2, 9}});
    SetTime(1, 3);
    fume_shroom_fixer.start(FUME_SHROOM, {{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 6}, {5, 6}, {1, 5}, {6, 5}}, 0);
    fume_shroom_fixer.isUseCoffee(true);
    for (auto wave : {9, 19}) {
        UseI(wave);
    }
    for (auto wave : {10, 20}) {
        SetTime(-654, wave);
        fume_shroom_fixer.pause();
        SetTime(97);
        ice_filler.coffee();
        fume_shroom_fixer.goOn();
    }
    for (auto wave : {2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18}) {
        P_I(wave);
    }
    RemoveGloomShroom(20, 2, 6);
    RemoveGloomShroom(20, 5, 6);
}