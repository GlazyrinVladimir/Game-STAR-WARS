#pragma once

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
			if (menuNum == 3) { kek = true; }
		}

		window.display();
	}
}

void change_level(Level &lvl, int &levelNumber)
{
	if (levelNumber == 1) { lvl.LoadFromFile("first_level.tmx"); }
	if (levelNumber == 2) { lvl.LoadFromFile("second_level.tmx"); }
	if (levelNumber == 3) { lvl.LoadFromFile("third_level.tmx"); }
}