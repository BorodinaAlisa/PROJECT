#pragma once

#include "GameOfLifeCore.hpp"
#include <SFML/Graphics.hpp>

class GameOfLifeRenderer {
public:

    explicit GameOfLifeRenderer(GameOfLifeCore& game);
    void run();

private:

    void handleEvents();
    void renderGame();
    void renderMenu();
    void renderRulesWindow();
    void renderControl();
    void renderMainMenuButtons();
    void renderInfoPanel();
    void renderCellHighlight(int offsetX, int offsetY);
    void handleMouseDrawing();
    void handleMouseRelease(sf::Mouse::Button button);
    void handleKeyPress(sf::Keyboard::Key key);
    void handleMousePress(const sf::Event& event);
    void handleMenuClick(const sf::Vector2i& mousePos);
    void handleGameFieldClick(const sf::Vector2i& mousePos, sf::Mouse::Button button);
    bool isInside(sf::Vector2i pos, float x, float y, float w, float h);

    static constexpr int CELL_SIZE = 15;
    static constexpr int FIELD_WIDTH = 90;
    static constexpr int FIELD_HEIGHT = 50;
    
    struct UIConstants {

        static constexpr float BUTTON_WIDTH = 300;
        static constexpr float BUTTON_HEIGHT = 70;
        static constexpr float BUTTON_SPACING = 90;
        static constexpr float BUTTON_X_OFFSET = 150;
        static constexpr float BUTTON_Y_OFFSET = 150;
        
        static constexpr int DEFAULT_DELAY = 200;
        static constexpr int MIN_DELAY = 50;
        static constexpr int DELAY_STEP = 50;
        
        static constexpr float HIGHLIGHT_ALPHA_ADD = 80;
        static constexpr float HIGHLIGHT_ALPHA_REMOVE = 80;
        static constexpr float FIELD_OFFSET_Y_RATIO = 1.5f;
        static constexpr float FIELD_BORDER_THICKNESS = 3.0f;
        
        static constexpr int MAIN_FONT_SIZE = 20;
        static constexpr int RULES_FONT_SIZE = 28;
        static constexpr int HINT_FONT_SIZE = 24;
        static constexpr int CONTROL_FONT_SIZE = 32;
        static constexpr float LINE_SPACING = 1.5f;

        static constexpr float INFO_TEXT_X = 10.0f;
        static constexpr float INFO_TEXT_Y_OFFSET = 40.0f;
        static constexpr float RULES_TEXT_X = 40.0f;
        static constexpr float RULES_TEXT_Y = 150.0f;
        static constexpr float HINT_TEXT_Y_OFFSET = 60.0f;
        static constexpr float CONTROL_TEXT_X = 50.0f;
        static constexpr float CONTROL_TEXT_Y_RATIO = 4.0f;
    };

    // Состояние приложения
    struct AppState {
        int delay = UIConstants::DEFAULT_DELAY;
        bool isPaused = false;
        bool showMainMenu = true;
        bool showRules = false;
        bool showControl = false;
        bool drawMode = true;
        bool isMouseLeftPressed = false;
        bool isMouseRightPressed = false;
        int lastRow = -1;
        int lastCol = -1;
        int WINDOW_WIDTH = 0;
        int WINDOW_HEIGHT = 0;
    };

    struct Resources {
        sf::Font font;
        sf::Texture menuBackgroundTexture;
        sf::Sprite menuBackgroundSprite;
        sf::Texture playButtonTexture;
        sf::Texture rulesButtonTexture;
        sf::Texture controlButtonTexture;
        sf::Texture exitButtonTexture;
        sf::Sprite playButtonSprite;
        sf::Sprite rulesButtonSprite;
        sf::Sprite controlButtonSprite;
        sf::Sprite exitButtonSprite;
    };

    sf::RenderWindow window;
    GameOfLifeCore& game;
    AppState state;
    Resources resources;
};