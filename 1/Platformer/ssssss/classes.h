#pragma once

#include <SFML/Graphics.hpp>
#include "level.hpp"

using namespace sf;

class Entity
{
public:
	float x, y, dx, dy, w, h;
	AnimationManager anim;
	std::vector<Object> obj;
	bool life, dir, repulse = false, hit = false, isChangePosition = false;
	float timer, attack_timer, timer_anim_dead;
	String Name;
	int Health, pos;
	float shootTime_clon = 0;
	float shootTime_droid = 0;
	int pos_Player;

	Entity(AnimationManager &A, int X, int Y)
	{
		anim = A;
		x = X;
		y = Y;
		life = true;
		timer = 0;
		timer_anim_dead = 0;
		dx = dy = 0;
	}

	virtual void update(float time) = 0;

	void draw(RenderWindow &window)
	{
		anim.draw(window, x, y + h);
	}

	FloatRect getRect()
	{
		return FloatRect(x, y, w, h);
	}

	void option(String NAME, float SPEED, int HEALTH, String FIRST_ANIM = "")
	{
		Name = NAME;
		if (FIRST_ANIM != "") anim.set(FIRST_ANIM);
		w = anim.getW();
		h = anim.getH();
		dx = SPEED;
		Health = HEALTH;
	}

};

class Bullet :public Entity
{
public:
	int last_dir;
	Bullet(AnimationManager &a, Level &lev, int x, int y, bool dir) :Entity(a, x, y)
	{
		option("Bullet", 0.3, 10, "move");

		if (dir) { dx = -0.3;	last_dir = 1; }
		else { last_dir = 0; }
		obj = lev.GetObjects("solid");

	}

	void update(float time)
	{
		x += dx*time;

		for (int i = 0;i < obj.size();i++)
		{
			if (getRect().intersects(obj[i].rect))
			{
				Health = 0;
			}
		}
		if (!life) { Health = 0; }

		if (Health <= 0) {
			anim.set("explode");dx = 0;
			if (anim.isPlaying() == false) life = false;
		}

		if (repulse == true)
		{
			if (last_dir == 1) { dx = 0.4; }
			if (last_dir == 0) { dx = -0.4; }
		}
		anim.tick(time);
	}

};

class light :public Entity
{
public:
	int last_dir;
	light(AnimationManager &a, Level &lev, int x, int y, bool dir) :Entity(a, x, y)
	{
		option("light", 0.3, 10, "move");

		if (dir) { dx = -0.3; anim.flip(1); last_dir = 1; }
		else { last_dir = 0; anim.flip(0);}
		obj = lev.GetObjects("solid");
	}

	void update(float time)
	{
		x += dx*time;

		for (int i = 0;i < obj.size();i++)
		{
			if (getRect().intersects(obj[i].rect))
			{
				Health = 0;
			}
		}
		if (!life) { Health = 0; }

		if (Health <= 0) 
		{
			timer += time;
			if (timer > 1000) { life = false; timer = 0; }
			else { anim.set("explode"); dx = 0; }		
		}

		anim.tick(time);
	}
};

class PLAYER : public Entity
{
public:
	enum { stay, walk, duck, jump, climb, swim, dead } STATE;
	bool onLadder, shoot = false, hit, superAttack = false;
	int mana = 1;
	int supAttackTimer = 0;
	int levelNumber = 0;
	std::map<std::string, bool> key;

	PLAYER(AnimationManager &a, Level &lev, int x, int y) :Entity(a, x, y)
	{
		option("Player", 0, 100, "stay");
		STATE = stay; hit = 0;
		obj = lev.GetAllObjects();
	}

	void KeyCheck(float time)
	{
		if (STATE != dead)
		{
			if (key["L"])
			{
				dir = 1;
				if (STATE != duck) dx = -0.08;
				if (STATE == stay) STATE = walk;
			}

			if (key["R"])
			{
				dir = 0;
				if (STATE != duck) dx = 0.08;
				if (STATE == stay) STATE = walk;
			}

			if (key["Up"])
			{
				if (onLadder) STATE = climb;
				if (STATE == stay || STATE == walk) { dy = -0.29; STATE = jump; anim.play("jump"); }
			}


			if (key["Down"])
			{
				if (STATE == stay || STATE == walk) { STATE = duck; dx = 0; }
			}

			if (key["Space"])
			{
				shoot = true;
			}

			if (key["Alt"] && STATE != jump)
			{
				superAttack = true;
			}

			if (!(key["R"] || key["L"]))
			{
				dx = 0;
				if (STATE == walk) STATE = stay;
			}

			if (!(key["Up"] || key["Down"]))
			{
				if (STATE == swim || STATE == climb) dy = 0;
			}

			if (!key["Down"])
			{
				if (STATE == duck) { STATE = stay; }
			}

		}
	}

	void update(float time)
	{
		KeyCheck(time);

		if (STATE == stay && life == true) anim.set("stay");
		if (STATE == walk) anim.set("walk");
		if (STATE == jump) anim.set("jump");
		if (STATE == duck) anim.set("duck");
		if (STATE == climb) { anim.set("climb"); anim.pause(); if (dy != 0) anim.play(); if (!onLadder) STATE = stay; }

		if (shoot && STATE == stay)
		{
			timer += time;
			if (timer > 1000) { shoot = false; timer = 0; }
			else anim.set("shoot");
		}

		if (STATE == walk && shoot)
		{
			timer += time;
			if (timer > 800) { shoot = false; timer = 0; }
			else anim.set("shootAndWalk");
		}

		if (STATE == jump && shoot)
		{
			timer += time;
			if (timer > 800) { shoot = false; timer = 0; }
			else anim.set("JumpAttack");
		}

		if (Health <= 0 && life == true)
		{
			dx = 0; dy = 0;
			timer += time;
			if (timer > 500) { life = false; timer = 0; anim.set("lie"); }
			else { hit = false; anim.set("dead"); STATE = dead; }
		}

		if (hit && Health > 0) {
			timer += time;
			if (timer > 600) { hit = false; timer = 0; }
			else anim.set("hit");
		}

		if (superAttack)
		{
			supAttackTimer += time;
			if (supAttackTimer > 1300) { superAttack = false; supAttackTimer = 0; }
			else { anim.set("light_shoot"); dx = 0; dy = 0; }
		}

		if (dir) anim.flip();

		x += dx * time;
		Collision(0);

		if (STATE != climb) dy += 0.0005*time;
		y += dy*time; onLadder = false;
		Collision(1);

		anim.tick(time);

		key["R"] = key["L"] = key["Up"] = key["Down"] = key["Alt"] = key["Space"] = false;
	}


	void Collision(int num)
	{

		for (int i = 0;i<obj.size();i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "nextLevel" ) { levelNumber = 1; }
				if (obj[i].name == "solid")
				{
					if (dy>0 && num == 1) { y = obj[i].rect.top - h;  dy = 0;   STATE = stay; }
					if (dy<0 && num == 1) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (dx>0 && num == 0) { x = obj[i].rect.left - w; }
					if (dx<0 && num == 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}

				if (obj[i].name == "ladder") { onLadder = true; }

				if (obj[i].name == "SlopeLeft")
				{
					FloatRect r = obj[i].rect;
					int y0 = (x + w / 2 - r.left) * r.height / r.width + r.top - h;
					if (y>y0)
						if (x + w / 2>r.left)
						{
							y = y0; dy = 0; STATE = stay;
						}
				}

				if (obj[i].name == "SlopeRight")
				{
					FloatRect r = obj[i].rect;
					int y0 = -(x + w / 2 - r.left) * r.height / r.width + r.top + r.height - h;
					if (y > y0)
						if (x + w / 2<r.left + r.width)
						{
							y = y0; dy = 0; STATE = stay;
						}
				}
			}
	}

	FloatRect getRectAttLeft()
	{
		return FloatRect(x, y, -w + 20, h);;
	}

	FloatRect getRectAttRight()
	{
		return FloatRect(x, y, w + 20, h);
	}
};

class ENEMY_CLON : public Entity
{
public:
	enum { walk, attack, stay } STATE;
	int stayTime = 0;
	int hitTime = 0;
	int hitTimeClon = 150;
	int hitTimeDroid = 250;

	ENEMY_CLON(AnimationManager &a, String Name, Level &lev, int x, int y) :Entity(a, x, y)
	{
		obj = lev.GetAllObjects();
		if (Name == "enemy_clon")
		{
			option("enemy_clon", 0.02, 15, "walk");
		}
		if (Name == "enemy_droid")
		{
			anim.flip();
			option("enemy_droid", 0.04, 60, "walk");
		}

	}

	void Collision()
	{

		for (int i = 0;i < obj.size();i++)
		{
			if (getRect().intersects(obj[i].rect))
			{

				if (obj[i].name == "back")
				{
					dx *= -1;
					timer = 0;
					if (dir = 0) { dir = 1; }
					else { dir = 0; }
				}
			}
		}
	}

	void keyCheck()
	{
	if (dx != 0) { anim.set("walk"); }
		else { anim.set("attack"); }		

		//{ std::cout << "new (*it)->x" << "kek" << "\n"; }
		if (dx > 0) anim.flip(1);
		else if (dx < 0) anim.flip(0);
		else
		if (dir) { anim.flip(1); }
		else { anim.flip(0); }
	}

	void getHit(float time)
	{
		if (hit) 
		{
			if (Name == "enemy_clon")
			{
				hitTimeClon += time;
				if (hitTimeClon > 500) { hit = false; hitTimeClon = 0; }
				else
					if (dir) anim.set("right_hit");
					else anim.set("left_hit");
			}
			if (Name == "enemy_droid")
			{
				hitTimeDroid += time;
				if (hitTimeDroid > 500) { hit = false; hitTimeDroid = 0; }
				else
					if (dir) anim.set("right_hit");
					else anim.set("left_hit");
			}
		}
	}

	void dead(float time)
	{
		if (Health <= 0)
		{
			timer_anim_dead += time;
			if (timer_anim_dead > 1200) { life = false; timer_anim_dead = 0; }
			else anim.set("dead");
		}
	}

	void update(float time)
	{
		if (Name == "enemy_clon")
		{
			
			x += dx * time;

			Collision();
			timer += dx;
			
			if (timer > 30 || timer < -30)
			{
				dx *= -1;
				timer = 0;			
				if (dx > 0) { dir = 1; }
				else { dir = 0; }
			}
			
			keyCheck();
			
			getHit(time);

			dead(time);

			anim.tick(time);
		}
		if (Name == "enemy_droid")
		{
	//		std::cout << "new (*it)->x" << Health << "\n";
			x += dx * time;
			Collision();

			keyCheck();

			getHit(time);
			
			dead(time);

			anim.tick(time);
		}
	}
};

class MovingPlatform : public Entity
{
public:
	MovingPlatform(AnimationManager &a, Level &lev, String Name, int x, int y) :Entity(a, x, y)
	{
		obj = lev.GetAllObjects();
		if (Name == "MovingPlatform")
		{
			option("MovingPlatform", 0.05, 1, "move");
		}
		if (Name == "Up")
		{
			option("Up", 0.05, 1, "move");
		}
	}

	void update(float time)
	{
		if (Name == "MovingPlatform")
		{
			x += dx * time;
			timer += time;
			if (timer > 7500) { dx *= -1;timer = 0; }
			anim.tick(time);
		}
		if (Name == "Up")
		{
			y += dx * time;
			timer += time;
			if (timer>2500) { dx *= -1;timer = 0; }
			anim.tick(time);
		}
	}
};

class PlayerScores
{
public:
	Texture t, textureMana;
	Image i, imageMana;
	Sprite s, spriteMana;
	int max;
	int change;
	float removal = 0;

	PlayerScores()
	{
		i.loadFromFile("files/laser_blade2.png");
		imageMana.loadFromFile("files/laserBlade.png");
		t.loadFromImage(i);
		textureMana.loadFromImage(imageMana);
		s.setTexture(t);
		spriteMana.setTexture(textureMana);
		s.setScale(0.15f, 0.15f);
		spriteMana.setScale(0.15f, 0.15f);
		s.setRotation(90);
		max = 100;
	}

	void update(int k, int mana)

	{
		if (k > 0)
			{
				change = max - k;
				max = k;
				removal += change * 5.7;
				s.setTextureRect(IntRect(0, removal, 336, 800));			
			}
		if (mana == 1) spriteMana.setTextureRect(IntRect(0, 0, 800, 240));	
		else spriteMana.setTextureRect(IntRect(0, 0, 220, 240));
	}

	void draw(RenderWindow &window)
	{
		Vector2f center = window.getView().getCenter();
		Vector2f size = window.getView().getSize();
		
		s.setPosition(center.x - size.x / 2 + 120 - removal / 6.7, center.y - size.y / 2);
		spriteMana.setPosition(center.x - size.x / 2 - 2, center.y - size.y / 2 + 30);
		window.draw(s);
		window.draw(spriteMana);
	}
};

class Bonus :public Entity
{
public:
	Bonus(AnimationManager &a, Level &lev, String Name, int x, int y) :Entity(a, x, y)
	{
		obj = lev.GetAllObjects();
		if (Name == "Health")
		{
			option("Health", 0, 1, "move");
		}
		if (Name == "Mana")
		{
			option("Mana", 0, 1, "move");
		}
	}

	void update(float time)
	{
		if (!life) { Health = 0; }

		anim.tick(time);
	}

};