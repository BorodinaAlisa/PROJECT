#include "GameOfLifeCore.hpp"
#include "GameOfLifeRenderer.hpp"

int main() {
    GameOfLifeCore core;
    GameOfLifeRenderer renderer(core);
    renderer.run();
    return 0;
}