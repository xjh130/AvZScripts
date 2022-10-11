#include "avz.h"
using namespace AvZ;
PaoOperator col_1;
PaoOperator col_6;
TickRunner smart_c;
bool IsGGExist()
{
    for (auto&& zombie : alive_zombie_filter) {
        if (zombie->type() == GIGA_GARGANTUAR && zombie->hp() > 600) {
            return true;
        }
    }
    return false;
}
void SmartC()
{
    auto zombie = GetMainObject()->zombieArray();
    auto seed = GetMainObject()->seedArray();
    if (seed[0].isUsable())
        for (int index = 0; index < GetMainObject()->zombieTotal(); index++)
            if (zombie[index].type() == GIGA_GARGANTUAR && !zombie[index].isDead() && !zombie[index].isDisappeared() && zombie[index].abscissa() < 650) {
                CardNotInQueue(1, zombie[index].row() + 1, 8);
                break;
            }
}
void Script()
{
    OpenMultipleEffective('X', MAIN_UI_OR_FIGHT_UI);
    SelectCards({M_FLOWER_POT, ICE_SHROOM, DOOM_SHROOM, KERNEL_PULT, COB_CANNON, FLOWER_POT, CHERRY_BOMB, GATLING_PEA, TWIN_SUNFLOWER, GLOOM_SHROOM});
    col_1.resetPaoList({{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}});
    col_6.resetPaoList({{1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}});
    smart_c.pushFunc(SmartC);
    SetTime(99, 1);
    col_1.roofPao(2, 9);
    col_6.roofPao(4, 9);
    for (auto wave : {2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20}) {
        SetTime(108, wave);
        col_1.roofPao(2, 9);
        col_6.roofPao(4, 9);
    }
    for (auto wave : {2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18}) {
        InsertTimeOperation(500, wave, [=]() {
            if (GetMainObject()->refreshCountdown() > 200) {
                CardNotInQueue(6, 4, 9);
                CardNotInQueue(7, 4, 9);
            }
        });
    }
    for (auto wave : {9, 19, 20}) {
        InsertTimeOperation(803, wave, [=]() {
            if (GetMainObject()->refreshCountdown() > 200 || IsGGExist()) {
                SetTime(803, wave);
                col_1.roofPao(2, 9);
                col_6.roofPao(4, 9);
            }
        });
        InsertTimeOperation(1498, wave, [=]() {
            if (IsGGExist() && GetMainObject()->wave() == wave) {
                SetTime(1498, wave);
                col_1.roofPao(2, 9);
                col_6.roofPao(4, 9);
            }
        });
    }
    SetTime(200, 10);
    col_1.roofPao(2, 9);
    col_6.roofPao(4, 9);
    InsertTimeOperation(590, 10, [=]() {
        if (GetMainObject()->refreshCountdown() > 200) {
            CardNotInQueue(6, {{3, 9}, {2, 9}});
            CardNotInQueue(3, {{3, 9}, {2, 9}});
        }
    });
    SetTime(-150, 20);
    Card({{FLOWER_POT, 3, 8}, {ICE_SHROOM, 3, 8}});
    InsertTimeOperation(0, 4, [=]() {
        auto plant = GetMainObject()->plantArray();
        for (int index = 0; index < GetMainObject()->plantTotal(); index++) {
            if (plant[index].type() == COB_CANNON && plant[index].hp() < 201 && plant[index].stateCountdown() > 1300 && !plant[index].isDisappeared()) {
                int row = plant[index].row() + 1;
                int col = plant[index].col() + 1;
                ShovelNotInQueue(row, col);
                CardNotInQueue(4, row, col);
                SetTime(751, 4);
                Card(KERNEL_PULT, row, col + 1);
                Card(COB_CANNON, row, col);
                col_1.resetPaoList({{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}});
                col_6.resetPaoList({{1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}});
                switch (row + col) {
                case 2: //(1, 1)
                    SetTime(0, 4);
                    col_1.resetPaoList({{2, 1}, {3, 1}, {4, 1}, {5, 1}});
                    break;
                case 3: //(2, 1)
                    SetTime(0, 4);
                    col_1.resetPaoList({{3, 1}, {4, 1}, {5, 1}, {1, 1}});
                    break;
                case 4: //(3, 1)
                    SetTime(0, 4);
                    col_1.resetPaoList({{4, 1}, {5, 1}, {1, 1}, {2, 1}});
                    break;
                case 5: //(4, 1)
                    SetTime(0, 4);
                    col_1.resetPaoList({{5, 1}, {1, 1}, {2, 1}, {3, 1}});
                    break;
                case 6: //(5, 1)
                    SetTime(0, 4);
                    col_1.resetPaoList({{1, 1}, {2, 1}, {3, 1}, {4, 1}});
                    break;
                case 7: //(1, 6)
                    SetTime(0, 4);
                    col_6.resetPaoList({{2, 6}, {3, 6}, {4, 6}, {5, 6}});
                    break;
                case 8: //(2, 6)
                    SetTime(0, 4);
                    col_6.resetPaoList({{3, 6}, {4, 6}, {5, 6}, {1, 6}});
                    break;
                case 9: //(3, 6)
                    SetTime(0, 4);
                    col_6.resetPaoList({{4, 6}, {5, 6}, {1, 6}, {2, 6}});
                    break;
                case 10: //(4, 6)
                    SetTime(0, 4);
                    col_6.resetPaoList({{5, 6}, {1, 6}, {2, 6}, {3, 6}});
                    break;
                default: //(5, 6)
                    SetTime(0, 4);
                    col_6.resetPaoList({{1, 6}, {2, 6}, {3, 6}, {4, 6}});
                    break;
                }
                break;
            }
        }
    });
}