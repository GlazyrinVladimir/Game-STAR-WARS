#pragma once
#include <SFML/Graphics.hpp>
#include "level.hpp"
#include "classes.h"

using namespace sf;

void init_map_objects(std::vector<Object> &e, std::list<Entity*> &entities, AnimationManager &anim_clon_enemy, AnimationManager &anim_droid_enemy, AnimationManager &anim4, Level &lvl)
{
	e = lvl.GetObjects("enemy_clon");

	for (int i = 0;i < e.size();i++)
		entities.push_back(new ENEMY_CLON(anim_clon_enemy, "enemy_clon", lvl, e[i].rect.left, e[i].rect.top, 1));

	e = lvl.GetObjects("enemy_droid");

	for (int i = 0;i < e.size();i++)
		entities.push_back(new ENEMY_CLON(anim_droid_enemy, "enemy_droid", lvl, e[i].rect.left, e[i].rect.top, 1));

	e = lvl.GetObjects("MovingPlatform");
	for (int i = 0;i < e.size();i++)
		entities.push_back(new MovingPlatform(anim4, lvl, "MovingPlatform", e[i].rect.left, e[i].rect.top));

	e = lvl.GetObjects("Up");
	for (int i = 0;i < e.size();i++)
		entities.push_back(new MovingPlatform(anim4, lvl, "Up", e[i].rect.left, e[i].rect.top));
}

void init_texture(Texture &enemy_droid, Texture &moveplatform, Texture &anakin, Texture &fang, Texture &enemy_clon, Texture &fire)
{
	enemy_droid.loadFromFile("files/droid.png");
	moveplatform.loadFromFile("files/movingPlatform.png");
	anakin.loadFromFile("files/Anakin2.png");
	fang.loadFromFile("files/fanq.png");
	enemy_clon.loadFromFile("files/enemy2.png");
	fire.loadFromFile("files/laser2.png");
}

void init_animation(AnimationManager &anim_player, AnimationManager &anim_clon_enemy, AnimationManager &anim_droid_enemy, AnimationManager &anim_bullet, AnimationManager &anim4, Texture &moveplatform, Texture &fire, Texture &fang, Texture &anakin, Texture &enemy_clon, Texture &enemy_droid)
{
	anim_player.loadFromXML("files/anim_anakin.xml", anakin);
	anim_clon_enemy.loadFromXML("files/enemy_clon_anim.xml", enemy_clon);
	anim_droid_enemy.loadFromXML("files/anim_droid.xml", enemy_droid);
	anim_bullet.create("move", fire, 0, 0, 30, 3, 1, 0);
	anim_bullet.create("explode", fang, 323, 583, 18, 18, 4, 0.01, 29, 0);
	anim4.create("move", moveplatform, 0, 0, 95, 22, 3, 0);
}