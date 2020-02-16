#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>
using namespace sf;
Texture t1, p1, p2, e1, e2, e3, e4, b, explo, sheild, dmg, hlkit, sh_disp;
int score = 0;

const double PI = 3.14159265;
const double DTR = PI / 180.0;
int num_explo = 0;
class explosion {
public:
	Sprite boom;
	IntRect frame;
	bool exists;
	int fr;
	explosion() {
		exists = false;
		boom.setTexture(explo);
		frame.height = 128;
		frame.width = 128;
	}
	void getPos(int x, int y) {
		boom.setPosition(x, y);
		exists = true;
		fr = 1;
	}
	void active() {
		if (exists) {
			if (fr <= 5) {
				frame.left = 0;
				frame.top = 0;
				fr++;
			}
			else if (fr <= 10) {
				frame.left = 128;
				frame.top = 0;
				fr++;
			}
			else if (fr <= 15) {
				frame.left = 256;
				frame.top = 0;
				fr++;
			}
			else if (fr <= 20) {
				frame.left = 384;
				frame.top = 0;
				fr++;
			}
			else if (fr <= 25) {
				frame.left = 0;
				frame.top = 128;
				fr++;
			}
			else if (fr <= 30) {
				frame.left = 128;
				frame.top = 128;
				fr++;
			}
			else if (fr <= 35) {
				frame.left = 256;
				frame.top = 128;
				fr++;
			}
			else if (fr <= 40) {
				frame.left = 384;
				frame.top = 128;
				fr++;
			}
			else if (fr <= 45) {
				frame.left = 0;
				frame.top = 256;
				fr++;
			}
			else if (fr <= 50) {
				frame.left = 128;
				frame.top = 256;
				fr++;
			}
			else if (fr <= 55) {
				frame.left = 256;
				frame.top = 256;
				fr++;
			}
			else if (fr <= 60) {
				frame.left = 384;
				frame.top = 256;
				fr++;
			}
			else if (fr <= 65) {
				frame.left = 0;
				frame.top = 384;
				fr++;
			}
			else if (fr <= 70) {
				frame.left = 128;
				frame.top = 384;
				fr++;
			}
			else {
				exists = false;
			}
			boom.setTextureRect(frame);
			boom.setOrigin(64, 64);
		}
		
	}
	void draw(RenderWindow &win) {
		if (exists) {
			win.draw(boom);
		}
	}
};
explosion bang[150];

class projectile {
	int x;
	int y;
	int d_x;
	int d_y;
	int speed;
	bool exists;
	int damage;
	bool enemy;
public:
	Sprite proj;
	projectile() : exists(false){};
	void set(int xp, int yp, int s, int d, Texture &t, bool e, int dx, int dy){
		proj.setTexture(t);
		exists = true;
		x = xp;
		y = yp;
		d_x = dx;
		d_y = dy;
		speed = s;
		damage = d;
		enemy = e;
	}
	bool check_exist() {
		return exists;
	}
	int hit() {
		return damage;
	}
	void des() {
		damage = 0;
		exists = false;
	}
	void move() {
		y += speed;
		proj.setPosition(x, y);
		if (y < 0) {
			exists = false;
		}
	}
	void angular_move(int xm, int ym) {
		x += xm;
		y += ym;
		proj.setPosition(x, y);
		if (y < 0 || y > 700) {
			exists = false;
		}
		if (x < 0 || x > 800) {
			exists = false;
		}
	}
	
};
class viking_orbs {
public:
	projectile radial[6];
	viking_orbs(){}
	void set_orbs(int x, int y){
		for (int i = 0; i < 6; i++) {
			radial[i].set(x, y, 8, 1, p2, true, 10, 10);
		}
	}
	void move() {
		radial[0].angular_move(10, 0);
		radial[1].angular_move(5, 9);
		radial[2].angular_move(-5, 9);
		radial[3].angular_move(-10, 0);
		radial[4].angular_move(-5, -9);
		radial[5].angular_move(5, -9);
	}
	void disp(RenderWindow &win) {
		for (int i = 0; i < 6; i++) {
			if (radial[i].check_exist()) {
				win.draw(radial[i].proj);
			}
		}
	}
};

class marauder_orbs {
public:
	projectile radial[3];
	marauder_orbs() {}
	void set_orbs(int x, int y) {
		for (int i = 0; i < 6; i++) {
			radial[i].set(x, y, 8, 1, p2, true, 10, 10);
		}
	}
	void move() {
		radial[0].angular_move(7, 7);
		radial[1].angular_move(0, 10);
		radial[2].angular_move(-7, 7);
		
	}
	void disp(RenderWindow &win) {
		for (int i = 0; i < 3; i++) {
			if (radial[i].check_exist()) {
				win.draw(radial[i].proj);
			}
		}
	}
};
viking_orbs vik_orbs[10];
marauder_orbs mau_orbs[10];
projectile guns[100];
projectile e_guns[400];

class ship {
protected:
	int health;
	int rate_of_fire;
	int move_speed;
	int dim_x;
	int dim_y;
public:
	ship(int h, int r, int m, int x, int y) : health(h), rate_of_fire(r), move_speed(m), dim_x(x), dim_y(y){}
	ship(){}
	void set_deets(int h, int r, int m, int x, int y) {
		health = h;
		rate_of_fire = r;
		move_speed = m;
		dim_x = x;
		dim_y = y;
	}
	int get_health() {
		return health;
	}
};


class player : public ship {
	int plyr_x, plyr_y;
	int in;
	int i = 0;
	bool dmgx2, sheild;
public:
	bool move_up, move_down, move_left, move_right;
	Sprite plyr;
	player(): ship(8, 20, 5, 58, 57), plyr(t1) {
		in = 0;
		move_up = false, move_down = false, move_left = false, move_right = false;
		plyr_x = 400;
		plyr_y = 600;
		plyr.setPosition(plyr_x, plyr_y);
		plyr.setOrigin(29, 28);
		dmgx2 = false;
		sheild = false;
	}
	void move() {
		if (move_up && plyr_y > 0) {
			plyr_y -= move_speed;
		}
		else if (move_down && plyr_y < 640) {
			plyr_y += move_speed;
		}

		if (move_right && plyr_x < 730) {
			plyr_x += move_speed;
		}
		else if (move_left && plyr_x > 0) {
			plyr_x -= move_speed;
		}
		plyr.setPosition(plyr_x, plyr_y);
	}
	void shoot(){
		if(i == rate_of_fire){
			if (dmgx2) {
				guns[in].set(plyr_x, plyr_y - 38, -15, 2, p1, false, 5, 15);
				in++;
				if (in == 99) {
					in = 0;
				}
				i = 0;
			}
			else {
				guns[in].set(plyr_x, plyr_y -38, -15, 1, p1, false, 5, 15);
				in++;
				if (in == 99) {
					in = 0;
				}
				i = 0;
			}
		}
		i++;
	}
	void  inc_health() {
		if (health < 8) {
			health++;
		}
	}
	void sDmgx2(bool v) {
		dmgx2 = v;
	}
	void sSheild(bool v) {
		sheild = v;
	}
	bool g_DM() {
		return dmgx2;
	}
	bool g_Sheild() {
		return sheild;
	}
	void ship_hit() {
		if (!sheild) {
			health--;
		}
	}
	void collision_detect() {
		int j;
		for (int i = 0; i < 10; i++) {
			for (j = 0; j < 6; j++) {
				if (plyr.getGlobalBounds().intersects(vik_orbs[i].radial[j].proj.getGlobalBounds()) && vik_orbs[i].radial[j].check_exist()) {
					ship_hit();
					vik_orbs[i].radial[j].des();
				}
			}
		}
		for (int i = 0; i < 10; i++) {
			for (j = 0; j < 3; j++) {
				if (plyr.getGlobalBounds().intersects(mau_orbs[i].radial[j].proj.getGlobalBounds()) && mau_orbs[i].radial[j].check_exist()) {
					ship_hit();
					mau_orbs[i].radial[j].des();
				}
			}
		}
		for (int i = 0; i < 400; i++) {
			if (plyr.getGlobalBounds().intersects(e_guns[i].proj.getGlobalBounds()) && e_guns[i].check_exist()) {
				ship_hit();
				e_guns[i].des();
			}
		}
	}
};

class powerup {
	int count;
	int behaviour;
	bool exists, active;
public:
	Sprite power;
	powerup() {
	}
	void set() {
		exists = true;
		active = false;
		power.setPosition(50 + (rand() % 700), -50);
		behaviour = rand() % 3;
		if (behaviour == 0) {
			power.setTexture(hlkit);
		}
		else if (behaviour == 1) {
			power.setTexture(dmg);
		}
		else {
			power.setTexture(sheild);
		}
	}

	void detect_collision(player &p) {
		if (power.getGlobalBounds().intersects(p.plyr.getGlobalBounds()) && exists) {
			count = 0;
			exists = false;
			active = true;
		}
	}

	void act(player &p) {
		if (active) {
			if (behaviour == 0) {
				p.inc_health();
				active = false;
			}
			else if (behaviour == 1 && count < 600) {
				p.sDmgx2(true);
			}
			else if (behaviour == 2 && count < 600) {
				p.sSheild(true);
			}
		}
		else {
			p.sSheild(false);
			p.sDmgx2(false);
		}
		count++;
		if (count >= 600) {
			active = false;
		}
	}
	void move() {
		power.setPosition(power.getPosition() + Vector2f(0, 5));
		if (power.getPosition().y >= 700) {
			exists = false;
		}
	}
	void draw(RenderWindow &win) {
		if (exists) {
			win.draw(power);
		}
	}

};

class enemy : public ship{
protected:
	int enm_x, enm_y;
	bool en_exists;
	float angle;
	int value;
	int i = 0;
	int in = 0;
public:
	Sprite en;
	enemy(){	
	}
	void setspawn(Texture &t, int h, int rof, int ms,int x, int y, int d, int dx, int dy, int val){
		value = val;
		en_exists = true;
		set_deets(h, rof, ms, dx, dy);
		en.setTexture(t);
		en.setOrigin(dim_x / 2, dim_y / 2);
		enm_x = x;
		enm_y = y;
		en.setPosition(enm_x, enm_y);
		if (d == 0 || d == 1) {
			angle = 90;
			en.setRotation(0);
		}
		else if (d == 2) {
			angle = 0;
			en.setRotation(90);
		}
		else if (d == 3) {
			angle = 0;
			en.setRotation(270);
		}
	}
	void explode() {
		bang[num_explo].getPos(enm_x, enm_y);
		num_explo++;
		if (num_explo == 150) {
			num_explo = 0;
		}
	}
	bool get_exi() {
		return en_exists;
	}
	void checks_exist() {
		if (health <= 0) {
			explode();
			score += value;
			en_exists = false;
		}
	}
	void collision_detect() {
		for (int i = 0; i < 100; i++) {
			if (en.getGlobalBounds().intersects(guns[i].proj.getGlobalBounds()) && en_exists && guns[i].check_exist()) {
				health -= guns[i].hit();
				guns[i].des();
				checks_exist();
			}
		}
	}
	void collision_detect(player &p) {
		if (en.getGlobalBounds().intersects(p.plyr.getGlobalBounds()) && en_exists) {
			health -= 1;
			p.ship_hit();
			checks_exist();
		}
	}
	
	void movey() {
		enm_y += move_speed;
		en.setPosition(enm_x, enm_y);
		collision_detect();
	}
	void movex() {
		enm_x += move_speed;
		en.setPosition(enm_x, enm_y);
		collision_detect();
	}
	void movexm() {
		enm_x -= move_speed;
		en.setPosition(enm_x, enm_y);
		collision_detect();
	}
	void rad_move(int d, int h) {
		if (d == 0) {
			if (enm_y >= h) {
				enm_y += move_speed * sin(angle * DTR);
				enm_x += move_speed * cos(angle * DTR);
				angle -= 0.5;
				en.setPosition(enm_x, enm_y);
				en.rotate(-0.5);
			}
			else {
				movey();
			}
		}
		else if(d == 1) {
			if (enm_y >= h) {
				enm_y += move_speed * sin(angle * DTR);
				enm_x += move_speed * cos(angle * DTR);
				angle += 0.5;
				en.setPosition(enm_x, enm_y);
				en.rotate(0.5);
			}
			else {
				movey();
			}
		}
		else if (d == 2) {
			if (enm_x >= h) {
				enm_y -= move_speed * sin(angle * DTR);
				enm_x += move_speed * cos(angle * DTR);
				angle += 0.5;
				en.setPosition(enm_x, enm_y);
				en.rotate(-0.5);
			}
			else {
				movex();
			}
		}
		else if (d == 3) {
			if (enm_x <= 800 - h) {
				enm_y -= move_speed * sin(angle * DTR);
				enm_x -= move_speed * cos(angle * DTR);
				angle += 0.5;
				en.setPosition(enm_x, enm_y);
				en.rotate(0.5);
			}
			else {
				movexm();
			}
		}
		collision_detect();
	}

	void shoot() {
		if (i == rate_of_fire) {
			e_guns[in].set(enm_x, enm_y, 8, 1, p2, true, 10, 10);
			in++;
			if (in == 399) {
				in = 0;
			}
			i = 0;
		}
		i++;
	}

	void rad_shoot(){
		if (i == rate_of_fire) {
			vik_orbs[in].set_orbs(enm_x, enm_y);
			in++;
			if (in > 9) {
				in = 0;
			}
			i = 0;
		}
		i++;
	}
	void shotgun() {
		if (i == rate_of_fire) {
			mau_orbs[in].set_orbs(enm_x, enm_y);
			in++;
			if (in > 9) {
				in = 0;
			}
			i = 0;
		}
		i++;
	}

	int get_x() {
		return enm_x;
	}
	int get_y() {
		return enm_y;
	}
};

enemy vikings[5];
enemy marauder[5];
enemy viper[24];

class kama_swarm {
	bool exists;
	int spawn_dir;
public:
	enemy kamakazi[10] = {};
	kama_swarm() {
		exists = false;
	}
	void set_exists(){
		exists = true;
		spawn_dir = rand() % 4;
	}
	void check_exists() {
		if (spawn_dir == 0 && (kamakazi[9].get_y() >= 760 || kamakazi[9].get_x() >= 860)) {
			exists = false;
		}
		if (spawn_dir == 1 && (kamakazi[9].get_y() >= 760 || kamakazi[9].get_x() <= -60)) {
			exists = false;
		}
		if (spawn_dir == 2 && kamakazi[9].get_y() <= 0) {
			exists = false;
		}
		if (spawn_dir == 3 && kamakazi[9].get_y() <= 0) {
			exists = false;
		}
	}
	bool get_exists() {
		return exists;
	}
	void set_swarm(){
		if (exists && (spawn_dir == 0 || spawn_dir == 1)) {
			int x = (rand() % 200) + 300;
			int y = -60;
			for (int i = 0; i < 10; i++, y -= 60) {
				kamakazi[i].setspawn(e1, 1, 0, 7, x, y, spawn_dir, 60, 60, 10);
			}
		}
		else if (exists && spawn_dir == 2) {
			int y = (rand() % 200) + 300;
			int x = -60;
			for (int i = 0; i < 10; i++, x -= 60) {
				kamakazi[i].setspawn(e1, 1, 0, 7, x, y, spawn_dir, 60, 60, 10);
			}
		}
		else if (exists && spawn_dir == 3) {
			int y = (rand() % 200) + 300;
			int x = 860;
			for (int i = 0; i < 10; i++, x += 60) {
				kamakazi[i].setspawn(e1, 1, 0, 7, x, y, spawn_dir, 60, 60, 10);
			}
		}
	}
	void move() {
		if (exists) {
			for (int i = 0; i < 10; i++) {
				kamakazi[i].rad_move(spawn_dir, 100);
			}
		}
		
	}
	void disp(RenderWindow &win) {
		if (exists) {
			for (int i = 0; i < 10; i++) {
				if (kamakazi[i].get_exi()) {
					win.draw(kamakazi[i].en);
				}	
			}
		}
	}
};

int main()
{
	srand(time(0));
    RenderWindow win(VideoMode(800, 700), "Space shooter");
    win.setFramerateLimit(60);
	win.setVerticalSyncEnabled(true);
	
	Font f1;
	f1.loadFromFile("arial.ttf");
	Text te1;
	te1.setFont(f1);
	te1.setFillColor(Color::White);
	te1.setCharacterSize(18);
	te1.setPosition(0, 0);
	
	Text hlth;
	int point;
	std::string h_percent;
	hlth.setFont(f1);
	hlth.setFillColor(Color::White);
	hlth.setCharacterSize(18);
	hlth.setPosition(0, 600);
	hlth.Bold;

	Text gameover;
	gameover.setFont(f1);
	gameover.setFillColor(Color::White);
	gameover.setCharacterSize(48);
	gameover.setPosition(200, 200);
	gameover.Bold;
	gameover.setString("GAME OVER");

	Text damagex2("DMG X 2", f1, 18);
	damagex2.setPosition(0, 580);
	Text sheild_on("SHEILD", f1, 18);
	sheild_on.setPosition(0, 560);
	
	t1.loadFromFile("images/spaceship.png");
	p1.loadFromFile("images/laser.png");
	p2.loadFromFile("images/orb.png");
	e1.loadFromFile("images/enemy_1.png");
	e2.loadFromFile("images/enemy_2.png");
	e3.loadFromFile("images/enemy_3.png");
	e4.loadFromFile("images/enemy_5.png");
	explo.loadFromFile("images/explosjon3.png");
	sheild.loadFromFile("images/sheild.png");
	hlkit.loadFromFile("images/health.png");
	dmg.loadFromFile("images/dmgx2.png");
	sh_disp.loadFromFile("images/sh_disp.png");
	Sprite sheild_orb(sh_disp);
	sheild_orb.setOrigin(30, 30);
	player hero;
	powerup energy;

	int looper, j;

	Time spawner;
	Clock k_en;
	Clock pup;
	kama_swarm k[4];
	int k_i = 0, vik_i = 0, mau_i = 0, vip_i = 0;
	bool k_spawn = false, e_spawn = false, vik_spawn = false, mau_spawn = false, vip_spawn = false;

	int pl_shoot_clock = 0;
    while (win.isOpen())
    {
        Event e;
        while (win.pollEvent(e))
        {
            if (e.type == Event::Closed)
                win.close();
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				hero.move_right = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				hero.move_left = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				hero.move_up = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				hero.move_down = true;
			}
			if (!Keyboard::isKeyPressed(Keyboard::Right)) {
				hero.move_right = false;

			}
			if (!Keyboard::isKeyPressed(Keyboard::Left)) {
				hero.move_left = false;
			}
			if (!Keyboard::isKeyPressed(Keyboard::Up)) {
				hero.move_up = false;

			}
			if (!Keyboard::isKeyPressed(Keyboard::Down)) {
				hero.move_down = false;

			}
			
        }

		if (hero.get_health() > 0) {
			spawner = k_en.getElapsedTime();
			te1.setString("Score: " + std::to_string(score));
			if ((int)spawner.asSeconds() % 2 == 0 && (int)spawner.asSeconds() != 0) {
				if (!k_spawn) {
					if (k[k_i].get_exists() == false) {
						k[k_i].set_exists();
						k[k_i].set_swarm();
						k_i++;
						if (k_i == 4) {
							k_i = 0;
						}
						k_spawn = true;
					}
				}
			}
			else {
				k_spawn = false;
			}

			if ((int)pup.getElapsedTime().asSeconds() % 5 == 0 && (int)pup.getElapsedTime().asSeconds() >= 30) {
				if (!vik_spawn) {
					vikings[vik_i].setspawn(e3, 4, 120, 2, 64 + (rand() % 672), -60, 0, 60, 60, 50);
					vik_i++;
					if (vik_i == 5) {
						vik_i = 0;
					}
					vik_spawn = true;
				}
			}
			else {
				vik_spawn = false;
			}

			if ((int)pup.getElapsedTime().asSeconds() % 7 == 0 && (int)pup.getElapsedTime().asSeconds() >= 20) {
				if (!mau_spawn) {
					marauder[mau_i].setspawn(e4, 2, 90, 1, 40 + (rand() % 720), -128, 0, 40, 64, 40);
					mau_i++;
					if (mau_i == 5) {
						mau_i = 0;
					}
					mau_spawn = true;
				}
			}
			else {
				mau_spawn = false;
			}

			if ((int)pup.getElapsedTime().asSeconds() % 3 == 0 && (int)pup.getElapsedTime().asSeconds() >= 10) {
				if (!vip_spawn) {
					viper[vip_i].setspawn(e2, 1, 120, 4, 40 + (rand() % 720), -128, 0, 40, 64, 20);
					vip_i++;
					if (vip_i == 24) {
						vip_i = 0;
					}
					vip_spawn = true;
				}
			}
			else {
				vip_spawn = false;
			}

			if ((int)pup.getElapsedTime().asSeconds() % 10 == 0 && (int)pup.getElapsedTime().asSeconds() != 0) {
				if (!e_spawn) {
					energy.set();
					e_spawn = true;
				}
			}
			else {
				e_spawn = false;
			}
			for (looper = 0; looper < 4; looper++) {
				k[looper].move();
				k[looper].check_exists();
				for (j = 0; j < 10; j++) {
					k[looper].kamakazi[j].collision_detect(hero);
				}

			}

			for (j = 0; j < 5; j++) {
				vikings[j].collision_detect(hero);
				marauder[j].collision_detect(hero);
			}
			for (j = 0; j < 24; j++) {
				viper[j].collision_detect(hero);
			}

			hero.move();
			hero.shoot();
			hero.collision_detect();

			energy.move();
			energy.detect_collision(hero);
			energy.act(hero);

			for (looper = 0; looper < 5; looper++) {
				if (vikings[looper].get_exi()) {
					vikings[looper].movey();
					vikings[looper].rad_shoot();
				}
			}

			for (looper = 0; looper < 5; looper++) {
				if (marauder[looper].get_exi()) {
					marauder[looper].movey();
					marauder[looper].shotgun();
				}
			}

			for (looper = 0; looper < 24; looper++) {
				if (viper[looper].get_exi()) {
					viper[looper].movey();
					viper[looper].shoot();
				}
			}

			if (hero.g_DM()) {
				damagex2.setFillColor(Color::White);
			}
			else {
				damagex2.setFillColor(Color(169, 169, 169));
			}
			if (hero.g_Sheild()) {
				sheild_on.setFillColor(Color::White);
				sheild_orb.setPosition(hero.plyr.getPosition());
			}
			else {
				sheild_on.setFillColor(Color(169, 169, 169));
			}


			for (looper = 0; looper <= 99; looper++) {
				if (guns[looper].check_exist()) {
					guns[looper].move();
				}
			}
			for (looper = 0; looper <= 399; looper++) {
				if (e_guns[looper].check_exist()) {
					e_guns[looper].move();
				}
			}


			h_percent = std::to_string((hero.get_health() / 8.0) * 100);
			point = h_percent.find(".");
			hlth.setString("HEALTH\n" + h_percent.substr(0, point + 2) + "%");

			win.clear();
			win.draw(hero.plyr);
			if (hero.g_Sheild()) {
				win.draw(sheild_orb);
			}
			for (looper = 0; looper <= 99; looper++) {
				if (guns[looper].check_exist()) {
					win.draw(guns[looper].proj);
				}
			}
			for (looper = 0; looper <= 399; looper++) {
				if (e_guns[looper].check_exist()) {
					win.draw(e_guns[looper].proj);
				}
			}
			
			for (looper = 0; looper < 5; looper++) {
				if (vikings[looper].get_exi()) {
					win.draw(vikings[looper].en);
				}
				if (marauder[looper].get_exi()) {
					win.draw(marauder[looper].en);
				}
			}
			for (looper = 0; looper < 24; looper++) {
				if (viper[looper].get_exi()) {
					win.draw(viper[looper].en);
				}
			}
			for (looper = 0; looper < 4; looper++) {
				k[looper].disp(win);
			}
			energy.draw(win);

			win.draw(te1);
			win.draw(hlth);
			win.draw(damagex2);
			win.draw(sheild_on);

			for (looper = 0; looper < 150; looper++) {
				if (bang[looper].exists) {
					bang[looper].active();
					bang[looper].draw(win);
				}
			}
			for (looper = 0; looper < 10; looper++) {
				vik_orbs[looper].move();
				vik_orbs[looper].disp(win);
			}
			for (looper = 0; looper < 10; looper++) {
				mau_orbs[looper].move();
				mau_orbs[looper].disp(win);
			}

			win.display();
			pl_shoot_clock++;
		}
		else {
			win.clear();
			win.draw(te1);
			win.draw(gameover);
			win.display();
		}
		
    }

    return 0;
}
