#include <SFML/Graphics.hpp>	//graphic
#include <SFML/Audio.hpp>		//audio
#include <time.h>				//cooldown??
#include <list>					//all stuff
#include <iostream>				//check process
#include <sstream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace sf;
class Animation																		

{
public:
	float Frame, speed;
	Sprite sprite;
	std::vector<IntRect> frames;
	Animation() {}
	Animation(Texture &t, int x, int y, float w, float h, int count, float Speed)
	{
		Frame = 0;
		speed = Speed;
		for (int i = 0;i < count;i++) { frames.push_back( IntRect(x + i*w, y, w, h) ); }
		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h);
		sprite.setTextureRect(frames[0]);
	}
	void update()
	{	
		Frame += speed;
		int n = frames.size();
		if (Frame >= n) Frame -= n;
		if (n > 0) sprite.setTextureRect(frames[int(Frame)]);
	}
	bool isEnd() { return Frame + speed >= frames.size(); }
};
class Entity
{
public:
	float x,y, dx, R;
	int hp, atk,type;
	bool alive,atking,atked;
	std::string name;
	Animation anim;
	Entity()
	{
		alive = true;
		atking = false;
		atked = false;
	}
	void settings(Animation &a, float X,float Y)
	{
		anim = a;
		x = X;
		y = Y;
	}
	virtual void update() {};
	void draw(RenderWindow &app)
	{
		anim.sprite.setPosition(x, y);
		app.draw(anim.sprite);
	}
	virtual ~Entity() {};
};
class menuanimation : public Entity
{
public : menuanimation() { name = "menu"; }
};
class banner : public Entity
{
public: banner() { name = "banner"; }
};
class upmax : public Entity
{
public: upmax() { name = "upmax"; }
};
class cat : public Entity
{
public:
	cat()
	{
		name = "cat";
		atking = false;
	}
	void update() { x += dx; }
};
class ballcat : public cat
{
public:
	ballcat()
	{
		hp = 100;
		atk = 8;
		type = 1;
		dx = -1;
		R = 80;
	}
};
class longcat : public cat
{
public :
	longcat()
	{
		hp = 400;
		atk = 2;
		type = 2;
		dx = -0.8;
		R = 80;
	}
};
class axecat : public cat
{
public :
	axecat()
	{
		hp = 200;
		atk = 25;
		type = 3;
		dx = -1.2;
		R = 80;
	}
};
class legcat : public cat
{
public:
	legcat() 
	{
		hp = 400;
		atk = 100;
		type = 4;
		dx = -1;
		R = 120;
	}
};
class cowcat : public cat
{
public:
	cowcat()
	{
		hp = 500;
		atk = 13;
		type = 5;
		dx = -3;
		R = 160;
	}
};
class zoo : public Entity
{
public:
	zoo() 
	{
		name = "zoo"; 
		atking = false;
	}
	void update() { x += dx; }
};
class dog : public zoo
{
public:
	dog()
	{
		atk = 8;
		hp = 90;
		type = 6;
		dx = 0.5;//speed
		R = 85;
	}
};
class snake : public zoo
{
public:
	snake()
	{
		atk = 15;
		hp = 100;
		type = 7;
		dx = 0.8;
		R = 85;
	}
};
class gangster : public zoo
{
public :
	gangster()
	{
		atk = 20;
		hp = 200;
		type = 8;
		dx = 1;
		R = 85;
	}
};
class hippo : public zoo
{
public :
	hippo() 
	{
		atk = 100;
		hp = 1000;
		type = 9;
		dx = 0.4;
		R = 90;
	}
};
class pig : public zoo
{
public :
	pig() 
	{
		atk = 120;
		hp = 1500;
		type = 10;
		dx = 0.5;
		R = 70;
	}
};
class cattower : public cat
{
public :
	cattower()
	{
		name = "cat";
		hp = 1000;
		atk = 0;
		type = 9999;
		dx = 0;
		R = 74;
	}
	void update() {}
};
class zootower : public zoo
{
public :
	zootower()
	{
		name = "zoo";
		hp = 1000;
		atk = 0;
		type = 9999;
		dx = 0;
		R = 63;
	}
	void update() {}
};
bool isCollide(Entity *a, Entity *b) { return (a->x - b->x) < (b->R + a->R); }
int main() 
{
	srand(time(NULL));
	bool menuline = 0 , drawmenu = 0 , gamemode = 0 , menumode = 1 , scoremode = 0, scoresong = 0, Cbase = 0 , Zbase = 0 , drawbanner = 0 , maxnotification = 0 , zooburst = 0 , pauseON = 0 , win = 0 , loss = 0;
	int level = 1, score[6] , spawnenemy = 500, debounce = 7;
	float catcan = 0, maxcatcan = 600 , scorenow = 10000;
	std::string player[6];
	score[0] = 6000;
	Clock clock;
	Time timeballcatcooldown = seconds(2.33), timelongcatcooldown = seconds(8.33), timeaxecatcooldown = seconds(7.33), timelegcatcooldown = seconds(3.33), timecowcatcooldown = seconds(2.33);
	RenderWindow app(VideoMode(1280, 720), "battlecat");
	app.setFramerateLimit(60);
//-----------------------------------------loading!!!----------------------------------------------------------
	SoundBuffer Bxp;
	Bxp.loadFromFile("audio/xp.wav");
	Sound Sxp(Bxp);
	Sxp.play();
	Texture load;
	load.loadFromFile("load.jpg");
	Sprite sloading(load);
	app.draw(sloading);
	app.display();
//-----------------------------------------set up font --------------------------------------------------
	Font font;
	font.loadFromFile("MotionControl-Bold.otf");
	Text money,maxmoney;
	std::stringstream catmoney,maxcatmoney;
	money.setFont(font);
	money.setFillColor(Color::Yellow);
	money.setPosition(980, 5);
	money.setCharacterSize(72);
	maxmoney.setFont(font);
	maxmoney.setFillColor(Color::Yellow);
	maxmoney.setPosition(1100, 5);
	maxmoney.setCharacterSize(72);
//-----------------------------------------name score----------------------------------------------------
	std::string nowname;
	Text yourname("Enter name", font, 72);
	yourname.setPosition(20, 20);
//-----------------------------------------load old score-----------------------------------------------
	std::ifstream ScoreFile;
	ScoreFile.open("score.txt");
	ScoreFile >> score[1] >> score[2] >> score[3] >> score[4] >> score[5];
	ScoreFile.close();
	std::ifstream PlayerFile;
	PlayerFile.open("name.txt");
	PlayerFile >> player[1] >> player[2] >> player[3] >> player[4] >> player[5];
	PlayerFile.close();
	std::stringstream playerone, playertwo, playerthree, playerfour, playerfive;
	playerone << score[1];
	playertwo << score[2];
	playerthree << score[3];
	playerfour << score[4];
	playerfive << score[5];
	Text sp1(playerone.str(), font, 72), sp2(playertwo.str(), font, 72), sp3(playerthree.str(), font, 72), sp4(playerfour.str(), font, 72), sp5(playerfive.str(), font, 72), p1(player[1], font, 72), p2(player[2], font, 72), p3(player[3], font, 72), p4(player[4], font, 72), p5(player[5], font, 72);
	sp1.setPosition(630, 170);	sp1.setFillColor(Color::Red);	sp2.setPosition(630, 270);	sp3.setPosition(630, 370);	sp4.setPosition(630, 470);	sp5.setPosition(630, 570);
	p1.setPosition(200, 170);	p1.setFillColor(Color::Red);	p2.setPosition(200, 270);	p3.setPosition(200, 370);	p4.setPosition(200, 470);	p5.setPosition(200, 570);
//----------------------------------------- set up music-------------------------------------------
	Music Smenu, Sbgm1, Sbgm2 ,Sbgm3 ,Sbgm4 ,Sbgm5 ,Songscore ;
	Smenu.openFromFile("audio/menu.ogg"); Smenu.setLoop(true);	
	Sbgm1.openFromFile("audio/bgm1.ogg"); Sbgm1.setLoop(true);
	Sbgm2.openFromFile("audio/bgm2.ogg"); Sbgm2.setLoop(true);
	Sbgm3.openFromFile("audio/bgm3.ogg"); Sbgm3.setLoop(true);
	Sbgm4.openFromFile("audio/bgm4.ogg"); Sbgm4.setLoop(true);
	Sbgm5.openFromFile("audio/bgm5.ogg"); Sbgm5.setLoop(true);
	Songscore.openFromFile("audio/score.wav"); Songscore.setLoop(true);
	SoundBuffer Batk, Batktower, Bbuy, Bdie, Btowerdown, Benough, Bloss, Bmax, Bstart, Bwin, Bupmax;
	Batk.loadFromFile("audio/atk.ogg"); Batktower.loadFromFile("audio/atktower.ogg"); Bbuy.loadFromFile("audio/buy.ogg"); Bdie.loadFromFile("audio/die.ogg"); Btowerdown.loadFromFile("audio/towerdown.ogg");
	Benough.loadFromFile("audio/enoughmoney.ogg"); Bloss.loadFromFile("audio/loss.ogg"); Bmax.loadFromFile("audio/maxmoney.ogg"); Bstart.loadFromFile("audio/start.ogg"); Bwin.loadFromFile("audio/win.ogg");
	Bupmax.loadFromFile("audio/upmax.ogg");	
	Sound Satk, Satktower, Sbuy, Sdie, Stowerdown, Senough, Sloss, Smax, Sstart, Swin, Supmax;
	Satk.setBuffer(Batk); Satktower.setBuffer(Batktower); Sbuy.setBuffer(Bbuy); Sdie.setBuffer(Bdie); Stowerdown.setBuffer(Btowerdown);
	Senough.setBuffer(Benough); Sloss.setBuffer(Bloss); Smax.setBuffer(Bmax); Sstart.setBuffer(Bstart); Swin.setBuffer(Bwin); Supmax.setBuffer(Bupmax);
//-----------------------------------------set up texture------------------------------------------- 
	Texture tscore, tbg1, tbg2, tbg3, tbg4, tbg5, twin, tloss, tbc, td, tlc, tac, tlegc, tcc, ts, tg, th, tp, menuanim, basecat, basezoo1, basezoo2, basezoo3, basezoo4, basezoo5, tbanner, tupmax;
	tbg1.loadFromFile("Bg01.png"); tbg2.loadFromFile("Bg02.png"); tbg3.loadFromFile("Bg03.png"); tbg4.loadFromFile("Bg04.png"); tbg5.loadFromFile("Bg05.png");
	tscore.loadFromFile("score.png"); twin.loadFromFile("win.png"); tloss.loadFromFile("loss.png"); 
	tbc.loadFromFile("ballcat.png");	tlc.loadFromFile("longcat.png");	tac.loadFromFile("axecat.png");		tlegc.loadFromFile("legcat.png");	tcc.loadFromFile("cowcat.png");
	td.loadFromFile("dog.png");			ts.loadFromFile("snake.png");		tg.loadFromFile("gangster.png");	th.loadFromFile("hippo.png");		tp.loadFromFile("pig.png");
	menuanim.loadFromFile("menusprite.png");
	basecat.loadFromFile("catbase.png");	basezoo1.loadFromFile("base1.png");	basezoo2.loadFromFile("base2.png");	basezoo3.loadFromFile("base3.png");	basezoo4.loadFromFile("base4.png");	basezoo5.loadFromFile("base5.png");
	tbanner.loadFromFile("banner.png");	tupmax.loadFromFile("upmax.png");
	tbc.setSmooth(true);	td.setSmooth(true);	tlc.setSmooth(true);	tac.setSmooth(true);	tlegc.setSmooth(true);	tcc.setSmooth(true);	ts.setSmooth(true);	tg.setSmooth(true);	th.setSmooth(true);	tp.setSmooth(true);
	Sprite Sscore(tscore), Sbg1(tbg1), Sbg2(tbg2), Sbg3(tbg3), Sbg4(tbg4), Sbg5(tbg5), swin(twin), sloss(tloss);
	Sbg1.setScale(1.5, 1.5); Sbg1.setPosition(0,-48);
	Sbg2.setScale(1.5, 1.5); Sbg2.setPosition(0, -48);
	Sbg3.setScale(1.5, 1.5); Sbg3.setPosition(0, -48);
	Sbg4.setScale(1.5, 1.5); Sbg4.setPosition(0, -48);
	Sbg5.setScale(1.5, 1.5); Sbg5.setPosition(0, -48);
	swin.setPosition(0, 0); sloss.setPosition(352, 30); Sscore.setScale(0.7, 0.7);
//-----------------------------------------all animation----high=fast------------------------------------------
	Animation sMenu0(menuanim, 0, 0, 1280, 720, 2, 0.05), sMenu1(menuanim, 0, 720, 1280, 720, 2, 0.05);
	Animation sCatbase(basecat, 0, 0, 148, 310, 1, 0); Animation sZoobase1(basezoo1, 0, 0, 126, 220, 1, 0), sZoobase2(basezoo2, 0, 0, 126, 251, 1, 0), sZoobase3(basezoo3, 0, 0, 120, 254, 1, 0), sZoobase4(basezoo4, 0, 0, 124, 214, 1, 0), sZoobase5(basezoo5, 0, 0, 126, 229, 1, 0);
	Animation sballw(tbanner, 0, 0, 174, 135, 1, 0), sballr(tbanner, 0, 0, 174, 135, 2, 0.05);
	Animation slongw(tbanner, 0, 135, 174, 135, 1, 0), slongr(tbanner, 0, 135, 174, 135, 2, 0.05);
	Animation saxew(tbanner, 0, 270, 174, 135, 1, 0), saxer(tbanner, 0, 270, 174, 135, 2, 0.05);
	Animation slegw(tbanner, 0, 405, 174, 135, 1, 0), slegr(tbanner, 0, 405, 174, 135, 2, 0.05);
	Animation scoww(tbanner, 0, 540, 174, 135, 1, 0), scowr(tbanner, 0, 540, 174, 135, 2, 0.05);
	Animation supmax1w(tupmax, 0, 0, 240, 256, 1, 0), supmax1r(tupmax, 0, 256, 240, 256, 1, 0), supmax2w(tupmax, 0, 512, 240, 256, 1, 0), supmax2r(tupmax, 0, 768, 240, 256, 1, 0), supmax3w(tupmax, 0, 1024, 240, 256, 1, 0), supmax3r(tupmax, 0, 1280, 240, 256, 1, 0);
	Animation sballCat(tbc,  0,   0, 173, 180, 4, 0.07), sballCatA(tbc, 0, 180, 173, 180, 4, 0.09);
	Animation slongCat(tlc, 0, 0, 290, 286, 4, 0.075), slongCatA(tlc, 0, 286, 290, 286, 3, 0.11);
	Animation saxeCat(tac, 0, 0, 295, 295, 4, 0.075), saxeCatA(tac, 0, 295, 295, 295, 3, 0.09);
	Animation slegCat(tlegc, 0, 0, 614, 645, 6, 0.075), slegCatA(tlegc, 0, 645, 614, 645, 3, 0.08);
	Animation scowCat(tcc, 0, 0, 338, 270, 4, 0.1), scowCatA(tcc, 0, 270, 338, 270, 4, 0.3);
	Animation sDog(td,  0,   0, 176, 176, 4, 0.04), sDogA(td, 0, 176, 176, 176, 5, 0.09);
	Animation sSnake(ts, 0, 0, 246, 246, 4, 0.09), sSnakeA(ts, 0, 246, 246, 246, 4, 0.1);
	Animation sGang(tg, 0, 0, 215, 154, 5, 0.15), sGangA(tg, 0, 154, 215, 154, 14, 0.2);
	Animation sHippo(th, 0, 0, 410, 322, 4, 0.07), sHippoA(th, 0, 322, 410, 322, 4, 0.08);
	Animation sPig(tp, 0, 0, 410, 410, 4, 0.07), sPigA(tp, 0, 410, 410, 410, 9, 0.2);	
	std::list<Entity*> entities;	std::cout << "battle cat start!!" << std::endl;
	while (app.isOpen())
	{
		if (timeballcatcooldown.asSeconds() >= 0 && gamemode == 1)	timeballcatcooldown -= clock.getElapsedTime(); 
		if (timelongcatcooldown.asSeconds() >= 0 && gamemode == 1)	timelongcatcooldown -= clock.getElapsedTime(); 
		if (timeaxecatcooldown.asSeconds() >= 0 && gamemode == 1)	timeaxecatcooldown -= clock.getElapsedTime(); 
		if (timelegcatcooldown.asSeconds() >= 0 && gamemode == 1)	timelegcatcooldown -= clock.getElapsedTime(); 
		if (timecowcatcooldown.asSeconds() >= 0 && gamemode == 1)	timecowcatcooldown -= clock.getElapsedTime();
		clock.restart();
		Event event;
		while (app.pollEvent(event)) { if (event.type == Event::Closed) app.close(); }
		if (menumode == 1)
			{	
				if (drawmenu == 0)
				{
					Smenu.play();
					drawmenu = 1;			
					menuanimation *bg = new menuanimation();
					bg->settings(sMenu0, 640, 720);
					entities.push_back(bg);
					menuline = 1;
					for (auto i = entities.begin();i != entities.end();)
					{
						Entity *e = *i;       //e->update();
						if (e->name == "menu") e->settings(sMenu1, 640, 720);
						i++;
					}					
				}
				if (Keyboard::isKeyPressed(Keyboard::BackSpace))
				{
					debounce--;
					if (debounce <= 0 && nowname.size() >= 1)
					{
						nowname.erase(nowname.size()-1,1);
						yourname.setString(nowname);
						debounce = 7;
					}
				}
				else if (event.type == sf::Event::TextEntered)
				{
					debounce--;
					if (event.text.unicode < 128 && debounce <= 0 && nowname.size() <=8 )
					{
						nowname += static_cast<char>(event.text.unicode);
						yourname.setString(nowname);
						debounce = 9;
					}
				}
				if (menuline == 0)
				{	
					if (Keyboard::isKeyPressed(Keyboard::Down))
						{
							menuline = 1;	std::cout << "down" << std::endl;
							for (auto i = entities.begin();i != entities.end();)
							{
								Entity *e = *i; //e->update();
								if (e->name == "menu") e->settings(sMenu1, 640, 720);
								i++;
							}
						}
					if (Keyboard::isKeyPressed(Keyboard::Return))
						{
							menumode = 0, gamemode = 1; std::cout << "play" << std::endl;
							for (auto i = entities.begin();i != entities.end();)
							{
								Entity *e = *i; //e->update(); e->anim.update();
								if (e->name == "menu") e->alive = false;
								if (e->alive == false)
								{
									i = entities.erase(i);
									delete e;
								}
								else i++;
							}
							Smenu.pause();
							drawmenu = 0;
						}
				}
				else if (menuline ==1)
				{
					if (Keyboard::isKeyPressed(Keyboard::Up))
					{
						menuline = 0;		std::cout << "up" << std::endl;
						for (auto i = entities.begin();i != entities.end();)
						{
							Entity *e = *i; //e->update();
							if (e->name == "menu") e->settings(sMenu0, 640, 720);
							i++;
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::Return))
					{
						menumode = 0; std::cout << "score" << std::endl;
						for (auto i = entities.begin();i != entities.end();)
						{
							Entity *e = *i; //e->update(); e->anim.update();
							if (e->name == "menu") e->alive = false;
							if (e->alive == false)
							{
								i = entities.erase(i);
								delete e;
							}
							else i++;
						}
						Smenu.pause();
						drawmenu = 0, scoremode = 1, gamemode = 0, pauseON = 1;
					}
				}
				if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(app).x > 443 && Mouse::getPosition(app).x < 837)
				{
					debounce--;
					if (debounce <= 0 && Mouse::getPosition(app).y > 410 && Mouse::getPosition(app).y < 475)
					{
						menumode = 0, gamemode = 1;  std::cout << "play" << std::endl;
						for (auto i = entities.begin();i != entities.end();)
						{
							Entity *e = *i; //e->update(); e->anim.update();
							if (e->name == "menu") e->alive = false;
							if (e->alive == false)
							{
								i = entities.erase(i);
								delete e;
							}
							else i++;
						}
						Smenu.pause();
						drawmenu = 0;	std::cout << "start" << std::endl;
						debounce = 8;
					}
					if (debounce <= 0 && Mouse::getPosition(app).y > 500 && Mouse::getPosition(app).y < 570)
					{
						menumode = 0;		std::cout << "score" << std::endl;
						for (auto i = entities.begin();i != entities.end();)
						{
							Entity *e = *i; //e->update(); e->anim.update();
							if (e->name == "menu") e->alive = false;
							if (e->alive == false)
							{
								i = entities.erase(i);
								delete e;
							}
							else i++;
						}
						Smenu.pause();
						drawmenu = 0, scoremode = 1, gamemode = 0, pauseON = 1; std::cout << "score" << std::endl;
						debounce = 8;
					}
				}
			}
		else if (gamemode == 1)
		{
			if (pauseON == 0&&scorenow>=40) scorenow -= 0.8;
//-------------------------------------------set up banner && upmax----------------------------------------------------
			if (drawbanner == 0)
			{
				drawbanner = 1; std::cout << "level = " << level << std::endl;
				if (level > 0)
				{
					banner  *b = new banner();
					b->settings(sballw, 370, 700);
					b->type = 1;
					b->R = 1;
					entities.push_back(b); std::cout << "ballcatban" << std::endl;	
					upmax *f = new upmax();
					f->settings(supmax1w, 120, 720);
					f->type = 1;
					f->R = 1;
					entities.push_back(f);
				}
				if (level > 1)
				{
					banner  *h = new banner();
					h->settings(slongw, 570, 700);
					h->type = 2;
					h->R = 1;
					entities.push_back(h); std::cout << "longcatban" << std::endl;
				}
				if (level > 2)
				{
					banner  *m = new banner();
					m->settings(saxew, 770, 700);
					m->type = 3;
					m->R = 1;
					entities.push_back(m); std::cout << "axecatban" << std::endl;
				}
				if (level > 3)
				{
					banner  *p = new banner();
					p->settings(slegw, 970, 700);
					p->type = 4;
					p->R = 1;
					entities.push_back(p); std::cout << "legcatban" << std::endl;
				}
				if (level > 4)
				{
					banner  *o = new banner();
					o->settings(scoww, 1170, 700);
					o->type = 5;
					o->R = 1;
					entities.push_back(o); std::cout << "cowcatban" << std::endl;
				}
			}
//-------------------------------------------update banner upmax-----------------------------------------------------
			for (auto k : entities)
			{
				if (k->name == "banner")
				{
					if (k->type == 1 && k->atking == 0 && catcan > 50 && timeballcatcooldown <= seconds(0))
					{
						k->atking = 1;
						k->settings(sballr, 370, 700);
						Senough.play();
					}
					if (k->type == 2 && k->atking == 0 && catcan > 100 && timelongcatcooldown <= seconds(0))
					{
						k->atking = 1;
						k->settings(slongr, 570, 700);
						Senough.play();
					}
					if (k->type == 3 && k->atking == 0 && catcan > 150 && timeaxecatcooldown <= seconds(0))
					{
						k->atking = 1;
						k->settings(saxer, 770, 700);
						Senough.play();
					}
					if (k->type == 4 && k->atking == 0 && catcan > 250 && timelegcatcooldown <= seconds(0))
					{
						k->atking = 1;
						k->settings(slegr, 970, 700);
						Senough.play();
					}
					if (k->type == 5 && k->atking == 0 && catcan > 400 && timecowcatcooldown <= seconds(0))
					{
						k->atking = 1;
						k->settings(scowr, 1170, 700);
						Senough.play();
					}
				}
				if (k->name == "upmax")
				{
					if (k->type == 1 && k->atking == 0 && catcan > 90)
					{
						k->atking = 1;
						k->type = 2;
						Supmax.play();
						k->settings(supmax1r, 120, 720);
					}
					if (k->type == 3 && k->atking == 0 && catcan > 180)
					{
						k->atking = 1;
						k->type = 4;
						Supmax.play();
						k->settings(supmax2r, 120, 720);
					}
				}
			}
//-------------------------------------------spawn cat base-----------------------------------------------------
			if (Cbase == 0)
				{
					Cbase = 1;
					cattower *b = new cattower();
					b->settings(sCatbase, 1200, 500);
					entities.push_back(b);
				}
//-------------------------------------------spawn zoo base---------------------------------------------------
			if (Zbase == 0)
				{
					Sstart.play(); std::cout << "startsongplay" << std::endl;
					Zbase = 1;
					if (level == 1) 
					{
						zootower *b = new zootower();
						b->settings(sZoobase1, 70, 500);
						entities.push_back(b);
						Sbgm1.play(); std::cout << "bgm1play" << std::endl;
						score[0] += 5000;
					}
					else if (level == 2)
					{
						Sbgm1.pause();
						zootower *b = new zootower();
						b->settings(sZoobase2, 70, 500);
						entities.push_back(b);
						Sbgm2.play(); std::cout << "bgm2play" << std::endl;
						score[0] += 5000;
					}
					else if (level == 3)
					{
						Sbgm2.pause();
						zootower *b = new zootower();
						b->settings(sZoobase3, 70, 500);
						entities.push_back(b);
						Sbgm3.play(); std::cout << "bgm3play" << std::endl;
						score[0] += 5000;
					}
					else if (level == 4)
					{
						Sbgm3.pause();
						zootower *b = new zootower();
						b->settings(sZoobase4, 70, 500);
						entities.push_back(b);
						Sbgm4.play(); std::cout << "bgm4play" << std::endl;
						score[0] += 5000;
					}
					else if (level == 5)
					{
						Sbgm5.pause();
						zootower *b = new zootower();
						b->settings(sZoobase5, 70, 500);
						entities.push_back(b);
						Sbgm5.play(); std::cout << "bgm5play" << std::endl;
						score[0] += 5000;
					}
				}
//----------------------------------------------show money----------------------------------------------------
				std::stringstream catmoney, maxcatmoney;
				catmoney << int(catcan);
				maxcatmoney << "/ " << int(maxcatcan);
				money.setString(catmoney.str());
				maxmoney.setString(maxcatmoney.str());
//----------------------------------------------regen money----------------------------------------------------
				if (catcan < maxcatcan && pauseON == 0)
				{
					catcan += maxcatcan / 3000;
					maxnotification = 0;
				}
				else if(catcan >= maxcatcan && maxnotification == 0)
				{
					maxnotification = 1;
					Smax.play();
				}
				if (catcan > maxcatcan) catcan = maxcatcan;
//----------------------------------------------spawn enemy----------------------------------------------------
				if (spawnenemy >= 0 && pauseON == 0) spawnenemy -= 1;
				if (spawnenemy == 0)
				{
					spawnenemy = 600;
					int randomnew = rand() % level;
					if(randomnew == 0)
					{
						dog *d = new dog(); std::cout << "spawn dog" << std::endl;
						d->settings(sDog, 100, 500 + rand() % 70);
						entities.push_back(d);
					}
					else if(randomnew ==1)
					{
						snake *d = new snake(); std::cout << "spawn snake" << std::endl;
						d->settings(sSnake, 100, 500 + rand() % 70);
						entities.push_back(d);
					}
					else if (randomnew == 2)
					{
						gangster *d = new gangster(); std::cout << "spawn gangster" << std::endl;
						d->settings(sGang, 100, 500 + rand() % 70);
						entities.push_back(d);
					}
					else if (randomnew == 3)
					{
						hippo *d = new hippo(); std::cout << "spawn hippo" << std::endl;
						d->settings(sHippo, 100, 500 + rand() % 70);
						entities.push_back(d);
					}
					else if (randomnew == 4)
					{
						if (rand() % 4 == 0)
						{
							pig *d = new pig();
							d->settings(sPig, 100, 500 + rand() % 70);
							entities.push_back(d);
						}
					}
				}				
//------------------------------------------------check spawn------------------------------------------------
				if (Keyboard::isKeyPressed(Keyboard::Q) && timeballcatcooldown <= seconds(0) && catcan > 50 && level > 0 )
				{
					timeballcatcooldown = seconds(4.33);
					ballcat *bc = new ballcat(); std::cout << "spawn ball cat" << std::endl;
					bc->settings(sballCat, 1200, 500 + rand() % 70);
					entities.push_back(bc);
					catcan -= 50;
					Sbuy.play();
					for (auto k : entities)
					{
						if (k->name == "banner")
						{
							if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); } 
							if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
							if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
							if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
							if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
						}
					}
				}
				else if (Keyboard::isKeyPressed(Keyboard::W) && timelongcatcooldown <= seconds(0) && catcan > 100 && level > 1 )
				{
					timelongcatcooldown = seconds(8.33);
					longcat *c = new longcat(); std::cout << "spawn long cat" << std::endl;
					c->settings(slongCat, 1200, 500 + rand() % 70);
					entities.push_back(c);
					catcan -= 100;
					Sbuy.play();
					for (auto k : entities)
					{
						if (k->name == "banner")
						{
							if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
							if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
							if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
							if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
							if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
						}
					}
				}
				else if (Keyboard::isKeyPressed(Keyboard::E) && timeaxecatcooldown <= seconds(0) && catcan > 150 && level > 2 )
				{
					timeaxecatcooldown = seconds(7.33);
					axecat *c = new axecat(); std::cout << "spawn axe cat" << std::endl;
					c->settings(saxeCat, 1200, 500 + rand() % 70);
					entities.push_back(c);
					catcan -= 150;
					Sbuy.play();
					for (auto k : entities)
					{
						if (k->name == "banner")
						{
							if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
							if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
							if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
							if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
							if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
						}
					}
				}
				else if (Keyboard::isKeyPressed(Keyboard::R) && timelegcatcooldown <= seconds(0) && catcan > 250 && level > 3 )
				{
					timelegcatcooldown = seconds(7.33);
					legcat *c = new legcat(); std::cout << "spawn leg cat" << std::endl;
					c->settings(slegCat, 1200, 500 + rand() % 70);
					entities.push_back(c);
					catcan -= 250;
					Sbuy.play();
					for (auto k : entities)
					{
						if (k->name == "banner")
						{
							if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
							if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
							if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
							if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
							if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
						}
					}
				}
				else if (Keyboard::isKeyPressed(Keyboard::T) && timecowcatcooldown <= seconds(0) && catcan > 400 && level > 4 )
				{
					timecowcatcooldown = seconds(2.33);
					cowcat *c = new cowcat(); std::cout << "spawn cow cat" << std::endl;
					c->settings(scowCat, 1200, 500 + rand() % 70);
					entities.push_back(c);
					catcan -= 400;
					Sbuy.play();
					for (auto k : entities)
					{
						if (k->name == "banner")
						{
							if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
							if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
							if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
							if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
							if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
						}
					}
				}
				else if (Keyboard::isKeyPressed(Keyboard::U)) { catcan = maxcatcan; }
				else if (Keyboard::isKeyPressed(Keyboard::Space))
				{
					if (win == 1)
					{
						win = 0;
						if(level < 5) level += 1;
						else 
						{ 
							level = 1, gamemode = 0, menumode = 1;
						}
						drawbanner = 0, Cbase = 0, Zbase = 0, catcan = 0, maxcatcan = 600, pauseON = 0;
					}
					if (loss == 1)
					{
						if (level == 1) Sbgm1.pause();
						else if (level == 2) Sbgm2.pause();
						else if (level == 3) Sbgm3.pause();
						else if (level == 4) Sbgm4.pause();
						else if (level == 5) Sbgm5.pause();
						level = 1, loss = 0, drawbanner = 0, drawmenu = 0, menumode = 1, pauseON = 0, Zbase = 0, Cbase = 0, gamemode = 0;
					}
					if (pauseON == 1) pauseON = 0;
					for (auto k : entities)
					{
						if (k->name == "upmax")
						{
							if (k->type == 2 && catcan >90)
							{
								maxcatcan = 800;
								catcan -= 90;
								Sbuy.play();
								k->atking = 0;
								k->settings(supmax2w, 120, 720); std::cout << "upmax" << std::endl;
								k->type = 3;
							}
							if (k->type == 4 && catcan >180)
							{
								maxcatcan = 1000;
								catcan -= 180;
								Sbuy.play();
								k->atking = 0;
								k->settings(supmax3w, 120, 720); std::cout << "upmax" << std::endl;
								k->type = 5;
							}
						}
						if (k->name == "banner")
						{
							if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
							if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
							if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
							if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
							if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
						}
					}
				}
				else if (Keyboard::isKeyPressed(Keyboard::P))
				{
					if (pauseON == 0) pauseON = 1;
					else pauseON = 0;
				}
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (Mouse::getPosition(app).x < 238 && Mouse::getPosition(app).y > 510)
					{
						for (auto k : entities)
						{
							if (k->name == "upmax")
							{
								if (k->type == 2 && catcan > 90)
								{
									maxcatcan = 800;
									catcan -= 90;
									Sbuy.play();
									k->atking = 0;
									k->settings(supmax2w, 120, 720); std::cout << "upmax" << std::endl;
									k->type = 3;
								}
								if (k->type == 4 && catcan > 180)
								{
									maxcatcan = 1000;
									catcan -= 180;
									Sbuy.play();
									k->atking = 0;
									k->settings(supmax3w, 120, 720); std::cout << "upmax" << std::endl;
									k->type = 5;
								}
							}
							if (k->name == "banner")
							{
								if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
								if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
								if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
								if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
								if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
							}
						}
					}
					if (Mouse::getPosition(app).y > 565 && Mouse::getPosition(app).y < 700)
					{
						if (Mouse::getPosition(app).x > 283 && Mouse::getPosition(app).x < 457 && timeballcatcooldown <= seconds(0) && catcan > 50 && level > 0)
						{
							timeballcatcooldown = seconds(4.33);
							ballcat *bc = new ballcat(); std::cout << "spawn ball cat" << std::endl;
							bc->settings(sballCat, 1200, 500 + rand() % 70);
							entities.push_back(bc);
							catcan -= 50;
							Sbuy.play();
							for (auto k : entities)
							{
								if (k->name == "banner")
								{
									if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
									if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
									if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
									if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
									if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
								}
							}
						}
						if (Mouse::getPosition(app).x > 483 && Mouse::getPosition(app).x < 657 && timelongcatcooldown <= seconds(0) && catcan > 100 && level > 1)
						{
							timelongcatcooldown = seconds(8.33);
							longcat *c = new longcat(); std::cout << "spawn long cat" << std::endl;
							c->settings(slongCat, 1200, 500 + rand() % 70);
							entities.push_back(c);
							catcan -= 100;
							Sbuy.play();
							for (auto k : entities)
							{
								if (k->name == "banner")
								{
									if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
									if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
									if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
									if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
									if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
								}
							}
						}
						if (Mouse::getPosition(app).x > 683 && Mouse::getPosition(app).x < 857 && timeaxecatcooldown <= seconds(0) && catcan > 150 && level > 2)
						{
							timeaxecatcooldown = seconds(7.33);
							axecat *c = new axecat(); std::cout << "spawn axe cat" << std::endl;
							c->settings(saxeCat, 1200, 500 + rand() % 70);
							entities.push_back(c);
							catcan -= 150;
							Sbuy.play();
							for (auto k : entities)
							{
								if (k->name == "banner")
								{
									if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
									if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
									if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
									if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
									if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
								}
							}
						}
						if (Mouse::getPosition(app).x > 883 && Mouse::getPosition(app).x < 1057 && timelegcatcooldown <= seconds(0) && catcan > 250 && level > 3)
						{
							timelegcatcooldown = seconds(7.33);
							legcat *c = new legcat(); std::cout << "spawn leg cat" << std::endl;
							c->settings(slegCat, 1200, 500 + rand() % 70);
							entities.push_back(c);
							catcan -= 250;
							Sbuy.play();
							for (auto k : entities)
							{
								if (k->name == "banner")
								{
									if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
									if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
									if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
									if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
									if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
								}
							}
						}
						if (Mouse::getPosition(app).x > 1083 && Mouse::getPosition(app).x < 1257 && timecowcatcooldown <= seconds(0) && catcan > 400 && level > 4)
						{
							timecowcatcooldown = seconds(2.33);
							cowcat *c = new cowcat(); std::cout << "spawn cow cat" << std::endl;
							c->settings(scowCat, 1200, 500 + rand() % 70);
							entities.push_back(c);
							catcan -= 400;
							Sbuy.play();
							for (auto k : entities)
							{
								if (k->name == "banner")
								{
									if (k->type == 5) { k->atking = 0;	k->settings(scoww, 1170, 700); }
									if (k->type == 4) { k->atking = 0;	k->settings(slegw, 970, 700); }
									if (k->type == 3) { k->atking = 0;	k->settings(saxew, 770, 700); }
									if (k->type == 2) { k->atking = 0;	k->settings(slongw, 570, 700); }
									if (k->type == 1) { k->atking = 0;	k->settings(sballw, 370, 700); }
								}
							}
						}
					}
					if (win == 1)
					{
						win = 0;
						if (level < 5) level += 1;
						else
						{
							level = 1, gamemode = 0, menumode = 1;
						}
						drawbanner = 0, Cbase = 0, Zbase = 0, catcan = 0, maxcatcan = 600, pauseON = 0;
					}
					if (loss == 1)
					{
						if (level == 1) Sbgm1.pause();
						else if (level == 2) Sbgm2.pause();
						else if (level == 3) Sbgm3.pause();
						else if (level == 4) Sbgm4.pause();
						else if (level == 5) Sbgm5.pause();
						level = 1, loss = 0, drawbanner = 0, drawmenu = 0, menumode = 1, pauseON = 0, Zbase = 0, Cbase = 0, gamemode = 0;
					}
					if (pauseON == 1) pauseON = 0;
				}
//-----------------------------------------------------check collide-------------------------------------------------
				for (auto c : entities)
					for (auto z : entities)
					{
						if (c->name == "cat" && z->name == "zoo" && c->alive == true && z->alive == true)
						{
//------------------------------------------------------first meet---------------------------------------------------
							if (isCollide(c, z) && c->atking == false && z->atking == false)
							{
								c->atking = 1;
								c->dx = 0;
								z->atking = 1;
								z->dx = 0;
								if (c->type == 1) { c->settings(sballCatA, c->x, c->y); std::cout << "ballcatcollide" << std::endl; }
								else if (c->type == 2) { c->settings(slongCatA, c->x, c->y); std::cout << "longcatcollide" << std::endl; }
								else if (c->type == 3) { c->settings(saxeCatA, c->x, c->y); std::cout << "axecatcollide" << std::endl; }
								else if (c->type == 4) { c->settings(slegCatA, c->x, c->y); std::cout << "legcatcollide" << std::endl; }
								else if (c->type == 5) { c->settings(scowCatA, c->x, c->y); std::cout << "cowcatcollide" << std::endl; }
								if (z->type == 6) { z->settings(sDogA, z->x, z->y); std::cout << "dogcollide" << std::endl; }
								else if (z->type == 7) { z->settings(sSnakeA, z->x, z->y); std::cout << "snakecollide" << std::endl; }
								else if (z->type == 8) { z->settings(sGangA, z->x, z->y); std::cout << "gangstercollide" << std::endl; }
								else if (z->type == 9) { z->settings(sHippoA, z->x, z->y); std::cout << "hippocollide" << std::endl; }
								else if (z->type == 10) { z->settings(sPigA, z->x, z->y); std::cout << "pigcollide" << std::endl; }
							}
//------------------------------------------------thai zoo mong----------------------------------------------------------
							if (isCollide(c, z) && c->atking == true && z->atking == false)
							{
								z->atking = true;
								z->dx = 0;
								if (z->type == 6) { z->settings(sDogA, z->x, z->y); std::cout << "new dog join fight" << std::endl; }
								else if (z->type == 7) { z->settings(sSnakeA, z->x, z->y); std::cout << "new snake join fight" << std::endl; }
								else if (z->type == 8) { z->settings(sGangA, z->x, z->y); std::cout << "new gangster join fight" << std::endl; }
								else if (z->type == 9) { z->settings(sHippoA, z->x, z->y); std::cout << "new hippo join fight" << std::endl; }
								else if (z->type == 10) { z->settings(sPigA, z->x, z->y); std::cout << "new pig join fight" << std::endl; }
							}
//----------------------------------------------thai cat mong------------------------------------------------------------
							if (isCollide(c, z) && c->atking == false && z->atking == true)
							{
								c->atking = 1;
								c->dx = 0;
								if (c->type == 1) { c->settings(sballCatA, c->x, c->y); std::cout << "new ballcat join fight" << std::endl; }
								else if (c->type == 2) { c->settings(slongCatA, c->x, c->y); std::cout << "new longcat join fight" << std::endl;}
								else if (c->type == 3) { c->settings(saxeCatA, c->x, c->y); std::cout << "new axecat join fight" << std::endl; }
								else if (c->type == 4) { c->settings(slegCatA, c->x, c->y); std::cout << "new legcat join fight" << std::endl; }
								else if (c->type == 5) { c->settings(scowCatA, c->x, c->y); std::cout << "new cowcat join fight" << std::endl; }
							}
//-------------------------------------------------------check fight------------------------------------------------------
							if (isCollide(c, z) && c->atking == true && z->atking == true)
							{
//--------------------------------------------------------atk!--------------------------------------------------------------------
								if (c->anim.isEnd() && c->atked == false)
								{
									if (z->type == 9999) Satktower.play();
									else Satk.play();
									z->hp -= c->atk; std::cout << "catatk zoo hp = " << z->hp << " point" << std::endl;
									c->atked = true;
						/*			if (z->type == 9999 && z->hp < 400 && zooburst == 0 && level > 4)
									{
										pig *d = new pig();
										d->settings(sPig, 100, 500 + rand() % 70);
										entities.push_back(d);                               std::cout << "zoo burst" << std::endl;
										zooburst = 1;
									}*/
								}
								if (z->anim.isEnd() && z->atked == false)
								{
									if (c->type == 9999) Satktower.play();
									else Satk.play();
									c->hp -= z->atk; std::cout << "zooatk cat hp = " << c->hp << " point" << std::endl;
									z->atked = true;
								}
//--------------------------------------------bye cat died-----------------------------------------------------------------
								if (c->hp <= 0)
								{
									c->alive = false;
									if (c->type == 9999)
									{
										Stowerdown.play();
										if (level == 1) Sbgm1.pause();
										if (level == 2) Sbgm2.pause();
										if (level == 3) Sbgm3.pause();
										if (level == 4) Sbgm4.pause();
										if (level == 5) Sbgm5.pause();
										for (auto f : entities)
										{
											f->alive = 0;
										}
										pauseON = 1, Cbase = 0, loss = 1, catcan = 0, maxcatcan = 600, zooburst = 0;
										Sloss.play(); std::cout << "loss" << std::endl;
										score[0] = scorenow;
										scorenow = 10000;
										player[0] = nowname;
										yourname.setString("Enter your name");
//------------------------------------------record new score---------------------------------------------------------------------------------
										std::ifstream ScoreFile;
										ScoreFile.open("score.txt");
										ScoreFile >> score[1] >> score[2] >> score[3] >> score[4] >> score[5];
										ScoreFile.close();
										std::ifstream PlayerFile;
										PlayerFile.open("name.txt");
										PlayerFile >> player[1] >> player[2] >> player[3] >> player[4] >> player[5];
										PlayerFile.close(); 
										int temp;
										std::string stemp;
										for (int i = 0; i < 6; i++) {
											for (int j = i + 1; j < 6; j++) 
											{
												if (score[i] < score[j]) 
												{
													temp = score[i];	score[i] = score[j];	score[j] = temp;
													stemp = player[i];	player[i] = player[j];	player[j] = stemp;
												}
											}
										}
										std::ofstream WritePlayer;
										WritePlayer.open("name.txt");
										for (int i = 0; i < 5; i++) { WritePlayer << player[i] << std::endl; }
										WritePlayer.close();
										std::ofstream WriteScore;
										WriteScore.open("score.txt");
										for (int i = 0; i < 5; i++) { WriteScore << score[i] << std::endl; }
										WriteScore.close();
									}
									else 
									{
										Sdie.play(); std::cout << "cat is dead" << std::endl;
										z->atking = false;
										for (auto y : entities)
										{
											if (isCollide(c, y) && y->name == "zoo")
											{
												y->atking = false;
												if (y->type == 6) { y->settings(sDog, y->x, y->y); y->dx = 0.5; }
												else if (y->type == 7) { y->settings(sSnake, y->x, y->y); y->dx = 0.8; }
												else if (y->type == 8) { y->settings(sGang, y->x, y->y); y->dx = 1; }
												else if (y->type == 9) { y->settings(sHippo, y->x, y->y); y->dx = 0.4; }
												else if (y->type == 10) { y->settings(sPig, y->x, y->y); y->dx = 0.5; }
											}
										}
									}	
								}
//--------------------------------------------bye zoo died-----------------------------------------------------------
								if (z->hp <= 0)
								{
									z->alive = false;
									if (z->type == 9999)
									{
										if (level == 1) Sbgm1.pause();
										if (level == 2) Sbgm2.pause();
										if (level == 3) Sbgm3.pause();
										if (level == 4) Sbgm4.pause();
										if (level == 5) Sbgm5.pause();
										for (auto f : entities)
										{
											f->alive = 0;
										}
										pauseON = 1, win = 1 ;
										Stowerdown.play(); Swin.play();
										zooburst = 0;
										if (level < 5) { score[0] += scorenow; scorenow = 10000; }
										else
										{
											player[0] = nowname;
											yourname.setString("Enter your name");
//------------------------------------------record new score---------------------------------------------------------------------------------
											std::ifstream ScoreFile;
											ScoreFile.open("score.txt");
											ScoreFile >> score[1] >> score[2] >> score[3] >> score[4] >> score[5];
											ScoreFile.close();
											std::ifstream PlayerFile;
											PlayerFile.open("name.txt");
											PlayerFile >> player[1] >> player[2] >> player[3] >> player[4] >> player[5];
											PlayerFile.close();
											int temp;
											std::string stemp;
											for (int i = 0; i < 6; i++) {
												for (int j = i + 1; j < 6; j++) 
												{
													if (score[i] < score[j]) 
													{
														temp = score[i];	score[i] = score[j];	score[j] = temp;
														stemp = player[i];	player[i] = player[j];	player[j] = stemp;
													}
												}
											}
											std::ofstream WritePlayer;
											WritePlayer.open("name.txt");
											for (int i = 0; i < 5; i++) { WritePlayer << player[i] << std::endl; }
											WritePlayer.close();
											std::ofstream WriteScore;
											WriteScore.open("score.txt");
											for (int i = 0; i < 5; i++) { WriteScore << score[i] << std::endl; }
											WriteScore.close();
										}
									}
									else
									{
										Sdie.play(); std::cout << "zoo is dead" << std::endl;
										c->atking = false;
										for (auto f : entities)
										{
											if (isCollide(f, z) && f->name == "cat")
											{
												f->atking = false;
												if (f->type == 1) { f->settings(sballCat, f->x, f->y); f->dx = -1; }
												else if (f->type == 2) { f->settings(slongCat, f->x, f->y); f->dx = -0.8; }
												else if (f->type == 3) { f->settings(saxeCat, f->x, f->y); f->dx = -0.8; }
												else if (f->type == 4) { f->settings(slegCat, f->x, f->y); f->dx = -1.2; }
												else if (f->type == 5) { f->settings(scowCat, f->x, f->y); f->dx = -3; }
											}
										}
										if (z->type == 6) catcan += 40;
										else if (z->type == 7) catcan += 45;
										else if (z->type == 8) catcan += 50;
										else if (z->type == 9) catcan += 200;			
										else if (z->type == 10) catcan += 400;
									}
								}
							}
						}//<--collide
					}
			}//<-gamemode=1
		else if (scoremode == 1)
		{
			if (scoresong == 0)
			{
				scoresong = 1;
				Songscore.play();
				
				ScoreFile.open("score.txt");
				ScoreFile >> score[1] >> score[2] >> score[3] >> score[4] >> score[5];
				ScoreFile.close();
				PlayerFile.open("name.txt");
				PlayerFile >> player[1] >> player[2] >> player[3] >> player[4] >> player[5];
				PlayerFile.close();
				
				std::stringstream first;
				first << score[1];
				sp1.setString(first.str());
				std::stringstream secon;
				secon << score[2];
				sp2.setString(secon.str());
				std::stringstream third;
				third << score[3];
				sp3.setString(third.str());
				std::stringstream fourth;
				fourth << score[4];
				sp4.setString(fourth.str());
				std::stringstream fifth;
				fifth << score[5];
				sp5.setString(fifth.str());

				p1.setString(player[1]);
				p2.setString(player[2]);
				p3.setString(player[3]);
				p4.setString(player[4]);
				p5.setString(player[5]);
			
				std::cout << "update" << std::endl;
			}
			if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				debounce--;
				if (debounce <= 0)
				{
					scoremode = 0;
					menumode = 1;
					gamemode = 0;
					debounce = 10;
					Songscore.pause();
					scoresong = 0;
					pauseON = 0;
				}
			}
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				debounce--;
				if (debounce <= 0)
				{
					scoremode = 0;
					menumode = 1;
					gamemode = 0;
					debounce = 10;
					Songscore.pause();
					scoresong = 0;
					pauseON = 0;
				}
			}
		}
//-------------------------------------check who die (alive==false) delete it-----------------------------------------------
			for (auto i = entities.begin();i != entities.end();)
			{
				
					Entity *e = *i;
					if (pauseON == 0)
					{
						e->update();
						e->anim.update();
						e->atked = false;
					}
					if (e->alive == false) {i = entities.erase(i); delete e;}
					else i++;
				
			}
			if (gamemode == 1) 
			{
				if (level == 1) app.draw(Sbg1);
				if (level == 2) app.draw(Sbg2);
				if (level == 3) app.draw(Sbg3);
				if (level == 4) app.draw(Sbg4);
				if (level == 5) app.draw(Sbg5);
				if (pauseON == 0) { app.draw(money); app.draw(maxmoney); }
				if (win == 1) app.draw(swin);
				if (loss == 1)app.draw(sloss);
			}
			if (scoremode == 1) { app.draw(Sscore); app.draw(sp1); app.draw(sp2); app.draw(sp3); app.draw(sp4); app.draw(sp5); app.draw(p1); app.draw(p2); app.draw(p3); app.draw(p4); app.draw(p5); Sleep(5); }
//-------------------------------------------draw every thing----------------------------------------------------------
			for (auto i : entities) { if(pauseON==0) i->draw(app); }
			if(gamemode == 1 || menumode == 1) app.draw(yourname);
			app.display();
			app.clear();
	}
	return 0;
}