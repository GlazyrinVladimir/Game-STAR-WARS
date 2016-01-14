#pragma once
#include <SFML/Graphics.hpp>
#include "iostream"
#include <vector>
#include <list>

using namespace sf;

void checking_key(PLAYER &Player)
{
	if (Keyboard::isKeyPressed(Keyboard::Left)) Player.key["L"] = true;
	if (Keyboard::isKeyPressed(Keyboard::Right)) Player.key["R"] = true;
	if (Keyboard::isKeyPressed(Keyboard::Up)) Player.key["Up"] = true;
	if (Keyboard::isKeyPressed(Keyboard::Down)) Player.key["Down"] = true;
	if (Keyboard::isKeyPressed(Keyboard::Space)) Player.key["Space"] = true;
	if (Keyboard::isKeyPressed(Keyboard::LAlt)) Player.key["Alt"] = true;
}