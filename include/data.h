#ifndef DATA_H
#define DATA_H

#define NUMBER_OF_ITEMS 16
#define MAX_NAME 100
#define MIN_ARMY 1          // minimum number of units in the army
#define MAX_ARMY 5          // maximum number of units in the army


typedef struct item {
    char name[MAX_NAME + 1];
    int att;
    int def;
    int slots;
    int range;
    int radius;
} ITEM;

typedef struct unit {
    char name[MAX_NAME + 1];
    const ITEM *item1;
    const ITEM *item2;
    int hp;
} UNIT;

extern const ITEM items[NUMBER_OF_ITEMS];

#endif //DATA_H


