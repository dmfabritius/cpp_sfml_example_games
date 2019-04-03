#pragma once
#include "AnimatedSprite.h"
#include "Entity.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Laser.h"
#include "Explosion.h"

class Game {
private:
    sf::RenderWindow window;
    sf::Sprite background;
    sf::Music* music;
    std::unordered_map <std::string, sf::Sound*> sounds;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, AnimatedSprite> sprites;
    std::list<Entity*> entities; // all the game objects to be updated and drawn during each game cycle
    Ship* ship; // the ship is just one of the entities, but it's handy to have a direct reference to it

public:
    Game() {
        window.create(sf::VideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT), "Ast*roids! SFML v2.5.1 TGUI v0.8");
        window.setFramerateLimit(60);

        loadBackgroundMusic();
        loadSoundEffects();
        loadTextures();
        createSprites();

        background.setTexture(textures["outerspace"]); // the background doesn't need to be animated, so it's just a normal sf::Sprite
        ship = new Ship(sprites["ship"], DISPLAY_WIDTH * 0.5f, DISPLAY_HEIGHT * 0.5f);
        entities.push_front(ship);
        for (int i = 0; i < 4; i++) addEntity("asteroid_lg"); // create some initial asteroids
    }
    ~Game() {
        for (Entity* e : entities) delete e; // deallocate any remaining entities
        for (auto s : sounds) delete s.second; // deallocate all sounds and their buffers
        delete music; // deallocate background music
    }
    void run() {
        while (window.isOpen()) {
            handleEvents();
            updateEntities();
            drawEntities();
        }
    }

private:
    void loadBackgroundMusic() {
        music = new sf::Music();
        music->openFromFile("audio/solveThePuzzle.ogg");
        music->setLoop(true);
        music->setVolume(25); // lower volume to 25%
        music->play();
    }
    void loadSoundEffects() {
        sounds["bang_lg"] = loadSound("audio/bang_lg.wav");
        sounds["bang_md"] = loadSound("audio/bang_md.wav");
        sounds["bang_sm"] = loadSound("audio/bang_sm.wav");
        sounds["laser"] = loadSound("audio/laser.wav");
        sounds["thrust"] = loadSound("audio/thrust.wav");
    }
    sf::Sound* loadSound(std::string filename) {
        sf::SoundBuffer* buffer = new sf::SoundBuffer();
        buffer->loadFromFile(filename); // loading sound files is slow, so just do it once as the game starts
        return new sf::Sound(*buffer);
    }
    void loadTextures() {
        textures["asteroid"].loadFromFile("images/asteroid.png");
        textures["explosion"].loadFromFile("images/explosion.png");
        textures["laser"].loadFromFile("images/laser.png");
        textures["ship"].loadFromFile("images/ship.png");
        textures["outerspace"].loadFromFile("images/outerspace.jpg");
    }
    void createSprites() {
        // textures can be re-used for multiple sprites; define which parts of each texture to use for each sprite
        sprites["asteroid_lg"] = AnimatedSprite(textures["asteroid"], 0, 0, 80, 80, 16, 0.2f);
        sprites["asteroid_md"] = AnimatedSprite(textures["asteroid"], 0, 80, 60, 60, 16, 0.2f);
        sprites["asteroid_sm"] = AnimatedSprite(textures["asteroid"], 0, 140, 40, 40, 16, 0.2f);
        sprites["explosion_asteroid"] = AnimatedSprite(textures["explosion"], 0, 0, 256, 256, 48, 0.5f);
        sprites["explosion_ship"] = AnimatedSprite(textures["explosion"], 0, 256, 192, 192, 64, 0.5f);
        sprites["laser"] = AnimatedSprite(textures["laser"], 0, 0, 32, 48, 8, 0.4f);
        sprites["ship"] = AnimatedSprite(textures["ship"], 0, 0, 32, 67, 1, 0);
        sprites["ship_thrusting"] = AnimatedSprite(textures["ship"], 32, 0, 32, 67, 3, 0.4f);
    }
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyReleased) // make the player let go of the space bar before firing
                if (event.key.code == sf::Keyboard::Space) {
                    addEntity("laser");
                    sounds["laser"]->play();
                }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ship->angle += 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  ship->angle -= 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) addEntity("laser"); // lots 'o lasers!!!!

        ship->thrusting = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    }
    void updateEntities() {
        // set the ship's animation depending on whether or not it's thrusting
        ship->anim = (ship->thrusting) ? sprites["ship_thrusting"] : sprites["ship"];
        if (ship->thrusting && sounds["thrust"]->getStatus() != sf::Sound::Playing)
            sounds["thrust"]->play(); // play when thrusting, but not if it's already playing

        if (rand() % 400 == 0) addEntity("asteroid_lg"); // throw in another asteroid occasionally

        for (auto i = entities.begin(); i != entities.end();) {
            Entity* e = *i;
            e->anim.nextFrame(); // advance the entity's animation
            e->update(); // update the heading, velocity, and/or position of the entity

            if (e->type == "asteroid") checkCollisions(e); // we only care when asteroids hit things
            if (e->type == "explosion") e->active = !e->anim.completed(); // explosions get removed when their animation completes

            if (!e->active) {
                i = entities.erase(i); // remove entity and advance to the next one
                delete e; // deallocate the entity's memory
            } else {
                i++; // advance to the next entity
            }
        }
    }
    void drawEntities() {
        window.draw(background);
        for (Entity* e : entities) e->draw(window);
        window.display();
    }
    void addEntity(const std::string& type, float x = 0, float y = 0) {
        Entity* e;
        if (type == "laser")
            e = new Laser(sprites[type], ship->x, ship->y, ship->angle);
        else if (type == "explosion_ship")
            e = new Explosion(sprites["explosion_ship"], ship->x, ship->y);
        else if (type == "explosion_asteroid")
            e = new Explosion(sprites["explosion_asteroid"], x, y);
        else if (type == "asteroid_lg")
            e = new Asteroid(sprites["asteroid_lg"], 0, randf(DISPLAY_HEIGHT));
        else if (type == "asteroid_md")
            e = new Asteroid(sprites["asteroid_md"], x, y);
        else if (type == "asteroid_sm")
            e = new Asteroid(sprites["asteroid_sm"], x, y);

        if (type == "laser")
            entities.push_front(e); // we want the lasers to get drawn first, so the ship is on top of them
        else
            entities.push_back(e); // everything else can go to the back of the list
    }
    void checkCollisions(Entity* asteroid) {
        for (Entity* e : entities) {
            if (e->type == "laser" && collision(asteroid, e)) {
                asteroid->active = false; // the asteroid gets destroyed
                e->active = false; // the laser also gets destroyed
                addEntity("explosion_asteroid", asteroid->x, asteroid->y);
                if (asteroid->anim.r > 30) {
                    sounds["bang_lg"]->play();
                    addEntity("asteroid_md", asteroid->x, asteroid->y);
                    addEntity("asteroid_md", asteroid->x, asteroid->y);
                } else if (asteroid->anim.r > 20) {
                    sounds["bang_md"]->play();
                    addEntity("asteroid_sm", asteroid->x, asteroid->y);
                    addEntity("asteroid_sm", asteroid->x, asteroid->y);
                } else {
                    sounds["bang_sm"]->play();
                }
            } else if (e->type == "ship" && collision(asteroid, e)) {
                asteroid->active = false;
                addEntity("explosion_ship");
                sounds["bang_lg"]->play();
                ship->x = DISPLAY_WIDTH / 2;
                ship->y = DISPLAY_HEIGHT / 2;
                ship->dx = ship->dy = 0;
            }
        }
    }
    bool collision(Entity* a, Entity* b) {
        float dx = a->x - b->x; // x separation
        float dy = a->y - b->y; // y separation
        float dr = a->anim.r + b->anim.r; // sum of the two objects' radii
        return dx*dx + dy*dy < dr*dr; // it's faster if we don't bother taking the square root of both sides
    }
};
