#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <filesystem>

using namespace sf;
using namespace std;
#include <fstream>

const int ROWS = 25;          // Количество строк
const int COLS = 80;          // Количество столбцов
const int CELL_SIZE = 10;     // Размер клетки в пикселях
const int WINDOW_WIDTH = COLS * CELL_SIZE;
const int WINDOW_HEIGHT = ROWS * CELL_SIZE + 50;  // +50 для панели управления

class GameOfLife {
private:
    std::vector<std::vector<bool>> grid;  // Игровое поле
    sf::RenderWindow window;              // Окно SFML
    int delay = 200;                      // Задержка между поколениями (мс)
    int generation = 0;                   // Счётчик поколений
    bool isPaused = false;                // Состояние паузы
    bool showMainMenu = true;             // Показывать главное меню
    bool showRules = false;               // Показывать правила
    sf::Font font;                        // Шрифт для текста

public:
    GameOfLife() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game of Life") {
        auto path = std::string("qww.ttf");
        if (!font.loadFromFile(path)) {
            std::cerr << std::filesystem::current_path() << std::endl;
            std::cerr << "Error: Failed to load main font. Trying fallback options...\n";
            if (!font.loadFromFile(std::string("qww.ttf")) && 
                !font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                std::cerr << "Critical error: No fonts available!\n";
                window.close();
                return;
            }
        }
        
        grid.resize(ROWS, std::vector<bool>(COLS, false));
        randomizeGrid();
    }

    void randomizeGrid() {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                grid[i][j] = (rand() % 10) > 7;  // 30% вероятность быть живой
            }
        }
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();  // Обработка ввода
            if (showMainMenu || showRules) {
                renderMenu();
            } else {
                if (!isPaused) {
                    update();    // Обновление состояния игры
                }
                renderGame();    // Отрисовка игры
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                window.close();
            }
            if (showMainMenu) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    // Кнопка ИГРАТЬ
                    if (mousePos.x >= WINDOW_WIDTH/2 - 100 && mousePos.x <= WINDOW_WIDTH/2 + 100 &&
                        mousePos.y >= WINDOW_HEIGHT/2 - 80 && mousePos.y <= WINDOW_HEIGHT/2 - 30) {
                        showMainMenu = false;
                    }
                    // Кнопка ПРАВИЛА
                    else if (mousePos.x >= WINDOW_WIDTH/2 - 100 && mousePos.x <= WINDOW_WIDTH/2 + 100 &&
                             mousePos.y >= WINDOW_HEIGHT/2 - 20 && mousePos.y <= WINDOW_HEIGHT/2 + 30) {
                        showRules = true;
                    }
                    // Кнопка ВЫХОД
                    else if (mousePos.x >= WINDOW_WIDTH/2 - 100 && mousePos.x <= WINDOW_WIDTH/2 + 100 &&
                             mousePos.y >= WINDOW_HEIGHT/2 + 40 && mousePos.y <= WINDOW_HEIGHT/2 + 90) {
                        window.close();
                    }
                }
            }
            else if (showRules) {
                if (event.type == sf::Event::KeyPressed ||
                    (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)) {
                    showRules = false;
                }
            }
            else {
                if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                        case sf::Keyboard::Space: // Пауза/продолжить
                            isPaused = !isPaused;
                            break;
                        case sf::Keyboard::W:     // Увеличить скорость
                            delay = std::max(50, delay - 50);
                            break;
                        case sf::Keyboard::S:     // Уменьшить скорость
                            delay += 50;
                            break;
                        case sf::Keyboard::R:     // Сбросить со случайным состоянием
                            randomizeGrid();
                            generation = 0;
                            break;
                        case sf::Keyboard::M:     // Вернуться в главное меню
                            showMainMenu = true;
                            break;
                    }
                }
            }
        }
    }

    void update() {
        std::vector<std::vector<bool>> newGrid = grid;
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                int neighbors = countNeighbors(i, j);
                if (grid[i][j]) {
                    newGrid[i][j] = (neighbors == 2 || neighbors == 3);  // Выживание
                } else {
                    newGrid[i][j] = (neighbors == 3);  // Рождение
                }
            }
        }
        grid = newGrid;
        generation++;
    }

    int countNeighbors(int x, int y) {
        int count = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;  // Пропустить себя
                int nx = (x + i + ROWS) % ROWS;  // Заворачивание краёв
                int ny = (y + j + COLS) % COLS;
                if (grid[nx][ny]) count++;
            }
        }
        return count;
    }

    void renderGame() {
        window.clear(sf::Color::Black);

        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(static_cast<float>(j * CELL_SIZE),
                                static_cast<float>(i * CELL_SIZE));
                cell.setFillColor(grid[i][j] ? sf::Color::Green : sf::Color::Black);
                window.draw(cell);
            }
        }

        sf::Text info;
        info.setFont(font);
        info.setCharacterSize(20);
        info.setFillColor(sf::Color::White);
        info.setPosition(10, WINDOW_HEIGHT - 40);
        info.setString(
            "Generation: " + std::to_string(generation) +
            " | Speed: " + std::to_string(delay) + "ms" +
            " | Controls: W/S - speed, Space - pause, R - reset, M - menu, Q - exit"
        );
        window.draw(info);

        window.display();
    }

    void renderMenu() {
        window.clear(sf::Color::Black);

        if (showRules) {
            sf::Text title;
            title.setFont(font);
            title.setCharacterSize(40);
            title.setFillColor(sf::Color::White);
            title.setString("Game of Life - Rules");
            title.setPosition(WINDOW_WIDTH/2 - title.getLocalBounds().width/2, 50);
            window.draw(title);

            std::string rulesText =
                "The Game of Life is a cellular automaton invented by mathematician John Conway.\n"
                "Rules:\n"
                "1. Any live cell with fewer than two live neighbors dies (loneliness).\n"
                "2. Any live cell with two or three live neighbors survives.\n"
                "3. Any live cell with more than three live neighbors dies (overpopulation).\n"
                "4. Any dead cell with exactly three live neighbors becomes alive.\n"
                "In-game controls:\n"
                "W/S - Change simulation speed\n"
                "Space - Pause/resume\n"
                "R - Reset to random pattern\n"
                "M - Return to main menu\n"
                "Q - Exit the game\n"
                "Press any key or click mouse to return to the main menu.";

            sf::Text rules;
            rules.setFont(font);
            rules.setCharacterSize(20);
            rules.setFillColor(sf::Color::White);
            rules.setString(rulesText);
            rules.setPosition(50, 150);
            window.draw(rules);
        } else {
            sf::Text title;
            title.setFont(font);
            title.setCharacterSize(50);
            title.setFillColor(sf::Color::White);
            title.setString("GAME OF LIFE");
            title.setPosition(WINDOW_WIDTH/2 - title.getLocalBounds().width/2, 100);
            window.draw(title);

            sf::RectangleShape playButton(sf::Vector2f(200, 50));
            playButton.setPosition(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 80);
            playButton.setFillColor(sf::Color(70, 70, 70));
            window.draw(playButton);

            sf::Text playText;
            playText.setFont(font);
            playText.setCharacterSize(30);
            playText.setFillColor(sf::Color::White);
            playText.setString("PLAY");
            playText.setPosition(WINDOW_WIDTH/2 - playText.getLocalBounds().width/2, WINDOW_HEIGHT/2 - 70);
            window.draw(playText);

            sf::RectangleShape rulesButton(sf::Vector2f(200, 50));
            rulesButton.setPosition(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 20);
            rulesButton.setFillColor(sf::Color(70, 70, 70));
            window.draw(rulesButton);

            sf::Text rulesText;
            rulesText.setFont(font);
            rulesText.setCharacterSize(30);
            rulesText.setFillColor(sf::Color::White);
            rulesText.setString("RULES");
            rulesText.setPosition(WINDOW_WIDTH/2 - rulesText.getLocalBounds().width/2, WINDOW_HEIGHT/2 - 10);
            window.draw(rulesText);

            sf::RectangleShape exitButton(sf::Vector2f(200, 50));
            exitButton.setPosition(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 40);
            exitButton.setFillColor(sf::Color(70, 70, 70));
            window.draw(exitButton);

            sf::Text exitText;
            exitText.setFont(font);
            exitText.setCharacterSize(30);
            exitText.setFillColor(sf::Color::White);
            exitText.setString("EXIT");
            exitText.setPosition(WINDOW_WIDTH/2 - exitText.getLocalBounds().width/2, WINDOW_HEIGHT/2 + 50);
            window.draw(exitText);
        }

        window.display();
    }
};

int main() {
    std::ofstream file("sfml-log.txt");  
    std::streambuf* previous = sf::err().rdbuf(file.rdbuf()); 
    std::cout << "Current directory: "
              << std::filesystem::current_path().string() << std::endl;

    GameOfLife game;
    game.run();
    return 0;
}