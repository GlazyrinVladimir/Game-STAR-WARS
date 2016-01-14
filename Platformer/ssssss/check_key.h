#pragma once
#include <SFML/Graphics.hpp>
#include "iostream"
#include <vector>
#include <list>

using namespace sf;

void checking_key(PLAYER &Player, Sound &saber, float time, float &attack_timer)
{
	if (Player.Health > 0)
	{
		if (Keyboard::isKeyPressed(Keyboard::Left)) Player.key["L"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Right)) Player.key["R"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Up)) Player.key["Up"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Down)) Player.key["Down"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Space)) { if (attack_timer > 1500) { Player.key["Space"] = true; saber.play(); attack_timer = 0; } else attack_timer += time; }
		else attack_timer += time;
		if (Keyboard::isKeyPressed(Keyboard::LAlt)) Player.key["Alt"] = true;
	}
}