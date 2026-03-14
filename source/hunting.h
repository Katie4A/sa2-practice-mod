#pragma once
#include "pch.h"



struct Fvec3 {
    float x;
    float y;
    float z;
};

struct Emerald {
    int id;
    struct Fvec3 position;
};

struct SearchEmeraldsGameManager {
    unsigned char state;
    unsigned char field1_0x1;
    unsigned char field2_0x2;
    unsigned char hints_read;
    unsigned char has_gotten_very_close;
    unsigned char emeralds_spawned;
    unsigned char slot_1_array_len;
    unsigned char slot_2_array_len;
    unsigned char slot_3_array_len;
    unsigned char slot_enemy_array_len;
    unsigned char field10_0xa;
    unsigned char field11_0xb;
    struct Emerald duplicate_emerald_1;
    struct Emerald duplicate_emerald_2;
    struct Emerald piece_1;
    struct Emerald piece_2;
    struct Emerald piece_3;
    struct Emerald* slot_1_emeralds;
    struct Emerald* slot_2_emeralds;
    struct Emerald* slot_3_emeralds;
    struct Emerald* slot_enemy_emeralds;
    int timer;
    unsigned char field22_0x70;
    unsigned char field23_0x71;
    unsigned char field24_0x72;
    unsigned char field25_0x73;
};



class HuntingSettings {
public:
    HuntingSettings() {}
    void init();
    void RenderTab();
};

void generateSet_impl(SearchEmeraldsGameManager* emeraldManager);
