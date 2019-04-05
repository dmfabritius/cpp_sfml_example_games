#include "pch.h"
#include "Game.h"

Game::Game() {
    window.create(sf::VideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT), "T*tris! SFML v2.5.1");

    background.loadFromFile("images/background.png");
    tiles.loadFromFile("images/tiles.png");

    top = sf::Sprite(background, sf::IntRect(0, 0, 320, 95));
    bottom = sf::Sprite(background, sf::IntRect(0, 95, 320, 385));
    bottom.setPosition(0, 95);
    tile.setTexture(tiles);

    music.openFromFile("audio/solveThePuzzle.ogg");
    music.setLoop(true);
    music.setVolume(25); // lower volume to 25%
    music.play();

    buffer.loadFromFile("audio/blip.wav"); // sound effect for when a row gets completed
    sound.setBuffer(buffer);

    std::memset(field, -1, sizeof(field)); // initialize the field to be empty (no colored tiles)

    addShape();
}

void Game::addShape() {
    currentShape = rand() % NUM_SHAPES; // pick a shape
    shapeColor = currentShape; // a given shape always gets the same color
    // shapeColor = rand() % NUM_TILE_COLORS; // shapes get random colors

    for (int i = 0; i < NUM_TILES; i++) { // initialize the current shape's position from the shapes table
        currentPosition[i].x = shapes[currentShape][i] % 2;
        currentPosition[i].y = (shapes[currentShape][i] / 2) - 4; // start above the field
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
            switch (event.key.code) {
                case sf::Keyboard::Left:   moveX = -1; break;
                case sf::Keyboard::Right:  moveX = 1; break;
                case sf::Keyboard::Up:     rotate = true; break;
                case sf::Keyboard::Escape: window.close(); break;
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05f; // make shape move quickly
}

bool Game::collision() {
    for (int i = 0; i < NUM_TILES; i++) {
        if (currentPosition[i].x < 0 || currentPosition[i].x >= FIELD_WIDTH ||
            currentPosition[i].y >= FIELD_HEIGHT)
            return true;
        if (currentPosition[i].y >= 0 && field[currentPosition[i].y][currentPosition[i].x] != -1)
            return true;
    }
    return false;
}

void Game::undo() {
    for (int i = 0; i < NUM_TILES; i++)
        currentPosition[i] = previousPosition[i];
}

void Game::move() {
    // make copy of the current shape and then move left or right 
    for (int i = 0; i < NUM_TILES; i++) {
        previousPosition[i] = currentPosition[i];
        currentPosition[i].x += moveX; // no change if moveX is zero
    }
    if (collision()) undo();

    if (rotate) {
        Point p = currentPosition[1]; // center of rotation
        for (int i = 0; i < NUM_TILES; i++) {
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
        for (int i = 0; i < NUM_TILES; i++) {
            previousPosition[i] = currentPosition[i]; // save where we were
            currentPosition[i].y++; // try moving down one row
        }
        if (collision()) { // after moving down one row, check to see if we hit anything
            for (int i = 0; i < NUM_TILES; i++) { // stick the current shape onto the field
                field[previousPosition[i].y][previousPosition[i].x] = shapeColor;
            }
            addShape(); // create a new shape and start over again at the top of the field
        }
    }
}

void Game::clearCompleted() { // check for completed lines
    int k = FIELD_HEIGHT - 1;
    for (int i = FIELD_HEIGHT - 1; i > 0; i--) {
        bool completed = true;
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (field[i][j] == -1) completed = false; // any unfilled tile means the row is not complete
            // if k equals i, then the tiles will stay as they are
            // but after a row completes, i will be "above" k and so completed rows will get overwritten
            field[k][j] = field[i][j];
        }
        if (completed) {
            // when row is completed, k does not decrement, so that tiles from the row above will overwrite the completed row
            sound.play(); // play a sound effect
        } else {
            k--; // when row is not completed, decrement k to keep it in sync with i
        }
    }
}

void Game::draw() {
    window.clear(sf::Color::White);
    window.draw(bottom); // tiles are drawn "above" this background
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            if (field[i][j] == -1) continue;
            tile.setTextureRect(sf::IntRect(field[i][j] * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            tile.setPosition(j * TILE_SIZE + FIELD_HORZ_OFFSET, i * TILE_SIZE + FIELD_VERT_OFFSET);
            window.draw(tile);
        }
    }
    for (int i = 0; i < NUM_TILES; i++) {
        tile.setTextureRect(sf::IntRect(shapeColor * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
        tile.setPosition(currentPosition[i].x * TILE_SIZE + FIELD_HORZ_OFFSET, currentPosition[i].y * TILE_SIZE + FIELD_VERT_OFFSET);
        window.draw(tile);
    }
    window.draw(top); // this background is drawn "above" the tiles (hiding them from view)
    window.display();
}
