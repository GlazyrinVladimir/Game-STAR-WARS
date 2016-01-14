#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view(FloatRect(0, 0, 400, 250));//�������� sfml ������ "���", ������� � �������� �������
void getPlayerCoordinateForView(float x, float y, RenderWindow &window, int &levelNumber) { //������� ��� ���������� ��������� ������
	float tempX = x; float tempY = y;//��������� ����� ������ � ��������� ��, ����� ������ ����

	if (levelNumber == 1)
	{
		if (x < 200) tempX = 200;//������� �� ���� ����� �������
		if (x > 2130) tempX = 2130;//������� �� ���� ������ �������
		if (y < 125) tempY = 125;//������� �������

	}
	if (levelNumber == 2)
	{
	if (x < 205) tempX = 205;//������� �� ���� ����� �������
	if (x > 3000) tempX = 3000;//������� �� ���� ������ �������
	}
	if (levelNumber == 3)
	{
		if (x < 205) tempX = 205;//������� �� ���� ����� �������
		if (x > 760) tempX = 760;//������� �� ���� ������ �������
		if (y < 130) tempY = 130;//������� �������
		if (y > 500) tempY = 500;//������ �������
	}
	view.setCenter(tempX, tempY); //������ �� �������, ��������� ��� ����������. 
	window.setView(view);
}