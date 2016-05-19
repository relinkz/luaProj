#include <string>
#include "Entity.h"
#include <ctime>
#include <cstdlib>
#include "sfml/System.hpp"
#include <SFML/Audio.hpp>

using namespace std;

//lua funktion som skapar spelare
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int ARENA_WIDTH = 640;
const int ARENA_HEIGHT = 480;

int globalXOffSet = (SCREEN_WIDTH - ARENA_WIDTH) / 2;
int globalYOffSet = (SCREEN_HEIGHT - ARENA_HEIGHT) / 2;

sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML works!");
//sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Doge andventures!!", sf::Style::Fullscreen);
sf::Event event;
sf::Font gameFont;
sf::Clock gameClock;

sf::Clock animationClock;

std::vector<sf::Texture*> textures;
float gameTime = 0;

lua_State *L;


void printLuaVar(const string &variable, lua_State *L);

void initializeLua();

void registerEngineFunctions(lua_State * L);

bool intersectX(Entity* Player, Entity* Enemy);
bool intersectY(Entity* Player, Entity* Enemy);
void adjustPlayerPos(float xDiff, float yDiff, Entity* Player, Entity* Wall);

void release();

Entity* CheckEntity(lua_State *L, int i);

static int renderPlayer(lua_State* L);
static int renderEnemy(lua_State *L);
static int renderParticle(lua_State *L);
static int renderMenu(lua_State *L);
static int renderArena(lua_State *L);
static int printScore(lua_State *L);

static int sendTimeToLua(lua_State *L);
static int resetTime(lua_State *L);
static int startMusic(lua_State *L);
static int getLevelSelected(lua_State *L);

static int windowDisplay(lua_State *L);
static int windowClear(lua_State *L);

void playGame();

sf::Text text;

sf::SoundBuffer soundBuffer;
sf::Sound sound;

int buttonPressed = 0;
int levelSelected = 0;


bool secretLevelPlay = false;
bool rickRolled = false;

void renderBox(float x, float y);
void loadTextures();

int main()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	initializeLua();
	registerEntityFunctions(L);
	registerEngineFunctions(L);
	std::srand(unsigned(time(0)));

	if (!gameFont.loadFromFile("imagine_font.ttf"))
	{
		cout << "error i font load" << endl;
	}
	text.setFont(gameFont);
	loadTextures();
	if (!soundBuffer.loadFromFile("canary.wav"))
		return -1;
	sound.setBuffer(soundBuffer);
	sound.play();

	playGame();

	release();


	return 0;
}

static int renderPlayer(lua_State *L)
{

	Entity* aPtr = Entity::CheckEntity(L, 1);

	if (aPtr != nullptr)
	{
		float x = aPtr->getXPos() + globalXOffSet;
		float y = aPtr->getYPos() + globalYOffSet;

		sf::Sprite* toDraw = nullptr;
		toDraw = aPtr->getPlayerSprite();

		toDraw->setPosition(toDraw->getPosition().x + globalXOffSet, toDraw->getPosition().y + globalYOffSet);

		//toDraw->setPosition(x, y);

		sf::RectangleShape shape(sf::Vector2f(aPtr->getWidth(), aPtr->getHeight()));
		shape.setFillColor(sf::Color::Black);
	    shape.setPosition(x, y);

		//shape.setFillColor(sf::Color::Green);
		//creating the playersheet

		window.draw(shape);
		window.draw(*toDraw);
		
	}


	return 0; // because fack you, thats why!
}

static int renderEnemy(lua_State *L)
{
	Entity* aPtr = Entity::CheckEntity(L, 1);

	if (aPtr != nullptr)
	{
		float x = aPtr->getXPos() + globalXOffSet;
		float y = aPtr->getYPos() + globalYOffSet;
		int width = aPtr->getWidth();
		int height = aPtr->getHeight();
		sf::RectangleShape shape(sf::Vector2f(width, height));
		int type = rand() % 3;
		int r = 255;
		int g = 0;
		int b = 0;
		if (levelSelected == 3)
		{
			if (type == 0)
			{
				r = rand() % 255;
			}
			if (type == 1)
			{
				g = rand() % 255;
			}
			if (type == 2)
			{
				b = rand() % 255;
			}
		}
		shape.setFillColor(sf::Color(r,g,b,255));
		//shape.setFillColor(sf::Color::Red);
		shape.setPosition(x, y);
		window.draw(shape);

	}
	return 0; // because fack you, thats why!
}

static int renderWall(lua_State *L)
{
	Entity* aPtr = Entity::CheckEntity(L, 1);

	if (aPtr != nullptr)
	{
		float x = aPtr->getXPos() + globalXOffSet;
		float y = aPtr->getYPos() + globalYOffSet;
		sf::RectangleShape shape(sf::Vector2f(10, 10));
		shape.setFillColor(sf::Color::Black);
		shape.setPosition(x, y);
		window.draw(shape);

	}
	return 0; // because fack you, thats why!
}

static int renderParticle(lua_State *L)
{
	Entity* aPtr = Entity::CheckEntity(L, 1);

	int index = lua_tonumber(L, 2);
	if (index < 0)
	{
		index = 0;
	}
	if (index >= textures.size())
	{
		index = textures.size() - 1;
	}

	if (aPtr != nullptr)
	{
		sf::Sprite temp;
		float x = aPtr->getXPos() + globalXOffSet;
		float y = aPtr->getYPos() + globalYOffSet;

		temp.setPosition(x, y);
		temp.setTexture(*textures.at(index));

		window.draw(temp);
	}

	//window.draw(*textures.at(0));

	return 0; // because fack you, thats why!
}

static int renderMenu(lua_State *L)
{
	Entity* aPtr = Entity::CheckEntity(L, 1);
	int buttonType = lua_tonumber(L, 2);

	if (aPtr != nullptr)
	{
		float x = aPtr->getXPos() + globalXOffSet;
		float y = aPtr->getYPos() + globalYOffSet;
		if (buttonType == 1)
		{
			text.setString("Play");
		}
		if (buttonType == 2)
		{
			text.setString("Level Editor");
		}
		if (buttonType == 3)
		{
			text.setString("Exit");
		}
		if ((x - globalXOffSet) == 450 && (y - globalYOffSet) == 0)
		{
			text.setString("secret Level?");
		}
		if ((x - globalXOffSet) == 450 && (y - globalYOffSet) == 150)
		{
			text.setString("		real \n secret level");
		}
		if ((x - globalXOffSet) == -50 && (y - globalYOffSet) == 150)
		{
			text.setString("		   WOW \n secret level \n much secret \n many wow");
		}
		sf::RectangleShape shape(sf::Vector2f(aPtr->getWidth(), aPtr->getHeight()));
		shape.setFillColor(sf::Color::Blue);
		shape.setPosition(x, y);
		window.draw(shape);

		text.setPosition(x, y);

		text.setCharacterSize(24);

		text.setColor(sf::Color::White);

		window.draw(text);
	}
	return 0;
}

static int renderArena(lua_State *L)
{
	float x = 0;
	float y = 0;
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;
	sf::RectangleShape shape(sf::Vector2f(width, height));
	int type = rand() % 3;
	int r = 0;
	int g = 0;
	int b = 0;
	if (levelSelected == 3)
	{
		if (type == 0)
		{
			r = rand() % 255;
		}
		if (type == 1)
		{
			g = rand() % 255;
		}
		if (type == 2)
		{
			b = rand() % 255;
		}
	}

	shape.setFillColor(sf::Color(r, g, b, 255));
	//shape.setFillColor(sf::Color::White);
	shape.setPosition(x, y);
	window.draw(shape);


	//renders the ARENA
	x = globalXOffSet;
	y = globalYOffSet;
	width = ARENA_WIDTH;
	height = ARENA_HEIGHT;
	shape = sf::RectangleShape(sf::Vector2f(width, height));
	shape.setFillColor(sf::Color::White);
	shape.setPosition(x, y);
	window.draw(shape);
	//
	return 0;
}

static int printScore(lua_State *L)
{
	string score = lua_tostring(L, 1);

	int SWAG_SCORE = lua_tonumber(L, 1);
	if (secretLevelPlay)
	{
		score = SWAG_SCORE;
	}
	else
	{
		int start = 0;
		int end = score.find(".");
		score = score.substr(start, end);
	}


	string toSay = "";
	toSay = lua_tostring(L, 2);

	float x = lua_tonumber(L, 3);
	float y = lua_tonumber(L, 4);


	text.setString(toSay + score);

	text.setCharacterSize(24);
	if (levelSelected == 2 || levelSelected == 4)
	{
		text.setColor(sf::Color::Black);
	}
	else
	{
		text.setColor(sf::Color::White);
	}


	text.setPosition(x, y);
	int width = text.getLocalBounds().width;
	if (toSay != "FPS: " && levelSelected != 2 && levelSelected != 4)
	{
		sf::Sprite temp;
		temp.setPosition(width + 10, 0);
		int index = 0;
		for (int i = 1; i <= textures.size(); i++)
		{
			if (SWAG_SCORE > (5000 / textures.size()) * i)
			{
				index++;
			}
		}
		if (index >= textures.size())
		{
			index = textures.size() - 1;
		}
		temp.setTexture(*textures.at(index));

		window.draw(temp);
	}

	window.draw(text);

	return 0;
}

static int windowClear(lua_State *L)
{
	window.clear();
	return 0;
}

static int windowDisplay(lua_State *L)
{
	window.display();
	return 0;
}

static int intersectionTest(lua_State *L)
{	
	Entity* Player = nullptr;
	Player = Entity::CheckEntity(L, 1);
	
	Entity* Enemy = nullptr;
	Enemy = Entity::CheckEntity(L, 2);

	float counter = lua_tonumber(L, 3);
	int enemiesClose = lua_tonumber(L, 4);

	float xDiff = abs(Player->getXPos() - Enemy->getXPos());
	float yDiff = abs(Player->getYPos() - Enemy->getYPos());

	float maxXDiffAllowed = 10;
	float maxYDiffAllowed = 10;

	if (Player->getXPos() < Enemy->getXPos())
	{
		maxXDiffAllowed += Player->getWidth();
	}
	else
	{
		maxXDiffAllowed += Enemy->getWidth();
	}
	if (Player->getYPos() < Enemy->getYPos())
	{
		maxYDiffAllowed += Player->getHeight();
	}
	else
	{
		maxYDiffAllowed += Enemy->getHeight();
	}
	if (xDiff < maxXDiffAllowed && yDiff < maxYDiffAllowed)
	{
		counter += gameTime;
		enemiesClose++;
	}
	lua_pushnumber(L, counter);
	lua_setglobal(L, "bonusScoreCounter");

	lua_pushinteger(L, enemiesClose);
	lua_setglobal(L, "enemiesClose");

	sf::IntRect playerRect(Player->getXPos(), Player->getYPos(), Player->getWidth(), Player->getHeight());
	sf::IntRect enenmyRect(Enemy->getXPos(), Enemy->getYPos(), Enemy->getWidth(), Enemy->getHeight());

	bool result = playerRect.intersects(enenmyRect);

	if (result)
	{
		lua_pushinteger(L, -1);
		lua_setglobal(L, "enemyIntersectionResult");
		sound.stop();
	}

	return 0;
}

static int buttonIntersectionTest(lua_State *L)
{
	bool result = false;

	Entity* Player = nullptr;
	Player = Entity::CheckEntity(L, 1);

	Entity* Button = nullptr;
	Button = Entity::CheckEntity(L, 2);


	sf::IntRect playerRect(Player->getXPos(), Player->getYPos(), Player->getWidth(), Player->getHeight());
	sf::IntRect enenmyRect(Button->getXPos(), Button->getYPos(), Button->getWidth(), Button->getHeight());

	result = playerRect.intersects(enenmyRect);


	if (result )
	{
		levelSelected = 1;
		lua_pushinteger(L, -1);
		lua_setglobal(L, "intersectionTest");
		buttonPressed = lua_tonumber(L, 3);
		if (Button->getXPos() == 450 && Button->getYPos() == 0)
		{
			levelSelected = 2;
			rickRolled = true;
		}
		if (Button->getXPos() == 450 && Button->getYPos() == 150)
		{
			levelSelected = 4;
		}
		if (Button->getXPos() == -50 && Button->getYPos() == 150)
		{
			levelSelected = 3;
			secretLevelPlay = true;
		}
	}
	else
	{
		lua_pushinteger(L, 1);
		lua_setglobal(L, "intersectionTest");
	}

	return 0;
}

static int wallIntersectionTest(lua_State *L)
{
	Entity* Player = nullptr;
	Player = Entity::CheckEntity(L, 1);

	Entity* Wall = nullptr;
	Wall = Entity::CheckEntity(L, 2);

	int state = lua_tonumber(L, 3);
	bool result = false;
	float xDiff = abs(Player->getXPos() - Wall->getXPos());
	float yDiff = abs(Player->getYPos() - Wall->getYPos());

	float maxXDiffAllowed = 10;
	float maxYDiffAllowed = 10;

	if (Player->getXPos() < Wall->getXPos())
	{
		maxXDiffAllowed = Player->getWidth();
	}
	else
	{
		maxXDiffAllowed = Wall->getWidth();
	}
	if (Player->getYPos() < Wall->getYPos())
	{
		maxYDiffAllowed = Player->getHeight();
	}
	else
	{
		maxYDiffAllowed = Wall->getHeight();
	}


	if (xDiff < maxXDiffAllowed && yDiff < maxYDiffAllowed)
	{
		result = true;

		if (state == 1)
		{
			adjustPlayerPos(xDiff, yDiff, Player, Wall);
		}

	}
	if (result)
	{
		lua_pushinteger(L, -1);
		lua_setglobal(L, "intersectionTest");
	}
	else
	{
		lua_pushinteger(L, 1);
		lua_setglobal(L, "intersectionTest");
	}

	return 1;
}

static int getInput(lua_State *L)
{
	window.pollEvent(event);
	if(event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::A)
		{
			lua_pushinteger(L, 1);
			lua_setglobal(L, "input");
		}
		else if (event.key.code == sf::Keyboard::S)
		{
			lua_pushinteger(L, 2);
			lua_setglobal(L, "input");
		}
		else if (event.key.code == sf::Keyboard::R)
		{
			lua_pushinteger(L, 3);
			lua_setglobal(L, "input");
		}
		else
		{
			lua_pushinteger(L, -1);
			lua_setglobal(L, "input");
		}
	}

	return 1;
}

bool intersectX(Entity* Player, Entity* Enemy)
{
	bool intersect = false;
	/*if (Player->getXPos() > Enemy->getXPos() && Player->getXPos() < Enemy->getXPos() + Enemy->getSideLength())
	{
		intersect = true;
	}*/
	/*if (Player->getXPos() + Player->getSideLength() < (Enemy->getXPos()))
	{
		//no intersect
		intersect = false;
	}
	else if (Enemy->getXPos() + Enemy->getSideLength() < (Player->getXPos()))
	{
		//no intersect
		intersect = false;
	}*/
	return intersect;
}

bool intersectY(Entity* Player, Entity* Enemy)
{
	bool intersect = true;
	/*if (Player->getYPos() + Player->getSideLength() < (Enemy->getYPos()))
	{
		//no intersect
		intersect = false;
	}
	else if (Enemy->getYPos() + Enemy->getSideLength() < (Player->getYPos()))
	{
		//no intersect
		intersect = false;
	}*/
	return intersect;
}

void adjustPlayerPos(float xDiff, float yDiff, Entity* Player, Entity* Wall)
{
	if (yDiff > xDiff)
	{
		if (Player->getYPos() > Wall->getYPos())
		{
			Player->move(0.0f, yDiff * 0.1f);
		}
		else
		{
			Player->move(0.0f, -yDiff * 0.1f);
		}
	}
	else
	{
		if (Player->getXPos() > Wall->getXPos())
		{
			Player->move(xDiff * 0.1f, 0.0f);
		}
		else
		{
			Player->move(-xDiff * 0.1f, 0.0f);
		}
	}
}

void printLuaVar(const string &variable, lua_State *L)
{
	int size = variable.length() + 1;

	char* test;
	test = new char[size];
	strcpy_s(test, size, variable.c_str());

	lua_getglobal(L, test);
	cout << lua_tostring(L, -1) << endl;

	lua_pop(L, 1);

	delete[] test;
}

void initializeLua()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	
	int error = luaL_loadstring(L,
		"print('welcome commander, Loading the scriptfile now:')"
		) || lua_pcall(L, 0, 0, 0);
}

void registerEngineFunctions(lua_State * L)
{
	//här inititierar metedata, such meta such wow
	luaL_newmetatable(L, "MainMeta");
	
	luaL_Reg EngineTable[] =
	{
		{ "renderPlayer",			renderPlayer },
		{ "renderEnemy",			renderEnemy},
		{ "renderWall",				renderWall },
		{ "renderMenu",				renderMenu },
		{ "renderArena",			renderArena },
		{ "windowClear",			windowClear },
		{ "windowDisplay",			windowDisplay },
		{ "intersectionTest",		intersectionTest},
		{ "wallIntersectionTest",	wallIntersectionTest },
		{ "getInput",				getInput },
		{ "printScore",				printScore },
		{ "renderParticle",			renderParticle },
		{ "buttonIntersectionTest", buttonIntersectionTest },
		{ "getGameTime",			sendTimeToLua },
		{"resetGameTime",			resetTime},
		{ "startMusic",				startMusic },
		{ "getLevelSelected",		getLevelSelected },
		{ NULL, NULL }
	};

	//sätt functions
	luaL_setfuncs(L, EngineTable, 0);

	//pusha upp metatable
	lua_pushvalue(L, -1);

	//sätter metadatan till __index
	lua_setfield(L, -1, "__index");

	lua_setglobal(L, "Engine");
}

void playGame()
{

	while (buttonPressed != 3)
	{
		int error = luaL_loadfile(L, "menuScript.lua") || lua_pcall(L, 0, 1, 0);

		


		if (error)
		{
			cerr << lua_tostring(L, -1) << endl;
		}

		if (buttonPressed == 1 ||buttonPressed >= 4)
		{
			if (levelSelected == 3)
			{
				if (!soundBuffer.loadFromFile("Nyan_Cat.wav"))
				{

				}
				sound.setBuffer(soundBuffer);
				sound.play();
			}
			if (levelSelected == 2)
			{
				if (!soundBuffer.loadFromFile("Never_Gonna_Give_You_Up.wav"))
				{

				}
				sound.setBuffer(soundBuffer);
				sound.play();
			}
			if (levelSelected == 4)
			{
				if (!soundBuffer.loadFromFile("Trololo_Sing_Along_.wav"))
				{

				}
				sound.setBuffer(soundBuffer);
				sound.play();
			}
			int error = luaL_loadfile(L, "luaScript.lua") || lua_pcall(L, 0, 1, 0);
			
			lua_getglobal(L, "playGame");
			lua_pcall(L, 0, 0, 0);

			if (error)
			{
				cerr << lua_tostring(L, -1) << endl;
			}
		}

		if (buttonPressed == 2)
		{
			int error = luaL_loadfile(L, "levelEditorScript.lua") || lua_pcall(L, 0, 1, 0);

			if (error)
			{
				cerr << lua_tostring(L, -1) << endl;
			}
		}

	}

}
void loadTextures()
{
	textures.push_back(new sf::Texture());
	if (!textures.at(textures.size() - 1)->loadFromFile("pleb.jpg"))
	{
		cout << "error i texture load" << endl;
	}
	textures.push_back(new sf::Texture());
	if (!textures.at(textures.size() - 1)->loadFromFile("top_pleb.jpg"))
	{
		cout << "error i texture load" << endl;
	}
	textures.push_back(new sf::Texture());
	if (!textures.at(textures.size() - 1)->loadFromFile("Doge_Much_wow_small.jpg"))
	{
		cout << "error i texture load" << endl;
	}
	textures.push_back(new sf::Texture());
	if (!textures.at(textures.size() - 1)->loadFromFile("next_level_swag_doge.png"))
	{
		cout << "error i texture load" << endl;
	}
}

int sendTimeToLua(lua_State *L)
{
	sf::Time eTime = gameClock.getElapsedTime();
	float s = eTime.asSeconds();
	float framerate = 1.f / gameClock.getElapsedTime().asSeconds();
	s = gameClock.restart().asSeconds();
	gameTime = s;
	lua_pushnumber(L, framerate);
	lua_setglobal(L, "FPS");
	lua_pushnumber(L,s);
	lua_setglobal(L, "gameTime");

	return 1;
}

int resetTime(lua_State *L)
{
	gameClock.restart().asSeconds();
	

	return 0;
}

void release()
{
	for (int i = 0; i < textures.size(); i++)
	{
		delete textures.at(i);
		textures.at(i) = nullptr;
	}
	lua_close(L);
}

static int startMusic(lua_State *L)
{
	if (secretLevelPlay == true)
	{
		if (!soundBuffer.loadFromFile("Nyan_Cat.wav"))
		{

		}
		sound.setBuffer(soundBuffer);
		sound.play();
	}
	if (rickRolled == true)
	{
		if (!soundBuffer.loadFromFile("Never_Gonna_Give_You_Up.wav"))
		{

		}
		sound.setBuffer(soundBuffer);
		sound.play();
	}


	return 0;
}

static int getLevelSelected(lua_State *L)
{
	lua_pushnumber(L, levelSelected);
	lua_setglobal(L, "LevelSelected");

	return 1;
}