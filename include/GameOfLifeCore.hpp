#pragma once

#include <vector>

class GameOfLifeCore {
private:
    std::vector<std::vector<bool>> grid; // двумерный массив, который делает игровое поле
    int generation;

public:
    static const int FIELD_WIDTH = 90;
    static const int FIELD_HEIGHT = 50;
    static const int CELL_SIZE = 15;
    static const int RANDOM_FILL_PERCENTAGE = 40;

    GameOfLifeCore(); //инициализирует пустое игровое поле
    void randomizeGrid(); 
    void reset();        

    void update();         

    const std::vector<std::vector<bool>>& getGrid() const; //возвращаетссылку на текущее игровое поле
    int getGeneration() const;

    int countNeighbors(int x, int y); //считаем кол-во живых соседей

    void setCell(int row, int col, bool alive); //установка конкретного состояния клетки
};