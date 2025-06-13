#include "GameOfLifeCore.hpp"
#include "GameOfLifeRenderer.hpp"
#include <gtest/gtest.h>

// Тест проверяет корректность инициализации игрового поля
TEST(GameOfLifeCoreTest, GridInitializedWithCorrectSize) {
    GameOfLifeCore game;
    const auto& grid = game.getGrid();

    EXPECT_EQ(grid.size(), GameOfLifeCore::FIELD_HEIGHT);
    EXPECT_EQ(grid[0].size(), GameOfLifeCore::FIELD_WIDTH);
}

// Проверяем метод countNeighbors()
TEST(GameOfLifeCoreTest, CountNeighbors_CorrectlyCounts) {
    GameOfLifeCore game;

    //делаем все клетки мертвыми
    for (int i = 0; i < GameOfLifeCore::FIELD_HEIGHT; ++i) {
        for (int j = 0; j < GameOfLifeCore::FIELD_WIDTH; ++j) {
            game.setCell(i, j, false);
        }
    }

    //выбираем центр клетку для тестирования
    int centerX = GameOfLifeCore::FIELD_HEIGHT / 2;
    int centerY = GameOfLifeCore::FIELD_WIDTH / 2;

    game.setCell(centerX - 1, centerY - 1, true);
    game.setCell(centerX - 1, centerY + 1, true);
    game.setCell(centerX + 1, centerY, true);

    int neighbors = game.countNeighbors(centerX, centerY);
    EXPECT_EQ(neighbors, 3);
}

// Тест проверяет правило: живая клетка с 1 соседом умирает
TEST(GameOfLifeCoreTest, LiveCellDiesWithOneNeighbor) {
    GameOfLifeCore game;

    for (int i = 0; i < GameOfLifeCore::FIELD_HEIGHT; ++i) {
        for (int j = 0; j < GameOfLifeCore::FIELD_WIDTH; ++j) {
            game.setCell(i, j, false);
        }
    }

    int centerX = GameOfLifeCore::FIELD_HEIGHT / 2;
    int centerY = GameOfLifeCore::FIELD_WIDTH / 2;

    game.setCell(centerX, centerY, true);         // центральная клетка жива
    game.setCell(centerX - 1, centerY - 1, true); // один сосед

    game.update();

    // Проверяем, что центральная клетка умерла
    const auto& grid = game.getGrid();
    EXPECT_FALSE(grid[centerX][centerY]);
}

// Тест проверяет правило: мертвая клетка с 3 соседями оживает
TEST(GameOfLifeCoreTest, DeadCellRebornWithThreeNeighbors) {
    GameOfLifeCore game;

    // Сбрасываем сетку
    for (int i = 0; i < GameOfLifeCore::FIELD_HEIGHT; ++i) {
        for (int j = 0; j < GameOfLifeCore::FIELD_WIDTH; ++j) {
            game.setCell(i, j, false);
        }
    }

    int centerX = GameOfLifeCore::FIELD_HEIGHT / 2;
    int centerY = GameOfLifeCore::FIELD_WIDTH / 2;

    game.setCell(centerX - 1, centerY - 1, true);
    game.setCell(centerX - 1, centerY + 1, true);
    game.setCell(centerX + 1, centerY, true);

    game.update();

    const auto& grid = game.getGrid();
    EXPECT_TRUE(grid[centerX][centerY]);
}

// Тест проверяет правило: живая клетка с 2 соседями выживает
TEST(GameOfLifeCoreTest, LiveCellSurvivesWithTwoNeighbors) {
    GameOfLifeCore game;

    for (int i = 0; i < GameOfLifeCore::FIELD_HEIGHT; ++i) {
        for (int j = 0; j < GameOfLifeCore::FIELD_WIDTH; ++j) {
            game.setCell(i, j, false);
        }
    }

    int centerX = GameOfLifeCore::FIELD_HEIGHT / 2;
    int centerY = GameOfLifeCore::FIELD_WIDTH / 2;

    game.setCell(centerX, centerY, true);
    game.setCell(centerX - 1, centerY, true); // сосед
    game.setCell(centerX + 1, centerY, true); // сосед

    game.update();

    const auto& grid = game.getGrid();
    EXPECT_TRUE(grid[centerX][centerY]);
}

// Тест проверяет правило: живая клетка с 4 соседями умирает от перенаселения
TEST(GameOfLifeCoreTest, LiveCellDiesFromOverpopulation) {
    GameOfLifeCore game;

    for (int i = 0; i < GameOfLifeCore::FIELD_HEIGHT; ++i) {
        for (int j = 0; j < GameOfLifeCore::FIELD_WIDTH; ++j) {
            game.setCell(i, j, false);
        }
    }

    int centerX = GameOfLifeCore::FIELD_HEIGHT / 2;
    int centerY = GameOfLifeCore::FIELD_WIDTH / 2;

    game.setCell(centerX, centerY, true);

    game.setCell(centerX - 1, centerY, true);
    game.setCell(centerX + 1, centerY, true);
    game.setCell(centerX, centerY - 1, true);
    game.setCell(centerX, centerY + 1, true);

    game.update();

    const auto& grid = game.getGrid();
    EXPECT_FALSE(grid[centerX][centerY]);
}

// Тест проверяет корректность увеличения счетчика поколений
TEST(GameOfLifeCoreTest, GenerationIncrementsAfterUpdate) {
    GameOfLifeCore game;
    int gen = game.getGeneration();

    game.update();

    EXPECT_EQ(game.getGeneration(), gen + 1);
}