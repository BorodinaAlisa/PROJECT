#include "GameOfLifeRenderer.hpp"
#include <iostream>
#include <thread>

GameOfLifeRenderer::GameOfLifeRenderer(GameOfLifeCore& g)
    : game(g),
      window(sf::VideoMode::getDesktopMode(), "Game of Life", sf::Style::Fullscreen),
      state{},
      resources{} {
    
    window.setVerticalSyncEnabled(false);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    state.WINDOW_WIDTH = desktop.width;
    state.WINDOW_HEIGHT = desktop.height;

    if (!resources.font.loadFromFile("/mnt/c/project1/PROJECT/resources/OpenSans-Regular.ttf")) {
        std::cerr << "Error: Could not load font 'OpenSans-Regular.ttf'\n";
        window.close();
        return;
    }

    if (!resources.menuBackgroundTexture.loadFromFile("/mnt/c/project1/PROJECT/resources/fon2.png")) {
        std::cerr << "Error: Could not load background image 'fon2.png'\n";
    } else {
        resources.menuBackgroundSprite.setTexture(resources.menuBackgroundTexture);
        resources.menuBackgroundSprite.setScale(
            static_cast<float>(state.WINDOW_WIDTH) / resources.menuBackgroundTexture.getSize().x,
            static_cast<float>(state.WINDOW_HEIGHT) / resources.menuBackgroundTexture.getSize().y
        );
    }

    if (!resources.playButtonTexture.loadFromFile("/mnt/c/project1/PROJECT/resources/play2.png") ||
        !resources.rulesButtonTexture.loadFromFile("/mnt/c/project1/PROJECT/resources/rules2.png") ||
        !resources.controlButtonTexture.loadFromFile("/mnt/c/project1/PROJECT/resources/game2.png") ||
        !resources.exitButtonTexture.loadFromFile("/mnt/c/project1/PROJECT/resources/exit2.png")) {
        std::cerr << "Error: Could not load one or more button textures\n";
        window.close();
        return;
    }

    resources.playButtonSprite.setTexture(resources.playButtonTexture);
    resources.rulesButtonSprite.setTexture(resources.rulesButtonTexture);
    resources.controlButtonSprite.setTexture(resources.controlButtonTexture);
    resources.exitButtonSprite.setTexture(resources.exitButtonTexture);

    auto scaleButton = [](sf::Sprite& sprite, const sf::Texture& texture) {
        sprite.setScale(
            UIConstants::BUTTON_WIDTH / texture.getSize().x,
            UIConstants::BUTTON_HEIGHT / texture.getSize().y
        );
    };

    scaleButton(resources.playButtonSprite, resources.playButtonTexture);
    scaleButton(resources.rulesButtonSprite, resources.rulesButtonTexture);
    scaleButton(resources.controlButtonSprite, resources.controlButtonTexture);
    scaleButton(resources.exitButtonSprite, resources.exitButtonTexture);
}

void GameOfLifeRenderer::run() {
    while (window.isOpen()) {
        handleEvents();
        
        if (state.showMainMenu || state.showRules || state.showControl) {
            renderMenu();
        } else {
            if (!state.isPaused) {
                game.update();
            }

            if (state.isMouseLeftPressed || state.isMouseRightPressed) {
                handleMouseDrawing();
            }

            renderGame();
            std::this_thread::sleep_for(std::chrono::milliseconds(state.delay));
        }
    }
}

void GameOfLifeRenderer::handleMouseDrawing() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int playableWidth = FIELD_WIDTH * CELL_SIZE;
    int playableHeight = FIELD_HEIGHT * CELL_SIZE;
    int offsetX = (state.WINDOW_WIDTH - playableWidth) / 2;
    int offsetY = (state.WINDOW_HEIGHT - playableHeight) / UIConstants::FIELD_OFFSET_Y_RATIO;

    if (mousePos.x >= offsetX && mousePos.x <= offsetX + playableWidth &&
        mousePos.y >= offsetY && mousePos.y <= offsetY + playableHeight) {
        
        int col = (mousePos.x - offsetX) / CELL_SIZE;
        int row = (mousePos.y - offsetY) / CELL_SIZE;

        if (row >= 0 && row < FIELD_HEIGHT && col >= 0 && col < FIELD_WIDTH) {
            auto& grid = const_cast<std::vector<std::vector<bool>>&>(game.getGrid());

            if (row != state.lastRow || col != state.lastCol) {
                if (state.isMouseLeftPressed) {
                    grid[row][col] = state.drawMode;
                } else if (state.isMouseRightPressed) {
                    grid[row][col] = false;
                }
                state.lastRow = row;
                state.lastCol = col;
            }
        }
    }
}

bool GameOfLifeRenderer::isInside(sf::Vector2i pos, float x, float y, float w, float h) {
    return pos.x >= x && pos.x <= x + w && pos.y >= y && pos.y <= y + h;
}

void GameOfLifeRenderer::renderGame() {
    window.clear();
    window.draw(resources.menuBackgroundSprite);

    int playableWidth = FIELD_WIDTH * CELL_SIZE;
    int playableHeight = FIELD_HEIGHT * CELL_SIZE;
    int offsetX = (state.WINDOW_WIDTH - playableWidth) / 2;
    int offsetY = (state.WINDOW_HEIGHT - playableHeight) / UIConstants::FIELD_OFFSET_Y_RATIO;

    sf::RectangleShape border(sf::Vector2f(playableWidth, playableHeight));
    border.setPosition(offsetX, offsetY);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Black);
    border.setOutlineThickness(UIConstants::FIELD_BORDER_THICKNESS);
    window.draw(border);

    const auto& grid = game.getGrid();
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE);
            cell.setFillColor(grid[i][j] ? sf::Color::Green : sf::Color::Black);
            window.draw(cell);
        }
    }

    if (state.isPaused && !state.showMainMenu && !state.showRules && !state.showControl) {
        renderCellHighlight(offsetX, offsetY);
    }

    renderInfoPanel();
    window.display();
}

void GameOfLifeRenderer::renderCellHighlight(int offsetX, int offsetY) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int playableWidth = FIELD_WIDTH * CELL_SIZE;
    int playableHeight = FIELD_HEIGHT * CELL_SIZE;

    if (mousePos.x >= offsetX && mousePos.x <= offsetX + playableWidth &&
        mousePos.y >= offsetY && mousePos.y <= offsetY + playableHeight) {

        int col = (mousePos.x - offsetX) / CELL_SIZE;
        int row = (mousePos.y - offsetY) / CELL_SIZE;

        if (row >= 0 && row < FIELD_HEIGHT && col >= 0 && col < FIELD_WIDTH) {
            sf::RectangleShape highlight(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            highlight.setPosition(offsetX + col * CELL_SIZE, offsetY + row * CELL_SIZE);
            highlight.setFillColor(state.drawMode ?
                sf::Color(255, 255, 255, UIConstants::HIGHLIGHT_ALPHA_ADD) :
                sf::Color(255, 0, 0, UIConstants::HIGHLIGHT_ALPHA_REMOVE));
            window.draw(highlight);
        }
    }
}

void GameOfLifeRenderer::renderInfoPanel() {
    sf::Text info;
    info.setFont(resources.font);
    info.setCharacterSize(UIConstants::MAIN_FONT_SIZE);
    info.setFillColor(sf::Color::White);
    info.setPosition(UIConstants::INFO_TEXT_X, state.WINDOW_HEIGHT - UIConstants::INFO_TEXT_Y_OFFSET);
    
    std::string modeStr = state.drawMode ? "Add" : "Remove";
    info.setString(
        "Generation: " + std::to_string(game.getGeneration()) +
        " | Speed: " + std::to_string(state.delay) + "ms" +
        " | Controls: W/S - speed, Space - pause, R - reset, M - menu, Q - exit" +
        ", T - Switch Mode (" + modeStr + ")"
    );
    window.draw(info);
}

void GameOfLifeRenderer::renderRulesWindow() {
    window.clear();
    window.draw(resources.menuBackgroundSprite);

    std::string rulesText =
        "The universe is a 2D grid of square cells\n"
        "Each cell can be ALIVE (filled) or DEAD (empty)\n"
        "Every cell interacts with 8 neighbors (adjacent cells)\n\n"
        "Basic Rules:\n"
        "1) Birth: Dead cell with exactly 3 live neighbors becomes alive\n"
        "2) Survival: Live cell with 2-3 neighbors stays alive\n"
        "3) Death: Live cell with <2 or >3 neighbors dies\n\n"
        "Game ends when:\n"
        "- No live cells remain\n"
        "- The pattern becomes stable (stops changing)\n\n"
        "You set the initial pattern,\n"
        "then watch it evolve automatically.";

    sf::Text rules;
    rules.setFont(resources.font);
    rules.setCharacterSize(UIConstants::RULES_FONT_SIZE);
    rules.setFillColor(sf::Color::White);
    rules.setString(rulesText);
    rules.setPosition(UIConstants::RULES_TEXT_X, UIConstants::RULES_TEXT_Y);
    rules.setLineSpacing(UIConstants::LINE_SPACING);
    window.draw(rules);

    sf::Text hint;
    hint.setFont(resources.font);
    hint.setCharacterSize(UIConstants::HINT_FONT_SIZE);
    hint.setFillColor(sf::Color::White);
    hint.setString("Press M or click outside text to return to menu");
    hint.setPosition(state.WINDOW_WIDTH / 2 - hint.getLocalBounds().width / 2, 
                    state.WINDOW_HEIGHT - UIConstants::HINT_TEXT_Y_OFFSET);
    window.draw(hint);

    window.display();
}

void GameOfLifeRenderer::renderMainMenuButtons() {
    float buttonX = state.WINDOW_WIDTH / 2 - UIConstants::BUTTON_X_OFFSET;
    float startY = state.WINDOW_HEIGHT / 2 - UIConstants::BUTTON_Y_OFFSET;

    resources.playButtonSprite.setPosition(buttonX, startY);
    resources.rulesButtonSprite.setPosition(buttonX, startY + UIConstants::BUTTON_SPACING);
    resources.controlButtonSprite.setPosition(buttonX, startY + UIConstants::BUTTON_SPACING * 2);
    resources.exitButtonSprite.setPosition(buttonX, startY + UIConstants::BUTTON_SPACING * 3);

    window.draw(resources.playButtonSprite);
    window.draw(resources.rulesButtonSprite);
    window.draw(resources.controlButtonSprite);
    window.draw(resources.exitButtonSprite);
}

void GameOfLifeRenderer::renderMenu() {
    window.clear();
    window.draw(resources.menuBackgroundSprite);

    if (state.showRules) {
        renderRulesWindow();
    } else if (state.showControl) {
        renderControl();
    } else {
        renderMainMenuButtons();
    }

    window.display();
}

void GameOfLifeRenderer::renderControl() {
    window.clear();
    window.draw(resources.menuBackgroundSprite);
    
    std::string controlText =
        "LMB - Add/remove cells (mode toggled with T)\n"
        "RMB - Always remove cells\n"
        "T - Toggle add/remove mode\n"
        "SPACE - Pause simulation\n"
        "R - Reset field\n"
        "W/S - Adjust speed\n"
        "M - Return to menu\n\n\n\n\n"
        "Click anywhere to return";

    sf::Text controls;
    controls.setFont(resources.font);
    controls.setCharacterSize(UIConstants::CONTROL_FONT_SIZE);
    controls.setFillColor(sf::Color::White);
    controls.setString(controlText);
    controls.setLineSpacing(UIConstants::LINE_SPACING);
    
    sf::FloatRect textBounds = controls.getLocalBounds();
    controls.setOrigin(0, 0); 
    controls.setPosition(UIConstants::CONTROL_TEXT_X, 
                        state.WINDOW_HEIGHT / UIConstants::CONTROL_TEXT_Y_RATIO); 

    window.draw(controls);
    window.display();
}

void GameOfLifeRenderer::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            handleMousePress(event);
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            handleMouseRelease(event.mouseButton.button);
        }
    }
}

void GameOfLifeRenderer::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Escape || key == sf::Keyboard::Q) {
        window.close();
    } else if (key == sf::Keyboard::Space) {
        state.isPaused = !state.isPaused;
    } else if (key == sf::Keyboard::R) {
        game.reset();
    } else if (key == sf::Keyboard::M) {
        state.showMainMenu = true;
        state.showRules = false;
        state.showControl = false;
    } else if (key == sf::Keyboard::W) {
        if (state.delay > UIConstants::MIN_DELAY) {
            state.delay -= UIConstants::DELAY_STEP;
        }
    } else if (key == sf::Keyboard::S) {
        state.delay += UIConstants::DELAY_STEP;
    } else if (key == sf::Keyboard::T) {
        state.drawMode = !state.drawMode;
    }
}

void GameOfLifeRenderer::handleMousePress(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (state.showMainMenu) {
        handleMenuClick(mousePos);
    } else if (state.isPaused && !state.showRules && !state.showControl) {
        handleGameFieldClick(mousePos, event.mouseButton.button);
    } else if (state.showRules || state.showControl) {
        state.showRules = false;
        state.showControl = false;
        state.showMainMenu = true;
    }

    // Handle mouse button states
    if (event.mouseButton.button == sf::Mouse::Left && 
        state.isPaused && !state.showMainMenu && !state.showRules && !state.showControl) {
        state.isMouseLeftPressed = true;
    }
    if (event.mouseButton.button == sf::Mouse::Right && 
        state.isPaused && !state.showMainMenu && !state.showRules && !state.showControl) {
        state.isMouseRightPressed = true;
    }
}

void GameOfLifeRenderer::handleMenuClick(const sf::Vector2i& mousePos) {
    float buttonX = state.WINDOW_WIDTH / 2 - UIConstants::BUTTON_X_OFFSET;
    float startY = state.WINDOW_HEIGHT / 2 - UIConstants::BUTTON_Y_OFFSET;

    if (isInside(mousePos, buttonX, startY, UIConstants::BUTTON_WIDTH, UIConstants::BUTTON_HEIGHT)) {
        state.showMainMenu = false;
    } else if (isInside(mousePos, buttonX, startY + UIConstants::BUTTON_SPACING, 
                       UIConstants::BUTTON_WIDTH, UIConstants::BUTTON_HEIGHT)) {
        state.showRules = true;
        state.showMainMenu = false;
        state.showControl = false;
    } else if (isInside(mousePos, buttonX, startY + UIConstants::BUTTON_SPACING * 2, 
                       UIConstants::BUTTON_WIDTH, UIConstants::BUTTON_HEIGHT)) {
        state.showControl = true;
        state.showMainMenu = false;
        state.showRules = false;
    } else if (isInside(mousePos, buttonX, startY + UIConstants::BUTTON_SPACING * 3, 
                       UIConstants::BUTTON_WIDTH, UIConstants::BUTTON_HEIGHT)) {
        window.close();
    }
}

void GameOfLifeRenderer::handleGameFieldClick(const sf::Vector2i& mousePos, sf::Mouse::Button button) {
    int playableWidth = FIELD_WIDTH * CELL_SIZE;
    int playableHeight = FIELD_HEIGHT * CELL_SIZE;
    int offsetX = (state.WINDOW_WIDTH - playableWidth) / 2;
    int offsetY = (state.WINDOW_HEIGHT - playableHeight) / UIConstants::FIELD_OFFSET_Y_RATIO;

    if (mousePos.x >= offsetX && mousePos.x <= offsetX + playableWidth &&
        mousePos.y >= offsetY && mousePos.y <= offsetY + playableHeight) {

        int col = (mousePos.x - offsetX) / CELL_SIZE;
        int row = (mousePos.y - offsetY) / CELL_SIZE;

        if (row >= 0 && row < FIELD_HEIGHT && col >= 0 && col < FIELD_WIDTH) {
            auto& grid = const_cast<std::vector<std::vector<bool>>&>(game.getGrid());
            if (button == sf::Mouse::Left) {
                grid[row][col] = state.drawMode;
            } else if (button == sf::Mouse::Right) {
                grid[row][col] = false;
            }
        }
    }
}

void GameOfLifeRenderer::handleMouseRelease(sf::Mouse::Button button) {
    if (button == sf::Mouse::Left) state.isMouseLeftPressed = false;
    if (button == sf::Mouse::Right) state.isMouseRightPressed = false;
    state.lastRow = -1;
    state.lastCol = -1;
}