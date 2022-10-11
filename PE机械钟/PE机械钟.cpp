#include "avz.h"
using namespace AvZ;
#define C_row(abs1, abs6) (abs1 < abs6) ? 1 : 6
#define C_col(abs) (abs > 520) ? 7 : 6
PlantFixer pumpkin_fixer;
PlantFixer fume_shroom_fixer;
PlantFixer lily_pad_fixer;
TickRunner C_a_user;
TickRunner gloom_shroom_fixer;
bool IsSafe(int row, int col)
{
    if (Asm::getPlantRejectType(1, row - 1, col - 1) != Asm::NIL) {
        return false;
    }
    for (auto&& zombie : alive_zombie_filter) {
        if ((zombie->type() == 32 || zombie->type() == 23) && zombie->row() == row - 1 && zombie->abscissa() <= 80 * (col + 1) && zombie->slowCountdown() == 0) {
            return false;
        }
    }
    return true;
}
bool IsPlantExist(int type, int row, int col)
{
    for (auto&& plant : alive_plant_filter) {
        if (plant->type() == type && plant->row() == row - 1 && plant->col() == col - 1) {
            return true;
        }
    }
    return false;
}
int LeftmostGigaGargantuarAbs(int row, int hp = 0, bool IsNotHammering = false)
{
    int result = 854;
    for (auto&& zombie : alive_zombie_filter) {
        if (zombie->type() == 32 && zombie->row() == row - 1 && zombie->hp() >= hp) {
            if (IsNotHammering) {
                if (zombie->state() != 70) {
                    result = (zombie->abscissa() < result) ? zombie->abscissa() : result;
                }
            } else {
                result = (zombie->abscissa() < result) ? zombie->abscissa() : result;
            }
        }
    }
    return result;
}
void SmartCardNotInQueue(int type, int row, int col, bool IsImitater = false)
{
    auto seed = GetMainObject()->seedArray() + GetSeedIndex(type, IsImitater);
    if (seed->isUsable() && Asm::getPlantRejectType(type, row - 1, col - 1) == Asm::NIL) {
        CardNotInQueue(GetSeedIndex(type, IsImitater) + 1, row, col);
    }
}
void UseI(int wave)
{
    InsertTimeOperation(0, wave, [=]() {
        auto coffee_bean = GetMainObject()->seedArray() + GetSeedIndex(COFFEE_BEAN);
        if (coffee_bean->isUsable()) {
            SetTime(0, wave);
            ice_filler.coffee();
        } else if (coffee_bean->cd() >= 609) {
            SetTime(751 - coffee_bean->cd(), wave);
            ice_filler.coffee();
        }
    });
}
void SmartN(int wave)
{
    InsertTimeOperation(-200, wave, [=]() {
        auto seed = GetMainObject()->seedArray();
        if ((seed[3].isUsable() || seed[3].cd() > 4551) && (seed[2].isUsable() || seed[2].cd() > 301)) {
            SetTime(-200, wave);
            fume_shroom_fixer.pause();
            ice_filler.resetFillList({{4, 3}, {3, 4}, {3, 3}, {3, 2}});
            InsertTimeOperation(250, wave, [=]() {
                if (IsSafe(2, 8)) {
                    CardNotInQueue(4, 2, 8);
                    CardNotInQueue(3, 2, 8);
                } else if (IsSafe(5, 7)) {
                    CardNotInQueue(4, 5, 7);
                    CardNotInQueue(3, 5, 7);
                } else if (IsSafe(2, 7)) {
                    CardNotInQueue(4, 2, 7);
                    CardNotInQueue(3, 2, 7);
                } else {
                    SetNowTime();
                    ice_filler.coffee();
                }
            });
            fume_shroom_fixer.goOn();
        } else {
            UseI(wave);
        }
    });
}
void AA(int time, int wave)
{
    InsertTimeOperation(0, wave, [=]() {
        auto seed = GetMainObject()->seedArray();
        if (seed[5].isUsable() || 5001 - seed[5].cd() <= time) {
            SetTime(time, wave);
            Card(CHERRY_BOMB, 5, 8);
            Shovel(2, 7);
            Card(JALAPENO, {{2, 9}, {2, 8}, {2, 7}, {2, 3}});
            InsertTimeOperation(time + 1, wave, [=]() {
                auto jalapeno = GetMainObject()->seedArray() + GetSeedIndex(JALAPENO);
                if (jalapeno->isUsable()) {
                    ShovelNotInQueue(2, 3);
                    CardNotInQueue(6, 2, 3);
                }
            });
            SetTime(time + 1, wave);
            ice_filler.resetFillList({{4, 3}, {3, 4}, {3, 3}, {3, 2}, {2, 3}});
        } else if (wave != 9 || wave != 20) {
            UseI(wave);
        }
    });
}
void SmartI(int wave)
{
    InsertTimeOperation(0, wave, [=]() {
        if (IsZombieExist(32) || IsZombieExist(15, 2) || IsZombieExist(15, 5) || IsZombieExist(7, 2) || IsZombieExist(7, 5)) {
            UseI(wave);
        } else if ((IsZombieExist(16) || IsZombieExist(23)) && (wave == 13 || wave == 14 || wave == 16 || wave == 19)) {
            UseI(wave);
        }
    });
}
void SmartC_a()
{
    int abs_6_0 = LeftmostGigaGargantuarAbs(6, 0, true);
    int abs_1_0 = LeftmostGigaGargantuarAbs(1, 0, true);
    if (abs_6_0 < 580 || abs_1_0 < 580) {
        SmartCardNotInQueue(PUFF_SHROOM, C_row(abs_1_0, abs_6_0), C_col(std::min(abs_6_0, abs_1_0)));
    }
    int abs_6_1000 = LeftmostGigaGargantuarAbs(6, 1000, true);
    int abs_1_1000 = LeftmostGigaGargantuarAbs(1, 1000, true);
    if (abs_6_1000 < 500 || abs_1_1000 < 500) {
        SmartCardNotInQueue(SQUASH, C_row(abs_1_1000, abs_6_1000), 6);
    }
    if (LeftmostGigaGargantuarAbs(2, 1000) < 560) {
        SmartCardNotInQueue(FUME_SHROOM, 2, 7);
    }
    if (LeftmostGigaGargantuarAbs(6, 500) < 500) {
        SmartCardNotInQueue(FUME_SHROOM, 6, 6);
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
    SetGameSpeed(10);
    SkipTick([=]() { return true; });
    SetErrorMode(NONE);
    OpenMultipleEffective('X', MAIN_UI_OR_FIGHT_UI);
    auto zombie_type = GetMainObject()->zombieTypeList();
    pumpkin_fixer.start(PUMPKIN, {{3, 9}, {3, 8}, {4, 9}, {4, 8}, {1, 1}, {1, 4}, {2, 1}, {2, 2}, {2, 4}, {3, 6}, {3, 7}, {4, 6}, {4, 7}, {5, 1}, {5, 4}, {6, 1}, {6, 4}, {2, 3}}, 2000);
    ice_filler.start({{4, 3}, {3, 4}, {3, 3}, {3, 2}, {2, 3}});
    SetTime(0, 1);
    Card({{DOOM_SHROOM, 2, 9}, {COFFEE_BEAN, 2, 9}});
    SetTime(1, 3);
    fume_shroom_fixer.start(FUME_SHROOM, {{1, 5}, {6, 5}, {1, 4}, {6, 4}}, 0);
    fume_shroom_fixer.isUseCoffee(true);
    SmartN(11);
    for (auto wave : {2, 12}) {
        AA(400, wave);
    }
    for (auto wave : {10, 20}) {
        SetTime(-654, wave);
        fume_shroom_fixer.pause();
        SetTime(97);
        ice_filler.coffee();
        fume_shroom_fixer.goOn();
    }
    if (zombie_type[GIGA_GARGANTUAR]) {
        SelectCards({ICE_SHROOM, M_ICE_SHROOM, COFFEE_BEAN, DOOM_SHROOM, CHERRY_BOMB, JALAPENO, SQUASH, PUMPKIN, PUFF_SHROOM, FUME_SHROOM});
        C_a_user.pushFunc(SmartC_a);
        for (auto wave : {5, 17}) {
            SmartN(wave);
            AA(400, wave + 1);
        }
        for (auto wave : {3, 4, 7, 8, 9, 13, 14, 15, 16, 19}) {
            SmartI(wave);
        }
        AA(2300, 9);
        AA(2300, 20);
    } else {
        SelectCards({ICE_SHROOM, M_ICE_SHROOM, COFFEE_BEAN, DOOM_SHROOM, CHERRY_BOMB, JALAPENO, GLOOM_SHROOM, PUMPKIN, LILY_PAD, FUME_SHROOM});
        gloom_shroom_fixer.pushFunc([]() {
            FixGloomShroom({{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}, {2, 6}, {5, 6}});
        });
        for (auto wave : {10, 20}) {
            SetTime(-654, wave);
            gloom_shroom_fixer.pause();
            SetTime(98, wave);
            gloom_shroom_fixer.goOn();
        }
        SetTime(-599, 1);
        lily_pad_fixer.start(LILY_PAD, {{3, 9}, {4, 9}, {3, 8}, {4, 8}, {3, 7}, {4, 7}}, 0);
        if (zombie_type[FOOTBALL_ZOMBIE] || zombie_type[JACK_IN_THE_BOX_ZOMBIE]) {
            for (auto wave : {7, 18}) {
                SmartN(wave);
                AA(400, wave + 1);
            }
            for (auto wave : {3, 4, 5, 6, 9, 13, 14, 15, 16, 17}) {
                SmartI(wave);
            }
        } else if (zombie_type[GARGANTUAR] || zombie_type[BALLOON_ZOMBIE]) {
            for (auto wave : {3, 5, 7, 9, 13, 15, 17, 19}) {
                UseI(wave);
            }
        }
        InsertTimeOperation(1000, 20, [=]() {
            auto gloom_shroom = GetMainObject()->seedArray() + GetSeedIndex(GLOOM_SHROOM);
            if (gloom_shroom->isUsable()) {
                for (auto&& plant : alive_plant_filter) {
                    if (plant->type() == 42 && (plant->row() == 1 || plant->row() == 4) && plant->col() == 5 && plant->hp() <= 100) {
                        ShovelNotInQueue(plant->row() + 1, 6);
                        break;
                    }
                }
            }
        });
    }
}