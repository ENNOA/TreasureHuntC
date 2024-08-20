#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Constants
const int GRID_SIZE = 10;
const char PLAYER_CHAR = 'P';
const char ENEMY_CHAR = 'E';
const char SEARCHED_CHAR = 'o';
const char EMPTY_CHAR = '.';

// Game state
std::vector<std::vector<char>> grid(GRID_SIZE, std::vector<char>(GRID_SIZE, EMPTY_CHAR));
std::vector<std::vector<bool>> searched(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
std::pair<int, int> player_pos;
std::pair<int, int> object_pos;
std::vector<std::pair<int, int>> enemy_pos;
bool is_hiding = false;

// Function prototypes
void initializeGame();
void printGrid();
void movePlayer(char direction);
void moveEnemies();
void searchSurroundings();
void hide();
void checkGameStatus();
void playerTurn();

int main() {
    srand(static_cast<unsigned>(time(0)));
    initializeGame();

    while (true) {
        playerTurn();
    }

    return 0;
}

void initializeGame() {
    // Initialize player and object positions randomly
    player_pos = {rand() % GRID_SIZE, rand() % GRID_SIZE};
    do {
        object_pos = {rand() % GRID_SIZE, rand() % GRID_SIZE};
    } while (object_pos == player_pos);

    // Initialize enemies
    int num_enemies = 5;
    for (int i = 0; i < num_enemies; ++i) {
        std::pair<int, int> pos;
        do {
            pos = {rand() % GRID_SIZE, rand() % GRID_SIZE};
        } while (pos == player_pos || pos == object_pos || std::find(enemy_pos.begin(), enemy_pos.end(), pos) != enemy_pos.end());
        enemy_pos.push_back(pos);
    }
}

void printGrid() {
    std::cout << "  N\nW + E\n  S" << std::endl;  // Compass
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            if (std::make_pair(x, y) == player_pos && !is_hiding) {
                std::cout << PLAYER_CHAR << " ";
            } else if (std::find(enemy_pos.begin(), enemy_pos.end(), std::make_pair(x, y)) != enemy_pos.end()) {
                std::cout << ENEMY_CHAR << " ";
            } else if (searched[x][y]) {
                std::cout << SEARCHED_CHAR << " ";
            } else {
                std::cout << EMPTY_CHAR << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void movePlayer(char direction) {
    is_hiding = false;
    std::pair<int, int> new_pos = player_pos;

    switch (direction) {
        case 'n': new_pos.second -= 1; break;
        case 'e': new_pos.first += 1; break;
        case 's': new_pos.second += 1; break;
        case 'w': new_pos.first -= 1; break;
        default: std::cout << "Invalid move. Try again." << std::endl; return;
    }

    if (new_pos.first >= 0 && new_pos.first < GRID_SIZE && new_pos.second >= 0 && new_pos.second < GRID_SIZE) {
        player_pos = new_pos;
        checkGameStatus();
    } else {
        std::cout << "Invalid move. Try again." << std::endl;
    }
}

void moveEnemies() {
    for (size_t i = 0; i < enemy_pos.size(); ++i) {
        std::pair<int, int> pos = enemy_pos[i];
        std::pair<int, int> new_pos = pos;

        int x_move = rand() % 3 - 1;  // Move -1, 0, or 1 in x
        int y_move = rand() % 3 - 1;  // Move -1, 0, or 1 in y

        new_pos.first += x_move;
        new_pos.second += y_move;

        // Ensure new position is within grid
        if (new_pos.first >= 0 && new_pos.first < GRID_SIZE && new_pos.second >= 0 && new_pos.second < GRID_SIZE) {
            enemy_pos[i] = new_pos;
        }
    }
}

void searchSurroundings() {
    is_hiding = false;
    std::cout << "Searching surrounding squares..." << std::endl;
    bool found_something = false;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            std::pair<int, int> search_pos = {player_pos.first + dx, player_pos.second + dy};

            if (search_pos.first >= 0 && search_pos.first < GRID_SIZE && search_pos.second >= 0 && search_pos.second < GRID_SIZE) {
                searched[search_pos.first][search_pos.second] = true;
                if (search_pos == object_pos) {
                    std::cout << "The treasure is near!" << std::endl;
                    found_something = true;
                } else if (std::find(enemy_pos.begin(), enemy_pos.end(), search_pos) != enemy_pos.end()) {
                    std::cout << "Enemy spotted nearby!" << std::endl;
                    found_something = true;
                }
            }
        }
    }

    if (!found_something) {
        std::cout << "Nothing found." << std::endl;
    }
}

void hide() {
    is_hiding = true;
    std::cout << "You are hiding. Enemies can't see you this turn." << std::endl;
}

void checkGameStatus() {
    if (player_pos == object_pos) {
        std::cout << "You found the Treasure! You win!" << std::endl;
        std::exit(0);
    }
    if (!is_hiding && std::find(enemy_pos.begin(), enemy_pos.end(), player_pos) != enemy_pos.end()) {
        std::cout << "An enemy got you! Game over." << std::endl;
        std::exit(0);
    }
}

void playerTurn() {
    printGrid();
    std::cout << "Choose action (move [n, e, s, w], search, or hide): ";
    std::string action;
    std::cin >> action;

    if (action == "n" || action == "e" || action == "s" || action == "w") {
        movePlayer(action[0]);
    } else if (action == "search") {
        searchSurroundings();
    } else if (action == "hide") {
        hide();
    } else {
        std::cout << "Invalid action." << std::endl;
    }

    moveEnemies();
    checkGameStatus();
}
