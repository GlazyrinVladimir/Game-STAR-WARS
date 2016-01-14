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
		if (Keyboard::isKeyPressed(Keyboard::A)) Player.key["L"] = true;
		if (Keyboard::isKeyPressed(Keyboard::D)) Player.key["R"] = true;
		if (Keyboard::isKeyPressed(Keyboard::W)) Player.key["Up"] = true;
		if (Keyboard::isKeyPressed(Keyboard::S)) Player.key["Down"] = true;
		if (Keyboard::isKeyPressed(Keyboard::J)) { if (attack_timer > 1500) { Player.key["Space"] = true; saber.play(); attack_timer = 0; } else attack_timer += time; }
		else attack_timer += time;
		if (Keyboard::isKeyPressed(Keyboard::K)) { Player.key["Alt"] = true; }
		if (Keyboard::isKeyPressed(Keyboard::L)) { Player.key["Ctrl"] = true; }
		if (Keyboard::isKeyPressed(Keyboard::L) && !Player.isChoke && Player.dy == 0)
		{
			Player.chokeTimer += time;  if (Player.chokeTimer > 100) { Player.chokeTimer = 0;  Player.isChoke = true; }
		}
		if (Keyboard::isKeyPressed(Keyboard::Q)) { Player.key["Q"] = true; }
		if (Keyboard::isKeyPressed(Keyboard::Q) && !Player.shieldActivation)
		{
			Player.shieldOnTimer += time;  if (Player.shieldOnTimer > 100) { Player.shieldOnTimer = 0;  Player.shieldActivation = true; }
		}
		
	}
}