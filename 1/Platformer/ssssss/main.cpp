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

void playing(RenderWindow &window,Clock &clock,View &view,PLAYER &Player, std::list<Entity*>::iterator &it, std::list<Entity*> &entities, PlayerScores &plScores, AnimationManager &anim_bullet, Level &lvl)
{
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

		checking_key(Player);

		plScores.update(Player.Health);

		Player.update(time);

		is_enemy_alive(it, entities, time);

		for (it = entities.begin();it != entities.end();it++)
		{
			

			definition_of_the_enemy(it, Player, anim_bullet, entities, lvl, time);

			definition_of_the_platform(it, Player, entities, lvl, time);

			if ((*it)->Health <= 0) continue;			
		}

		view.setCenter(Player.x, Player.y);
		window.setView(view);		

		draw_game(window, lvl, it, entities, Player, plScores);
	
	}
}

void start_game()
{
	RenderWindow window(VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "SFML works!");

	View view(FloatRect(0, 0, 400, 250));

	Level lvl;
	lvl.LoadFromFile("test.tmx");

	Texture enemy_droid, moveplatform, anakin, fang, enemy_clon, fire;

	AnimationManager anim_player;	
	AnimationManager anim_clon_enemy;	
	AnimationManager anim_droid_enemy;
	AnimationManager anim_bullet;
	AnimationManager anim4;

	init_texture(enemy_droid, moveplatform, anakin, fang, enemy_clon, fire);

	init_animation(anim_player, anim_clon_enemy, anim_droid_enemy, anim_bullet, anim4, moveplatform, fire, fang, anakin, enemy_clon, enemy_droid);

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;

	std::vector<Object> e;

	init_map_objects(e, entities, anim_clon_enemy, anim_droid_enemy, anim4, lvl);

	Object pl = lvl.GetObject("player");
	PLAYER Player(anim_player, lvl, pl.rect.left, pl.rect.top);

	PlayerScores plScores;

	Clock clock;


	float distance;

	playing(window, clock, view, Player, it, entities, plScores, anim_bullet, lvl);
}

int main()
{
	start_game();
}



