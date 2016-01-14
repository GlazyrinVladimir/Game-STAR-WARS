#pragma once

#include <SFML/Graphics.hpp>
#include "level.hpp"

using namespace sf;

class Entity
{
public:
	float x, y, dx, dy, w, h;
	double timer, attack_timer, timer_anim_dead;
	double shootTime_clon = 0, shootTime_droid = 0;
	AnimationManager anim;
	std::vector<Object> obj;
	bool life, dir, repulse = false, hit = false, boss_do_attack = false, doHit = false, isChangePosition = false;	
	String Name;
	int Health, pos;	 
	int pos_Player;
	bool bossWin = false;

	Entity(AnimationManager &A, float X, float Y)
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
		anim.draw(window, int(x), int(y + h));
	}

	FloatRect getRect()
	{
		return FloatRect(x, y, w, h);
	}

	FloatRect getRectAttRight()
	{
		return FloatRect(x, y, w + 20, h);
	}
	FloatRect getRectAttLeft()
	{
		return FloatRect(x, y, w, h);;
	}

	void option(String NAME, float SPEED, int HEALTH, String FIRST_ANIM = "")
	{
		Name = NAME;
		if (FIRST_ANIM != "") anim.set(FIRST_ANIM);
		w = float(anim.getW());
		h = float(anim.getH());
		dx = SPEED;
		Health = HEALTH;
	}

};

class Bullet :public Entity
{
public:
	int last_dir;
	Bullet(AnimationManager &a, Level &lev, float x, float y, bool dir) :Entity(a, x, y)
	{
		option("Bullet", float(0.3), 10, "move");

		if (dir) { dx = float(-0.3);	last_dir = 1; }
		else { last_dir = 0; }
		obj = lev.GetObjects("solid");

	}

	void update(float time)
	{
		x += dx*time;

		for (unsigned int i = 0;i < obj.size();i++)
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
			if (last_dir == 1) { dx = float(0.4); }
			if (last_dir == 0) { dx = float(-0.4); }
		}
		anim.tick(time);
	}

};

class light :public Entity
{
public:
	int last_dir;
	light(AnimationManager &a, Level &lev, float x, float y, bool dir) :Entity(a, x, y)
	{
		option("light", float(0.3), 10, "move");

		if (dir) { dx = float(-0.3); anim.flip(1); last_dir = 1; }
		else { last_dir = 0; anim.flip(0);}
		obj = lev.GetObjects("solid");
	}

	void update(float time)
	{
		x += dx*time;

		for (unsigned int i = 0;i < obj.size();i++)
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
	bool onLadder, shoot = false, hit, superAttack = false, isPlaySound = true, isDoSupAttack = true, isWin = false;
	int mana = 1;
	float supAttackTimer = 0, hitTimer = 0;
	int levelNumber = 0;
	 
	std::map<std::string, bool> key;

	PLAYER(AnimationManager &a, Level &lev, float x, float y) :Entity(a, x, y)
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
				if (STATE != duck) dx = float(-0.08);
				if (STATE == stay) STATE = walk;
			}

			if (key["R"])
			{
				dir = 0;
				if (STATE != duck) dx = float(0.08);
				if (STATE == stay) STATE = walk;
			}

			if (key["Up"])
			{
				if (onLadder) STATE = climb;
				if (STATE == stay || STATE == walk) { dy = float(-0.29); STATE = jump; anim.play("jump"); }
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
		if (life)
		{
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
				hitTimer += time;
				if (hitTimer > 500) { if (hitTimer > 1500) { hit = false; hitTimer = 0; } }
				else anim.set("hit");
			}

			if (superAttack && mana == 1)
			{
				
				
				supAttackTimer += time;
				if (supAttackTimer > 1300) { superAttack = false; supAttackTimer = 0; mana = 0; isDoSupAttack = true;
				}
				else { anim.set("light_shoot"); dx = 0; dy = 0; }
			}

			if (dir) anim.flip();

			x += dx * time;
			Collision(0);

			if (STATE != climb) dy += float(0.0005*time);
			y += dy*time; onLadder = false;
			Collision(1);

			anim.tick(time);

			key["R"] = key["L"] = key["Up"] = key["Down"] = key["Alt"] = key["Space"] = false;
		}
		else anim.set("lie");
	}


	void Collision(int num)
	{

		for (unsigned int i = 0;i<obj.size();i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "fall") { Health = 0; dy = 0; }
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
					float y0 = (x + w / 2 - r.left) * r.height / r.width + r.top - h;
					if (y>y0)
						if (x + w / 2>r.left)
						{
							y = y0; dy = 0; STATE = stay;
						}
				}

				if (obj[i].name == "SlopeRight")
				{
					FloatRect r = obj[i].rect;
					float y0 = -(x + w / 2 - r.left) * r.height / r.width + r.top + r.height - h;
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
	float hitTimeClon = 150;
	float hitTimeDroid = 250;
	float hitTimeObiwan = 0;
	int boss_attack = 1;
	float stay_timer = 0;
	bool playDeadSound;

	ENEMY_CLON(AnimationManager &a, String Name, Level &lev, float x, float y) :Entity(a, x, y)
	{
		obj = lev.GetAllObjects();
		if (Name == "enemy_clon")
		{
			option("enemy_clon", float(0.02), 15, "walk");
		}
		if (Name == "enemy_droid")
		{
			anim.flip();
			option("enemy_droid", float(0.05), 60, "walk");
		}
		if (Name == "enemy_obiwan")
		{
			option("enemy_obiwan", float(0.04), 200, "walk");
		}
	}

	void Collision()
	{

		for (unsigned int i = 0;i < obj.size();i++)
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
		if (Name == "enemy_obiwan")
		{
			if (dx != 0) { anim.set("walk"); }
			
			if (dx > 0) anim.flip(0);
			else if (dx < 0) anim.flip(1);
		}
		else
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
	}
	void getHit(float time)
	{
		if (hit) 
		{
			if (Name == "enemy_obiwan")
			{
				hitTimeObiwan += time;
				if (hitTimeObiwan > 1500) { hit = false; hitTimeObiwan = 0; }
			}
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
			dx = 0;
			boss_do_attack = false;
			doHit = false;
			if (Name == "enemy_obiwan")
			{
				timer_anim_dead += time;
				if (timer_anim_dead > 3000)
					if (timer_anim_dead > 15000) { life = false; timer_anim_dead = 0; }
					else anim.set("dead2");
				else anim.set("dead");
			}
			else
			{
				timer_anim_dead += time;
				if (timer_anim_dead > 1200) { life = false; timer_anim_dead = 0; }
				else anim.set("dead");
			}

		}
	}

	void update(float time)
	{
		if (Name == "enemy_clon")
		{
			
			x += dx * time;

			timer += dx;

			Collision();

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
		if (Name == "enemy_obiwan")
		{
			if (Health > 0)
			{
				x += dx * time;
			}
			

			Collision();
			keyCheck();
			getHit(time);

			if (doHit)
			{
				if (stay_timer > 800)
				{
					if (boss_attack == 1)
					{
						boss_do_attack = true;
						timer += time;
						if (timer > 1900) { boss_attack = rand() % 3 + 1; timer = 0; stay_timer = 0; doHit = false; boss_do_attack = false; }
						else if (dir) anim.set("first_attack_right");
						else anim.set("first_attack_left");
					}
					if (boss_attack == 2)
					{
						boss_do_attack = true;
						timer += time;
						if (timer > 6500) { boss_attack = rand() % 3 + 1; timer = 0; stay_timer = 0; doHit = false; boss_do_attack = false; }
						else if (dir) anim.set("second_attack_right");
						else anim.set("second_attack_left");
					}
					if (boss_attack == 3)
					{
						boss_do_attack = true;
						timer += time;
						if (timer > 2000) { boss_attack = rand() % 3 + 1; timer = 0; stay_timer = 0; doHit = false; boss_do_attack = false; }
						else if (dir) anim.set("third_attack_right");
						else anim.set("third_attack_left");
					}
				}
				else { if (dir) anim.set("stay_right"); else anim.set("stay_left"); stay_timer += time; }
			}

			dead(time);
			anim.tick(time);


		}
	}

};

class MovingPlatform : public Entity
{
public:
	MovingPlatform(AnimationManager &a, Level &lev, String Name, float x, float y) :Entity(a, x, y)
	{
		obj = lev.GetAllObjects();
		if (Name == "MovingPlatform")
		{
			option("MovingPlatform", float(0.07), 1, "move");
		}
		if (Name == "Up")
		{
			option("Up", float(0.05), 1, "move");
		}
	}

	void collision()
	{
			for (unsigned int i = 0;i<obj.size();i++)
				if (getRect().intersects(obj[i].rect))

					if (obj[i].name == "solid" ) { dx *= -1; }
					else if (obj[i].name == "back") { dx *= -1; }
				
	}

	void update(float time)
	{
		if (Name == "MovingPlatform")
		{
			x += dx * time;
			
			collision();

			/*timer += time;
			if (timer > 7500) { dx *= -1;timer = 0; }*/


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
	Sprite spriteHp, spriteMana;
	Sprite hpBoss;
	int max, max_b;
	int change_p, change_b;
	double removal_p = 0, removal_b = 0;

	PlayerScores()
	{
		i.loadFromFile("files/laser_blade2.png");
		imageMana.loadFromFile("files/laserBlade.png");
		t.loadFromImage(i);
		textureMana.loadFromImage(imageMana);
		spriteHp.setTexture(t);
		spriteMana.setTexture(textureMana);
		hpBoss.setTexture(textureMana);
		spriteHp.setScale(0.15f, 0.15f);
		spriteMana.setScale(0.15f, 0.15f);
		hpBoss.setScale(0.15f, 0.15f);
		hpBoss.setRotation(180);
		spriteHp.setRotation(90);
		max = 100;
		max_b = 200;
	}

	void update(int hpPlayer, int mana, int hpObi)

	{
		if (hpPlayer > 0)
			{
				change_p = max - hpPlayer;
				max = hpPlayer;
				removal_p += change_p * 5.9;
				spriteHp.setTextureRect(IntRect(0, int(removal_p), 336, 800));
			}
		if (hpObi > 0)
		{
			change_b = max_b - hpObi;
			max_b = hpObi;
			removal_b += change_b * 3;
			hpBoss.setTextureRect(IntRect(0, 0, int(800 - removal_b), 240));
		}
		if (mana == 1) spriteMana.setTextureRect(IntRect(0, 0, 800, 240));	
		else spriteMana.setTextureRect(IntRect(0, 0, 220, 240));
		
	}

	void draw(RenderWindow &window, int levelNumber)
	{
		Vector2f center = window.getView().getCenter();
		Vector2f size = window.getView().getSize();

		spriteHp.setPosition(float(center.x - size.x / 2 + 120 - removal_p / 6.7),float(center.y - size.y / 2));
		spriteMana.setPosition(center.x - size.x / 2 - 2, center.y - size.y / 2 + 30);
		if (levelNumber == 3) { hpBoss.setPosition(center.x + size.x / 2, center.y + size.y / 2); window.draw(hpBoss); }

		window.draw(spriteHp);
		window.draw(spriteMana);
		
	}
};

class Bonus :public Entity
{
public:
	Bonus(AnimationManager &a, Level &lev, String Name, float x, float y) :Entity(a, x, y)
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