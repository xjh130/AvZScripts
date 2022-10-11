#include "avz.h"
using namespace AvZ;
TickRunner fix_cannon;
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
        for (auto&& plant : alive_plant_filter) {
            if (plant.type() == COB_CANNON && plant.hp() < 100 && plant.stateCountdown() == 2500) {
                ShovelNotInQueue(plant.row() + 1, plant.col() + 1);
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
            if (IsZombieExist(32)) {
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
        if (GetMainObject()->refreshCountdown() > 200 || IsZombieExist(32)) {
            SetTime(413, 9);
            pao_operator.pao(5, 9);
            for (auto&& zombie : alive_zombie_filter) {
                if ((zombie.row() == 0 || zombie.row() == 1 || zombie.row() == 2) && zombie.type() != IMP) {
                    SetTime(546, 9);
                    pao_operator.pao(2, 9);
                    break;
                }
            }
            if (IsZombieExist(32)) {
                SetTime(628, 9);
                pao_operator.pao(5, 9);
            }
        }
    });
    InsertTimeOperation(413, 19, [=]() {
        if (GetMainObject()->refreshCountdown() > 200 || IsZombieExist(32)) {
            SetTime(413, 19);
            pao_operator.pao(2, 9);
            for (auto&& zombie : alive_zombie_filter) {
                if ((zombie.row() == 3 || zombie.row() == 4 || zombie.row() == 5) && zombie.type() != IMP) {
                    SetTime(546, 19);
                    pao_operator.pao(5, 9);
                    break;
                }
            }
            if (IsZombieExist(32)) {
                SetTime(628, 19);
                pao_operator.pao(2, 9);
            }
        }
    });
    for (auto wave : {9, 19}) {
        InsertTimeOperation(1016, wave, [=]() {
            if (IsZombieExist(32)) {
                SetTime(1016, wave);
                pao_operator.pao({{2, 9}, {5, 9}});
            }
        });
        InsertTimeOperation(1617, wave, [=]() {
            if (IsZombieExist(32) && GetMainObject()->wave() == wave) {
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