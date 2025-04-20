#include "data.h"

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
