#pragma once
#include "init_struct.h"

void menu(RenderWindow &window, Music &musicInMenu)
{
	init_texture textu;

	musicInMenu.play();

	bool isMenu = true;
	int menuNum = 0;

	while (isMenu)
	{
		textu.menu1.setColor(Color::White);
		textu.menu2.setColor(Color::White);

		menuNum = 0;

			if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { textu.menu1.setColor(Color::Blue); menuNum = 1; }
			if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { textu.menu2.setColor(Color::Blue); menuNum = 2; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;
			if (menuNum == 2) { window.close(); isMenu = false; }
		}

		window.draw(textu.menuBg);
		window.draw(textu.menu1);
		window.draw(textu.menu2);
		window.display();
	}
}

void change_level(Level &lvl, int &levelNumber)
{
	if (levelNumber == 1) { lvl.LoadFromFile("first_level.tmx"); }
	if (levelNumber == 2) { lvl.LoadFromFile("second_level.tmx"); }
	if (levelNumber == 3) { lvl.LoadFromFile("third_level.tmx"); }
}