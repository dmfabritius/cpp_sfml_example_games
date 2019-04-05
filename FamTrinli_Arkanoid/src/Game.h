#pragma once
constexpr int DISPLAY_WIDTH = 520;
constexpr int DISPLAY_HEIGHT = 450;

enum EntityType { SILVER, BLUE, GREEN, RED, CYAN, BLOCK, PADDLE, BALL, BACKGROUND };
#include "Entity.h"
#include "Ball.h"
#include "Block.h"
#include "Paddle.h"

class Game {
public:
    sf::RenderWindow window;
    sf::Music music;
    sf::SoundBuffer buffer;
    sf::Sound blip;
    std::unordered_map<EntityType, sf::Texture> textures;
    std::unordered_map<EntityType, sf::Sprite> sprites;
    std::list<Entity*> entities; // all the game objects to be updated and drawn during each game cycle
    Paddle* paddle; // the paddle is just one of the entites, but it's useful to have a direct reference to it
    Ball* ball;

    Game() {
        window.create(sf::VideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT), "Ark*noid! SFML v2.5.1");
        window.setFramerateLimit(60);

        loadSounds();
        loadTextures();
        createSprites();

        paddle = new Paddle(sprites[PADDLE]);
        ball = new Ball(sprites[BALL]);
        entities.push_front(paddle);
        entities.push_front(ball);
        for (int x = 1; x < 11; x++)
            for (int y = 1; y < 10; y++) {
                EntityType color = static_cast<EntityType>(y % 5); // make it colorful
                entities.push_front(new Block(sprites[color], x * 43.0f, y * 26.0f));
            }
    }
    ~Game() {
        for (Entity* e : entities) delete e; // deallocate any remaining entities
    }
    void run() {
        while (window.isOpen()) {
            events();
            update();
            draw();
        }
    }
    void loadSounds() {
        music.openFromFile("audio/solveThePuzzle.ogg");
        music.setLoop(true);
        music.setVolume(20); // lower volume to 20%
        music.play();

        buffer.loadFromFile("audio/blip.wav");
        blip.setBuffer(buffer);
    }
    void loadTextures() {
        textures[BACKGROUND].loadFromFile("images/background.jpg");
        textures[BALL].loadFromFile("images/ball.png");
        textures[BLOCK].loadFromFile("images/blocks.png");
        textures[PADDLE].loadFromFile("images/paddle.png");
    }
    void createSprites() {
        sprites[BACKGROUND].setTexture(textures[BACKGROUND]);
        sprites[BALL].setTexture(textures[BALL]);
        sprites[PADDLE].setTexture(textures[PADDLE]);
        // textures can be re-used for multiple sprites; define which parts of the texture to use for each sprite
        for (int i = SILVER; i <= CYAN; i++)
            sprites[(EntityType)i] = sf::Sprite(textures[BLOCK], sf::IntRect(i * 42, 0, 42, 20));
    }
    void events() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
    }
    void update() {
        paddle->update();
        ball->update();
        for (Entity* e : entities) {
            if (e->type != BALL && ball->sprite.getGlobalBounds().intersects(e->sprite.getGlobalBounds())) {
                if (e->type == PADDLE) {
                    if (ball->dy > 0) ball->dy = -ball->dy; // make the ball go up
                    ball->y -= 12; // tweak ball position to be above the paddle
                } else if (e->type == BLOCK) {
                    if (ball->dy < 0) ball->dy = -ball->dy; // make the ball go down
                    entities.remove(e);
                    delete e; // deallocate the block's memory
                }
                blip.play();
                break; // once the ball hits something, stop checking
            }
        }
    }
    void draw() {
        window.draw(sprites[BACKGROUND]);
        for (Entity* e : entities) e->draw(window);
        window.display();
    }
};
