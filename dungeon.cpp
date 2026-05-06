#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <conio.h>
#include <algorithm>
#include <windows.h>

using namespace std;

const int MAP_ROWS = 6;
const int MAP_COLS = 10;

void showGameIntro() {
    cout << "================================================\n";
    cout << "          WELCOME TO DUNGEON OF SHADOWS         \n";
    cout << "================================================\n\n";
    cout << "  EXPLORATION RULES:\n";
    cout << "  - Move into '?' to reveal the room's identity.\n";
    cout << "  - Once revealed, symbols (G, T, X) STAY on the map.\n";
    cout << "  - G (Goblins): 4 enemies. | T (Treasure): 2 heals.\n";
    cout << "  - X (Exit): Your goal.\n\n";
    cout << "  Press any key to begin your adventure...\n";
    _getch(); 
}

struct Room {
    char type;
    bool visited;
    bool cleared;
    Room() : type('.'), visited(false), cleared(false) {}
};

class Player {
public:
    int row, col, hp;
    Player() : row(0), col(0), hp(100) {}
    
    void move(int dr, int dc) {
        row = max(0, min(MAP_ROWS - 1, row + dr));
        col = max(0, min(MAP_COLS - 1, col + dc));
    }

    void takeDamage(int dmg) { hp = max(0, hp - dmg); }
    void heal(int amt) { hp = min(100, hp + amt); }
    bool isAlive() { return hp > 0; }
};

void renderMap(Room map[MAP_ROWS][MAP_COLS], Player& p, const char* name) {
    system("cls");
    cout << "--- DUNGEON OF SHADOWS: DISCOVERY ---\n\n";
    for (int r = 0; r < MAP_ROWS; r++) {
        cout << "  ";
        for (int c = 0; c < MAP_COLS; c++) {
            // Priority 1: Show the Hero
            if (r == p.row && c == p.col) {
                cout << "@ ";
            } 
            // Priority 2: If not visited, keep it hidden
            else if (!map[r][c].visited) {
                cout << "? ";
            } 
            // Priority 3: If visited, ALWAYS show the type (G, T, X, or .)
            else {
                cout << map[r][c].type << " ";
            }
        }
        cout << "\n";
    }
    cout << "\n  HERO: " << name << " | HP: " << p.hp << "/100\n";
    cout << "  [W/A/S/D] Move | [Q] Quit\n";
}

int main() {
    srand(time(0));
    char pname[30];
    cout << "Enter your Hero's Name: ";
    cin >> pname;

    showGameIntro();

    Player hero;
    Room dungeon[MAP_ROWS][MAP_COLS];

    // 1. Place 4 Goblins (G)
    int gPlaced = 0;
    while (gPlaced < 4) {
        int r = rand() % MAP_ROWS;
        int c = rand() % MAP_COLS;
        if ((r != 0 || c != 0) && (r != MAP_ROWS-1 || c != MAP_COLS-1) && dungeon[r][c].type == '.') {
            dungeon[r][c].type = 'G';
            gPlaced++;
        }
    }

    // 2. Place 2 Treasures (T)
    int tPlaced = 0;
    while (tPlaced < 2) {
        int r = rand() % MAP_ROWS;
        int c = rand() % MAP_COLS;
        if ((r != 0 || c != 0) && dungeon[r][c].type == '.') {
            dungeon[r][c].type = 'T';
            tPlaced++;
        }
    }

    // 3. Place Exit (X)
    dungeon[MAP_ROWS-1][MAP_COLS-1].type = 'X';

    bool playing = true;
    while (playing && hero.isAlive()) {
        int r = hero.row;
        int c = hero.col;
        
        // Mark the room as visited as soon as we step on it
        dungeon[r][c].visited = true; 

        renderMap(dungeon, hero, pname);

        // Event Handling
        if (!dungeon[r][c].cleared) {
            if (dungeon[r][c].type == 'G') {
                int dmg = 15 + (rand() % 11);
                hero.takeDamage(dmg);
                renderMap(dungeon, hero, pname); // Refresh to show @ on G
                cout << "\n  !! AMBUSH !! You hit a Goblin! Lost " << dmg << " HP.\n";
                if (hero.isAlive()) {
                    cout << "  Press any key to move on.";
                    dungeon[r][c].cleared = true; // Prevents re-triggering damage
                    _getch();
                }
            }
            else if (dungeon[r][c].type == 'T') {
                hero.heal(30);
                renderMap(dungeon, hero, pname); // Refresh to show @ on T
                cout << "\n  ++ TREASURE ++ You found a Potion! +30 HP\n";
                cout << "  Press any key to continue.";
                dungeon[r][c].cleared = true;
                _getch();
            }
            else if (dungeon[r][c].type == 'X') {
                cout << "\n--------------------- VICTORY! You found the Exit!------------\n";
                playing = false;
                break;
            }
        }

        if (playing && hero.isAlive()) {
            int input = _getch();
            char cmd = toupper((char)input);
            if      (cmd == 'W') hero.move(-1, 0);
            else if (cmd == 'S') hero.move(1, 0);
            else if (cmd == 'A') hero.move(0, -1);
            else if (cmd == 'D') hero.move(0, 1);
            else if (cmd == 'Q') playing = false;
        }
    }

    if (!hero.isAlive()) {
        system("cls");
        cout << "GAME OVER. " << pname << " fell in the darkness.\n";
    }

    cout << "\nPress any key to exit.";
    _getch();
    return 0;
}