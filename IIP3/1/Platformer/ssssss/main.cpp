#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "iostream"
#include <vector>
#include <list>
#include "level.hpp"
#include "anim.hpp"
#include "classes.h"
#include "var.h"
#include "init_obj.h"
#include "interaction_with_enemy.h"
#include "definition_with_objects.h"
#include "check_key.h"
#include "View.h"
#include "init_struct.h"
#include "menu.h"
#include "draw.h"


using namespace sf;

bool playing(RenderWindow &window,Clock &clock, PLAYER &Player, std::list<Entity*>::iterator &it, std::list<Entity*> &entities,  Level &lvl, int &levelNumber, init_sounds &sound)
{
	init_texture texture;

	PlayerScores plScores;

	if (levelNumber == 1 || levelNumber == 2)
	{
		sound.musicInGame.play();
		sound.musicInGame.setLoop(true);
	}
	else {
		sound.musicInGame.stop();
		sound.levelThird.play();
		sound.levelThird.setLoop(true);
	}

	float attack_timer = 0;
	int dead_enemy = 0;
	int bossHp = 200;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 500;
		if (time > 40) time = 40;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		
		if (!Player.life || Player.isWin) 
		{
			
			if (Keyboard::isKeyPressed(Keyboard::Tab)) { levelNumber = 0; return true;  }
		
			if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }
			
		}
		if ((dead_enemy == 8 || dead_enemy >= 8) && levelNumber == 1) { return true; }
		
		if (Player.levelNumber == 1) { return true; }
		

		checking_key(Player, sound.saber, time, attack_timer);
		
		plScores.update(Player.Health, Player.mana, bossHp);

		Player.update(time);

		is_enemy_alive(it, entities, time, lvl, texture.anim_mana, dead_enemy, sound);

		if (Player.life == true)

			check_obj(it, Player, texture.anim_bullet, entities, lvl, time, sound.shoot, bossHp, texture.anim_light, levelNumber);

		else { if (Player.isPlaySound) { sound.musicInGame.stop(); sound.levelThird.stop(); sound.deadEnakin.play(); Player.isPlaySound = false; } }

		getPlayerCoordinateForView(Player.x, Player.y, window, levelNumber);

		draw_game(window, lvl, it, entities, Player, plScores, levelNumber);

	}
	return 0;
}



bool start_game(int &levelNumber, RenderWindow &window)
{

	init_sounds sound; 
	init_texture texture;

	Level lvl;
	change_level(lvl, levelNumber);

	if (levelNumber == 1 && !wasInMenu) 
	{
		wasInMenu = true; menu(window, sound.musicInMenu);
	}

	sound.musicInMenu.stop();

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;

	std::vector<Object> e;

	init_map_objects(e, entities, lvl, enemiesNumber, texture);
	Object pl = lvl.GetObject("player");

	PLAYER Player(texture.anim_player, lvl, int(pl.rect.left), int(pl.rect.top));

	Clock clock;

	return playing(window, clock, Player, it, entities, lvl, levelNumber, sound);
}

void game_Running(int &levelNumber, RenderWindow &window)
{
	if (start_game(levelNumber, window)) { levelNumber += 1; game_Running(levelNumber, window ); }
}

int main()
{
	RenderWindow window(VideoMode(WINDOW_SIZE_GAME.x, WINDOW_SIZE_GAME.y), "STAR WARS");
	
	int levelNumber = 1;
	
	game_Running(levelNumber, window);
	return 0;
}



