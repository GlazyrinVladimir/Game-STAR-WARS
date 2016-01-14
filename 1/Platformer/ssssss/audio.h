#pragma once
#include "init_struct.h"

void is_life_it(std::list<Entity*>::iterator &it, Sound &deadEnemy)
{
	if ((*it)->Health <= 0 && (*it)->playDeadSound)
	{
		(*it)->playDeadSound = false;
		//sound.levelThird.stop();
		deadEnemy.play();
	}	
}