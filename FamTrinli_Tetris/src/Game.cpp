#include "pch.h"
#include "Game.h"

Game::Game() {
    window.create(sf::VideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT), "T*tris! SFML v2.5.1 TGUI v0.8");

    backgroundTexture.loadFromFile("images/bkg.png");
    backgroundSprite.setTexture(backgroundTexture);

    tileTexture.loadFromFile("images/tiles.png");
    tileSprite.setTexture(tileTexture);
    tileSprite.setTextureRect(sf::IntRect(0, 0, SHAPE_SIZE, SHAPE_SIZE));

    music = new sf::Music(); // background music
    music->openFromFile("audio/solveThePuzzle.ogg");
    music->setLoop(true);
    music->setVolume(25); // lower volume to 25%
    music->play();

    buffer = new sf::SoundBuffer();
    buffer->loadFromFile("audio/blip.wav"); // sound effect for when a row gets completed
    sound = new sf::Sound(*buffer);

    std::memset(field, -1, sizeof(field)); // initialize the field to be empty (no colored blocks)

    currentShape = rand() % NUM_SHAPES; // pick an initial shape
    tileColor = currentShape; // a given shape always gets the same color
    // tileColor = rand() % NUM_TILE_COLORS; // shapes get random colors

    for (int i = 0; i < NUM_BLOCKS; i++) { // initialize the current shape's position from the shapes table
        currentPosition[i].x = shapes[currentShape][i] % 2;
        currentPosition[i].y = shapes[currentShape][i] / 2;
    }
}

void Game::run() {
    while (window.isOpen()) {
        // reset update parameters for cycle thru the loop
        moveX = 0;
        rotate = false;
        delay = 0.3f; // set the pace of the game

        handleEvents();
        move();
        update();
        clearCompleted();
        draw();
    }
}

void Game::handleEvents() {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up)
                rotate = true;
            else if (event.key.code == sf::Keyboard::Left)
                moveX = -1;
            else if (event.key.code == sf::Keyboard::Right)
                moveX = 1;
            else if (event.key.code == sf::Keyboard::Escape)
                window.close();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05f; // make shape move quickly
}

bool Game::collision() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (currentPosition[i].x < 0 || currentPosition[i].x >= FIELD_WIDTH || currentPosition[i].y >= FIELD_HEIGHT)
            return true;
        if (field[currentPosition[i].y][currentPosition[i].x] != -1)
            return true;
    }
    return false;
}

void Game::undo() {
    for (int i = 0; i < NUM_BLOCKS; i++)
        currentPosition[i] = previousPosition[i];
}

void Game::move() {
    // make copy of the current shape and then move left or right 
    for (int i = 0; i < NUM_BLOCKS; i++) {
        previousPosition[i] = currentPosition[i];
        currentPosition[i].x += moveX; // no change if moveX is zero
    }
    if (collision()) undo();

    if (rotate) {
        Point p = currentPosition[1]; // center of rotation
        for (int i = 0; i < NUM_BLOCKS; i++) {
            int x = currentPosition[i].y - p.y;
            int y = currentPosition[i].x - p.x;
            currentPosition[i].x = p.x - x;
            currentPosition[i].y = p.y + y;
        }
        if (collision()) undo();
    }
}

void Game::update() {
    timer += clock.restart().asSeconds();
    if (timer > delay) {
        timer = 0;
        for (int i = 0; i < NUM_BLOCKS; i++) {
            previousPosition[i] = currentPosition[i];
            currentPosition[i].y++;
        }
        if (collision()) {
            for (int i = 0; i < NUM_BLOCKS; i++) {
                field[previousPosition[i].y][previousPosition[i].x] = tileColor;
            }
            currentShape = rand() % NUM_SHAPES;
            tileColor = currentShape;
            // tileColor = rand() % NUM_TILE_COLORS;
            for (int i = 0; i < NUM_BLOCKS; i++) {
                currentPosition[i].x = shapes[currentShape][i] % 2;
                currentPosition[i].y = shapes[currentShape][i] / 2;
            }
        }
    }
}

void Game::clearCompleted() {
    // check for completed lines
    int k = FIELD_HEIGHT - 1;
    for (int i = FIELD_HEIGHT - 1; i > 0; i--) {
        int count = 0;
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (field[i][j] != -1) count++;
            // if k equals i, then the blocks will stay as they are
            // but after a row completes, i will be "above" k and so completed rows will get overwritten
            field[k][j] = field[i][j];
        }
        if (count < FIELD_WIDTH) {
            k--; // when row is not completed, decrement k to keep it in sync with i
        } else {
            // when row is completed, k does not decrement, so that blocks from the row above will overwrite the completed row
            sound->play(); // play a sound effect
        }
    }
}

void Game::draw() {
    window.clear(sf::Color::White);
    window.draw(backgroundSprite);
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (field[i][j] == -1) continue;
            tileSprite.setTextureRect(sf::IntRect(field[i][j] * SHAPE_SIZE, 0, SHAPE_SIZE, SHAPE_SIZE));
            tileSprite.setPosition(j * SHAPE_SIZE + FIELD_HORZ_OFFSET, i * SHAPE_SIZE + FIELD_VERT_OFFSET);
            window.draw(tileSprite);

        }
    }
    for (int i = 0; i < NUM_BLOCKS; i++) {
        tileSprite.setTextureRect(sf::IntRect(tileColor * SHAPE_SIZE, 0, SHAPE_SIZE, SHAPE_SIZE));
        tileSprite.setPosition(currentPosition[i].x * SHAPE_SIZE + FIELD_HORZ_OFFSET, currentPosition[i].y * SHAPE_SIZE + FIELD_VERT_OFFSET);
        window.draw(tileSprite);
    }
    window.display();
}
