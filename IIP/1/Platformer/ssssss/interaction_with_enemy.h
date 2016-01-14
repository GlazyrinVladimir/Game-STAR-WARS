#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "iostream"
#include <vector>
#include <list>
#include "level.hpp"
#include "anim.hpp"
#include "classes.h"
#include "var.h"
#include "init_anim_and_obj.h"

using namespace sf;

void interaction_with_enemy_clon(PLAYER &Player, AnimationManager &anim2, std::list<Entity*> &entities, std::list<Entity*>::iterator &it, Level &lvl, float &time, Sound &shoot)
{
	double distance;

	if (Keyboard::isKeyPressed(Keyboard::Space))
	{

		if (Player.getRectAttLeft().intersects((*it)->getRect()) || Player.getRectAttRight().intersects((*it)->getRect()))
		{
			(*it)->dx = 0;
			(*it)->Health = 0;
			(*it)->hit = true;
		}

		if (Player.getRect().intersects((*it)->getRect()))
		{
			(*it)->dx = 0;
			(*it)->Health = 0;
			(*it)->hit = true;
		}
	}

	distance = sqrt((Player.x - (*it)->x)*(Player.x - (*it)->x));

	if ((distance < 240) && (((*it)->y + 50 >= Player.y) && ((*it)->y - 95 <= Player.y))) { (*it)->shootTime_clon += 0.005*time; }

	if ((distance < 240) && (((*it)->y + 50 >= Player.y) && ((*it)->Health > 0) && ((*it)->y - 95 <= Player.y)) && Player.Health > 0)
	{
		if ((*it)->shootTime_clon > 6)
		{
			if (Player.x < (*it)->x) {
				
				(*it)->dir = 0;
				 entities.push_back(new Bullet(anim2, lvl, float((*it)->x), float((*it)->y + 18), 1));
				 shoot.play();
				//std::cout << "new (*it)->x" << (*it)->pos_Player << "\n";
				 
			}
			else
			{
				(*it)->dir = 1;
				//std::cout << "new (*it)->x" << (*it)->pos_Player << "\n";
				  entities.push_back(new Bullet(anim2, lvl, float((*it)->x), float((*it)->y + 18), 0));
				  shoot.play();
			}
			(*it)->shootTime_clon = 0;
			(*it)->dx = 0;
		}
	}
	else
		if ((*it)->dx == 0)
		{
			(*it)->dx = float(0.02);
			(*it)->timer = 0;
		}

	for (std::list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)
	{
		Entity *bullet = *it2;
		if ((*it2)->Name == "Bullet")
			if ((*it2)->life == true)
			{
				if ((*it2)->getRect().intersects(Player.getRect()))
				{
					if (Keyboard::isKeyPressed(Keyboard::Down))
					{
						(*it2)->repulse = true;
					}
					else
					{
						if (Player.Health > 0)
						{
							(*it2)->life = false;
							Player.Health -= 5;
							Player.hit = true;
						}
					}
				}
				if (((*it2)->getRect().intersects((*it)->getRect())) && ((*it2)->repulse == true))
				{
					(*it2)->repulse = false;
					(*it2)->life = false;
					(*it)->Health -= 5;
					(*it)->hit = true;
				}
			}
	}
}

void interaction_with_enemy_droid(PLAYER &Player, AnimationManager &anim2, std::list<Entity*> &entities, std::list<Entity*>::iterator &it, Level &lvl, float &time, Sound &shoot)
{

	double distance;

	if (Keyboard::isKeyPressed(Keyboard::Space))
	{

		if ((Player.getRectAttLeft().intersects((*it)->getRect()) || Player.getRectAttRight().intersects((*it)->getRect())) && ((*it)->hit == false))
		{
			(*it)->dx = 0;
			(*it)->Health -= 10;
			(*it)->hit = true;
		}
		if (Player.getRect().intersects((*it)->getRect()) && ((*it)->hit == false))
		{
			(*it)->dx = 0;
			(*it)->Health -= 10;
			(*it)->hit = true;
		}
	}

	distance = sqrt((Player.x - (*it)->x)*(Player.x - (*it)->x));

	if ((distance < 170) && (((*it)->y + 50 >= Player.y) && ((*it)->y - 105 <= Player.y))) { (*it)->shootTime_clon += 0.005*time; }

	if ((distance < 170) && (((*it)->y + 50 >= Player.y) && ((*it)->Health > 0) && ((*it)->y - 105 <= Player.y)) && Player.Health > 0)
	{
		
		if ((*it)->shootTime_clon > 4)
		{
			if (Player.x < (*it)->x) {

				(*it)->dir = 0;
				(*it)->dx = 0;
				entities.push_back(new Bullet(anim2, lvl, float((*it)->x), float((*it)->y + 18), 1));
				shoot.play();
				//std::cout << "new (*it)->x" << (*it)->pos_Player << "\n";

			}
			else
			{
				(*it)->dir = 1;
				(*it)->dx = 0;
				//std::cout << "new (*it)->x" << (*it)->pos_Player << "\n";
				entities.push_back(new Bullet(anim2, lvl, float((*it)->x), float((*it)->y + 18), 0));
				shoot.play();
			}
			(*it)->shootTime_clon = 0;

		}
	}
	else
		if ((*it)->dx == 0)
		{
			(*it)->dx = float(0.04);
			(*it)->timer = 0;
		}

	for (std::list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)
	{
		Entity *bullet = *it2;
		if ((*it2)->Name == "Bullet")
			if ((*it2)->life == true)
			{
				if ((*it2)->getRect().intersects(Player.getRect()))
				{
					if (Keyboard::isKeyPressed(Keyboard::Down))
					{
						(*it2)->repulse = true;
					}
					else
					{
						if (Player.Health > 0)
						{
							(*it2)->life = false;
							Player.Health -= 5;
							Player.hit = true;
						}
					}
				}
				if (((*it2)->getRect().intersects((*it)->getRect())) && ((*it2)->repulse == true))
				{
					(*it2)->repulse = false;
					(*it2)->life = false;
					(*it)->hit = true;
				}
			}
	}
}

void interaction_with_enemy_obiwan(PLAYER &Player, std::list<Entity*>::iterator &it, Level &lvl, float &time)
{
	double distance;

	distance = sqrt((Player.x - (*it)->x)*(Player.x - (*it)->x));

	if (Player.x <= (*it)->x && !(*it)->doHit && Player.Health > 0 && (*it)->Health >= 0)
	{
		(*it)->dir = 0;
		(*it)->dx = float(-0.04);
		if (((*it)->getRectAttLeft()).intersects(Player.getRect()) && !(Player.hit))
		{
			(*it)->dx = 0;
			(*it)->doHit = true;
			if ((*it)->boss_do_attack && (((*it)->getRectAttLeft()).intersects(Player.getRect()) && !(Player.hit)))
			{
				Player.Health -= 10;
				Player.hit = true;
			}
		}
	}
	else
		if (Player.Health > 0 && (*it)->Health >= 0)

		{
			if (!(*it)->doHit)
			{
				(*it)->dir = 1;
				(*it)->dx = float(0.04);
			}

			if (((*it)->getRect()).intersects(Player.getRect()) && !(Player.hit))
			{
				(*it)->dx = 0;
				(*it)->doHit = true;
				if ((*it)->boss_do_attack && (((*it)->getRectAttRight()).intersects(Player.getRect()) && !(Player.hit)))
				{

					Player.Health -= 10;
					Player.hit = true;
				}
				else (*it)->boss_do_attack = false;


			}
			if (((Player.getRect().intersects((*it)->getRect()) || Player.getRectAttRight().intersects((*it)->getRect())) || Player.getRect().intersects((*it)->getRect())) && ((*it)->hit == false) && Keyboard::isKeyPressed(Keyboard::Space))
			{
				(*it)->dx = 0;
				(*it)->Health -= 10;
				(*it)->hit = true;
			}
			else (*it)->boss_do_attack = false;
		}
		else
			if ((*it)->Health > 0)
			{
				(*it)->boss_do_attack = false;
				(*it)->doHit = false;
				(*it)->dir = 1;
				(*it)->dx = float(0.03);
			}
}

