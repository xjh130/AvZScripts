#include "avz.h"
using namespace AvZ;
TickRunner fix_cannon;
bool IsGGExist()
{
    auto zombie = GetMainObject()->zombieArray();
    for (int index = 0; index < GetMainObject()->zombieTotal(); ++index) {
        if (zombie[index].type() == GIGA_GARGANTUAR && !zombie[index].isDead() && !zombie[index].isDisappeared()) {
            return true;
        }
    }
    return false;
}
void PlantCANNON(int row, int col)
{
    auto kernel_pult = GetMainObject()->seedArray() + GetSeedIndex(KERNEL_PULT);
    if (!(GetPlantIndex(row, col, 47) >= 0) && kernel_pult->isUsable())
        CardNotInQueue(6, row, col);
    if (!(GetPlantIndex(row, col, 47) >= 0) && kernel_pult->isUsable())
        CardNotInQueue(6, row, col + 1);
    auto cannon = GetMainObject()->seedArray() + GetSeedIndex(COB_CANNON);
    if (cannon->isUsable() && GetPlantIndex(row, col, 34) >= 0 && GetPlantIndex(row, col + 1, 34) >= 0)
        CardNotInQueue(7, row, col);
}
void FixCANNON()
{
    auto cannon = GetMainObject()->seedArray() + GetSeedIndex(COB_CANNON);
    if (cannon->isUsable() || cannon->cd() >= 4250) {
        auto plant = GetMainObject()->plantArray();
        for (int index = 0; index < GetMainObject()->plantTotal(); index++) {
            if (plant[index].type() == COB_CANNON && plant[index].hp() < 100 && !plant[index].isDisappeared() && plant[index].stateCountdown() == 2500) {
                ShovelNotInQueue(plant[index].row() + 1, plant[index].col() + 1);
            }
        }
    }
    PlantCANNON(2, 1);
    PlantCANNON(5, 1);
    PlantCANNON(1, 5);
    PlantCANNON(2, 5);
    PlantCANNON(5, 5);
    PlantCANNON(6, 5);
    PlantCANNON(1, 7);
    PlantCANNON(2, 7);
    PlantCANNON(5, 7);
    PlantCANNON(6, 7);
}
void Script()
{
    OpenMultipleEffective('X', MAIN_UI_OR_FIGHT_UI);
    SelectCards({ICE_SHROOM, COFFEE_BEAN, CHERRY_BOMB, PUMPKIN, DOOM_SHROOM, KERNEL_PULT, COB_CANNON, PUFF_SHROOM, GATLING_PEA, TWIN_SUNFLOWER});
    pao_operator.autoGetPaoList();
    plant_fixer.start(PUMPKIN, {{3, 9}, {4, 9}}, 1000);
    fix_cannon.pushFunc(FixCANNON);
    auto zombie_type_list = GetMainObject()->zombieTypeList();
    if (zombie_type_list[BUNGEE_ZOMBIE]) {
        ice_filler.start({{4, 9}});
        for (auto wave : {10, 20}) {
            SetTime(97, wave);
            ice_filler.coffee();
        }
        InsertTimeOperation(1000, 10, [=]() {
            if (GetMainObject()->refreshCountdown() > 200 && GetMainObject()->wave() == 10) {
                CardNotInQueue(5, {{2, 9}, {5, 9}});
                CardNotInQueue(2, {{2, 9}, {5, 9}});
            }
        });
        SetTime(1457, 20);
        pao_operator.pao({{2, 9}, {5, 9}});
    } else {
        InsertTimeOperation(500, 10, [=]() {
            if (GetMainObject()->refreshCountdown() > 200) {
                CardNotInQueue(5, {{2, 9}, {5, 9}});
                CardNotInQueue(2, {{2, 9}, {5, 9}});
            }
        });
        InsertTimeOperation(501, 20, [=]() {
            if (IsGGExist()) {
                SetTime(501, 20);
                pao_operator.recoverPao({{2, 9}, {5, 9}});
            }
        });
    }
    SetTime(-60, 1);
    pao_operator.pao({{2, 9}, {5, 9}});
    SetTime(25, 1);
    pao_operator.pao(2, 9);
    for (auto wave : {2, 4, 6, 8, 11, 13, 15, 17, 19}) {
        SetTime(-188, wave);
        pao_operator.pao(5, 9);
        SetTime(-55, wave);
        pao_operator.pao(2, 9);
        SetTime(27, wave);
        pao_operator.pao(5, 9);
    }
    for (auto wave : {3, 5, 7, 9, 12, 14, 16, 18}) {
        SetTime(-188, wave);
        pao_operator.pao(2, 9);
        SetTime(-55, wave);
        pao_operator.pao(5, 9);
        SetTime(27, wave);
        pao_operator.pao(2, 9);
    }
    InsertTimeOperation(413, 9, [=]() {
        if (GetMainObject()->refreshCountdown() > 200 || IsGGExist()) {
            SetTime(413, 9);
            pao_operator.pao(5, 9);
            auto zombie = GetMainObject()->zombieArray();
            for (int index = 0; index < GetMainObject()->zombieTotal(); index++) {
                if ((zombie[index].row() == 0 || zombie[index].row() == 1 || zombie[index].row() == 2) && !zombie[index].isDead() && !zombie[index].isDisappeared() && zombie[index].type() != IMP) {
                    SetTime(546, 9);
                    pao_operator.pao(2, 9);
                    break;
                }
            }
            if (IsGGExist()) {
                SetTime(628, 9);
                pao_operator.pao(5, 9);
            }
        }
    });
    InsertTimeOperation(413, 19, [=]() {
        if (GetMainObject()->refreshCountdown() > 200 || IsGGExist()) {
            SetTime(413, 19);
            pao_operator.pao(2, 9);
            auto zombie = GetMainObject()->zombieArray();
            for (int index = 0; index < GetMainObject()->zombieTotal(); index++) {
                if ((zombie[index].row() == 3 || zombie[index].row() == 4 || zombie[index].row() == 5) && !zombie[index].isDead() && !zombie[index].isDisappeared() && zombie[index].type() != IMP) {
                    SetTime(546, 19);
                    pao_operator.pao(5, 9);
                    break;
                }
            }
            if (IsGGExist()) {
                SetTime(628, 19);
                pao_operator.pao(2, 9);
            }
        }
    });
    for (auto wave : {9, 19}) {
        InsertTimeOperation(1016, wave, [=]() {
            if (IsGGExist()) {
                SetTime(1016, wave);
                pao_operator.pao({{2, 9}, {5, 9}});
            }
        });
        InsertTimeOperation(1617, wave, [=]() {
            if (IsGGExist() && GetMainObject()->wave() == wave) {
                SetTime(1617, wave);
                pao_operator.pao({{2, 9}, {5, 9}});
            }
        });
    }
    for (auto wave : {10, 20}) {
        SetTime(-60, wave);
        pao_operator.pao({{2, 9}, {5, 9}});
        SetTime(298, wave);
        Card(CHERRY_BOMB, 5, 9);
    }
    SetTime(25, 10);
    pao_operator.pao(2, 9);
    SetTime(-150, 20);
    pao_operator.pao(4, 7.5);
    SetTime(25, 20);
    pao_operator.recoverPao(2, 9);
    SetTime(500, 20);
    pao_operator.recoverPao({{2, 9}, {5, 9}});
}