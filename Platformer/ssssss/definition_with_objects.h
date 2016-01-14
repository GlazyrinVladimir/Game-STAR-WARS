#pragma once
#include <SFML/Graphics.hpp>
#include "iostream"
#include <vector>
#include <list>
#include "level.hpp"
#include "anim.hpp"
#include "init_anim_and_obj.h"
#include "interaction_with_enemy.h"

using namespace sf;

void definition_of_the_enemy(std::list<Entity*>::iterator &it, PLAYER &Player, AnimationManager &anim_bullet, std::list<Entity*> &entities, Level &lvl, float &time, Sound &shoot)
{
	Entity *enemy = *it;

	if ((*it)->Name == "enemy_clon")
	{
		interaction_with_enemy_clon(Player, anim_bullet, entities, it, lvl, time, shoot);
	}
	if ((*it)->Name == "enemy_droid")
	{
		interaction_with_enemy_droid(Player, anim_bullet, entities, it, lvl, time, shoot);
	}
}

void definition_of_the_platform(std::list<Entity*>::iterator &it, PLAYER &Player, std::list<Entity*> &entities, Level &lvl, float &time)
{
	if ((*it)->Name == "MovingPlatform")
	{
		Entity *movPlat = *it;
		if (Player.getRect().intersects(movPlat->getRect()))
			if (Player.dy > 0)
				if (Player.y + Player.h < movPlat->y + movPlat->h)
				{
					Player.y = movPlat->y - Player.h + 3; Player.x += movPlat->dx*time; Player.dy = 0; Player.STATE = PLAYER::stay;
				}
	}

	if ((*it)->Name == "Up")
	{
		Entity *movUpPlat = *it;
		if (Player.getRect().intersects(movUpPlat->getRect()))
			if (Player.dy>0)
				if (Player.y + Player.h < movUpPlat->y + movUpPlat->h)
				{
					Player.y = movUpPlat->y - Player.h + 3; Player.y += movUpPlat->dy*time; Player.dy = 0; Player.STATE = PLAYER::stay;
				}
	}
}

void take_bonus(std::list<Entity*>::iterator &it, PLAYER &Player, std::list<Entity*> &entities, Level &lvl)
{
	if ((*it)->Name == "Health")
	{
		if (Player.Health < 100) 
		if (Player.getRect().intersects((*it)->getRect()))
		{
			(*it)->life = false;
			Player.Health += 10;
			if (Player.Health > 100) { Player.Health = 100; }
		}
	}
}

void is_enemy_alive(std::list<Entity*>::iterator &it, std::list<Entity*> &entities, float &time)
{
	for (it = entities.begin();it != entities.end();)
	{
		Entity *b = *it;
		b->update(time);
		if (b->life == false) { it = entities.erase(it); delete b; }
		else it++;
	}
}