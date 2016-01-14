#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "iostream"
#include <vector>
#include <list>
#include "level.hpp"
#include "anim.hpp"

using namespace sf;

class Entity
{
public:
	float x,y,dx,dy,w,h;
	AnimationManager anim;
	std::vector<Object> obj;
	bool life, dir, repulse;
	float timer, attack_timer, timer_end;
	String Name;
	int Health, pos;

	Entity(AnimationManager &A,int X, int Y, int pos)
	{ 

		anim = A;
		x = X;
		y = Y;
		dir = 1;
		life=true;
		timer=0;
		timer_end=0;
		dx=dy=0;
	}

	
	

	virtual void update(float time) = 0;

	void draw(RenderWindow &window)
	{
		anim.draw(window,x,y+h);
	}

	FloatRect getRect()
	{
		return FloatRect(x,y,w,h);
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

class Bullet:public Entity
{
public:

	Bullet(AnimationManager &a, Level &lev,int x,int y,bool dir):Entity(a,x,y,pos)
	{
		option("Bullet", 0.3, 10, "move");

		if (dir) dx=-0.3;
   		obj = lev.GetObjects("solid");
		
	}

	void update(float time)
	{
		x+=dx*time;
		
		for (int i = 0;i < obj.size();i++)
		{
			if (getRect().intersects(obj[i].rect))
			{
				Health = 0;
			}
		}
		if (!life) { Health = 0; }

		if (Health<=0) {anim.set("explode");dx=0;
		              if (anim.isPlaying()==false) life=false;
		                }

		anim.tick(time);		 
	}

};

class PLAYER: public Entity
{
public:
	enum {stay,walk,duck,jump,climb,swim, dead} STATE;
	bool onLadder, shoot = false, hit, superAttack = false;
	std::map<std::string,bool> key;

	PLAYER(AnimationManager &a, Level &lev,int x,int y):Entity(a,x,y,pos)
	{	
		option("Player",0,100,"stay");
        STATE=stay; hit=0;
		obj = lev.GetAllObjects();
	}

	void KeyCheck(float time)
	{
		if (key["L"])
		{
			dir=1;  
			if (STATE!=duck) dx=-0.1; 
			if (STATE==stay) STATE=walk;
		}

		if (key["R"])
		{ 		    
			dir=0; 	
			if (STATE!=duck) dx=0.1;
			if (STATE==stay) STATE=walk;  
		}

		if (key["Up"])
		{
			if (onLadder) STATE=climb;
			if (STATE==stay || STATE==walk) { dy=-0.28; STATE=jump; anim.play("jump");}  
		}


		if (key["Down"])
		{ 
			if (STATE==stay || STATE==walk) { STATE=duck; dx=0;} 
			if (STATE==swim || STATE==climb) dy=0.05;    
		}

		if (key["Space"])
		{
			shoot = true;			
		}

		if (key["Alt"])
		{
			superAttack = true;
		}

		if (!(key["R"] || key["L"]))
		{
			dx=0; 
			if (STATE==walk) STATE=stay;
		}

		if (!(key["Up"] || key["Down"]))
		{
			if (STATE==swim || STATE==climb) dy=0;  
		}

		if (!key["Down"])
		{
			if (STATE==duck) { STATE=stay;} 	
		}

		if (!key["Alt"]) 
		{
			superAttack = false;
		}                    
	}

	void update(float time)
	{	
		KeyCheck(time);

		if (Health <= 0)
		{
			life = false;
			anim.set("dead");
			dx = 0; dy = 0;
		}

		if (STATE==stay) anim.set("stay");
		if (STATE==walk) anim.set("walk");
		if (STATE==jump) anim.set("jump");
		if (STATE==duck) anim.set("duck");
		if (STATE==climb) {anim.set("climb"); anim.pause(); if (dy!=0) anim.play(); if (!onLadder) STATE=stay;} 
		attack_timer = 0;
		if (shoot && STATE == stay)
		{
			timer += time;
			if (timer > 900) { shoot = false; timer = 0; }
			else anim.set("shoot");
		}
				         	
		if (STATE==walk && shoot) 
		{
			timer += time;
			if (timer > 600) { shoot = false; timer = 0; }
			else anim.set("shootAndWalk"); 
		}   
		
		if (STATE == jump && shoot) 
		{	
			timer += time;
			if (timer > 800) { shoot = false; timer = 0; }
			else anim.set("JumpAttack");
		}

		if (hit) {
			timer += time;
			if (timer > 600) { hit = false; timer = 0; }
			else anim.set("hit");
		}

		if (dir) anim.flip();

		if (superAttack) { dx = 0; anim.set("supAtt");}

		x += dx * time;	
		Collision(0);

		if (STATE!=climb) dy+=0.0005*time;
		y += dy*time; onLadder=false; 
		Collision(1);
		
		anim.tick(time);

		key["R"]=key["L"]=key["Up"]=key["Down"]=key["Alt"]=key["Space"]= false; 
	}

	void Collision(int num)
	{	
		
		for (int i=0;i<obj.size();i++) 
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name=="solid")
				{           
					if (dy>0 && num==1)	{ y = obj[i].rect.top -  h;  dy=0;   STATE=stay;}
					if (dy<0 && num==1)	{ y = obj[i].rect.top + obj[i].rect.height ;   dy=0;}
					if (dx>0 && num==0)	{ x =  obj[i].rect.left -  w; }
					if (dx<0 && num==0)	{ x =  obj[i].rect.left + obj[i].rect.width ;}
				} 

				if (obj[i].name=="ladder")	{ onLadder=true; }

				if (obj[i].name=="SlopeLeft")
				{  FloatRect r = obj[i].rect;
				   int y0 = (x+w/2-r.left) * r.height/r.width+ r.top - h;
				   if (y>y0)
					  if (x+w/2>r.left)
					   {y =  y0; dy=0; STATE=stay;}
				}

				if (obj[i].name=="SlopeRight")
				{   FloatRect r = obj[i].rect;
					int y0 = - (x+w/2-r.left) * r.height/r.width + r.top+r.height - h;
					if (y > y0)
					  if (x+w/2<r.left+r.width)
					   {y = y0 ; dy=0; STATE=stay;}
				}

			}
	}
	
	FloatRect getRectAttLeft()
	{
		return FloatRect(x, y, w, h);
	}

	FloatRect getRectAttRight()
	{
		return FloatRect(x, y, w, h);
	}
};

class ENEMY: public Entity
{
public:
	enum { walk, attack } STATE;
	bool shoot = false;
	bool onGround = false;
	int pos = 0;
	ENEMY(AnimationManager &a, Level &lev,int x, int y, int pos):Entity(a,x,y,pos)
	{
		option("Enemy", 0.02, 15, "walk");		
		obj = lev.GetAllObjects();
	}

	/*void Collision(int num)
	{
		for (int i = 0;i<obj.size();i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (dy > 0 && num == 1) { y = obj[i].rect.top - h;  dy = 0;   STATE = walk; }
					if (dy < 0 && num == 1) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				//	if (dx > 0 && num == 0) { x = obj[i].rect.left - w; }
					if (dx < 0 && num == 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0; }
				}
			}

	}*/
	
	void keyCheck()
	{
		if (dx != 0) { anim.set("walk"); }
		else { anim.set("attack"); }
	}

	void update(float time)
	{	
		keyCheck();
		x += dx * time;
	/*	Collision(0);
		Collision(1);*/
		timer += dx;
		if (timer > 15 || timer < -15)
		{
			dx *= -1;
			timer = 0;
			if (dx > 0) { dir = 1; }
			else { dir = 0; }
		}
		
		if (dir) { anim.flip(1); }
		else { anim.flip(0); }

		if (Health <= 0)
		{
			anim.set("dead"); dx = 0;
			life = false;
	    }
		
	//	Collision(dx, 0);
	/*	if (pos)
		{anim.set("attack"); std::cout << "new p.x" << pos << "\n\n";
		}
		else
		{anim.set("walk"); std::cout << "new p.x" << pos << "\n\n";
		}*/
		anim.tick(time);
	}

	
};

class MovingPlatform: public Entity
{
public:
	MovingPlatform(AnimationManager &a, Level &lev,int x,int y):Entity(a,x,y,pos)
	{
		option("MovingPlatform", 0.05, 0, "move");
	}

	void update(float time)
	{	
		x += dx * time;
		timer+=time;
		if (timer>7500) {dx*=-1;timer=0;}
		anim.tick(time);
	}

};

class MovingUpPlatform : public Entity
{
public:
	MovingUpPlatform(AnimationManager &a, Level &lev, int x, int y) :Entity(a, x, y,pos)
	{
		option("Up", 0.05, 0, "move");
	}

	void update(float time)
	{
		y += dx * time;
		timer += time;
		if (timer>2500) { dx *= -1;timer = 0; }
		anim.tick(time);
	}

};

class PlayerScores
{
public:
	Texture t;
	Sprite s;
	int max;
	RectangleShape bar;

	PlayerScores()
	{
		t.loadFromFile("files/megaman_bullets.png");
		s.setTexture(t);
		s.setTextureRect( IntRect(783,2,15,84) );

		bar.setFillColor(Color(0,0,0));
		max = 100;
	}

	void update(int k)

	{
		if (k>0) 
			if (k<max)
        		bar.setSize( Vector2f(10, (max-k)*70/max));
		
	}

   	void draw(RenderWindow &window)
	{  
		Vector2f center = window.getView().getCenter();
		Vector2f size = window.getView().getSize();

		s.setPosition(   center.x - size.x/2 + 10, center.y - size.y/2 + 10);
	    bar.setPosition( center.x - size.x/2 + 14, center.y - size.y/2 + 14);
		
		window.draw(s);
		window.draw(bar);
	}

};

int main()
{  
	
	int pos;

	RenderWindow window(VideoMode(400, 250), "SFML works!");

	View view( FloatRect(0, 0, 400, 250) );
	
	Level lvl;
	lvl.LoadFromFile("test.tmx");

	Texture tileSet, moveplatform, anakin, fang, enemy_clon, fire;
	tileSet.loadFromFile("files/enemy2.png");
	moveplatform.loadFromFile("files/movingPlatform.png");
	anakin.loadFromFile("files/Anakin2.png");
	fang.loadFromFile("files/fanq.png");
	enemy_clon.loadFromFile("files/enemy2.png");
	fire.loadFromFile("files/laser2.png");

	AnimationManager anim;
	anim.loadFromXML("files/anim_anakin.xml", anakin);

	AnimationManager anim3;
	anim3.loadFromXML("files/enemy_clon_anim.xml", enemy_clon);

	AnimationManager anim2;
	anim2.create("move",fire,0,0,30,3,1,0);
	anim2.create("explode", fang, 323, 583, 18, 18, 4, 0.01, 29, 0);

	AnimationManager anim4;
	anim4.create("move", moveplatform, 0, 0, 95, 22, 3, 0);
	
	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;

	std::vector<Object> e = lvl.GetObjects("enemy");
	for (int i=0;i < e.size();i++)
		entities.push_back(new ENEMY(anim3, lvl, e[i].rect.left, e[i].rect.top, 1) );

	e = lvl.GetObjects("MovingPlatform");
	for (int i=0;i < e.size();i++)
		entities.push_back(new MovingPlatform(anim4, lvl, e[i].rect.left, e[i].rect.top) );

	e = lvl.GetObjects("Up");
	for (int i = 0;i < e.size();i++)
		entities.push_back(new MovingUpPlatform(anim4, lvl, e[i].rect.left, e[i].rect.top));

	Object pl = lvl.GetObject("player");
	PLAYER Mario(anim, lvl, pl.rect.left, pl.rect.top);

	PlayerScores plScores;

	Clock clock;

	float shootTime = 0;
	float shootFlag = false;
	float distance;
	/////////////////// основной цикл  /////////////////////
	while (window.isOpen())
	{ 
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time/500;  // здесь регулируем скорость игры
		if (time > 40) time = 40;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)      
				window.close();

			/*if (event.type == Event::KeyPressed)
				if (event.key.code==Keyboard::Down)
				{
					entities.push_back(new Bullet(anim2, lvl, Mario.x + 18, Mario.y + 18, Mario.dir));
					
				}*/
		       			
		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) Mario.key["L"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Right)) Mario.key["R"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Up)) Mario.key["Up"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Down)) Mario.key["Down"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Space)) Mario.key["Space"]=true;
		if (Keyboard::isKeyPressed(Keyboard::LAlt)) Mario.key["Alt"] = true;
		
		Mario.update(time);

		for(it=entities.begin();it!=entities.end();)
		{
			Entity *b = *it; 
			b->update(time);
			if (b->life==false)	{ it  = entities.erase(it); delete b;}
			else it++;
		}

		
		plScores.update(Mario.Health);

		for(it=entities.begin();it!=entities.end();it++)
		{   
			//1. враги
			if ((*it)->Name == "Enemy")
			{
				Entity *enemy = *it;

				if (enemy->Health <= 0) continue;
				distance = sqrt((Mario.x - (*it)->x)*(Mario.x - (*it)->x) + (Mario.y - (*it)->y)*(Mario.y - (*it)->y));

				if (Mario.getRectAttLeft().intersects(enemy->getRect()) || Mario.getRectAttRight().intersects(enemy->getRect()))
					if (Keyboard::isKeyPressed(Keyboard::Space)) {
						/*std::cout << "new p.x" << Mario.x << "\n\n";
std::cout << "new (*it)->x" << (*it)->x << "\n";*/ enemy->dx = 0; enemy->Health = 0;
					}
					else if (Keyboard::isKeyPressed(Keyboard::LAlt)) { enemy->dx = 0; enemy->Health = 0; }
					/*else if (!Mario.hit) {
						Mario.Health -= 5; Mario.hit = true;
						if (Mario.dir) Mario.x += 10; else Mario.x -= 10;
					}*/

					for (std::list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)
					{
						Entity *bullet = *it2;
						if (bullet->Name == "Bullet")
							if (bullet->life == true)
							{
								if (bullet->getRect().intersects(Mario.getRect()))
								{
									if (Keyboard::isKeyPressed(Keyboard::Down))
									{
										bullet->dx = 0.4;
										bullet->repulse = true;
									}
									else
									{
										bullet->life = false;
										Mario.Health -= 5;
										Mario.hit = true;
									}
								}
								if ((bullet->getRect().intersects((*it)->getRect())) && (bullet->repulse == true))
								{
									bullet->repulse = false;
									bullet->life = false;
									enemy->Health -= 5;
								}
							}
					}

					if (distance < 200) { shootTime += 0.005*time; }

					if (distance < 200)
					{

						if (shootTime > 7)
						{
							(*it)->pos = 1;
								std::cout << "new p.x" << (*it)->pos << "\n\n";
							if (Mario.x < (*it)->x) {
								(*it)->dir = 0; entities.push_back(new Bullet(anim2, lvl, (*it)->x, (*it)->y + 18, 1));
							}
							else {
								(*it)->dir = 1; entities.push_back(new Bullet(anim2, lvl, (*it)->x, (*it)->y + 18, 0));
							}

							
							shootTime = 0.0;
							(*it)->dx = 0;
						}

					}
					else { (*it)->pos = 0; }
			}
		
			
			//2. движущиеся платформы
		if ((*it)->Name=="MovingPlatform")
			{
			    Entity *movPlat = *it;
				if  (Mario.getRect().intersects( movPlat->getRect() ) )
				   if (Mario.dy>0)
					  if (Mario.y+Mario.h<movPlat->y+movPlat->h)
					  {Mario.y=movPlat->y - Mario.h + 3; Mario.x+=movPlat->dx*time; Mario.dy=0; Mario.STATE=PLAYER::stay;}
			}

			if ((*it)->Name == "Up")
			{
				Entity *movUpPlat = *it;
				if (Mario.getRect().intersects(movUpPlat->getRect()))
					if (Mario.dy>0)
						if (Mario.y + Mario.h<movUpPlat->y + movUpPlat->h)
						{
							Mario.y = movUpPlat->y - Mario.h + 3; Mario.y += movUpPlat->dy*time; Mario.dy = 0; Mario.STATE = PLAYER::stay;
						}
			}

		}

		/////////////////////отображаем на экран/////////////////////

		view.setCenter( Mario.x,Mario.y);
		window.setView(view);

		window.clear(Color(107,140,255));

		lvl.Draw(window);

		for(it=entities.begin();it!=entities.end();it++)
			(*it)->draw(window);

		Mario.draw(window);
		plScores.draw(window);

		window.display();
	}

	return 0;
}



