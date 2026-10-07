#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
using namespace std;
using namespace sf;

struct Bullet {
	CircleShape bullet;
	Vector2f bullDirect;
};

class Game
{
private:

	RenderWindow* window;
	VideoMode vdm;
	Event ev;
	vector<RectangleShape> Enemies;
	vector<Bullet> Bullets;
	RectangleShape player;
	int time;
	int score;
	int gameState;
	int blinkTime;
	SoundBuffer shootBuffer;
	Sound shootSound;
	SoundBuffer vanishBuffer;
	Sound vanishSound;
	Font font;
	Text scoreText;
	Text pauseText;
	Text menuText;
	Text quitText;
	Text titleText;

	void initVari();
	void initWindow();
	void initEnem();
	void spawnEnem();
	void spawnBullet(Vector2i mp);
	void initPlayer();
	void initScoreText();
	void initMenuText();
	void initPauseText();

public:
	Game();
	virtual ~Game();

	const bool running() const;
	Color getRandomColor();

	void pollEvent();
	void update();
	void updateEnem();
	void updateBull();
	void render();
	void renderBull();
	void renderEnem();
	void collision();
};


