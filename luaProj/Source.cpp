#include "SFML\Graphics.hpp"
#include <string>
#include "Entity.h"

using namespace std;

//lua funktion som skapar spelare

sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
lua_State *L;


void printLuaVar(const string &variable, lua_State *L);

void initializeLua();

void registerEngineFunctions(lua_State * L);

static int renderPlayer(lua_State* L);
static int renderEnemy(lua_State *L);

static int windowDisplay(lua_State *L);
static int windowClear(lua_State *L);

void playGame();

sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
sf::CircleShape shape(100.f);

void renderBox(float x, float y);
int main()
{	
	initializeLua();
	registerEntityFunctions(L);
	registerEngineFunctions(L);

	playGame();

	return 0;
}

static int renderPlayer(lua_State *L)
{
	Entity* aPtr = Entity::CheckEntity(L, 1);

	if (aPtr != nullptr)
	{
		float x = aPtr->getXPos();
		float y = aPtr->getYPos();

<<<<<<< HEAD
	shape.setFillColor(sf::Color::Green);
=======
		sf::RectangleShape shape(sf::Vector2f(10, 10));
		shape.setFillColor(sf::Color::Green);
		shape.setPosition(x, y);
		window.draw(shape);
	}
	return 0; // because fack you, thats why!
}
>>>>>>> 9d06a6a9805dd487742fcc8f7e8ac025cf9bb59f

static int renderEnemy(lua_State *L)
{
	Entity* aPtr = Entity::CheckEntity(L, 1);

	if (aPtr != nullptr)
	{
<<<<<<< HEAD
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		renderBox(0,0);
		window.display();
=======
		float x = aPtr->getXPos();
		float y = aPtr->getYPos();

		sf::RectangleShape shape(sf::Vector2f(10, 10));
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(x, y);
		window.draw(shape);
>>>>>>> 9d06a6a9805dd487742fcc8f7e8ac025cf9bb59f
	}
	return 0; // because fack you, thats why!
}

static int windowClear(lua_State *L)
{
	window.clear();
	return 0;
}
<<<<<<< HEAD
void renderBox(float x, float y)
{
	window.draw(shape);

=======

static int windowDisplay(lua_State *L)
{
	window.display();
	return 0;
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
		{ "renderPlayer",		renderPlayer },
		{ "renderEnemy",		renderEnemy},
		{ "windowClear",		windowClear },
		{ "windowDisplay",		windowDisplay },
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
>>>>>>> 9d06a6a9805dd487742fcc8f7e8ac025cf9bb59f
}