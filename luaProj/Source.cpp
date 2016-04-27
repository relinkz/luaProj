#include <string>
#include "Entity.h"

using namespace std;

//lua funktion som skapar spelare

sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
sf::Event event;
sf::Font gameFont;
std::vector<sf::Sprite*> textures;

lua_State *L;


void printLuaVar(const string &variable, lua_State *L);

void initializeLua();

void registerEngineFunctions(lua_State * L);

bool intersectX(Entity* Player, Entity* Enemy);
bool intersectY(Entity* Player, Entity* Enemy);
void adjustPlayerPos(float xDiff, float yDiff, Entity* Player, Entity* Wall);

Entity* CheckEntity(lua_State *L, int i);

static int renderPlayer(lua_State* L);
static int renderEnemy(lua_State *L);
static int printScore(lua_State *L);

static int windowDisplay(lua_State *L);
static int windowClear(lua_State *L);

void playGame();

sf::Text text;


void renderBox(float x, float y);

int main()
{	
	initializeLua();
	registerEntityFunctions(L);
	registerEngineFunctions(L);

	if (!gameFont.loadFromFile("imagine_font.ttf"))
	{
		cout << "error i font load" << endl;
	}
	textures.push_back(new sf::Sprite());
	sf::Texture temp;
	if (!temp.loadFromFile("Doge_Much_wow.jpg"))
	{
		cout << "error i texture load" << endl;
	}
	else
	{
		textures.at(textures.size() - 1)->setTexture(temp);
	}
	text.setFont(gameFont);


	playGame();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;
}

static int renderPlayer(lua_State *L)
{
	Entity* aPtr = Entity::CheckEntity(L, 1);

	if (aPtr != nullptr)
	{
		float x = aPtr->getXPos();
		float y = aPtr->getYPos();
		int width = aPtr->getWidth();
		int height = aPtr->getHeight();
		sf::RectangleShape shape(sf::Vector2f(width, height));
		shape.setFillColor(sf::Color::Green);
		shape.setPosition(x, y);
		window.draw(shape);
	}
	return 0; // because fack you, thats why!
}

static int renderEnemy(lua_State *L)
{
	Entity* aPtr = Entity::CheckEntity(L, 1);

	if (aPtr != nullptr)
	{
		float x = aPtr->getXPos();
		float y = aPtr->getYPos();
		int width = aPtr->getWidth();
		int height = aPtr->getHeight();
		sf::RectangleShape shape(sf::Vector2f(width, height));
		shape.setFillColor(sf::Color::Red);
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
		float x = aPtr->getXPos();
		float y = aPtr->getYPos();
		sf::RectangleShape shape(sf::Vector2f(10, 10));
		shape.setFillColor(sf::Color::White);
		shape.setPosition(x, y);
		window.draw(shape);

	}
	return 0; // because fack you, thats why!
}

static int printScore(lua_State *L)
{
	string score = lua_tostring(L, 1);

	text.setString("score: " + score);

	text.setCharacterSize(24);

	text.setColor(sf::Color::White);


	text.setPosition(0, 0);

	//window.draw(*textures.at(0));

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

	int counter = lua_tonumber(L, 3);

	float xDiff = abs(Player->getXPos() - Enemy->getXPos());
	float yDiff = abs(Player->getYPos() - Enemy->getYPos());

	if (xDiff < 14 && yDiff < 14)
	{
		counter++;
	}

	lua_pushinteger(L, counter);
	lua_setglobal(L, "bonusScoreCounter");

	sf::IntRect playerRect(Player->getXPos(), Player->getYPos(), Player->getWidth(), Player->getHeight());
	sf::IntRect enenmyRect(Enemy->getXPos(), Enemy->getYPos(), Enemy->getWidth(), Enemy->getHeight());

	bool result = playerRect.intersects(enenmyRect);

	if (result)
	{
		cout << "You died" << endl;
		system("pause");
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
	if (xDiff < 11 && yDiff < 11)
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
		{ "windowClear",			windowClear },
		{ "windowDisplay",			windowDisplay },
		{ "intersectionTest",		intersectionTest},
		{ "wallIntersectionTest",	wallIntersectionTest },
		{ "getInput",				getInput },
		{ "printScore",				printScore },
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
	int error = luaL_loadfile(L, "luaScript.lua") || lua_pcall(L, 0, 1, 0);

	if (error)
	{
		cerr << lua_tostring(L,-1) << endl;
	}
	/*int error = luaL_loadfile(L, "levelEditorScript.lua") || lua_pcall(L, 0, 1, 0);

	if (error)
	{
		cerr << lua_tostring(L, -1) << endl;
	}*/
}