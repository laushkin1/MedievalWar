#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#include "data.h"

// Shrek
const int SHREK_LINES = 25;
const char* SHREK[] = {
"                            ,.--------._",
"                           /            ''.",
"                         ,'                \\     |\"\\                /\\          /\\",
"                /\"|     /                   \\    |__\"              ( \\\\        // )",
"               \"_\"|    /           z#####z   \\  //                  \\ \\\\      // /",
"                 \\\\  #####        ##------\".  \\//                    \\_\\\\||||//_/",
"                  \\\\/-----\\     /          \".  \\                      \\/ _  _ \\",
"                   \\|      \\   |   ,,--..       \\                    \\/|(O)(O)|",
"                   | ,.--._ \\  (  | ##   \\)      \\                  \\/ |      |",
"                   |(  ##  )/   \\ `-....-//       |///////////////_\\/  \\      /",
"                     '--'.\"      \\                \\              //     |____|",
"                  /'    /         ) --.            \\            ||     /      \\",
"               ,..|     \\.________/    `-..         \\   \\       \\|     \\ 0  0 /",
"            _,##/ |   ,/   /   \\           \\         \\   \\       U    / \\_//_/",
"          :###.-  |  ,/   /     \\        /' \"\"\\      .\\        (     /",
"         /####|   |   (.___________,---',/    |       |\\=._____|  |_/",
"        /#####|   |     \\__|__|__|__|_,/             |####\\    |  ||",
"       /######\\   \\      \\__________/                /#####|   \\  ||",
"      /|#######`. `\\                                /#######\\   | ||",
"     /++\\#########\\  \\                      _,'    _/#########\\ | ||",
"    /++++|#########|  \\      .---..       ,/      ,'##########.\\|_||",
"   //++++|#########\\.  \\.              ,-/      ,'########,+++++\\\\_\\\\",
"  /++++++|##########\\.   '._        _,/       ,'######,''++++++++\\",
" |+++++++|###########|       -----.\"        _'#######' +++++++++++\\",
" |+++++++|############\\.     \\\\     //      /#######/++++ SHREK +++\\",
"      ________________________\\\\___//_________________________"
};


// Modes
void AI_vs_AI(void);
void Player_vs_Player(void);
void Player_vs_AI(void);

// Ncurses
void init_colors(void);
WindowDimensions calculate_window_dimensions(int items_count);
void draw_menu(WINDOW *menu_win, MenuItem *items, int items_count, const char *title);
int main_menu(void);
int new_game_menu(void);

// Frontend(GUI)
void place_army_on_field(UNIT* field[FIELD_ROWS][FIELD_COLS], UNIT* army, int armySize, int rowStart, int rowEnd);
void print_battlefield(UNIT* field[FIELD_ROWS][FIELD_COLS]);
void print_army(UNIT army1[], UNIT army2[], int army1Size, int army2Size);
void show_winner_screen(UNIT army1[], UNIT army2[], int army1Size, int army2Size);

// Backend(Logic)
// Create army
void create_random_army(UNIT *emptyArray, int num_of_units, bool *used_flags);
int unit_selection_menu(UNIT* emptyArray, bool* unitAvailability, bool *used_flags);

// Make fight
bool is_attacking_unit(UNIT* unit, UNIT AttackingArmy[], int AttackingArmySize);
bool is_underattack_unit(UNIT* unit, UNIT UnderAttackArmy[], int UnderAttackArmySize);
void fight(UNIT AttackingArmy[], UNIT UnderAttackArmy[], int AttackingArmySize, int UnderAttackArmySize, UNIT* field[FIELD_ROWS][FIELD_COLS]);
void autopilot(UNIT AttackingArmy[], UNIT UnderAttackArmy[], int AttackingArmySize, int UnderAttackArmySize, UNIT* field[FIELD_ROWS][FIELD_COLS], int sleep_second);

bool is_game_over(UNIT army1[], UNIT army2[], int army1Size, int army2Size);

// main function
void print_shrek(void);



int main(void) {
    // Ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    srand(time(NULL)); // Initializes the random number generator

    init_colors();
    bkgd(COLOR_PAIR(3));

    // Catch error
    if (LINES < 25 || COLS < 80) {
        endwin();
        printf("Terminal size too small. Please resize to at least 80x25.\n");
        return 1;
    }

    if (LINES != 25 || COLS != 80)
    {
        const char *line1 = "Strongly recommend that you make your terminal 80x25.";
        const char *line2 = "Enter any button to continue...";

        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(LINES / 2,     (COLS - strlen(line1)) / 2, "%s", line1);
        attroff(A_BOLD);
        mvprintw(LINES / 2 + 1, (COLS - strlen(line2)) / 2, "%s", line2);
        attroff(COLOR_PAIR(1));

        getch();
        clear();
    }

    // resizeterm(24, 80);

    // Draw menu
    while (1) {
        int choice = main_menu();

        switch (choice) {
        case 0: // New game
            {
                int sub_choice = new_game_menu();
                switch (sub_choice) {
                case 0: // AI vs AI
                        AI_vs_AI();
                        endwin();
                    return 0;
                case 1: // Player vs AI
                        Player_vs_AI();
                        endwin();
                    return 0;
                case 2: // Player vs Player
                        Player_vs_Player();
                        endwin();
                    return 0;
                case 3: // Exit
                    endwin();
                    return 0;
                }
            }
            break;
        case 1: // Exit
            endwin();
            return 0;
        }
    }

    endwin();
    return 0;
}



void AI_vs_AI(void)
{
    bool used_flags[NUMBER_OF_UNITS] = { false };

    // Create First Army with random size
    int army1Size = rand() % (MAX_ARMY - MIN_ARMY + 1) + MIN_ARMY;
    UNIT first_team[army1Size];
    create_random_army(first_team, army1Size, used_flags);

    // Create Second Army with random size
    int army2Size = rand() % (MAX_ARMY - MIN_ARMY + 1) + MIN_ARMY;
    UNIT second_team[army2Size];
    create_random_army(second_team, army2Size, used_flags);


    print_army(first_team, second_team, army1Size, army2Size);

    // Create battle field
    UNIT* battlefield[FIELD_ROWS][FIELD_COLS] = { {NULL} };
    place_army_on_field(battlefield, first_team, army1Size, 0, 1);
    place_army_on_field(battlefield, second_team, army2Size, 3, 4);

    // game core
    while(true) {
        if (is_game_over(first_team, second_team, army1Size, army2Size)) break;
        autopilot(first_team, second_team, army1Size, army2Size, battlefield, 50000);
        if (is_game_over(first_team, second_team, army1Size, army2Size)) break;
        autopilot(second_team, first_team, army2Size, army1Size, battlefield, 50000);
    }

    show_winner_screen(first_team, second_team, army1Size, army2Size);
}

void Player_vs_AI(void)
{
    bool used_flags[NUMBER_OF_UNITS] = { false };

    bool unitAvailable[NUMBER_OF_UNITS];
    for (int i = 0; i < NUMBER_OF_UNITS; i++)
        unitAvailable[i] = true;

    // Create the first army (ask the player to choose characters)
    UNIT first_team[MAX_ARMY];
    int army1Size = unit_selection_menu(first_team, unitAvailable, used_flags);

    // Create Second Army with random size
    int army2Size = rand() % (MAX_ARMY - MIN_ARMY + 1) + MIN_ARMY;
    UNIT second_team[army2Size];
    create_random_army(second_team, army2Size, used_flags);


    print_army(first_team, second_team, army1Size, army2Size);

    // Create battle field
    UNIT* battlefield[FIELD_ROWS][FIELD_COLS] = { {NULL} };
    place_army_on_field(battlefield, first_team, army1Size, 0, 1);
    place_army_on_field(battlefield, second_team, army2Size, 3, 4);

    // game core
    while(true) {
        if (is_game_over(first_team, second_team, army1Size, army2Size)) break;
        fight(first_team, second_team, army1Size, army2Size, battlefield);
        if (is_game_over(first_team, second_team, army1Size, army2Size)) break;
        autopilot(second_team, first_team, army2Size, army1Size, battlefield, 500000);
    }

    show_winner_screen(first_team, second_team, army1Size, army2Size);
}

void Player_vs_Player(void)
{
    bool used_flags[NUMBER_OF_UNITS] = { false };
    bool unitAvailable[NUMBER_OF_UNITS];
    for (int i = 0; i < NUMBER_OF_UNITS; i++)
        unitAvailable[i] = true;

    // Create the first army (ask the player to choose characters)
    UNIT first_team[MAX_ARMY];
    int army1Size = unit_selection_menu(first_team, unitAvailable, used_flags);

    // Create the second army (ask the player to choose characters)
    UNIT second_team[MAX_ARMY];
    int army2Size = unit_selection_menu(second_team, unitAvailable, used_flags);


    print_army(first_team, second_team, army1Size, army2Size);

    // Create battle field
    UNIT* battlefield[FIELD_ROWS][FIELD_COLS] = { {NULL} };
    place_army_on_field(battlefield, first_team, army1Size, 0, 1);
    place_army_on_field(battlefield, second_team, army2Size, 3, 4);

    // game core
    while(true) {
        if (is_game_over(first_team, second_team, army1Size, army2Size)) break;
        fight(first_team, second_team, army1Size, army2Size, battlefield);
        if (is_game_over(first_team, second_team, army1Size, army2Size)) break;
        fight(second_team, first_team, army2Size, army1Size, battlefield);
    }

    show_winner_screen(first_team, second_team, army1Size, army2Size);
}



void init_colors(void) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
}

WindowDimensions calculate_window_dimensions(int items_count) {
    WindowDimensions dim;

    dim.height = items_count + 4;
    dim.width = 24;

    dim.x = (COLS - dim.width) / 2;
    dim.y = (LINES - dim.height) / 2;

    return dim;
}

void draw_menu(WINDOW *menu_win, MenuItem *items, int items_count, const char *title) {
    int maxx, maxy;

    getmaxyx(menu_win, maxy, maxx);
    maxy = maxy + 0;

    box(menu_win, 0, 0);

    // Draw a headline
    mvwprintw(menu_win, 1, (maxx - strlen(title)) / 2, "%s", title);

    // Drawing menu items
    for (int i = 0; i < items_count; i++) {
        int y_pos = i + 3;
        int x_pos = (maxx - strlen(items[i].label)) / 2;

        if (items[i].selected) {
            wattron(menu_win, COLOR_PAIR(2));
            mvwprintw(menu_win, y_pos, x_pos, "%s", items[i].label);
            wattroff(menu_win, COLOR_PAIR(2));
        } else {
            wattron(menu_win, COLOR_PAIR(1));
            mvwprintw(menu_win, y_pos, x_pos, "%s", items[i].label);
            wattroff(menu_win, COLOR_PAIR(1));
        }
    }

    wrefresh(menu_win);
}

int main_menu(void) {
    int choice = -1;
    int selected = 0;
    int ch;
    const int NUM_ITEMS = 2;

    MenuItem items[NUM_ITEMS] = {
        {"New game", 0},
        {"Exit", 0}
    };

    WindowDimensions dim = calculate_window_dimensions(NUM_ITEMS);
    WINDOW *menu_win = newwin(dim.height, dim.width, dim.y, dim.x);

    keypad(menu_win, TRUE);
    curs_set(0); // Hide cursor

    items[selected].selected = 1;
    print_shrek();
    while (1) {
        // Draw menu on top
        draw_menu(menu_win, items, NUM_ITEMS, "Medieval War");

        ch = wgetch(menu_win);

        switch (ch) {
        case KEY_UP:
            items[selected].selected = 0;
            selected--;
            if (selected < 0) selected = NUM_ITEMS - 1;
            items[selected].selected = 1;
            break;
        case KEY_DOWN:
            items[selected].selected = 0;
            selected = (selected + 1) % NUM_ITEMS;
            items[selected].selected = 1;
            break;
        case '\n':
            choice = selected;
            break;
        default:
            break;
        }

        if (choice != -1) break;
    }

    delwin(menu_win);
    return choice;
}


int new_game_menu(void) {
    int choice = -1;
    int selected = 0;
    int ch;
    const int NUM_ITEMS = 4;

    MenuItem items[NUM_ITEMS] = {
        {"AI vs AI", 0},
        {"Player vs AI", 0},
        {"Player vs Player", 0},
        {"Exit", 0}
    };

    WindowDimensions dim = calculate_window_dimensions(NUM_ITEMS);
    WINDOW *menu_win = newwin(dim.height, dim.width, dim.y, dim.x);

    keypad(menu_win, TRUE);

    items[selected].selected = 1;

    while (1) {
        draw_menu(menu_win, items, NUM_ITEMS, "New Game");
        ch = wgetch(menu_win);

        switch (ch) {
        case KEY_UP:
            items[selected].selected = 0;
            selected--;
            if (selected < 0) selected = NUM_ITEMS - 1;
            items[selected].selected = 1;
            break;
        case KEY_DOWN:
            items[selected].selected = 0;
            selected = (selected + 1) % NUM_ITEMS;
            items[selected].selected = 1;
            break;
        case '\n':
            choice = selected;
            break;
        default:
            break;
        }

        if (choice != -1) break;
    }

    delwin(menu_win);
    return choice;
}



void place_army_on_field(UNIT* field[FIELD_ROWS][FIELD_COLS], UNIT* army, int armySize, int rowStart, int rowEnd) {
    int placed = 0;

    while (placed < armySize) {
        int row = rand() % (rowEnd - rowStart + 1) + rowStart;
        int col = rand() % FIELD_COLS;

        if (field[row][col] == NULL) {
            field[row][col] = &army[placed];
            placed++;
        }
    }
}

void print_battlefield(UNIT* field[FIELD_ROWS][FIELD_COLS]) {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    int cell_width = 6;
    int cell_height = 3;
    int offset_y = 2;
    int offset_x = 4;

    clear();


    // Print the numbering of columns
    for (int col = 0; col < FIELD_COLS; col++) {
        int col_x = offset_x + col * cell_width + cell_width / 2 - 1;
        mvprintw(offset_y - 2, col_x, "%d", col);
    }

    // Display the contents of the field
    for (int i = 0; i < FIELD_ROWS; i++) {
        // Numbering of lines
        mvprintw(offset_y + i * cell_height + cell_height / 2, offset_x - 3, "%d", i);

        for (int j = 0; j < FIELD_COLS; j++) {
            int y = offset_y + i * cell_height;
            int x = offset_x + j * cell_width;

            // Draw a dot in the center of the cell or the initials of the unit
            if (field[i][j]) {
                const char* name = field[i][j]->name;
                int color = (i < 2) ? 1 : 2;

                attron(COLOR_PAIR(color));
                mvprintw(y + cell_height / 2, x + cell_width / 2 - 1, "%.3s", name);
                attroff(COLOR_PAIR(color));
            } else {
                attron(COLOR_PAIR(3));
                mvaddch(y + cell_height / 2, x + cell_width / 2, '.');
                attroff(COLOR_PAIR(3));
            }
        }
    }

    // Draw an outer frame
    int total_width = FIELD_COLS * cell_width;
    int total_height = FIELD_ROWS * cell_height;
    int top = offset_y - 1;
    int left = offset_x - 1;
    int bottom = top + total_height;
    int right = left + total_width;

    // Horizontal lines
    for (int x = left; x <= right; x++) {
        mvaddch(top, x, '-');
        mvaddch(bottom, x, '-');
    }

    // Vertical lines
    for (int y = top; y <= bottom; y++) {
        mvaddch(y, left, '|');
        mvaddch(y, right, '|');
    }

    // Angles
    mvaddch(top, left, '+');
    mvaddch(top, right, '+');
    mvaddch(bottom, left, '+');
    mvaddch(bottom, right, '+');

    refresh();
    getch();
}

void print_army(UNIT army1[], UNIT army2[], int army1Size, int army2Size) {
    // Initialize colors
    init_pair(1, COLOR_RED, COLOR_BLACK);      // For army names
    init_pair(2, COLOR_CYAN, COLOR_BLACK);     // For unit names
    init_pair(3, COLOR_GREEN, COLOR_BLACK);    // For HP
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);   // For items
    init_pair(5, COLOR_WHITE, COLOR_BLACK);    // Normal text

    clear();

    int max_rows = LINES;
    int mid_col = COLS / 2;
    int current_line = 0;

    // Add continue prompt at bottom
    attron(COLOR_PAIR(5) | A_DIM);
    mvprintw(LINES - 1, 0, "Press any key to continue...");
    attroff(COLOR_PAIR(5) | A_DIM);

    // Print Army 1 in left half
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(current_line++, 0, "Army 1");
    attroff(COLOR_PAIR(1) | A_BOLD);

    for (int i = 0; i < army1Size && current_line < max_rows - 1; i++) {
        attron(COLOR_PAIR(5));
        mvprintw(current_line++, 0, "    Unit: %d", i);
        attroff(COLOR_PAIR(5));

        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(current_line++, 0, "    Name: %s ", army1[i].name);
        attroff(COLOR_PAIR(2) | A_BOLD);

        attron(COLOR_PAIR(3));
        printw("(%d)", army2[i].hp);
        attroff(COLOR_PAIR(3));

        attron(COLOR_PAIR(4));
        mvprintw(current_line++, 0, "    Item 1: %s,%d,%d,%d,%d,%d",
               army1[i].item1->name,
               army1[i].item1->att,
               army1[i].item1->def,
               army1[i].item1->slots,
               army1[i].item1->range,
               army1[i].item1->radius);
        attroff(COLOR_PAIR(4));

        if (army1[i].item2 != NULL) {
            attron(COLOR_PAIR(4));
            mvprintw(current_line++, 0, "    Item 2: %s,%d,%d,%d,%d,%d",
                   army1[i].item2->name,
                   army1[i].item2->att,
                   army1[i].item2->def,
                   army1[i].item2->slots,
                   army1[i].item2->range,
                   army1[i].item2->radius);
            attroff(COLOR_PAIR(4));
        }

        current_line++; // Add empty line between units
    }

    // Print Army 2 in right half
    current_line = 0;
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(current_line++, mid_col, "Army 2");
    attroff(COLOR_PAIR(1) | A_BOLD);

    for (int i = 0; i < army2Size && current_line < max_rows - 1; i++) {
        attron(COLOR_PAIR(5));
        mvprintw(current_line++, mid_col, "    Unit: %d", i);
        attroff(COLOR_PAIR(5));

        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(current_line++, mid_col, "    Name: %s ", army2[i].name);
        attroff(COLOR_PAIR(2) | A_BOLD);

        attron(COLOR_PAIR(3));
        printw("(%d)", army2[i].hp);
        attroff(COLOR_PAIR(3));

        attron(COLOR_PAIR(4));
        mvprintw(current_line++, mid_col, "    Item 1: %s,%d,%d,%d,%d,%d",
               army2[i].item1->name,
               army2[i].item1->att,
               army2[i].item1->def,
               army2[i].item1->slots,
               army2[i].item1->range,
               army2[i].item1->radius);
        attroff(COLOR_PAIR(4));

        if (army2[i].item2 != NULL) {
            attron(COLOR_PAIR(4));
            mvprintw(current_line++, mid_col, "    Item 2: %s,%d,%d,%d,%d,%d",
                   army2[i].item2->name,
                   army2[i].item2->att,
                   army2[i].item2->def,
                   army2[i].item2->slots,
                   army2[i].item2->range,
                   army2[i].item2->radius);
            attroff(COLOR_PAIR(4));
        }

        current_line++; // Add empty line between units
    }
    getch();
    clear();
    refresh();
}

void show_winner_screen(UNIT army1[], UNIT army2[], int army1Size, int army2Size) {
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    clear();
    print_shrek();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    bool army1Dead = true;
    bool army2Dead = true;

    for (int i = 0; i < army1Size; i++)
        if (army1[i].hp > 0) {army1Dead = false; break;}

    for (int i = 0; i < army2Size; i++)
        if (army2[i].hp > 0) {army2Dead = false; break;}

    attron(COLOR_PAIR(2));
    if (army2Dead)
    {
        mvprintw(max_y / 2 -1, (max_x - 10) / 2, "           ");
        mvprintw(max_y / 2, (max_x - 10) / 2, " WINNER: 1 ");
        mvprintw(max_y / 2 +1, (max_x - 10) / 2, "           ");
    }
    else if (army1Dead)
    {
        mvprintw(max_y / 2 -1, (max_x - 10) / 2, "           ");
        mvprintw(max_y / 2, (max_x - 10) / 2, " WINNER: 2 ");
        mvprintw(max_y / 2 +1, (max_x - 10) / 2, "           ");
    }
    attroff(COLOR_PAIR(2));


    refresh();
    getch();
}



void create_random_army(UNIT *emptyArray, int num_of_units, bool *used_flags)
{
    int filled = 0;
    while (filled < num_of_units) {
        int idx = rand() % NUMBER_OF_UNITS;
        // Control already used units
        if (!used_flags[idx]) {
            emptyArray[filled] = units[idx];
            used_flags[idx] = true;
            filled++;
        }
    }
}

int unit_selection_menu(UNIT* emptyArray, bool* unitAvailability, bool *used_flags) {
    init_pair(1, COLOR_WHITE, COLOR_BLACK);   // Normal text
    init_pair(2, COLOR_BLACK, COLOR_WHITE);   // Selected item
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   // Selected unit
    init_pair(4, COLOR_CYAN, COLOR_BLACK);    // Description
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);  // Items


    int selected_count = 0;
    int current_selection = 0;
    bool done = false;

    // Create an array for the menu
    MENU_UNIT menu_units[NUMBER_OF_UNITS];
    int available_count = 0;

    // Fill the menu with available units only
    for (int i = 0; i < NUMBER_OF_UNITS; i++) {
        menu_units[i].unit = &units[i];
        menu_units[i].selected = false;
        menu_units[i].available = unitAvailability[i];

        if (unitAvailability[i]) {
            available_count++;
        }
    }

    while (!done && available_count > 0) {
        clear();

        // Title
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(0, 0, "Select units (%d/%d):", selected_count, MAX_ARMY);
        attroff(COLOR_PAIR(1) | A_BOLD);

        // Output all available units in two columns
        int col1_x = 2;
        int col2_x = COLS/2;
        int current_line = 2;

        for (int i = 0; i < NUMBER_OF_UNITS; i++) {
            if (!menu_units[i].available) continue;

            int x_pos = (i % 2 == 0) ? col1_x : col2_x;
            int y_pos = current_line + (i / 2);

            // Select the current item
            if (i == current_selection) {
                attron(COLOR_PAIR(2));
            }
            // Select selected units
            else if (menu_units[i].selected) {
                attron(COLOR_PAIR(3));
            }

            // Output unit name
            mvprintw(y_pos, x_pos+4, "%s", menu_units[i].unit->name);

            // Reset color
            if (i == current_selection || menu_units[i].selected) {
                attroff(COLOR_PAIR(i == current_selection ? 2 : 3));
            }

            // Choice Marker
            if (menu_units[i].selected) {
                mvprintw(y_pos, x_pos, "[X]");
            } else {
                mvprintw(y_pos, x_pos, "[ ]");
            }
        }

        // Display information about the current unit (right side of the screen)
        UNIT* current_unit = (UNIT*)menu_units[current_selection].unit;
        int info_x = COLS/2 + 20;

        // Description
        attron(COLOR_PAIR(4));
        mvprintw(2, info_x, "Description:");
        mvprintw(3, info_x, "%.*s", COLS-info_x-1, current_unit->description);
        attroff(COLOR_PAIR(4));

        attron(COLOR_PAIR(3));
        mvprintw(5, info_x, "HP: %d", current_unit->hp);
        attroff(COLOR_PAIR(3));


        // Item 1
        attron(COLOR_PAIR(5));
        mvprintw(7, info_x, "Item 1: %s", current_unit->item1->name);
        mvprintw(8, info_x, "  Attack: %d", current_unit->item1->att);
        mvprintw(9, info_x, "  Defense: %d", current_unit->item1->def);
        mvprintw(10, info_x, "  Range: %d", current_unit->item1->range);
        mvprintw(11, info_x, "  Radius: %d", current_unit->item1->radius);
        attroff(COLOR_PAIR(5));

        // Item 2
        if (current_unit->item2) {
            attron(COLOR_PAIR(5));
            mvprintw(13, info_x, "Item 2: %s", current_unit->item2->name);
            mvprintw(14, info_x, "  Attack: %d", current_unit->item2->att);
            mvprintw(15, info_x, "  Defense: %d", current_unit->item2->def);
            mvprintw(16, info_x, "  Range: %d", current_unit->item2->range);
            mvprintw(17, info_x, "  Radius: %d", current_unit->item2->radius);
            attroff(COLOR_PAIR(5));
        }

        // Done button (if at least one unit is selected)
        if (selected_count > 0) {
            attron(COLOR_PAIR(1) | A_BOLD);
            mvprintw(LINES - 2, 2, "Done - press D");
            attroff(COLOR_PAIR(1) | A_BOLD);
        }

        // Control hints
        attron(COLOR_PAIR(1) | A_DIM);
        mvprintw(LINES - 1, 2, "Arrows: Navigate | Enter: Select/Deselect | D: Done");
        attroff(COLOR_PAIR(1) | A_DIM);

        refresh();

        // Input processing
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                do {
                    current_selection -= 2;
                    if (current_selection < 0) {
                        current_selection = ((available_count + 1) / 2) * 2 - (current_selection % 2 == 0 ? 2 : 1);
                        if (current_selection >= NUMBER_OF_UNITS) current_selection = NUMBER_OF_UNITS - 1;
                    }
                } while (!menu_units[current_selection].available);
                break;

            case KEY_DOWN:
                do {
                    current_selection += 2;
                    if (current_selection >= NUMBER_OF_UNITS) {
                        current_selection = current_selection % 2;
                    }
                } while (!menu_units[current_selection].available);
                break;

            case KEY_LEFT:
                do {
                    current_selection--;
                    if (current_selection < 0) {
                        current_selection = NUMBER_OF_UNITS - 1;
                    }
                } while (!menu_units[current_selection].available);
                break;

            case KEY_RIGHT:
                do {
                    current_selection++;
                    if (current_selection >= NUMBER_OF_UNITS) {
                        current_selection = 0;
                    }
                } while (!menu_units[current_selection].available);
                break;

            case '\n': // Enter
                if (menu_units[current_selection].available) {
                    if (!menu_units[current_selection].selected && selected_count < MAX_ARMY) {
                        menu_units[current_selection].selected = true;
                        selected_count++;
                    } else if (menu_units[current_selection].selected) {
                        menu_units[current_selection].selected = false;
                        selected_count--;
                    }

                    // Autocomplete when maximum is reached
                    if (selected_count == MAX_ARMY) {
                        done = true;
                    }
                }
                break;

            case 'd':
            case 'D':
                if (selected_count > 0) {
                    done = true;
                }
                break;
        }
    }



    // Fill the array with selected units and update availability
    if (selected_count > 0) {
        int empty_idx = 0;
        for (int i = 0; i < NUMBER_OF_UNITS; i++) {
            if (menu_units[i].selected) {
                attroff(COLOR_PAIR(1));
                emptyArray[empty_idx++] = *menu_units[i].unit;
                unitAvailability[i] = false;
                used_flags[i] = true;
            }
        }
    }
    return selected_count;
}


bool is_attacking_unit(UNIT* unit, UNIT AttackingArmy[], int AttackingArmySize) {
    for (int i = 0; i < AttackingArmySize; i++) {
        if (strcmp(AttackingArmy[i].name, unit->name) == 0) {
            return true;
        }
    }
    return false;
}

bool is_underattack_unit(UNIT* unit, UNIT UnderAttackArmy[], int UnderAttackArmySize) {
    for (int i = 0; i < UnderAttackArmySize; i++) {
        if (strcmp(UnderAttackArmy[i].name, unit->name) == 0) {
            return true;
        }
    }
    return false;
}

void fight(UNIT AttackingArmy[], UNIT UnderAttackArmy[], int AttackingArmySize, int UnderAttackArmySize, UNIT* field[FIELD_ROWS][FIELD_COLS]) {
    init_pair(1, COLOR_RED, COLOR_BLACK);      // UnderAttackArmy
    init_pair(2, COLOR_GREEN, COLOR_BLACK);    // AttackingArmy
    init_pair(3, COLOR_WHITE, COLOR_BLACK);    // Empty cell
    init_pair(4, COLOR_BLACK, COLOR_WHITE);    // Selected cell (highlight)
    init_pair(5, COLOR_BLACK, COLOR_GREEN);    // Selected AttackingArmy unit
    init_pair(6, COLOR_BLUE, COLOR_BLACK);     // Cells available for relocation
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);   // UnderAttackArmy
    init_pair(8, COLOR_CYAN, COLOR_BLACK);

    int cell_width = 6;
    int cell_height = 3;
    int offset_y = 2;
    int offset_x = 4;
    int info_panel_x = offset_x + FIELD_COLS * cell_width + 10;
    int selected_row = -1, selected_col = -1;
    int cursor_row = 0, cursor_col = 0;
    bool unit_selected = false;
    int max_radius = 0;


    while (1) {
        clear();
        // Output column numbers
        for (int col = 0; col < FIELD_COLS; col++) {
            int col_x = offset_x + col * cell_width + cell_width / 2 - 1;
            mvprintw(offset_y - 2, col_x, "%d", col);
        }

        // Output field contents
        for (int i = 0; i < FIELD_ROWS; i++) {
            // Line numbering
            mvprintw(offset_y + i * cell_height + cell_height / 2, offset_x - 3, "%d", i);

            for (int j = 0; j < FIELD_COLS; j++) {
                int y = offset_y + i * cell_height;
                int x = offset_x + j * cell_width;
                bool in_range = false;
                bool can_attack = false;

                // Check if the cell is within the radius of the selected unit
                if (unit_selected && selected_row != -1 && selected_col != -1) {
                    int distance = abs(i - selected_row) + abs(j - selected_col);
                    if (distance <= max_radius) {
                        in_range = true;
                        if (field[i][j] && is_underattack_unit(field[i][j], UnderAttackArmy, UnderAttackArmySize)) {
                            can_attack = true;
                        }
                    }
                }

                // Determine the color of the cell
                if (i == cursor_row && j == cursor_col) {
                    if (field[i][j] && field[i][j]->hp > 0) {
                        if (is_attacking_unit(field[i][j], AttackingArmy, AttackingArmySize)) {
                            attron(COLOR_PAIR(5));
                        } else {
                            attron(COLOR_PAIR(4));
                        }
                    } else {
                        attron(COLOR_PAIR(4));
                    }
                } else if (in_range) {
                    if (can_attack) {
                        // Light up enemy units that can be attacked in yellow.
                        attron(COLOR_PAIR(7));
                    } else if (!field[i][j]) {
                        // Light the empty cells in blue.
                        attron(COLOR_PAIR(6));
                    } else if (is_attacking_unit(field[i][j], AttackingArmy, AttackingArmySize)) {
                        // Your units remain green even if they are in the radius of the
                        attron(COLOR_PAIR(2));
                    } else {
                        // Enemy units that cannot be attacked remain red
                        attron(COLOR_PAIR(1));
                    }
                } else if (field[i][j] && field[i][j]->hp > 0) {
                    if (is_attacking_unit(field[i][j], AttackingArmy, AttackingArmySize)) {
                        attron(COLOR_PAIR(2));
                    } else {
                        attron(COLOR_PAIR(1));
                    }
                } else {
                    attron(COLOR_PAIR(3));
                }

                // Draw the contents of the cell
                if (field[i][j] && field[i][j]->hp > 0) {
                    mvprintw(y + cell_height / 2, x + cell_width / 2 - 1, "%.3s", field[i][j]->name);
                } else {
                    mvaddch(y + cell_height / 2, x + cell_width / 2, '.');
                }

                attroff(COLOR_PAIR(1));
                attroff(COLOR_PAIR(2));
                attroff(COLOR_PAIR(3));
                attroff(COLOR_PAIR(4));
                attroff(COLOR_PAIR(5));
                attroff(COLOR_PAIR(6));
                attroff(COLOR_PAIR(7));
            }
        }

        // Draw field frame
        int total_width = FIELD_COLS * cell_width;
        int total_height = FIELD_ROWS * cell_height;
        int top = offset_y - 1;
        int left = offset_x - 1;
        int bottom = top + total_height;
        int right = left + total_width;

        // Horizontal lines
        for (int x = left; x <= right; x++) {
            mvaddch(top, x, '-');
            mvaddch(bottom, x, '-');
        }

        // Vertical lines
        for (int y = top; y <= bottom; y++) {
            mvaddch(y, left, '|');
            mvaddch(y, right, '|');
        }

        // Corners
        mvaddch(top, left, '+');
        mvaddch(top, right, '+');
        mvaddch(bottom, left, '+');
        mvaddch(bottom, right, '+');

        // Display unit info
        if (field[cursor_row][cursor_col] && field[cursor_row][cursor_col]->hp > 0) {
            UNIT* unit = field[cursor_row][cursor_col];

            attron(COLOR_PAIR(3));
            mvprintw(offset_y, info_panel_x, "Name: %s", unit->name);
            attron(COLOR_PAIR(2));
            mvprintw(offset_y + 2, info_panel_x, "HP: %d", unit->hp);
            attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(8));
            mvprintw(offset_y + 4, info_panel_x, "Description:");

            // Print description with wrapping
            const char* desc = unit->description;
            int desc_line = 0;
            int max_line_length = COLS - info_panel_x - 5;

            while (*desc && desc_line < 5) {
                int line_length = (int)strlen(desc) > max_line_length ? max_line_length : strlen(desc);
                char line[256];
                strncpy(line, desc, line_length);
                line[line_length] = '\0';
                mvprintw(offset_y + 6 + desc_line, info_panel_x, "%s", line);
                desc += line_length;
                desc_line++;
            }
            attroff(COLOR_PAIR(8));

            attron(COLOR_PAIR(7));
            mvprintw(offset_y + 11, info_panel_x, "Item 1: %s", unit->item1->name);
            mvprintw(offset_y + 12, info_panel_x, "  Attack: %d", unit->item1->att);
            mvprintw(offset_y + 13, info_panel_x, "  Defense: %d", unit->item1->def);

            if (unit->item2) {
                mvprintw(offset_y + 16, info_panel_x, "Item 2: %s", unit->item2->name);
                mvprintw(offset_y + 17, info_panel_x, "  Attack: %d", unit->item2->att);
                mvprintw(offset_y + 18, info_panel_x, "  Defense: %d", unit->item2->def);
            }
            attroff(COLOR_PAIR(7));
            attroff(COLOR_PAIR(3));
        }

        // Action messages
        if (unit_selected && selected_row != -1 && selected_col != -1) {
            attron(COLOR_PAIR(3));
            mvprintw(bottom + 2, offset_x, "Selected unit: %s (HP: %d)",
                   field[selected_row][selected_col]->name,
                   field[selected_row][selected_col]->hp);
            mvprintw(bottom + 3, offset_x, "Move radius: %d", max_radius);
            attroff(COLOR_PAIR(3));
        }

        refresh();

        // Input handling
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (unit_selected) {
                    // Check if we can move to this cell
                    int new_row = cursor_row - 1;
                    if (new_row >= 0 && abs(new_row - selected_row) + abs(cursor_col - selected_col) <= max_radius) {
                        cursor_row = new_row;
                    }
                } else {
                    cursor_row = (cursor_row - 1 + FIELD_ROWS) % FIELD_ROWS;
                }
                break;
            case KEY_DOWN:
                if (unit_selected) {
                    int new_row = cursor_row + 1;
                    if (new_row < FIELD_ROWS && abs(new_row - selected_row) + abs(cursor_col - selected_col) <= max_radius) {
                        cursor_row = new_row;
                    }
                } else {
                    cursor_row = (cursor_row + 1) % FIELD_ROWS;
                }
                break;
            case KEY_LEFT:
                if (unit_selected) {
                    int new_col = cursor_col - 1;
                    if (new_col >= 0 && abs(cursor_row - selected_row) + abs(new_col - selected_col) <= max_radius) {
                        cursor_col = new_col;
                    }
                } else {
                    cursor_col = (cursor_col - 1 + FIELD_COLS) % FIELD_COLS;
                }
                break;
            case KEY_RIGHT:
                if (unit_selected) {
                    int new_col = cursor_col + 1;
                    if (new_col < FIELD_COLS && abs(cursor_row - selected_row) + abs(new_col - selected_col) <= max_radius) {
                        cursor_col = new_col;
                    }
                } else {
                    cursor_col = (cursor_col + 1) % FIELD_COLS;
                }
                break;
            case '\n': // Enter
                if (!unit_selected) {
                    // Select only AttackingArmy units
                    if (field[cursor_row][cursor_col] && is_attacking_unit(field[cursor_row][cursor_col], AttackingArmy, AttackingArmySize)) {
                        selected_row = cursor_row;
                        selected_col = cursor_col;
                        unit_selected = true;

                        // Calculate max radius
                        max_radius = field[cursor_row][cursor_col]->item1->radius + 1;
                        if (field[cursor_row][cursor_col]->item2) {
                            int item2_radius = field[cursor_row][cursor_col]->item2->radius + 1;
                            if (item2_radius > max_radius) {
                                max_radius = item2_radius;
                            }
                        }
                    }
                } else {
                    if (cursor_row == selected_row && cursor_col == selected_col) {
                        // Deselect
                        unit_selected = false;
                        selected_row = -1;
                        selected_col = -1;
                        max_radius = 0;
                    } else if (abs(cursor_row - selected_row) + abs(cursor_col - selected_col) <= max_radius) {
                        if (!field[cursor_row][cursor_col]) {
                            // Move unit
                            field[cursor_row][cursor_col] = field[selected_row][selected_col];
                            field[selected_row][selected_col] = NULL;

                            // Update selection
                            selected_row = cursor_row;
                            selected_col = cursor_col;

                            // Print move message
                            attron(COLOR_PAIR(3));
                            mvprintw(bottom + 4, offset_x, "Unit moved to (%d, %d)", cursor_row, cursor_col);
                            attroff(COLOR_PAIR(3));
                            refresh();
                            getch();
                            return;
                        } else if (is_underattack_unit(field[cursor_row][cursor_col], UnderAttackArmy, UnderAttackArmySize)) {
                            // Attack enemy unit
                            UNIT* attacker = field[selected_row][selected_col];
                            UNIT* defender = field[cursor_row][cursor_col];

                            // Calculate damage
                            int damage = attacker->item1->att - defender->item1->def;
                            if (attacker->item2) damage += attacker->item2->att;
                            if (defender->item2) damage -= defender->item2->def;
                            if (damage < 1) damage = 1;

                            // Apply damage
                            defender->hp -= damage;

                            // Print attack message
                            attron(COLOR_PAIR(3));
                            mvprintw(bottom + 4, offset_x, "%s attacks %s",
                                   attacker->name, defender->name);
                            mvprintw(bottom + 5, offset_x, "for %d damage!", damage);
                            mvprintw(bottom + 6, offset_x, "%s HP: %d",
                                   defender->name, defender->hp > 0 ? defender->hp : 0);
                            attroff(COLOR_PAIR(3));
                            refresh();
                            getch();

                            // Remove dead unit
                            if (defender->hp <= 0) {
                                field[cursor_row][cursor_col] = NULL;
                            }
                            return;
                        }

                        // Reset selection
                        unit_selected = false;
                        selected_row = -1;
                        selected_col = -1;
                        max_radius = 0;
                    }
                }
                break;
            case 27: // ESC to skip a move
                if (unit_selected) {
                    unit_selected = false;
                    selected_row = -1;
                    selected_col = -1;
                    max_radius = 0;
                } else {
                    return;
                }
                break;
        }
    }
}

void autopilot(UNIT AttackingArmy[], UNIT UnderAttackArmy[], int AttackingArmySize, int UnderAttackArmySize, UNIT* field[FIELD_ROWS][FIELD_COLS], int sleep_second) {
    init_pair(1, COLOR_RED, COLOR_BLACK);      // UnderAttackArmy
    init_pair(2, COLOR_GREEN, COLOR_BLACK);    // AttackingArmy
    init_pair(3, COLOR_WHITE, COLOR_BLACK);    // Empty cell
    init_pair(4, COLOR_BLACK, COLOR_WHITE);    // Selected cell (highlight)
    init_pair(5, COLOR_BLACK, COLOR_GREEN);    // Selected AttackingArmy unit
    init_pair(6, COLOR_BLUE, COLOR_BLACK);     // Cells available for relocation
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);   // UnderAttackArmy
    init_pair(8, COLOR_CYAN, COLOR_BLACK);

    // Constants for rendering
    const int cell_width = 6;
    const int cell_height = 3;
    const int offset_y = 2;
    const int offset_x = 4;
    const int info_panel_x = offset_x + FIELD_COLS * cell_width + 10;
    const int delay_time = sleep_second;

    // Variables for autopilot operation
    int unit_positions[FIELD_ROWS * FIELD_COLS][2];
    int cell_positions[FIELD_ROWS * FIELD_COLS][2];

    int distance;
    UNIT* attacker;
    UNIT* defender;

    char line[256];

    // Find all units available for a move
    int available_units = 0;
    for (int i = 0; i < FIELD_ROWS; i++) {
        for (int j = 0; j < FIELD_COLS; j++) {
            if (field[i][j] && is_attacking_unit(field[i][j], AttackingArmy, AttackingArmySize)) {
                unit_positions[available_units][0] = i;
                unit_positions[available_units][1] = j;
                available_units++;
            }
        }
    }

    if (available_units == 0) return; // No units to move

    // Select a random unit
    int selected_row, selected_col;
    int random_unit = rand() % available_units;

    selected_row = unit_positions[random_unit][0];
    selected_col = unit_positions[random_unit][1];

    int cursor_row = 0, cursor_col = 0;
    cursor_row = selected_row;
    cursor_col = selected_col;

    bool unit_selected = false;
    unit_selected = true;

    // Calculate the travel radius
    int max_radius = field[selected_row][selected_col]->item1->radius + 1;
    if (field[selected_row][selected_col]->item2) {
        int item2_radius = field[selected_row][selected_col]->item2->radius + 1;
        if (item2_radius > max_radius) {
            max_radius = item2_radius;
        }
    }

    // Drawing of the field with the selected unit
    clear();

    // Column numbers
    for (int col = 0; col < FIELD_COLS; col++) {
        int col_x = offset_x + col * cell_width + cell_width / 2 - 1;
        mvprintw(offset_y - 2, col_x, "%d", col);
    }

    // Field contents
    for (int i = 0; i < FIELD_ROWS; i++) {
        // Field contents
        mvprintw(offset_y + i * cell_height + cell_height / 2, offset_x - 3, "%d", i);

        for (int j = 0; j < FIELD_COLS; j++) {
            int y = offset_y + i * cell_height;
            int x = offset_x + j * cell_width;
            bool in_range = false;
            bool can_attack = false;

            // Check the range of the selected unit
            if (unit_selected && selected_row != -1 && selected_col != -1) {
                distance = abs(i - selected_row) + abs(j - selected_col);
                if (distance <= max_radius) {
                    in_range = true;
                    if (field[i][j] && is_underattack_unit(field[i][j], UnderAttackArmy, UnderAttackArmySize)) {
                        can_attack = true;
                    }
                }
            }

            // Determine the color of the cell
                if (i == cursor_row && j == cursor_col) {
                    if (field[i][j] && field[i][j]->hp > 0) {
                        if (is_attacking_unit(field[i][j], AttackingArmy, AttackingArmySize)) {
                            attron(COLOR_PAIR(5));
                        } else {
                            attron(COLOR_PAIR(4));
                        }
                    } else {
                        attron(COLOR_PAIR(4));
                    }
                } else if (in_range) {
                    if (can_attack)
                    {
                        attron(COLOR_PAIR(7));
                    }
                    else if (!field[i][j]) {
                        // Light the empty cells in blue.
                        attron(COLOR_PAIR(6));
                    } else if (is_attacking_unit(field[i][j], AttackingArmy, AttackingArmySize)) {
                        // Your units remain green even if they are within the radius of the
                        attron(COLOR_PAIR(2));
                    } else {
                        // Enemy units that cannot be attacked remain red
                        attron(COLOR_PAIR(1));
                    }
                } else if (field[i][j] && field[i][j]->hp > 0) {
                    if (is_attacking_unit(field[i][j], AttackingArmy, AttackingArmySize)) {
                        attron(COLOR_PAIR(2));
                    } else {
                        attron(COLOR_PAIR(1));
                    }
                } else {
                    attron(COLOR_PAIR(3));
                }

                // Draw the contents of the cell
                if (field[i][j] && field[i][j]->hp > 0) {
                    mvprintw(y + cell_height / 2, x + cell_width / 2 - 1, "%.3s", field[i][j]->name);
                } else {
                    mvaddch(y + cell_height / 2, x + cell_width / 2, '.');
                }

                attroff(COLOR_PAIR(1));
                attroff(COLOR_PAIR(2));
                attroff(COLOR_PAIR(3));
                attroff(COLOR_PAIR(4));
                attroff(COLOR_PAIR(5));
                attroff(COLOR_PAIR(6));
                attroff(COLOR_PAIR(7));
        }
    }

    // Field frame
    int total_width = FIELD_COLS * cell_width;
    int total_height = FIELD_ROWS * cell_height;
    int top = offset_y - 1;
    int left = offset_x - 1;
    int bottom = top + total_height;
    int right = left + total_width;

    // Horizontal lines
    for (int x = left; x <= right; x++) {
        mvaddch(top, x, '-');
        mvaddch(bottom, x, '-');
    }

    // Vertical lines
    for (int y = top; y <= bottom; y++) {
        mvaddch(y, left, '|');
        mvaddch(y, right, '|');
    }

    // Angles
    mvaddch(top, left, '+');
    mvaddch(top, right, '+');
    mvaddch(bottom, left, '+');
    mvaddch(bottom, right, '+');

    // Information about the unit
    if (field[cursor_row][cursor_col] && field[cursor_row][cursor_col]->hp > 0) {
        UNIT* unit = field[cursor_row][cursor_col];

        attron(COLOR_PAIR(3));
        mvprintw(offset_y, info_panel_x, "Name: %s", unit->name);
        attron(COLOR_PAIR(2));
        mvprintw(offset_y + 2, info_panel_x, "HP: %d", unit->hp);
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(8));
        mvprintw(offset_y + 4, info_panel_x, "Description:");

        // Description with hyphenation
        const char* desc = unit->description;
        int desc_line = 0;
        int max_line_length = COLS - info_panel_x - 5;
        int line_length;
        while (*desc && desc_line < 5) {
            line_length = (int)strlen(desc) > max_line_length ? max_line_length : strlen(desc);
            strncpy(line, desc, line_length);
            line[line_length] = '\0';
            mvprintw(offset_y + 6 + desc_line, info_panel_x, "%s", line);
            desc += line_length;
            desc_line++;
        }
        attroff(COLOR_PAIR(8));

        attron(COLOR_PAIR(7));
        mvprintw(offset_y + 11, info_panel_x, "Item 1: %s", unit->item1->name);
        mvprintw(offset_y + 12, info_panel_x, "  Attack: %d", unit->item1->att);
        mvprintw(offset_y + 13, info_panel_x, "  Defense: %d", unit->item1->def);

        if (unit->item2) {
            mvprintw(offset_y + 16, info_panel_x, "Item 2: %s", unit->item2->name);
            mvprintw(offset_y + 17, info_panel_x, "  Attack: %d", unit->item2->att);
            mvprintw(offset_y + 18, info_panel_x, "  Defense: %d", unit->item2->def);
        }
        attroff(COLOR_PAIR(7));
        attroff(COLOR_PAIR(3));
    }

    // Action messages
    if (unit_selected && selected_row != -1 && selected_col != -1) {
        attron(COLOR_PAIR(3));
        mvprintw(bottom + 2, offset_x, "Selected unit: %s (HP: %d)",
               field[selected_row][selected_col]->name,
               field[selected_row][selected_col]->hp);
        mvprintw(bottom + 3, offset_x, "Move radius: %d", max_radius);
        attroff(COLOR_PAIR(3));
    }

    refresh();
    usleep(delay_time);

    // Look for enemy units in the attack radius
    bool enemy_found = false;
    int enemy_row = -1, enemy_col = -1;
    for (int i = 0; i < FIELD_ROWS && !enemy_found; i++) {
        for (int j = 0; j < FIELD_COLS && !enemy_found; j++) {
            if (field[i][j] && is_underattack_unit(field[i][j], UnderAttackArmy, UnderAttackArmySize)) {
                distance = abs(i - selected_row) + abs(j - selected_col);
                if (distance <= max_radius) {
                    enemy_found = true;
                    enemy_row = i;
                    enemy_col = j;
                }
            }
        }
    }

    if (enemy_found) {
        cursor_row = enemy_row;
        cursor_col = enemy_col;


        refresh();
        usleep(delay_time);

        // Attack
        attacker = field[selected_row][selected_col];
        defender = field[enemy_row][enemy_col];

        // Calculate the damage
        int damage = attacker->item1->att - defender->item1->def;
        if (attacker->item2) damage += attacker->item2->att;
        if (defender->item2) damage -= defender->item2->def;
        if (damage < 1) damage = 1;


        defender->hp -= damage;

        // Attack message
        attron(COLOR_PAIR(3));
        mvprintw(bottom + 4, offset_x, "%s attacks %s",
                                   attacker->name, defender->name);
        mvprintw(bottom + 5, offset_x, "for %d damage!", damage);
        mvprintw(bottom + 6, offset_x, "%s HP: %d",
               defender->name, defender->hp > 0 ? defender->hp : 0);
        attroff(COLOR_PAIR(3));
        refresh();

        // Remove the dead unit
        if (defender->hp <= 0) field[enemy_row][enemy_col] = NULL;
    } else {
        // Looking for available cells to move
        int available_cells = 0;
        for (int i = 0; i < FIELD_ROWS; i++) {
            for (int j = 0; j < FIELD_COLS; j++) {
                distance = abs(i - selected_row) + abs(j - selected_col);
                if (distance <= max_radius && !field[i][j]) {
                    cell_positions[available_cells][0] = i;
                    cell_positions[available_cells][1] = j;
                    available_cells++;
                }
            }
        }

        if (available_cells > 0) {
            // Select a random cell
            int random_cell = rand() % available_cells;
            cursor_row = cell_positions[random_cell][0];
            cursor_col = cell_positions[random_cell][1];

            refresh();
            usleep(delay_time);

            // Move the unit
            field[cursor_row][cursor_col] = field[selected_row][selected_col];
            field[selected_row][selected_col] = NULL;

            // Move message
            attron(COLOR_PAIR(3));
            mvprintw(bottom + 4, offset_x, "Unit moved to (%d, %d)", cursor_row, cursor_col);
            attroff(COLOR_PAIR(3));
            refresh();
        }
    }
    if (delay_time >= 500000) getch();
    usleep(delay_time);

}


bool is_game_over(UNIT army1[], UNIT army2[], int army1Size, int army2Size)
{
    bool army1Dead = true;
    bool army2Dead = true;

    for (int i = 0; i < army1Size; i++)
        if (army1[i].hp > 0) {army1Dead = false; break;}

    for (int i = 0; i < army2Size; i++)
        if (army2[i].hp > 0) {army2Dead = false; break;}

    return army1Dead || army2Dead;
}

void print_shrek(void)
{
    // Print Shrek ASCII art in the background
    int shrek_y = (LINES - SHREK_LINES) / 4;
    int shrek_x = (COLS - strlen(SHREK[0])) / 2;

    attron(COLOR_PAIR(1));
    for (int i = 0; i < SHREK_LINES; i++) {
        mvprintw(shrek_y + i, shrek_x, "%s", SHREK[i]);
    }
    attroff(COLOR_PAIR(1));
    refresh();
}
