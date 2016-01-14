#pragma once
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
		font.textWin.setPosition(view.getCenter().x - 200, view.getCenter().y - 20);
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