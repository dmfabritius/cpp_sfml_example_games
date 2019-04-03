#pragma once

constexpr int DISPLAY_WIDTH = 320;
constexpr int DISPLAY_HEIGHT = 480;

constexpr int SHAPE_SIZE = 18;
constexpr int NUM_SHAPES = 7;
constexpr int NUM_BLOCKS = 4;
constexpr int NUM_TILE_COLORS = 7;
constexpr int FIELD_WIDTH = 10;
constexpr int FIELD_HEIGHT = 20;
constexpr int FIELD_HORZ_OFFSET = 32;
constexpr int FIELD_VERT_OFFSET = 96;

class Game {
private:
    sf::RenderWindow window;
    sf::Event event;
    sf::Clock clock;
    float delay; // control the pace of the game
    float timer = 0; // when timer exceeds delay, pick a new shape & color

    sf::Texture backgroundTexture, tileTexture;
    sf::Sprite backgroundSprite, tileSprite;
    sf::Music* music;
    sf::Sound* sound;
    sf::SoundBuffer* buffer;

    int field[FIELD_HEIGHT][FIELD_WIDTH];

    struct Point {
        int x, y;
    };
    Point currentPosition[NUM_BLOCKS];
    Point previousPosition[NUM_BLOCKS];

    // define the tetromino shapes, all 7 possible orthogonally connected sets of 4 blocks
    int shapes[NUM_SHAPES][NUM_BLOCKS] = {
        1,3,5,7, // I
        2,4,5,7, // Z
        3,5,4,6, // S
        3,5,4,7, // T
        2,3,5,7, // L
        3,5,7,6, // J
        2,3,4,5  // O
    };

    int currentShape;
    int tileColor;
    int moveX;
    bool rotate;

public:
    Game();
    void run();

private:
    void handleEvents();
    bool collision();
    void undo();
    void move();
    void update();
    void clearCompleted();
    void draw();
};

