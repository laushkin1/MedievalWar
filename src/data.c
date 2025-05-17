#include "data.h"
#include <stddef.h>

const ITEM items[NUMBER_OF_ITEMS] = {
        {.name = "wand", .att = 12, .def = 4, .slots = 1, .range = 4, .radius = 2}, //                  0
        {.name = "fireball", .att = 11, .def = 0, .slots = 1, .range = 3, .radius = 3}, //              1
        {.name = "sword", .att = 9, .def = 2, .slots = 1, .range = 0, .radius = 0}, //                  2
        {.name = "spear", .att = 6, .def = 1, .slots = 1, .range = 1, .radius = 1}, //                  3
        {.name = "dagger", .att = 4, .def = 0, .slots = 1, .range = 0, .radius = 0}, //                 4
        {.name = "rock", .att = 3, .def = 0, .slots = 1, .range = 2, .radius = 1}, //                   5
        {.name = "armor", .att = 2, .def = 7, .slots = 1, .range = 0, .radius = 0}, //                  6
        {.name = "shield", .att = 2, .def = 6, .slots = 1, .range = 0, .radius = 0}, //                 7
        {.name = "gloves", .att = 1, .def = 4, .slots = 1, .range = 0, .radius = 0}, //                 8
        {.name = "helmet", .att = 1, .def = 5, .slots = 1, .range = 0, .radius = 0}, //                 9
        {.name = "aura", .att = 0, .def = 8, .slots = 1, .range = 0, .radius = 0}, //                   10

        {.name = "cannon", .att = 12, .def = 0, .slots = 2, .range = 4, .radius = 4}, //                11
        {.name = "axe", .att = 10, .def = 2, .slots = 2, .range = 1, .radius = 1}, //                   12
        {.name = "hammer", .att = 8, .def = 2, .slots = 2, .range = 1, .radius = 2}, //                 13
        {.name = "crossbow", .att = 5, .def = 1, .slots = 2, .range = 3, .radius = 0}, //               14
        {.name = "slingshot", .att = 2, .def = 0, .slots = 2, .range = 2, .radius = 1} //               15
};

const UNIT units[NUMBER_OF_UNITS] = {
    {
        .name = "Pyromancer",
        .item1 = &items[0],     // wand
        .item2 = &items[1],     // fireball
        .hp = 60,
        .description = "A fiery spellcaster combining pinpoint attacks and area damage."
    },
    {
        .name = "Warrior",
        .item1 = &items[2],     // sword
        .item2 = &items[6],     // armor
        .hp = 85,
        .description = "A melee fighter with balanced attack and solid defense."
    },
    {
        .name = "Vanguard",
        .item1 = &items[3],     // spear
        .item2 = &items[9],     // helmet
        .hp = 75,
        .description = "A frontline soldier who keeps enemies at bay while staying protected."
    },
    {
        .name = "Assassin",
        .item1 = &items[4],     // dagger
        .item2 = &items[8],     // gloves
        .hp = 55,
        .description = "A fast and stealthy killer that strikes swiftly in close combat."
    },
    {
        .name = "Stonecaller",
        .item1 = &items[5],     // rock
        .item2 = &items[10],    // aura
        .hp = 70,
        .description = "A mystical unit using natural forces and protective energy."
    },
    {
        .name = "Guardian",
        .item1 = &items[7],     // shield
        .item2 = &items[9],     // helmet
        .hp = 90,
        .description = "A heavily armored unit designed to absorb damage and protect allies."
    },
    {
        .name = "HeavyGunner",
        .item1 = &items[11],    // cannon
        .item2 = NULL,
        .hp = 80,
        .description = "An artillery expert delivering massive area damage from afar."
    },
    {
        .name = "Berserker",
        .item1 = &items[12],    // axe
        .item2 = NULL,
        .hp = 85,
        .description = "A savage fighter wielding a brutal weapon for devastating attacks."
    },
    {
        .name = "Crusader",
        .item1 = &items[13],    // hammer
        .item2 = NULL,
        .hp = 88,
        .description = "A holy warrior using a mighty hammer to crush foes."
    },
    {
        .name = "Sharpshooter",
        .item1 = &items[14],    // crossbow
        .item2 = NULL,
        .hp = 65,
        .description = "A precise ranged attacker effective from a safe distance."
    },
    {
        .name = "Trickster",
        .item1 = &items[15],    // slingshot
        .item2 = NULL,
        .hp = 60,
        .description = "A nimble fighter using improvised ranged tactics to harass enemies."
    },
    {
        .name = "Templar",
        .item1 = &items[0],     // wand
        .item2 = &items[10],    // aura
        .hp = 70,
        .description = "A divine caster with both offensive and defensive magic."
    },
    {
        .name = "Skirmisher",
        .item1 = &items[3],     // spear
        .item2 = &items[5],     // rock
        .hp = 68,
        .description = "An agile fighter combining medium-range thrusts with throwable rocks."
    },
    {
        .name = "Juggernaut",
        .item1 = &items[6],     // armor
        .item2 = &items[7],     // shield
        .hp = 100,
        .description = "An unstoppable defensive wall, ideal for absorbing damage."
    },
    {
        .name = "BladeDancer",
        .item1 = &items[2],     // sword
        .item2 = &items[8],     // gloves
        .hp = 70,
        .description = "A fast and graceful swordsman focused on finesse and precision."
    },
    {
        .name = "Raider",
        .item1 = &items[12],    // axe
        .item2 = &items[4],     // dagger
        .hp = 80,
        .description = "A ruthless attacker using brutal melee combinations."
    }
};
