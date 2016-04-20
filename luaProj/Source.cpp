#include <string>
#include "Entity.h"

using namespace std;

//lua funktion som skapar spelare

sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
lua_State *L;


void printLuaVar(const string &variable, lua_State *L);

void initializeLua();

void registerEngineFunctions(lua_State * L);

bool intersectX(Entity* Player, Entity* Enemy);
bool intersectY(Entity* Player, Entity* Enemy);

Entity* CheckEntity(lua_State *L, int i);

static int renderPlayer(lua_State* L);
static int renderEnemy(lua_State *L);

static int windowDisplay(lua_State *L);
static int windowClear(lua_State *L);

void playGame();


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
		sf::RectangleShape shape(sf::Vector2f(10, 10));
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
		sf::RectangleShape shape(sf::Vector2f(10, 10));
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(x, y);
		window.draw(shape);

	}
	return 0; // because fack you, thats why!
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
	
	if (intersectX(Player, Enemy) == true && intersectY(Player, Enemy))
	{
		cout << "You died" << endl;
		system("pause");
	}

	return 0;
}

bool intersectX(Entity* Player, Entity* Enemy)
{
	bool intersect = true;
	if (Player->getXPos() > (Enemy->getXPos() + Enemy->getSideLength()))
	{
		//no intersect
		intersect = false;
	}
	else if (Enemy->getXPos() > (Player->getXPos() + Player->getSideLength()))
	{
		//no intersect
		intersect = false;
	}
	return intersect;
}

bool intersectY(Entity* Player, Entity* Enemy)
{
	bool intersect = true;
	if (Player->getYPos() > (Enemy->getYPos() + Enemy->getSideLength()))
	{
		//no intersect
		intersect = false;
	}
	else if (Enemy->getYPos() > (Player->getYPos() + Player->getSideLength()))
	{
		//no intersect
		intersect = false;
	}
	return intersect;
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
		{ "intersectionTest",    intersectionTest},
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
}