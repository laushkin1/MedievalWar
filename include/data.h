#ifndef DATA_H
#define DATA_H

#define NUMBER_OF_ITEMS 16  // number of available items in the database
#define NUMBER_OF_UNITS 16  // number of available units in the database
#define MAX_NAME 100        // maximum length of item or unit name
#define MIN_ARMY 1          // minimum number of combat units in an army
#define MAX_ARMY 5          // maximum number of combat units in an army
#define FIELD_ROWS 5        // number of battlefield rows
#define FIELD_COLS 5        // number of battlefield cols
#include <stdbool.h>


// Structure representing an item in inventory
typedef struct item {
    char name[MAX_NAME + 1];
    int att;
    int def;
    int slots;
    int range;
    int radius;
} ITEM;

// Structure representing a combat unit
typedef struct unit {
    char name[MAX_NAME + 1];
    const ITEM *item1;
    const ITEM *item2;
    int hp;
    const char *description;
} UNIT;

// Structure representing a window dimensions
typedef struct {
    int x;
    int y;
    int width;
    int height;
} WindowDimensions;

// Structure representing a menu item (button)
typedef struct {
    char *label;
    int selected;
} MenuItem;

// Structure representing a selectable unit in a menu
typedef struct {
    const UNIT* unit;
    bool selected;
    bool available;
} MENU_UNIT;

// Database of items available to combat units
extern const ITEM items[NUMBER_OF_ITEMS];
extern const UNIT units[NUMBER_OF_UNITS];

#endif
