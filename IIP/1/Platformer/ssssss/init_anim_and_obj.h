#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "level.hpp"
#include "classes.h"
#include "anim.hpp"
#include "init_struct.h"

using namespace sf;



void init_map_objects(std::vector<Object> &e, std::list<Entity*> &entities, Level &lvl, int &enemiesNumber, init_texture texture)
{
	
	e = lvl.GetObjects("enemy_clon");
	for (unsigned int i = 0;i < e.size();i++)
	{
		enemiesNumber += 1;
		entities.push_back(new ENEMY_CLON(texture.anim_clon_enemy, "enemy_clon", lvl, float(e[i].rect.left), float(e[i].rect.top)));
	}

	e = lvl.GetObjects("enemy_droid");
	for (unsigned int i = 0;i < e.size();i++)
	{
		enemiesNumber += 1;
		entities.push_back(new ENEMY_CLON(texture.anim_droid_enemy, "enemy_droid", lvl, float(e[i].rect.left), float(e[i].rect.top)));
	}

	e = lvl.GetObjects("obiwan");
	for (unsigned int i = 0;i < e.size();i++)
		entities.push_back(new ENEMY_CLON(texture.anim_obiwan, "enemy_obiwan", lvl, float(e[i].rect.left), float(e[i].rect.top)));

	e = lvl.GetObjects("MovingPlatform");
	for (unsigned int i = 0;i < e.size();i++)
		entities.push_back(new MovingPlatform(texture.anim4, lvl, "MovingPlatform", float(e[i].rect.left), float(e[i].rect.top)));

	e = lvl.GetObjects("Up");
	for (unsigned int i = 0;i < e.size();i++)
		entities.push_back(new MovingPlatform(texture.anim4, lvl, "Up", float(e[i].rect.left), float(e[i].rect.top)));

	e = lvl.GetObjects("health");
	for (unsigned int i = 0;i < e.size();i++)
		entities.push_back(new Bonus(texture.anim_health, lvl, "Health", float(e[i].rect.left), float(e[i].rect.top)));
}



