#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "data.h"

// errors
#define ERR_UNIT_COUNT "ERR_UNIT_COUNT"
#define ERR_ITEM_COUNT "ERR_ITEM_COUNT"
#define ERR_WRONG_ITEM "ERR_WRONG_ITEM"
#define ERR_SLOTS "ERR_SLOTS"

bool raise_unit_count_err(int unit_count);
bool raise_item_count_err(int itemCount);
bool inventory_owerflow(UNIT character);
bool is_game_over(UNIT army1[], UNIT army2[]);

int countItems(char* itemsName, char* arrayWithOutItems[]);
int create_army(UNIT *emptyArray, int num_of_units);
void print_army(UNIT army1[], UNIT army2[], int army1Size, int army2Size);
void print_units(UNIT army1[], UNIT army2[], int army1Size, int army2Size);
void delete_unit(UNIT army[], int unitIndex);
void fight(UNIT AttackingArmy[], UNIT UnderAttackArmy[], int AttackingArmySize, int UnderAttackArmySize, int numAttackingArmy);
void update_army(UNIT army[], int* armySize);


int main(const int argc, char *argv[]) {

    int maxRound = 1000000;
    if (argc > 1) maxRound = atoi(argv[1]);

    // Create First Army
    int army1Size = 0;
    scanf("%d", &army1Size);

    UNIT first_team[army1Size];
    if (create_army(first_team, army1Size)) return 0;

    // Create Second Army
    int army2Size = 0;
    scanf("%d", &army2Size);

    UNIT second_team[army2Size];
    if (create_army(second_team, army2Size)) return 0;

    print_army(first_team, second_team, army1Size, army2Size);

    if (maxRound)
    {
        printf("\n");
        int number_of_round = 1;

        while(true) {
            if (number_of_round > maxRound) break;
            printf("Round %d\n", number_of_round++);
            print_units(first_team, second_team, army1Size, army2Size); // vypis armad na zaciatku kola
            fight(first_team, second_team, army1Size, army2Size, 1); // vypis sposobenych zraneni
            fight(second_team, first_team, army2Size, army1Size, 2);
            update_army(first_team, &army1Size); // armada 1 sa aktualizuje
            update_army(second_team, &army2Size); // armada 2 sa aktualizuje
            print_units(first_team, second_team, army1Size, army2Size); // vypis armad na konci kola
            printf("\n");
            if (is_game_over(first_team, second_team)) break;
        }

        if (first_team[0].name[0] == '\0' && second_team[0].name[0] == '\0')
            printf("NO WINNER\n");
        else if (second_team[0].name[0] == '\0')
            printf("WINNER: 1\n");
        else if (first_team[0].name[0] == '\0')
            printf("WINNER: 2\n");
    }
    return 0;
}



bool raise_unit_count_err(const int unit_count)
{
    if (unit_count < MIN_ARMY || unit_count > MAX_ARMY) {
        printf("%s", ERR_UNIT_COUNT);
        return true;
    }
    return false;
}

bool raise_item_count_err(int itemCount)
{
    if (itemCount > 2 || itemCount < 1)
    {
        printf("%s", ERR_ITEM_COUNT);
        return true;
    }
    return false;
}

bool inventory_owerflow(UNIT character)
{
    if (character.item2 != NULL)
        if (character.item1->slots + character.item2->slots > 2){printf("%s", ERR_SLOTS); return true;}
    return false;
}

bool is_game_over(UNIT army1[], UNIT army2[])
{
    if (army1[0].name[0] == '\0' || army2[0].name[0] == '\0') return true;
    return false;
}


int countItems(char* itemsName, char* arrayWithOutItems[])
{
    int itemCount = 0;
    char* token = strtok(itemsName, " ");
    while (token != NULL && itemCount < 3)
    {
        arrayWithOutItems[itemCount] = token;
        token = strtok(NULL, " ");
        itemCount++;
    }
    return itemCount;
}

int create_army(UNIT *emptyArray, int num_of_units)
{

    if (raise_unit_count_err(num_of_units)) return 1;

    // Create arrow of units
    char unitName[MAX_NAME];
    char itemsName[MAX_NAME];

    for (int i = 0; i < num_of_units; i++)
    {
        itemsName[0] = '\0';
        // Read unit Name
        scanf("%s", unitName);
        // Read unit Items
        scanf("%[^\n]", itemsName);


        // Check how many items user give
        char* itemsNameArr[3];
        int itemCount = countItems(itemsName, itemsNameArr);
        // Raise Error
        if (raise_item_count_err(itemCount)) return 1;

        // create unit
        UNIT unit = {.name = "", .item1 = NULL, .item2 = NULL, .hp = 0};
        bool wrong_item = true;
        if (itemCount == 1)
        {
            for (int n = 0; n < NUMBER_OF_ITEMS; n++)
            {
                // check if this item exist in db
                if (strcmp(itemsNameArr[0], items[n].name) == 0)
                {
                    strcpy(unit.name, unitName);
                    unit.item1 = &items[n];
                    unit.hp = 100;

                    wrong_item = false;
                    break;
                }
            }
        } else if (itemCount == 2)
        {
            int firstNameIdx = -1;
            int secondNameIdx = -1;
            for (int n = 0; n < NUMBER_OF_ITEMS; n++)
            {
                if (strcmp(itemsNameArr[0], items[n].name) == 0) firstNameIdx = n;
                if (strcmp(itemsNameArr[1], items[n].name) == 0) secondNameIdx = n;

                if (firstNameIdx >= 0 && secondNameIdx >= 0)
                {
                    strcpy(unit.name, unitName);
                    unit.item1 = &items[firstNameIdx];
                    unit.item2 = &items[secondNameIdx];
                    unit.hp = 100;

                    wrong_item = false;
                    break;
                }
            }
        }

        // check if item wrong
        if (wrong_item) {printf("%s", ERR_WRONG_ITEM); return 1;}

        // check if the inventory is overflowing
        if (inventory_owerflow(unit)) return 1;

        emptyArray[i] = unit;
    }

    return 0;
}

void print_army(UNIT army1[], UNIT army2[], int army1Size, int army2Size)
{
    printf("Army 1\n");
    for (int i = 0; i < army1Size; i++)
    {
        printf("    Unit: %d\n", i);
        printf("    Name: %s\n", army1[i].name);
        printf("    HP: %d\n", army1[i].hp);
        printf("    Item 1: %s,%d,%d,%d,%d,%d\n", army1[i].item1->name,
                                                  army1[i].item1->att,
                                                  army1[i].item1->def,
                                                  army1[i].item1->slots,
                                                  army1[i].item1->range,
                                                  army1[i].item1->radius);
        if (army1[i].item2 != NULL)
            printf("    Item 2: %s,%d,%d,%d,%d,%d\n", army1[i].item2->name,
                                                      army1[i].item2->att,
                                                      army1[i].item2->def,
                                                      army1[i].item2->slots,
                                                      army1[i].item2->range,
                                                      army1[i].item2->radius);

        printf("\n");
    }


    printf("Army 2\n");
    for (int i = 0; i < army2Size; i++)
    {
        printf("    Unit: %d\n", i);
        printf("    Name: %s\n", army2[i].name);
        printf("    HP: %d\n", army2[i].hp);
        printf("    Item 1: %s,%d,%d,%d,%d,%d\n", army2[i].item1->name,
                                                  army2[i].item1->att,
                                                  army2[i].item1->def,
                                                  army2[i].item1->slots,
                                                  army2[i].item1->range,
                                                  army2[i].item1->radius);
        if (army2[i].item2 != NULL)
            printf("    Item 2: %s,%d,%d,%d,%d,%d\n", army2[i].item2->name,
                                                      army2[i].item2->att,
                                                      army2[i].item2->def,
                                                      army2[i].item2->slots,
                                                      army2[i].item2->range,
                                                      army2[i].item2->radius);


        if (i != army2Size-1) printf("\n");
    }
}

void print_units(UNIT army1[], UNIT army2[], int army1Size, int army2Size)
{
    printf("1: ");
    for (int i = 0; i < army1Size; i++)
        if (army1[i].name[0] != '\0') printf("%s,%d ", army1[i].name, army1[i].hp);
    printf("\n");

    printf("2: ");
    for (int i = 0; i < army2Size; i++)
        if (army2[i].name[0] != '\0') printf("%s,%d ", army2[i].name, army2[i].hp);
    printf("\n");
}

void delete_unit(UNIT army[], int unitIndex)
{
    army[unitIndex].name[0] = '\0';
    army[unitIndex].item1 = NULL;
    army[unitIndex].item2 = NULL;
    army[unitIndex].hp = 0;
}

void fight(UNIT AttackingArmy[], UNIT UnderAttackArmy[], int AttackingArmySize, int UnderAttackArmySize, int numAttackingArmy)
{
    // take all units from attacking army
    for (int i = 0; i < AttackingArmySize; i++)
    {
        char label[100];
        // Check if unit can attack
        // Attack with first item
        if (i <= AttackingArmy[i].item1->range){

            snprintf(label, sizeof(label), "%d,%s,%s:",
                numAttackingArmy, AttackingArmy[i].name, AttackingArmy[i].item1->name);
            printf("%-21s", label);
            // how match characters unit can attack
            int unitRadius = AttackingArmy[i].item1->radius;
            for (int j = 0; j <= unitRadius && j < UnderAttackArmySize; j++)
            {
                int damage = AttackingArmy[i].item1->att - UnderAttackArmy[j].item1->def;
                if (UnderAttackArmy[j].item2 != NULL) damage -= UnderAttackArmy[j].item2->def;
                if (damage < 1) damage = 1;
                UnderAttackArmy[j].hp = UnderAttackArmy[j].hp - damage;
                // Print damage
                printf("[%s,%d] ", UnderAttackArmy[j].name, damage);
            }
            printf("\n");
        }

        if (AttackingArmy[i].item2 != NULL)
        {
            if (i <= AttackingArmy[i].item2->range)
            {
                snprintf(label, sizeof(label), "%d,%s,%s:",
                    numAttackingArmy, AttackingArmy[i].name, AttackingArmy[i].item2->name);
                printf("%-21s", label);
                int unitRadius = AttackingArmy[i].item2->radius;
                for (int j = 0; j <= unitRadius && j < UnderAttackArmySize; j++)
                {
                    int damage = AttackingArmy[i].item2->att - UnderAttackArmy[j].item1->def;
                    if (UnderAttackArmy[j].item2 != NULL) damage -= UnderAttackArmy[j].item2->def;
                    if (damage < 1) damage = 1;
                    UnderAttackArmy[j].hp = UnderAttackArmy[j].hp - damage;
                    // Print damage
                    printf("[%s,%d] ", UnderAttackArmy[j].name, damage);
                }
                printf("\n");
            }
        }
    }
}

void update_army(UNIT army[], int* armySize)
{
    int aliveUnits = 0;
    for (int i = 0; i < *armySize; i++)
        if (army[i].hp > 0) army[aliveUnits++] = army[i];

    for (int i = aliveUnits; i < *armySize; i++)
        delete_unit(army, i);

    *armySize = aliveUnits;
}
