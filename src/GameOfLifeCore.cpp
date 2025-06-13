#include "GameOfLifeCore.hpp"
#include <vector>
#include <cstdlib>

const int GameOfLifeCore::FIELD_WIDTH;
const int GameOfLifeCore::FIELD_HEIGHT;
const int GameOfLifeCore::CELL_SIZE;
const int GameOfLifeCore::RANDOM_FILL_PERCENTAGE;

GameOfLifeCore::GameOfLifeCore()
    : generation(0) {
    grid.resize(FIELD_HEIGHT, std::vector<bool>(FIELD_WIDTH, false)); // создаем двумерный вектор, заполненный мертвые клетки
    randomizeGrid();
}

void GameOfLifeCore::randomizeGrid() {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            grid[i][j] = (std::rand() % 100 < RANDOM_FILL_PERCENTAGE);
        }
    }
}

//устанавливаем состояние конкретной клетки
void GameOfLifeCore::setCell(int row, int col, bool alive) {
    if (row >= 0 && row < FIELD_HEIGHT && col >= 0 && col < FIELD_WIDTH) {
        grid[row][col] = alive;
    }
}

// создаем временную копию текущего состояния поля
void GameOfLifeCore::update() {
    std::vector<std::vector<bool>> newGrid = grid;
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            int neighbors = countNeighbors(i, j);
            if (grid[i][j]) { // если клетка живая
                newGrid[i][j] = (neighbors == 2 || neighbors == 3);
            } else { // мертвая
                newGrid[i][j] = (neighbors == 3);
            }
        }
    }
    grid = newGrid;
    generation++;
}

// подсчитывает количество живых соседей для указанной клетки
int GameOfLifeCore::countNeighbors(int x, int y) {
    int count = 0;
    // проверяем все 8 соседних клеток
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int nx = (x + i + FIELD_HEIGHT) % FIELD_HEIGHT;
            int ny = (y + j + FIELD_WIDTH) % FIELD_WIDTH;
            if (grid[nx][ny]) count++;
        }
    }
    return count;
}

const std::vector<std::vector<bool>>& GameOfLifeCore::getGrid() const {
    return grid;
}

int GameOfLifeCore::getGeneration() const {
    return generation;
}

void GameOfLifeCore::reset() {
    generation = 0;
    randomizeGrid();
}