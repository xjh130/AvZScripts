#include "avz.h"
using namespace AvZ;
int GGnum = 1437;
struct I {
    int row;
    int index;
};
void CountGG()
{
    for (int wave = 1; wave < 20; ++wave) {
        InsertTimeOperation(1, wave, [=]() {
            for (auto&& zombie : alive_zombie_filter) {
                if (zombie->type() == 32 && zombie->abscissa() > 840) {
                    GGnum = GGnum + 1;
                }
            }
        });
    }
}
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
void SmartCNotInQueue(int row)
{
    auto seed = GetMainObject()->seedArray();
    if (seed[4].isUsable()) {
        CardNotInQueue(5, row, 9);
        return;
    } else if (seed[5].isUsable()) {
        CardNotInQueue(6, row, 9);
        return;
    } else if (seed[8].isUsable()) {
        CardNotInQueue(9, row, 9);
        return;
    } else if (seed[7].isUsable()) {
        CardNotInQueue(8, row, 9);
        return;
    }
}
I GetIrowindex()
{
    auto plant = GetMainObject()->plantArray();
    for (int index = 0; index < GetMainObject()->plantCountMax(); ++index) {
        if (!plant[index].isDisappeared() && plant[index].type() == ICE_SHROOM) {
            return {plant[index].row() + 1, index};
        }
    }
}
void ICE3(int P_time, int wave)
{
    SetTime(P_time, wave);
    plant_fixer.pause();
    InsertTimeOperation(P_time + 180, wave, [=]() {
        I i = GetIrowindex();
        int plant_next = GetMainObject()->plantNext();
        if (plant_next > i.index) {
            SetTime(P_time + 180, wave);
            Card(COFFEE_BEAN, i.row, 9);
        } else {
            SetTime(P_time + 181, wave);
            Card(COFFEE_BEAN, i.row, 9);
        }
    });
    SetTime(P_time + 182, wave);
    plant_fixer.goOn();
}
void CCCC(int time, int wave)
{
    SetTime(time, wave);
    Card({{PUFF_SHROOM, 1, 9}, {SUN_SHROOM, 2, 9}, {SCAREDY_SHROOM, 5, 9}, {FLOWER_POT, 6, 9}});
    SetTime(time + 100, wave);
    Shovel({{1, 9}, {2, 9}, {5, 9}, {6, 9}});
}
void PPDD_J(int wave)
{
    SetTime(-14, wave);
    pao_operator.pao({{2, 9}, {5, 9}});
    SetTime(93, wave);
    pao_operator.pao({{1, 7.8}, {5, 7.8}});
}

void PPDD(int wave)
{
    SetTime(-55, wave);
    pao_operator.pao({{2, 9}, {5, 9}});
    SetTime(45, wave);
    pao_operator.pao({{2, 9}, {5, 9}});
    if (wave != 9 && wave != 19 && wave != 20) {
        InsertTimeOperation(600, wave, [=]() {
            if (GetMainObject()->refreshCountdown() > 200) {
                SetTime(600, wave);
                pao_operator.pao({{2, 9}, {5, 9}});
            }
        });
    }
}
void PPI(int wave)
{
    SetTime(-84, wave);
    pao_operator.pao({{2, 9}, {5, 9}});
    ICE3(-84, wave);
}
void PPSSDD(int time, int wave)
{
    SetTime(time - 55, wave);
    pao_operator.pao({{2, 9}, {5, 9}, {2, 9}, {5, 9}});
    SetTime(time + 55, wave);
    pao_operator.pao({{1, 8.8}, {5, 8.8}});
}
void cc_row_12(int wave)
{
    InsertTimeOperation(168, wave, [=]() {
        if (IsZombieExist(8, 1)) {
            SmartCNotInQueue(1);
        }
        if (IsZombieExist(8, 2)) {
            SmartCNotInQueue(2);
        }
    });
    SetTime(268, wave);
    Shovel({{1, 9}, {2, 9}});
}
void cc_row_56(int wave)
{
    InsertTimeOperation(168, wave, [=]() {
        if (IsZombieExist(8, 5)) {
            SmartCNotInQueue(5);
        }
        if (IsZombieExist(8, 6)) {
            SmartCNotInQueue(6);
        }
    });
    SetTime(268, wave);
    Shovel({{5, 9}, {6, 9}});
}
void _PP(int wave)
{
    InsertTimeOperation(500, wave, [=]() {
        if (GetMainObject()->refreshCountdown() > 200) {
            if (IsZombieExist(-1, 1) || IsZombieExist(-1, 2) || IsZombieExist(-1, 3)) {
                SetTime(500, wave);
                pao_operator.pao(2, 9);
            }
            if (IsZombieExist(-1, 4) || IsZombieExist(-1, 5) || IsZombieExist(-1, 6)) {
                SetTime(500, wave);
                pao_operator.pao(5, 9);
            }
        }
    });
}
void w8_18_PD_PI_PS(int wave)
{
    PPDD_J(wave);
    InsertTimeOperation(420, wave, [=]() {
        if (GetMainObject()->refreshCountdown() > 200) {
            CCCC(647, wave);
        }
    });
    InsertTimeOperation(500, wave, [=]() {
        if (GetMainObject()->refreshCountdown() > 200) {
            SetTime(546 + 65, wave);
            pao_operator.pao({{2, 9}, {5, 9}});
            SetTime(621 + 65, wave);
            ice_filler.coffee();
            PPSSDD(0, wave + 1);
            _PP(wave + 1);
        } else {
            PPI(wave + 1);
            PPSSDD(601, wave + 1);
        }
    });
}
void PPDD_PPDD_PPDD(int wave)
{
    for (int w = wave; w < 20; ++w) {
        InsertTimeOperation(-84, w, [=]() {
            if (ReadyCOB() >= 4) {
                PPDD(w);
            } else {
                PPI(w);
                InsertTimeOperation(500, w, [=]() {
                    if (GetMainObject()->refreshCountdown() > 200) {
                        SetTime(600, w);
                        pao_operator.pao({{2, 9}, {5, 9}});
                    }
                });
            }
        });
    }
}
void PPDD_PPI_PPSS(int wave)
{
    PPDD_J(wave);
    SetTime(546, wave);
    pao_operator.pao({{2, 9}, {5, 9}});
    InsertTimeOperation(621, wave, [=]() {
        if (IsZombieExist(32)) {
            SetTime(621, wave);
            ice_filler.coffee();
            SetTime(1147, wave);
            pao_operator.pao({{2, 9}, {5, 9}, {2, 9}, {5, 9}});
        }
    });
}
void PD_PI_PSD(int wave)
{
    InsertTimeOperation(-200, wave, [=]() {
        if (GGnum >= 50 && wave > 10) {
            PPDD_PPDD_PPDD(wave);
            _PP(19);
        } else {
            PPDD_J(wave);
            InsertTimeOperation(420, wave, [=]() {
                if (GetMainObject()->refreshCountdown() > 200) {
                    CCCC(647, wave);
                }
            });
            InsertTimeOperation(500, wave, [=]() {
                if (GetMainObject()->refreshCountdown() > 200) {
                    SetTime(610, wave);
                    pao_operator.pao({{2, 9}, {5, 9}});
                    SetTime(685, wave);
                    ice_filler.coffee();
                    if (GGnum >= 50) {
                        PPDD(wave + 1);
                    } else {
                        PPSSDD(0, wave + 1);
                    }
                    if (wave < 6 || (wave > 10 && wave < 16)) {
                        PD_PI_PSD(wave + 2);
                    } else if (wave == 6 || wave == 16) {
                        w8_18_PD_PI_PS(wave + 2);
                    } else {
                        PPDD_PPI_PPSS(wave + 2);
                    }
                } else {
                    PPI(wave + 1);
                    if (GGnum >= 50 && wave > 10) {
                        InsertTimeOperation(500, wave + 1, [=]() {
                            if (GetMainObject()->refreshCountdown() > 200) {
                                SetTime(650, wave + 1);
                                pao_operator.pao({{2, 9}, {5, 9}});
                                SetTime(750, wave + 1);
                                pao_operator.pao({{2, 9}, {5, 9}});
                            }
                        });
                        PPDD_PPDD_PPDD(wave + 2);
                        _PP(19);
                    } else {
                        InsertTimeOperation(500, wave + 1, [=]() {
                            if (GetMainObject()->refreshCountdown() > 200) {
                                PPSSDD(705, wave + 1);
                                if (wave < 6 || (wave > 10 && wave < 16)) {
                                    PD_PI_PSD(wave + 2);
                                } else if (wave == 6 || wave == 16) {
                                    w8_18_PD_PI_PS(wave + 2);
                                } else {
                                    PPDD_PPI_PPSS(wave + 2);
                                }
                            } else {
                                PPSSDD(0, wave + 2);
                                if (wave < 5 || (wave > 10 && wave < 15)) {
                                    PD_PI_PSD(wave + 3);
                                } else if (wave == 5 || wave == 15) {
                                    w8_18_PD_PI_PS(wave + 3);
                                } else if (wave == 6 || wave == 16) {
                                    PPDD_PPI_PPSS(wave + 3);
                                } else {
                                    _PP(wave + 2);
                                }
                            }
                        });
                    }
                }
            });
        }
    });
}
void w10_PPSSDD_w20_PPSSDD()
{
    for (auto wave : {10, 20}) {
        SetTime(-78, wave);
        pao_operator.pao(2, 9);
        SetTime(-55, wave);
        pao_operator.pao({{5, 9}, {5, 9}});
        SetTime(55, wave);
        pao_operator.pao(5, 8.8);
        SetTime(132, wave);
        pao_operator.pao(1, 8.8);
        if (wave == 10) {
            SetTime(22, wave);
            pao_operator.pao(2, 9);
        }
    }
    InsertTimeOperation(22, 20, [=]() {
        if (ReadyCOB() >= 3) {
            SetTime(22, 20);
            pao_operator.pao(2, 9);
        } else {
            SetTime(295, 20);
            Card(CHERRY_BOMB, 2, 9);
        }
    });
}
void Script()
{
    OpenMultipleEffective('X', MAIN_UI_OR_FIGHT_UI);
    pao_operator.autoGetPaoList();
    ice_filler.start({{4, 9}, {3, 9}});
    plant_fixer.start(PUMPKIN, {{4, 9}, {3, 9}});
    auto zombie_type_list = GetMainObject()->zombieTypeList();
    MaidCheats::dancing();
    SetTime(-200, 10);
    MaidCheats::stop();
    SetTime(359, 11);
    MaidCheats::dancing();
    SetTime(-200, 20);
    MaidCheats::stop();
    SetTime(-150, 20);
    pao_operator.pao(4, 7.5);
    InsertTimeOperation(500, 20, [=]() {
        if (IsZombieExist()) {
            SetTime(500, 20);
            pao_operator.pao({{2, 9}, {5, 9}});
        }
    });
    if (zombie_type_list[GIGA_GARGANTUAR]) {
        InsertTimeOperation(-599, 1, [=]() { GGnum = 0; });
        CountGG();
        w10_PPSSDD_w20_PPSSDD();
        if (zombie_type_list[GARGANTUAR]) {
            SelectCards({ICE_SHROOM, M_ICE_SHROOM, COFFEE_BEAN, CHERRY_BOMB, PUMPKIN, PUFF_SHROOM, SUN_SHROOM, SCAREDY_SHROOM, FLOWER_POT, STARFRUIT});
            PD_PI_PSD(1);
            PD_PI_PSD(11);
        } else {
            SelectCards({ICE_SHROOM, COFFEE_BEAN, CHERRY_BOMB, PUMPKIN, PUFF_SHROOM, M_PUFF_SHROOM, SUN_SHROOM, SCAREDY_SHROOM, FLOWER_POT, STARFRUIT});
            SetTime(-94, 1);
            pao_operator.pao({{2, 9}, {2, 9}});
            SetTime(16, 1);
            pao_operator.pao(1, 8.8);
            InsertTimeOperation(500, 1, [=]() {
                if (GetMainObject()->refreshCountdown() > 200) {
                    SetTime(629, 1);
                    pao_operator.pao({{2, 9}, {5, 9}, {5, 9}, {5, 9}});
                }
            });
            for (auto wave : {3, 5, 7, 9, 13, 15, 17, 19}) {
                InsertTimeOperation(-200, wave - 1, [=]() {
                    if (GGnum < 50) {
                        SetTime(-94, wave);
                        pao_operator.pao({{2, 9}, {2, 9}});
                        SetTime(16, wave);
                        pao_operator.pao(1, 8.8);
                        InsertTimeOperation(500, wave, [=]() {
                            if (GetMainObject()->refreshCountdown() > 200 && (wave != 9) && (wave != 19)) {
                                SetTime(629, wave);
                                pao_operator.pao({{2, 9}, {5, 9}, {5, 9}, {5, 9}});
                            }
                        });
                    } else {
                        PPDD(wave);
                    }
                });
            }
            for (auto wave : {2, 4, 6, 8, 12, 14, 16, 18}) {
                InsertTimeOperation(-200, wave - 1, [=]() {
                    if (GGnum < 50) {
                        SetTime(-94, wave);
                        pao_operator.pao({{5, 9}, {5, 9}});
                        SetTime(16, wave);
                        pao_operator.pao(5, 8.8);
                        InsertTimeOperation(500, wave, [=]() {
                            if (GetMainObject()->refreshCountdown() > 200) {
                                SetTime(629, wave);
                                pao_operator.pao({{5, 9}, {2, 9}, {2, 9}, {2, 9}});
                            }
                        });
                    } else {
                        PPDD(wave);
                    }
                });
            }
            if (zombie_type_list[DANCING_ZOMBIE] && !zombie_type_list[POLE_VAULTING_ZOMBIE]) {
                SetTime(-55, 1);
                pao_operator.pao(5, 9);
                for (auto wave : {3, 5, 7, 9, 13, 15, 17, 19}) {
                    InsertTimeOperation(-200, wave - 1, [=]() {
                        if (GGnum < 50) {
                            SetTime(-55, wave);
                            pao_operator.pao(5, 9);
                        }
                    });
                }
                for (auto wave : {2, 4, 6, 8, 14, 16, 18}) {
                    InsertTimeOperation(-200, wave - 1, [=]() {
                        if (GGnum < 50) {
                            SetTime(-55, wave);
                            pao_operator.pao(2, 9);
                        }
                    });
                }
                SetTime(-55, 11);
                pao_operator.pao({{2, 9}, {5, 9}});
            } else {
                SetTime(-14, 1);
                pao_operator.pao(5, 9);
                if (zombie_type_list[DANCING_ZOMBIE]) {
                    cc_row_56(1);
                }
                for (auto wave : {3, 5, 7, 9, 13, 15, 17, 19}) {
                    InsertTimeOperation(-200, wave - 1, [=]() {
                        if (GGnum < 50) {
                            SetTime(-14, wave);
                            pao_operator.pao(5, 9);
                            if (zombie_type_list[DANCING_ZOMBIE]) {
                                cc_row_56(wave);
                            }
                        }
                    });
                }
                for (auto wave : {2, 4, 6, 8, 14, 16, 18}) {
                    InsertTimeOperation(-200, wave - 1, [=]() {
                        if (GGnum < 50) {
                            SetTime(-14, wave);
                            pao_operator.pao(2, 9);
                            if (zombie_type_list[DANCING_ZOMBIE]) {
                                cc_row_12(wave);
                            }
                        }
                    });
                }
                SetTime(-14, 11);
                pao_operator.pao({{2, 9}, {5, 9}});
                if (zombie_type_list[DANCING_ZOMBIE]) {
                    CCCC(168, 11);
                }
            }
            _PP(9);
            _PP(19);
            for (auto wave : {9, 19}) {
                InsertTimeOperation(500, wave, [=]() {
                    if (IsZombieExist(32, 5) || IsZombieExist(32, 6)) {
                        SetTime(500, wave);
                        pao_operator.pao(5, 9);
                        SetTime(610, wave);
                        pao_operator.pao(5, 8.8);
                    }
                });
            }
            InsertTimeOperation(2, 10, [=]() {
                if (GGnum < 50) {
                    InsertTimeOperation(500, 11, [=]() {
                        if (GetMainObject()->refreshCountdown() > 200) {
                            if (!(zombie_type_list[DANCING_ZOMBIE] && zombie_type_list[POLE_VAULTING_ZOMBIE])) {
                                CCCC(500, 11);
                            }
                            SetTime(629, 11);
                            pao_operator.pao({{2, 9}, {2, 9}, {5, 9}});
                            ICE3(629, 11);
                            if (zombie_type_list[DANCING_ZOMBIE] && !zombie_type_list[POLE_VAULTING_ZOMBIE]) {
                                SetTime(-55, 12);
                                pao_operator.pao(2, 9);
                            } else {
                                SetTime(-14, 12);
                                pao_operator.pao(2, 9);
                                if (zombie_type_list[DANCING_ZOMBIE]) {
                                    cc_row_12(12);
                                }
                            }
                        } else {
                            SetTime(-82, 12);
                            pao_operator.pao(2, 9);
                            ICE3(-82, 12);
                        }
                    });
                } else {
                    InsertTimeOperation(500, 11, [=]() {
                        if (GetMainObject()->refreshCountdown() > 200) {
                            if (!(zombie_type_list[DANCING_ZOMBIE] && zombie_type_list[POLE_VAULTING_ZOMBIE])) {
                                CCCC(500, 11);
                            }
                            SetTime(629, 11);
                            pao_operator.pao({{2, 9}, {2, 9}, {5, 9}});
                            ICE3(629, 11);
                        }
                    });
                }
            });
        }
    } else {
        SelectCards({ICE_SHROOM, M_ICE_SHROOM, COFFEE_BEAN, CHERRY_BOMB, PUMPKIN, PUFF_SHROOM, SUN_SHROOM, SCAREDY_SHROOM, FLOWER_POT, STARFRUIT});
        for (int wave = 1; wave < 21; ++wave) {
            PPDD(wave);
        }
        _PP(9);
        _PP(19);
    }
}
