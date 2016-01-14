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

bool playing(RenderWindow &window,Clock &clock,View &view,PLAYER &Player, std::list<Entity*>::iterator &it, std::list<Entity*> &entities, PlayerScores &plScores, AnimationManager &anim_bullet, Level &lvl, Sound &shoot, Sound &saber, AnimationManager &anim_light, AnimationManager &anim_mana, int &levelNumber)
{
	float attack_timer = 0;
	int dead_enemy = 0;
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

		if ((dead_enemy == 8 || dead_enemy >= 8) && levelNumber == 1) { return true; }
		if (Player.levelNumber == 1) {  return true; }

		checking_key(Player, saber, time, attack_timer);
		
		plScores.update(Player.Health, Player.mana);

		Player.update(time);

		is_enemy_alive(it, entities, time, lvl, anim_mana, dead_enemy);

		if (Player.life == true)

		for (it = entities.begin();it != entities.end();it++)
		{
			player_sup_attack(it, Player, anim_light, entities, lvl, time);

			definition_of_the_enemy(it, Player, anim_bullet, entities, lvl, time, shoot);

			definition_of_the_platform(it, Player, entities, lvl, time);

			take_bonus(it, Player, lvl, levelNumber);
		
			if ((*it)->Health <= 0) continue;			
		}

		getPlayerCoordinateForView(Player.x, Player.y, window, levelNumber);

		draw_game(window, lvl, it, entities, Player, plScores);

	}
}

void change_level(Level &lvl, int &levelNumber)
{
	if (levelNumber == 1) { lvl.LoadFromFile("first_level.tmx"); }
	if (levelNumber == 2) { lvl.LoadFromFile("second_level.tmx"); }
	if (levelNumber == 3) { lvl.LoadFromFile("third_level.tmx"); }
}

bool start_game(int &levelNumber, RenderWindow &window)
{

	int enemiesNumber = 0;

	float distance;

	Level lvl;
	change_level(lvl, levelNumber);

	Music musicInMenu;
	Music musicInGame;

	SoundBuffer shootBuffer;
	Sound shoot(shootBuffer);

	SoundBuffer saberBuffer;
	Sound saber(saberBuffer);

	init_music(musicInMenu, musicInGame, shootBuffer, saberBuffer);

	if (levelNumber == 1) menu(window, musicInMenu);

	musicInMenu.pause();
	musicInGame.play();
	musicInGame.setLoop(true);

	View view(FloatRect(0, 0, 400, 250));

	Texture enemy_droid, moveplatform, anakin, fang, enemy_clon, fire, bonus, light, mana;

	AnimationManager anim_player;	
	AnimationManager anim_clon_enemy;	
	AnimationManager anim_droid_enemy;
	AnimationManager anim_bullet;
	AnimationManager anim4;
	AnimationManager anim_health;
	AnimationManager anim_light;
	AnimationManager anim_mana;

	init_texture(enemy_droid, moveplatform, anakin, fang, enemy_clon, fire, bonus, light, mana);

	init_animation(anim_player, anim_clon_enemy, anim_droid_enemy, anim_bullet, anim4, moveplatform, fire, fang, anakin, enemy_clon, enemy_droid, anim_health, bonus, light, anim_light, mana, anim_mana);

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;

	std::vector<Object> e;

	init_map_objects(e, entities, anim_clon_enemy, anim_droid_enemy, anim4, lvl, anim_health, enemiesNumber);
	Object pl = lvl.GetObject("player");
	PLAYER Player(anim_player, lvl, pl.rect.left, pl.rect.top);

	PlayerScores plScores;

	Clock clock;

	return playing(window, clock, view, Player, it, entities, plScores, anim_bullet, lvl, shoot, saber, anim_light, anim_mana, levelNumber);
}

void game_Running(int &levelNumber, RenderWindow &window)
{
	if (start_game(levelNumber, window)) { levelNumber += 1; game_Running(levelNumber, window); }
}

int main()
{
	RenderWindow window(VideoMode(WINDOW_SIZE_GAME.x, WINDOW_SIZE_GAME.y), "STAR WARS");

	int levelNumber = 1 ;

	game_Running(levelNumber, window);
	return 0;
}



