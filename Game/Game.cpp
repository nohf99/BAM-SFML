#include "Game.h"

void Game::initVari()
{
	this->window = nullptr;
	time = 0;
	this->shootBuffer.loadFromFile("shoot.wav");
	this->shootSound.setBuffer(this->shootBuffer);
	this->vanishBuffer.loadFromFile("vanish.wav");
	this->vanishSound.setBuffer(this->vanishBuffer);
	this->font.loadFromFile("PixeloidMono-nAOpP.ttf");
	score = 0;
	gameState = 0;
	blinkTime = 0;
}

void Game::initMenuText()
{
	this->titleText.setFont(this->font);
	this->titleText.setCharacterSize(100);
	this->titleText.setPosition(145.f, 180.f);
	this->titleText.setFillColor(Color::White);
	this->titleText.setString("BAM!!");
	this->menuText.setFont(this->font);
	this->menuText.setCharacterSize(27);
	this->menuText.setPosition(255.f, 370.f);
	this->menuText.setFillColor(Color::White);
	this->menuText.setString("PLAY");
	this->quitText.setFont(this->font);
	this->quitText.setCharacterSize(27);
	this->quitText.setPosition(255.f, 450.f);
	this->quitText.setFillColor(Color::White);
	this->quitText.setString("QUIT");
}

void Game::initScoreText()
{
	this->scoreText.setFont(this->font);
	this->scoreText.setCharacterSize(24);
	this->scoreText.setPosition(10.f, 10.f);
	this->scoreText.setFillColor(Color::White);
	this->scoreText.setString("Score: 0");
}

void Game::initPauseText()
{
	this->pauseText.setFont(this->font);
	this->pauseText.setCharacterSize(15);
	this->pauseText.setPosition(200.f, 570.f);
	this->pauseText.setFillColor(Color::White);
	this->pauseText.setString("Press Space to pause");
}

void Game::initWindow()
{
	this->vdm.height = 600;
	this->vdm.width = 600;
	this->window = new RenderWindow(this->vdm, "Game 1");
	this->window->setFramerateLimit(60);
}

void Game::initPlayer()
{
	this->player.setSize(Vector2f(40.f, 40.f));
	this->player.setFillColor(Color::White);
	float px = this->player.getSize().x / 2;
	float py = this->player.getSize().y / 2;
	this->player.setOrigin(px, py);
	this->player.setPosition(this->window->getSize().x / 2, this->window->getSize().y / 2);
}

void Game::spawnEnem()
{	
	RectangleShape Enemy;
	int num = rand() % 4;
	if (num == 0) Enemy.setPosition(rand() % 600, -50);
	else if (num == 1) Enemy.setPosition(rand() % 600, 650);
	else if (num == 2) Enemy.setPosition(-50, rand() % 600);
	else if (num == 3) Enemy.setPosition(650, rand() % 600);

	Enemy.setSize(Vector2f(25.f, 25.f));
	Enemy.setFillColor(getRandomColor());
	Enemy.setOutlineColor(Color::White);
	Enemy.setOutlineThickness(1.f);
	float px = Enemy.getSize().x / 2;
	float py = Enemy.getSize().y / 2;
	Enemy.setOrigin(px, py);
	Enemies.push_back(Enemy);
}

void Game::spawnBullet(Vector2i mp)
{
	Bullet b;
	b.bullet.setRadius(5.f);
	b.bullet.setFillColor(Color::Black);
	b.bullet.setOutlineColor(Color::White);
	b.bullet.setOutlineThickness(1.f);
	b.bullet.setOrigin(5.f, 5.f);
	Vector2f posPlayer = this->player.getPosition();
	float dx = (float)mp.x - posPlayer.x;
	float dy = (float)mp.y - posPlayer.y ;
	float distance = sqrt(dx * dx + dy * dy);
	b.bullDirect.x = dx / distance;
	b.bullDirect.y = dy / distance;
	b.bullet.setPosition(posPlayer);
	Bullets.push_back(b);
}


Game::Game()
{
	this->initVari();
	this->initWindow();
	this->spawnEnem();
	this->initPlayer();
	this->initScoreText();
	this->initPauseText();
	this->initMenuText();
	gameState = 0;
}

Game::~Game()
{
	delete this->window;
}

void Game::pollEvent()
{
	while (this->window->pollEvent(this->ev))
	{
		if (this->ev.type == Event::Closed)
		{
			this->window->close();
			break;
		}
		else if (this->ev.type == Event::KeyPressed)
		{
			if (ev.key.code == Keyboard::Escape)
			{
				this->window->close();
				break;
			}
			else if (ev.key.code == Keyboard::Space)
			{
				if (gameState == 1) gameState = 2;
				else if (gameState == 2) gameState = 1;
			}
		}
		else if (this->ev.type == Event::MouseButtonPressed)
		{
				if (ev.mouseButton.button == Mouse::Left)
				{
					if (gameState == 1) {
						this->shootSound.play();
						spawnBullet(Mouse::getPosition(*this->window));
					}
					else if (gameState == 0) {
						if (this->menuText.getGlobalBounds().contains(Vector2f(Mouse::getPosition(*this->window)))) {
							gameState = 1;
							blinkTime = 0;
						}
						else if (this->quitText.getGlobalBounds().contains(Vector2f(Mouse::getPosition(*this->window)))) {
							this->window->close();
							break;
						}
					}
				}
		}
	}
}

void Game::update()
{	
	this->pollEvent();
	if (gameState == 0) {
		if (blinkTime >= 60) blinkTime = 0;
		else blinkTime++;
	}
	else if (gameState == 1) {
		if (time > 45) {

			spawnEnem();
			time = 0;
		}
		else time++;

		if (blinkTime >= 60) blinkTime = 0;
		else blinkTime++;

		this->updateEnem();

		this->collision();

		this->scoreText.setString("Score: " + to_string(this->score));

		this->updateBull();
	}
	else if (gameState == 2) {

	}
}

void Game::updateEnem()
{
	for (int i = 0; i < Enemies.size(); i++) {
		Vector2f posEnem = Enemies[i].getPosition();

		Vector2f posPlayer = this->player.getPosition();

		float dx = posPlayer.x - posEnem.x;
		float dy = posPlayer.y - posEnem.y;

		float distance = sqrt(dx * dx + dy * dy);
		float spd = 2.f;
		if (score >= 5) spd = 4.f;
		else if (score >= 20) spd = 8.f;
		if (distance > 0) Enemies[i].move((dx / distance) * spd, (dy / distance) * spd);
	}
}

void Game::updateBull()
{
	for (int i = Bullets.size()-1; i >= 0; i--)
	{
		Bullets[i].bullet.move(Bullets[i].bullDirect.x * 10.f, Bullets[i].bullDirect.y * 10.f);
		Vector2f posbull = Bullets[i].bullet.getPosition();
		if (posbull.x < 0 || posbull.x > 600 || posbull.y < 0 || posbull.y > 600) Bullets.erase(Bullets.begin() + i);
	}

}

void Game::collision()
{
	for (int i = Bullets.size()-1 ;i >= 0; i--)
	{
		for(int j = Enemies.size()-1 ; j >= 0; j--)
			if (Bullets[i].bullet.getGlobalBounds().intersects(Enemies[j].getGlobalBounds()))
			{
				score++;
				this->vanishSound.play();
				Enemies.erase(Enemies.begin() + j);
				Bullets.erase(Bullets.begin() + i);
				break;
			}
	}
}

void Game::renderEnem()
{
	for (int i = 0;i < Enemies.size(); i++)
		this->window->draw(Enemies[i]);
}

void Game::renderBull()
{
	for (int i = 0;i < Bullets.size(); i++)
		this->window->draw(Bullets[i].bullet);
}

void Game::render()
{
	this->window->clear();

	if (gameState == 0) {
		if (blinkTime < 30) this->window->draw(this->titleText);
		this->window->draw(this->menuText);
		this->window->draw(this->quitText);
	}
	else if (gameState == 1 || gameState == 2) {
		renderEnem();

		renderBull();

		this->window->draw(this->scoreText);

		this->window->draw(this->player);

		if (blinkTime < 30) this->window->draw(this->pauseText);
	}

	this->window->display();
}


const bool Game::running() const
{
	return this->window->isOpen();
}

Color Game::getRandomColor()
{
	int r = rand() % 255;
	int g = rand() % 255;
	int b = rand() % 255;
	return Color(r, g, b, 255);
}
