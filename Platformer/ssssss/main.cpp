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

using namespace sf;

void menu(RenderWindow & window, Music &musicInMenu)
{
	musicInMenu.play();
	Texture menuTexture1, menuTexture2, menuBackground;
	menuTexture1.loadFromFile("files/111.png");
	menuTexture2.loadFromFile("files/333.png");
	menuBackground.loadFromFile("files/menu.png");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(75, 30);
	menu2.setPosition(75, 90);
	menuBg.setPosition(0, 0);

	while (isMenu)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;
			if (menuNum == 2) { window.close(); isMenu = false; }
		}
		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.display();
	}
}

void draw_game(RenderWindow &window,Level &lvl, std::list<Entity*>::iterator &it, std::list<Entity*> &entities, PLAYER &Player, PlayerScores &plScores)
{
	window.clear(Color(107, 140, 255));

	lvl.Draw(window);

	for (it = entities.begin();it != entities.end();it++)
		(*it)->draw(window);

	Player.draw(window);
	plScores.draw(window);

	window.display();
}

void playing(RenderWindow &window,Clock &clock,View &view,PLAYER &Player, std::list<Entity*>::iterator &it, std::list<Entity*> &entities, PlayerScores &plScores, AnimationManager &anim_bullet, Level &lvl, Sound &shoot, Sound &saber)
{
	float attack_timer = 0;
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

		checking_key(Player, saber, time, attack_timer);
		
		plScores.update(Player.Health);

		Player.update(time);

		is_enemy_alive(it, entities, time);

		for (it = entities.begin();it != entities.end();it++)
		{
			
			definition_of_the_enemy(it, Player, anim_bullet, entities, lvl, time, shoot);

			definition_of_the_platform(it, Player, entities, lvl, time);

			take_bonus(it, Player, entities, lvl);

			if ((*it)->Health <= 0) continue;			
		}

		view.setCenter(Player.x, Player.y);
		window.setView(view);		

		draw_game(window, lvl, it, entities, Player, plScores);
	
	}
}

void start_game()
{
	RenderWindow window(VideoMode(WINDOW_SIZE_GAME.x, WINDOW_SIZE_GAME.y), "STAR WARS");

	Music musicInMenu;//создаем объект музыки
	Music musicInGame;

	SoundBuffer shootBuffer;
	Sound shoot(shootBuffer);

	SoundBuffer saberBuffer;
	Sound saber(saberBuffer);

	init_music(musicInMenu, musicInGame, shootBuffer, saberBuffer);

	menu(window, musicInMenu);

	musicInMenu.pause();
	musicInGame.play();
	musicInGame.setLoop(true);

	View view(FloatRect(0, 0, 400, 250));

	Level lvl;
	lvl.LoadFromFile("test.tmx");

	Texture enemy_droid, moveplatform, anakin, fang, enemy_clon, fire, bonus;

	AnimationManager anim_player;	
	AnimationManager anim_clon_enemy;	
	AnimationManager anim_droid_enemy;
	AnimationManager anim_bullet;
	AnimationManager anim4;
	AnimationManager anim_health;

	
	
	init_texture(enemy_droid, moveplatform, anakin, fang, enemy_clon, fire, bonus);

	init_animation(anim_player, anim_clon_enemy, anim_droid_enemy, anim_bullet, anim4, moveplatform, fire, fang, anakin, enemy_clon, enemy_droid, anim_health, bonus);

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;

	std::vector<Object> e;

	init_map_objects(e, entities, anim_clon_enemy, anim_droid_enemy, anim4, lvl, anim_health);

	Object pl = lvl.GetObject("player");
	PLAYER Player(anim_player, lvl, pl.rect.left, pl.rect.top);

	PlayerScores plScores;

	Clock clock;

	float distance;

	playing(window, clock, view, Player, it, entities, plScores, anim_bullet, lvl, shoot, saber);
}

int main()
{
	start_game();
}



