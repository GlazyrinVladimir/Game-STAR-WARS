#pragma once

#include <SFML/Graphics.hpp>
#include "level.hpp"

using namespace sf;

class Entity
{
public:
	float x, y, dx, dy, w, h;
	double timer;
	double attack_timer; 
	double timer_anim_dead;
	double shootTime_clon = 0; 
	double shootTime_droid = 0;
	AnimationManager anim;
	std::vector<Object> obj;
	bool life;
	bool dir;
	bool repulse = false;
	bool hit = false;
	bool boss_do_attack = false;
	bool doHit = false;
	bool isChangePosition = false;
	bool playDeadSound = true;
	bool giveBonus = true;
	bool sawPlayer = false;
	bool alive = false;
	String Name;
	int Health, pos;	 
	int pos_Player;
	bool bossWin = false;
	bool heIsChoke = false;

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

class get_choke :public Entity
{
public:
	int last_dir;
	get_choke(AnimationManager &a, Level &lev, float x, float y, bool dir) :Entity(a, x, y)
	{
		option("choke", float(0.5), 10, "move");

		if (dir) { dx = float(-0.3); anim.flip(1); last_dir = 1; }
		else { last_dir = 0; anim.flip(0); }
		obj = lev.GetObjects("solid");

	}

	void update(float time)
	{
		x += dx*time;

		for (unsigned int i = 0;i < obj.size();i++)
		{
			if ((getRect().intersects(obj[i].rect) && (obj[i].name == "solid")))
			{
				life = false;
			}
		}

		anim.tick(time);
	}
};

class SHIELD :public Entity
{
public:
	int last_dir;
	SHIELD(AnimationManager &a, float x, float y, bool dir) :Entity(a, x, y)
	{
		option("Shield", 0, 1, "stay");
		
		if (dir) {  anim.flip(1); }
		else {  anim.flip(0); }
	}

	void update(float time)
	{
		if (doHit)
		{
			if (timer > 50) { timer = 0; doHit = false; life = false; 
			}
			else timer += time;
			if (!doHit) {
				life = false; 
			}
		}

		anim.tick(time);
	}
};

class PLAYER : public Entity
{
public:
	enum { stay, walk, duck, jump, climb, swim, dead } STATE;
	bool onLadder;
	bool shoot = false; 
	bool hit;
	bool isLossMana = false;
	bool superAttack = false;
	bool isPlaySound = true;
	bool isDoSupAttack = true; 
	bool doRepulse = false;
	bool isWin = false;
	bool shieldActiv = false;
	bool isChoke = false;
	bool shieldOn = false;
	bool shieldActivation = false;
	bool doIt = true;
	int whereAttackMe = 0;
	int mana = 100;
	float supAttackTimer = 0;
	float shieldOnTimer;
	float hitTimer = 0;
	int levelNumber = 0;
	int chokeTimer = 0;
	float lossTimer = 0;
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

			if (key["Up"] && isDoSupAttack)
			{
				if (onLadder) STATE = climb;
				if (STATE == stay || STATE == walk) { dy = float(-0.29); STATE = jump; anim.play("jump"); }
			}


			if (key["Down"])
			{
				if (STATE == stay || STATE == walk) { STATE = duck; dx = 0;}
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

			if (!key["Q"]) { shieldActivation = false; doIt = true; }

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
			if (STATE != climb) dy += float(0.0005*time);
			
			if (shieldActivation && doIt) 
			{ 
				if (shieldOn) shieldOn = false;
				else shieldOn = true;
				doIt = false;
			}

			if (isLossMana && mana > 0) {
				lossTimer += time;
				if (lossTimer > 300) { mana -= 1; lossTimer = 0; }
			}

			if (!key["Ctrl"]) { isChoke = false; doHit = false; }

			if ((isChoke) && (key["Ctrl"]) && (isLossMana))
			{
				anim.set("light_shoot");
				dx = 0; dy = 0;
			}
			else
			{
				if (shoot && STATE == stay)
				{
					timer += time;
					if (timer > 1450) { shoot = false; timer = 0; }
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

				if (hit && Health > 0) 
				{
					hitTimer += time;
					if (hitTimer > 500) { if (hitTimer > 1500) { hit = false; hitTimer = 0; } }
					else anim.set("hit");
				}

				if (superAttack && mana > 40)
				{
					supAttackTimer += time;
					if (supAttackTimer > 1300) 
					{
						superAttack = false; supAttackTimer = 0; mana -= 40; isDoSupAttack = true;
					}
					else { anim.set("light_shoot"); dx = 0; dy = 0; }
				}
				else superAttack = false;
			}
				if (dir) anim.flip();

				x += dx * time;
				Collision(0);
								
				y += dy*time; onLadder = false;
				Collision(1);

				anim.tick(time);

				key["R"] = key["L"] = key["Up"] = key["Down"] = key["Alt"] = key["Space"] = key["Ctrl"] = key["Q"] = false;
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
					int y0 = int((x + w / 2 - r.left) * r.height / r.width + r.top - h);
					if (y>y0)
						if (x + w / 2>r.left)
						{
							y = float(y0); dy = 0; STATE = stay;
						}
				}

				if (obj[i].name == "SlopeRight")
				{
					FloatRect r = obj[i].rect;
					int y0 = int(-(x + w / 2 - r.left) * r.height / r.width + r.top + r.height - h);
					if (y > y0)
						if (x + w / 2<r.left + r.width)
						{
							y = float(y0); dy = 0; STATE = stay;
						}
				}
			}
	}

	FloatRect getRectAttLeft()
	{
		return FloatRect(x, y, -w + 10, h);;
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
	int bossChokeTimer = 0;
	float stay_timer = 0;
	bool playDeadSound;

	ENEMY_CLON(AnimationManager &a, String Name, Level &lev, float x, float y) :Entity(a, x, y)
	{
		obj = lev.GetAllObjects();
		if (Name == "enemy_clon")
		{
			option("enemy_clon", float(0.02), 15, "walk");
			alive = true;
		}
		if (Name == "enemy_droid")
		{
			anim.flip();
			option("enemy_droid", float(0.05), 60, "walk");
			alive = false;
		}
		if (Name == "enemy_obiwan")
		{
			option("enemy_obiwan", float(0.04), 300, "walk");
			alive = true;
		}
	}

	void Collision(float speed)
	{
		for (unsigned int i = 0;i < obj.size();i++)
		{
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "back_l")
				{
					dx = speed;
					timer = 0;
					if (dir = 0) { dir = 1; }
					else { dir = 0; }
				}
				if (obj[i].name == "back_r")
				{
					dx = -speed;
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
				if (hitTimeObiwan > 1000) { hit = false; hitTimeObiwan = 0; }
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
		if (heIsChoke)
		{
			dx = 0;

			if (!dir) { anim.set("choke"); }
			else { anim.set("choke"); anim.flip(); }

			timer += time;
			bossChokeTimer += time;
			if (timer > 300) { Health -= 1; timer = 0; }
			dead(time);
			anim.tick(time);
		}
		else
		{
			if (Name == "enemy_clon")
			{
				x += dx * time;

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

				Collision(float(0.02));

				anim.tick(time);
			}
			if (Name == "enemy_droid")
			{
				x += dx * time;

				keyCheck();

				getHit(time);

				dead(time);

				Collision(float(0.05));

				anim.tick(time);
			}
			if (Name == "enemy_obiwan")
			{
				if (Health > 0)
				{
					x += dx * time;
				}

				Collision(float(0.04));

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
			option("Up", float(0.07), 1, "move");
		}
	}

	void collision()
	{
			for (unsigned int i = 0;i<obj.size();i++)
				if (getRect().intersects(obj[i].rect))
				{
					if (obj[i].name == "back_l")
					{
						dx = float(0.07);
						timer = 0;
						if (dir = 0) { dir = 1; }
						else { dir = 0; }
					}
					if (obj[i].name == "back_r")
					{
						dx = float(-0.07);
						timer = 0;
						if (dir = 0) { dir = 1; }
						else { dir = 0; }
					}
				}
	}

	void update(float time)
	{
		if (Name == "MovingPlatform")
		{
			x += dx * time;
			
			collision();

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
	Texture textureHp, textureMana;
	Image imageHp, imageMana;
	Sprite spriteHp, spriteMana;
	Sprite hpBoss;
	int max, max_b, max_mana;
	int change_p, change_b, change_mana;
	double removal_p = 0, removal_b = 0, removal_mana = 0;

	PlayerScores()
	{
		imageHp.loadFromFile("files/laser_blade2.png");
		imageMana.loadFromFile("files/laserBlade.png");
		textureHp.loadFromImage(imageHp);
		textureMana.loadFromImage(imageMana);
		spriteHp.setTexture(textureHp);
		spriteMana.setTexture(textureMana);
		hpBoss.setTexture(textureMana);
		spriteHp.setScale(0.15f, 0.15f);
		spriteMana.setScale(0.15f, 0.15f);
		hpBoss.setScale(0.15f, 0.15f);
		hpBoss.setRotation(180);
		spriteHp.setRotation(90);
		max = 100;
		max_b = 300;
		max_mana = 100;
	}

	void update(int hpPlayer, int mana, int hpObi)

	{
		if (hpPlayer > 0)
		{
			change_p = max - hpPlayer;
			max = hpPlayer;
			removal_p += change_p * 5.4;
			spriteHp.setTextureRect(IntRect(0, int(removal_p), 336, 746));
		}
		if (hpObi > 0)
		{
			change_b = max_b - hpObi;
			max_b = hpObi;
			removal_b += change_b * 1.85;
			hpBoss.setTextureRect(IntRect(0, 0, int(728 - removal_b), 238));
		}
		if (mana > 0)
		{	
			change_mana = max_mana - mana;
			max_mana = mana;
			removal_mana += change_mana * 5.4;
			spriteMana.setTextureRect(IntRect(0, 0, int(728 - removal_mana), 238));
		}
		if (mana == 0) spriteMana.setTextureRect(IntRect(0, 0, 185, 240)); 		
	}

	void draw(RenderWindow &window, int levelNumber)
	{
		Vector2f center = window.getView().getCenter();
		Vector2f size = window.getView().getSize();

		spriteHp.setPosition(float(center.x - size.x / 2 + 120 - removal_p / 6.7),float(center.y - size.y / 2));
		spriteMana.setPosition(center.x - size.x / 2 - 2 + 12, center.y - size.y / 2 + 30);
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