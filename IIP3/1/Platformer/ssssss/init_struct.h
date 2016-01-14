#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "level.hpp"
#include "classes.h"
#include "anim.hpp"

struct init_sounds
{
	Music musicInMenu;
	Music musicInGame;
	Music levelThird;

	SoundBuffer shootBuffer;
	SoundBuffer saberBuffer;
	SoundBuffer deathClonBuffer;
	SoundBuffer deathDroidBuffer;
	SoundBuffer deathEnakinBuffer;
	SoundBuffer deathObiBuffer;

	Sound deadEnakin;
	Sound deadObi;
	Sound shoot;
	Sound saber;
	Sound deathClon;
	Sound deathDroid;
	init_sounds()
	{
		musicInMenu.openFromFile("sounds/music.ogg");
		musicInGame.openFromFile("sounds/game.ogg");
		levelThird.openFromFile("sounds/thirdLevel.ogg");

		shootBuffer.loadFromFile("sounds/lazer.ogg");
		saberBuffer.loadFromFile("sounds/saberdown.ogg");
		deathClonBuffer.loadFromFile("sounds/pain1.ogg");
		deathDroidBuffer.loadFromFile("sounds/light.ogg");
		deathEnakinBuffer.loadFromFile("sounds/dead.ogg");
		deathObiBuffer.loadFromFile("sounds/dead_obi.ogg");
		deathDroid.setBuffer(deathDroidBuffer);
		shoot.setBuffer(shootBuffer);
		saber.setBuffer(saberBuffer);
		deathClon.setBuffer(deathClonBuffer);
		deadEnakin.setBuffer(deathEnakinBuffer);
		deadObi.setBuffer(deathObiBuffer);
	}
};

struct init_font
{
	Font font;
	Text textWin;
	Text textDead;
	Text textFinish;
	init_font()
	{
		font.loadFromFile("Caberne.ttf");
		textWin.setString("YOU KILLED YOUR TEACHER, YOU BECOME A POWERFUL SITH");
		textWin.setFont(font);
		textWin.setCharacterSize(12);
		textWin.setColor(Color::White);
		textWin.setStyle(sf::Text::Bold);

		textDead.setString("YOU ARE DEAD");
		textDead.setFont(font);
		textDead.setCharacterSize(12);
		textDead.setColor(Color::White);
		textDead.setStyle(sf::Text::Bold);

		textFinish.setString("Press TAB to restart or ESC to exit");
		textFinish.setFont(font);
		textFinish.setCharacterSize(12);
		textFinish.setColor(Color::White);
		textFinish.setStyle(sf::Text::Bold);
	}
};

struct init_texture
{
	Texture enemy_droid;
	Texture moveplatform;
	Texture anakin;
	Texture fang;
	Texture enemy_clon;
	Texture fire;
	Texture bonus;
	Texture light_attack;
	Texture mana;
	Texture boss_obiwan;
	Texture menuTexture1;
	Texture  menuTexture3;
	Texture  menuTexture2;
	Texture menuBackground;
	Texture trainingBack;

	AnimationManager anim_player;
	AnimationManager anim_clon_enemy;
	AnimationManager anim_droid_enemy;
	AnimationManager anim_bullet;
	AnimationManager anim4;
	AnimationManager anim_health;
	AnimationManager anim_light;
	AnimationManager anim_mana;
	AnimationManager anim_obiwan;

	Sprite menu1;
	Sprite menu2;
	Sprite menu3;
	Sprite menuBg;
	Sprite training;


	init_texture()
	{
		boss_obiwan.loadFromFile("files/obi.png");
		bonus.loadFromFile("files/heart2.png");
		enemy_droid.loadFromFile("files/droid2.png");
		moveplatform.loadFromFile("files/movingPlatform.png");
		anakin.loadFromFile("files/Anakin3.png");
		fang.loadFromFile("files/fanq.png");
		enemy_clon.loadFromFile("files/enemy2.png");
		fire.loadFromFile("files/laser2.png");
		light_attack.loadFromFile("files/light.png");
		mana.loadFromFile("files/mana.png");

		menuTexture1.loadFromFile("files/111.png");
		menuTexture3.loadFromFile("files/222.png");
		menuTexture2.loadFromFile("files/333.png");
		menuBackground.loadFromFile("files/menu.png");
		trainingBack.loadFromFile("files/training2.png");

		menu1.setTexture(menuTexture1);
		menu2.setTexture(menuTexture2);
		menu3.setTexture(menuTexture3);
		menuBg.setTexture(menuBackground);
		training.setTexture(trainingBack);

		menu1.setPosition(75, 30);
		menu3.setPosition(75, 90);
		menu2.setPosition(75, 150);
		training.setPosition(250, 100);

		anim_obiwan.loadFromXML("files/anim_enemy_obiwan.xml", boss_obiwan);
		anim_light.loadFromXML("files/light_anim.xml", light_attack);
		anim_player.loadFromXML("files/anim_anakin.xml", anakin);
		anim_clon_enemy.loadFromXML("files/enemy_clon_anim.xml", enemy_clon);
		anim_droid_enemy.loadFromXML("files/anim_droid.xml", enemy_droid);
		anim_bullet.create("move", fire, 0, 0, 30, 3, 1, 0);
		anim_bullet.create("explode", fang, 323, 583, 18, 18, 4, float(0.01), 29, 0);
		anim4.create("move", moveplatform, 0, 0, 95, 22, 3, 0);
		anim_health.create("move", bonus, 0, 0, 20, 20, 1, 0);
		anim_mana.create("move", mana, 0, 0, 25, 25, 1, 0);
	}
};
#pragma once
