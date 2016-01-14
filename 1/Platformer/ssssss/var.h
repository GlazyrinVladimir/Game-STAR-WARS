#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

static sf::Vector2f WINDOW_SIZE_GAME = { 1500, 800 };
int enemiesNumber = 0;
bool wasInMenu = false;
int randBonus;
float attack_timer = 0;
int dead_enemy = 0;
int bossHp = 300;
int levelNumber = 1;