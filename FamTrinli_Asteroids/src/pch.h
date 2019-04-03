#pragma once

constexpr float PI = 3.14159265359f;
constexpr float TWO_PI = PI * 2.0f;
constexpr float DEG2RAD = TWO_PI / 360.0f;

constexpr int DISPLAY_WIDTH = 1200;
constexpr int DISPLAY_HEIGHT = 800;

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <unordered_map>
#include <random>
#include <time.h>
#include <algorithm>

// SFML v2.5.1
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
// TGUI v0.8.4
#include <TGUI/TGUI.hpp>

// a little global helper function to return a random number as a float
static inline float randf(int n) {
    return static_cast<float>(rand() % n); // number between 0 and n-1
}
