#pragma once
#include <SFML/Graphics.hpp>
#include "iostream"
#include <vector>
#include <list>
#include "level.hpp"
#include "anim.hpp"
#include "interaction_with_enemy.h"
#include "audio.h" 
#include "init_struct.h"
#include "var.h"

using namespace sf;

void definition_of_the_enemy(std::list<Entity*>::iterator &it, PLAYER &Player, AnimationManager &anim_bullet, std::list<Entity*> &entities, Level &lvl, float &time, Sound &shoot, int &bossHp)
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
	if ((*it)->Name == "enemy_obiwan")
	{
		interaction_with_enemy_obiwan(Player, it, lvl, time);
		bossHp = (*it)->Health;
		if ((*it)->Health <= 0) { Player.isWin = true; }
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
		if (Player.mana == 1 && Player.isDoSupAttack)
		{
			if (Player.dir == 1) entities.push_back(new light(anim_light, lvl, float(Player.x), float(Player.y + 18), 1));
			else entities.push_back(new light(anim_light, lvl, float(Player.x), float(Player.y + 18), 0));
			Player.isDoSupAttack = false;
		}
	}
	for (std::list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)
	{
		if ((*it2)->Name == "light")
			if ((*it2)->life == true)
			{
				if ((*it2)->getRect().intersects((*it)->getRect()) && (*it)->Health > 0)
				{
					if (((*it)->Name == "enemy_clon") || ((*it)->Name == "enemy_droid") || (((*it)->Name == "enemy_clon") && ((*it)->Name == "enemy_droid")))
					{
						(*it)->Health -= 100;
						(*it2)->Health = 0;
					}
					if ((*it)->Name == "enemy_obiwan" && !(*it)->hit)
					{
						(*it)->Health -= 50;
						(*it)->hit = true;
						(*it2)->Health = 0;
					}
				}
			}
	}
}

void after_dead(std::list<Entity*> &entities, std::list<Entity*>::iterator &it, Level lvl, AnimationManager anim_mana, int &dead_enemy, Sound &music)
{
	
	randBonus = rand() % 15;
	if (randBonus == 1 && (*it)->giveBonus == true)
	{
		entities.push_back(new Bonus(anim_mana, lvl, "Mana", float((*it)->x), float((*it)->y))); 
	}
	(*it)->giveBonus = false;
	is_life_it(it, music);
}

void check_obj(std::list<Entity*>::iterator &it, PLAYER &Player, AnimationManager &anim_bullet, std::list<Entity*> &entities, Level &lvl, float &time, Sound &shoot, int &bossHp, AnimationManager &anim_light, int levelNumber)
{
	for (it = entities.begin();it != entities.end();it++)
	{
		player_sup_attack(it, Player, anim_light, entities, lvl, time);

		definition_of_the_enemy(it, Player, anim_bullet, entities, lvl, time, shoot, bossHp);

		definition_of_the_platform(it, Player, entities, lvl, time);

		take_bonus(it, Player, lvl, levelNumber);

		if ((*it)->Health <= 0) continue;
	}
}

void is_enemy_alive(std::list<Entity*>::iterator &it, std::list<Entity*> &entities, float &time, Level lvl, AnimationManager anim_mana, int &dead_enemy, init_sounds &sound)
{
	for (it = entities.begin();it != entities.end();)
	{
		Entity *b = *it;
		b->update(time);
		if (!(b->Health > 0))
		{
			if ((*it)->Name == "enemy_clon")
			{
				after_dead(entities, it, lvl, anim_mana, dead_enemy, sound.deathClon);
			}
			if ((*it)->Name == "enemy_droid")
			{
				after_dead(entities, it, lvl, anim_mana, dead_enemy, sound.deathDroid);
			}
			if ((*it)->Name == "enemy_obiwan")
			{
				after_dead(entities, it, lvl, anim_mana, dead_enemy, sound.deadObi);
			}
		}
		if (b->life == false) 
		{ 	
			if (((*it)->Name == "enemy_droid") || ((*it)->Name == "enemy_clon") || ((*it)->Name == "enemy_droid") && ((*it)->Name == "enemy_clon"))
			{
				dead_enemy++;
				if (randBonus == 1)
				entities.push_back(new Bonus(anim_mana, lvl, "Mana", float((*it)->x), float((*it)->y)));
			}
			it = entities.erase(it);	
			delete b;	
		}
		else it++;
	}
}