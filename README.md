# Game of Life - Реализация на C++

![Game of Life Preview](resources/gun_pentadec.gif)

## Описание
Реализация клеточного автомата "Игра Жизнь" Джона Конвея с графическим интерфейсом на SFML.

## Особенности
- Симуляция клеточных поколений
- Главное меню с настройками
- Регулируемая скорость симуляции
- Добавление/удаление клеток

### Требования
- C++17 компилятор
- CMake ≥ 3.12
- SFML 2.5.1

### Сборка
1. Клонирование репозитория
git clone https://github.com/BorodinaAlisa/PROJECT.git
cd PROJECT
2. Сборка проекта
mkdir build && cd build
cmake ..
make
3. Запуск игры
./GameOfLife

### Управление
ЛКМ: Добавить клетку
ПКМ: Удвлить клетку
Пробел: Пауза/продолжить симуляцию
T: Изменение режима
W/S: Изменение скорости
R: Сбросить поле
M: Возврат в начальное меню
Q: Выход из игры

### Структура проекта
PROJECT/
│
├── include/
│   ├── GameOfLifeCore.hpp
│   └── GameOfLifeRenderer.hpp
│
├── resources/
│   ├── exit2.png
│   ├── fon2.png
│   ├── game2.png
│   ├── gun_pentadec.gif     
│   ├── OpenSans-Regular.tif
│   ├── play2.png
│   └── rules2.png
│
├── src/
│   ├── GameOfLifeCore.cpp
│   ├── GameOfLifeRenderer.cpp
│   └── main.cpp
│
├── tests/
│   └── GameOfLifeCoreTest.cpp
│
├── README.md
└── CMakeLists.txt

### Лицензия
Этот проект распространяется под лицензией MIT.