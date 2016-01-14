#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view(FloatRect(0, 0, 400, 250));//объявили sfml объект "вид", который и является камерой
void getPlayerCoordinateForView(float x, float y, RenderWindow &window, int &levelNumber) { //функция для считывания координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и проверяем их, чтобы убрать края

	if (levelNumber == 1)
	{
		if (x < 200) tempX = 200;//убираем из вида левую сторону
		if (x > 2130) tempX = 2130;//убираем из вида правую сторону
		if (y < 125) tempY = 125;//верхнюю сторону

	}
	if (levelNumber == 2)
	{
	if (x < 205) tempX = 205;//убираем из вида левую сторону
	if (x > 3000) tempX = 3000;//убираем из вида правую сторону
	}
	if (levelNumber == 3)
	{
		if (x < 205) tempX = 205;//убираем из вида левую сторону
		if (x > 760) tempX = 760;//убираем из вида правую сторону
		if (y < 130) tempY = 130;//верхнюю сторону
		if (y > 500) tempY = 500;//нижнюю сторону
	}
	view.setCenter(tempX, tempY); //следим за игроком, передавая его координаты. 
	window.setView(view);
}