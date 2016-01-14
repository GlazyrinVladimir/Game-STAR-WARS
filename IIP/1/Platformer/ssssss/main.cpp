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
#include "interaction_with_enemy.h"
#include "definition_with_objects.h"
#include "check_key.h"
#include "View.h"
#include "init_struct.h"

using namespace sf;

void menu(RenderWindow & window, Music &musicInMenu)
{
	init_texture textu;
	bool kek = false;

	musicInMenu.play();
	
	bool isMenu = 1;
	int menuNum = 0;

	while (isMenu)
	{
		textu.menu1.setColor(Color::White);
		textu.menu2.setColor(Color::White);
		textu.menu3.setColor(Color::White);

		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { textu.menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { textu.menu3.setColor(Color::Blue); menuNum = 3; }
		if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) { textu.menu2.setColor(Color::Blue); menuNum = 2; }
		
		window.draw(textu.menuBg);
		window.draw(textu.menu1);
		window.draw(textu.menu2);
		window.draw(textu.menu3);
		if (kek) { window.draw(textu.training); if (IntRect(1200, 90, 50, 50).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left)) { kek = false; } }
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;
			if (menuNum == 2) { window.close(); isMenu = false; }
			if (menuNum == 3) { kek = true;  }
		}

		window.display();
	}
}

void draw_game(RenderWindow &window, Level &lvl, std::list<Entity*>::iterator &it, std::list<Entity*> &entities, PLAYER &Player, PlayerScores &plScores, int levelNumber)
{
	init_font font;
	window.clear(Color(107, 140, 255));

	lvl.Draw(window);

	for (it = entities.begin();it != entities.end();it++)
		(*it)->draw(window);

	Player.draw(window);
	plScores.draw(window, levelNumber);

	if (Player.isWin)
	{
		font.textFinish.setPosition(view.getCenter().x - 120, view.getCenter().y);
		window.draw(font.textFinish);
		font.textWin.setPosition(view.getCenter().x - 200, view.getCenter().y- 20);
		window.draw(font.textWin);
	}
	else
	if (!Player.life)
	{
		font.textFinish.setPosition(view.getCenter().x - 120, view.getCenter().y);
		window.draw(font.textFinish);
		font.textDead.setPosition(view.getCenter().x - 40, view.getCenter().y - 20);
		window.draw(font.textDead);
	}
	window.display();
}

bool playing(RenderWindow &window,Clock &clock, PLAYER &Player, std::list<Entity*>::iterator &it, std::list<Entity*> &entities,  Level &lvl, int &levelNumber)
{
	init_sounds sound;
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
	bool playDeadSound = true;

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

		is_enemy_alive(it, entities, time, lvl, texture.anim_mana, dead_enemy, sound.deathClon, sound.deathDroid);

		if (Player.life == true)	

		for (it = entities.begin();it != entities.end();it++)
		{
			player_sup_attack(it, Player, texture.anim_light, entities, lvl, time);

			definition_of_the_enemy(it, Player, texture.anim_bullet, entities, lvl, time, sound.shoot);

			definition_of_the_platform(it, Player, entities, lvl, time);

			take_bonus(it, Player, lvl, levelNumber);
		
			if (((*it)->Name == "enemy_obiwan"))
			{
				if ((*it)->Health <= 0 && playDeadSound)
				{
					playDeadSound = false;
					sound.levelThird.stop();
					sound.deadObi.play();
				}
				else bossHp = (*it)->Health;
			}
			if ((*it)->Health <= 0) continue;			
		}
		else { if (Player.isPlaySound) { sound.musicInGame.stop(); sound.levelThird.stop(); sound.deadEnakin.play(); Player.isPlaySound = false; } }

		getPlayerCoordinateForView(Player.x, Player.y, window, levelNumber);

		draw_game(window, lvl, it, entities, Player, plScores, levelNumber);

	}
	return 0;
}

void change_level(Level &lvl, int &levelNumber)
{
	if (levelNumber == 1) { lvl.LoadFromFile("first_level.tmx"); }
	if (levelNumber == 2) { lvl.LoadFromFile("second_level.tmx"); }
	if (levelNumber == 3) { lvl.LoadFromFile("third_level.tmx"); }
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

	PLAYER Player(texture.anim_player, lvl, float(pl.rect.left), float(pl.rect.top));

	Clock clock;

	return playing(window, clock, Player, it, entities, lvl, levelNumber);
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



