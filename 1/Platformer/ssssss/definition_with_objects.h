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

void take_bonus(std::list<Entity*>::iterator &it, PLAYER &Player,  Level &lvl, int &levelNumber)
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
	if ((*it)->Name == "Mana")
	{
		if (Player.mana != 1)
			if (Player.getRect().intersects((*it)->getRect()))
			{
				(*it)->life = false;
				Player.mana = 1;
			}
	}
	
}

void player_sup_attack(std::list<Entity*>::iterator &it, PLAYER &Player, AnimationManager &anim_light, std::list<Entity*> &entities, Level &lvl, float &time)
{
	if ((Keyboard::isKeyPressed(Keyboard::LAlt)) && (Player.dy == 0))
	{
		if (Player.mana == 1)
		{
			if (Player.dir == 1) entities.push_back(new light(anim_light, lvl, Player.x, Player.y + 18, 1));
			else entities.push_back(new light(anim_light, lvl, Player.x, Player.y + 18, 0));
			Player.mana = 0;
		}
	}
	for (std::list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)
	{
		if ((*it2)->Name == "light")
			if ((*it2)->life == true)
			{
				if ((*it2)->getRect().intersects((*it)->getRect()))
				{
					if (((*it)->Name == "enemy_clon") || ((*it)->Name == "enemy_droid") || (((*it)->Name == "enemy_clon") && ((*it)->Name == "enemy_droid")))
					{
						(*it)->Health -= 100;
						(*it2)->Health = 0;
					}
				}
			}
	}
}

void is_enemy_alive(std::list<Entity*>::iterator &it, std::list<Entity*> &entities, float &time, Level lvl, AnimationManager anim_mana, int &dead_enemy)
{
	int randBonus;
	for (it = entities.begin();it != entities.end();)
	{
		Entity *b = *it;
		b->update(time);
		if (b->life == false) 
		{ 	
			std::cout << "new (*it)->x" << dead_enemy << "\n";
			randBonus = rand() % 10;
			if (((*it)->Name == "enemy_droid") || ((*it)->Name == "enemy_clon") || ((*it)->Name == "enemy_droid") && ((*it)->Name == "enemy_clon"))
			{
				dead_enemy++;
				if (randBonus == 1)
				entities.push_back(new Bonus(anim_mana, lvl, "Mana", (*it)->x, (*it)->y));
			}
			it = entities.erase(it);	
			delete b;	
		}
		else it++;
	}
}