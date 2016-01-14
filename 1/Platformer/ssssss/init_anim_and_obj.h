#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "level.hpp"
#include "classes.h"

using namespace sf;

void init_map_objects(std::vector<Object> &e, std::list<Entity*> &entities, AnimationManager &anim_clon_enemy, AnimationManager &anim_droid_enemy, AnimationManager &anim4, Level &lvl, AnimationManager &anim_health, int &enemiesNumber)
{
	e = lvl.GetObjects("enemy_clon");

	for (int i = 0;i < e.size();i++)
	{
		enemiesNumber += 1;
		entities.push_back(new ENEMY_CLON(anim_clon_enemy, "enemy_clon", lvl, e[i].rect.left, e[i].rect.top));
	}

	e = lvl.GetObjects("enemy_droid");

	for (int i = 0;i < e.size();i++)
	{
		enemiesNumber += 1;
		entities.push_back(new ENEMY_CLON(anim_droid_enemy, "enemy_droid", lvl, e[i].rect.left, e[i].rect.top));
	}
	e = lvl.GetObjects("MovingPlatform");
	for (int i = 0;i < e.size();i++)
		entities.push_back(new MovingPlatform(anim4, lvl, "MovingPlatform", e[i].rect.left, e[i].rect.top));

	e = lvl.GetObjects("Up");
	for (int i = 0;i < e.size();i++)
		entities.push_back(new MovingPlatform(anim4, lvl, "Up", e[i].rect.left, e[i].rect.top));

	e = lvl.GetObjects("health");
	for (int i = 0;i < e.size();i++)
		entities.push_back(new Bonus(anim_health, lvl, "Health", e[i].rect.left, e[i].rect.top));
}

void init_texture(Texture &enemy_droid, Texture &moveplatform, Texture &anakin, Texture &fang, Texture &enemy_clon, Texture &fire, Texture &bonus, Texture &light, Texture &mana)
{
	bonus.loadFromFile("files/heart2.png");
	enemy_droid.loadFromFile("files/droid2.png");
	moveplatform.loadFromFile("files/movingPlatform.png");
	anakin.loadFromFile("files/Anakin2.png");
	fang.loadFromFile("files/fanq.png");
	enemy_clon.loadFromFile("files/enemy2.png");
	fire.loadFromFile("files/laser2.png");
	light.loadFromFile("files/light.png");
	mana.loadFromFile("files/mana.png");
}

void init_animation(AnimationManager &anim_player, AnimationManager &anim_clon_enemy, AnimationManager &anim_droid_enemy, AnimationManager &anim_bullet, AnimationManager &anim4, Texture &moveplatform, Texture &fire, Texture &fang, Texture &anakin, Texture &enemy_clon, Texture &enemy_droid, AnimationManager &anim_health, Texture &bonus, Texture &light, AnimationManager &anim_light, Texture &mana, AnimationManager &anim_mana)
{
	anim_light.loadFromXML("files/light_anim.xml", light);
	anim_player.loadFromXML("files/anim_anakin.xml", anakin);
	anim_clon_enemy.loadFromXML("files/enemy_clon_anim.xml", enemy_clon);
	anim_droid_enemy.loadFromXML("files/anim_droid.xml", enemy_droid);
	anim_bullet.create("move", fire, 0, 0, 30, 3, 1, 0);
	anim_bullet.create("explode", fang, 323, 583, 18, 18, 4, 0.01, 29, 0);
	anim4.create("move", moveplatform, 0, 0, 95, 22, 3, 0);
	anim_health.create("move", bonus, 0, 0, 20, 20, 1, 0);
	anim_mana.create("move", mana, 0, 0, 25, 25, 1, 0);
}
void init_music(Music &musicInMenu, Music &musicInGame, SoundBuffer &shootBuffer, SoundBuffer &saberBuffer)
{
	musicInMenu.openFromFile("music.ogg");
	shootBuffer.loadFromFile("lazer.ogg");
	saberBuffer.loadFromFile("saberdown.ogg");
	musicInGame.openFromFile("game.ogg");
}