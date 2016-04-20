#include "Entity.h"

void registerEntityFunctions(lua_State * L)
{
	//h�r inititierar metedata, such meta such wow
	luaL_newmetatable(L, "MetaEntity");

	luaL_Reg EntityTable[] =
	{
		{"New",				Entity::New },
		{"Update",			Entity::Update},
		{"UpdatePlayer",	Entity::UpdatePlayer },
		{"getPos",			Entity::getPos },
		{"GetEntityData",},
		{ NULL, NULL}
	};

	//s�tt functions
	luaL_setfuncs(L, EntityTable, 0);
	
	//pusha upp metatable
	lua_pushvalue(L, -1);

	//s�tter metadatan till __index
	lua_setfield(L, -1, "__index");

	lua_setglobal(L, "Entity");
}

Entity::Entity()
{
	this->xPos = 0;
	this->yPos = 0;

	this->sideLength = 10;
}

Entity::Entity(float x, float y, float xDir, float yDir)
{
	this->xPos = x;
	this->yPos = y;

	this->xDir = xDir;
	this->yDir = yDir;

	this->sideLength = 10;
}

Entity::~Entity()
{
}

float Entity::getXPos() const
{
	return this->xPos;
}

float Entity::getYPos() const
{
	return this->yPos;
}

void Entity::setXPos(const float & newPos)
{
	this->xPos = newPos;
}

void Entity::setYPos(const float & newPos)
{
	this->yPos = newPos;
}

void Entity::move()
{
	this->xPos += this->xDir;
	this->yPos += this->yDir;
}
void Entity::move(float xDir,float  yDir)
{
	this->xPos += xDir;
	this->yPos += yDir;
}

float Entity::getSideLength() const
{
	return this->sideLength;
}

Entity* Entity::CheckEntity(lua_State * L, int i)
{
	Entity* entityPtr = nullptr;
	void* aPtr = luaL_testudata(L, i, "MetaEntity");

	if (aPtr != nullptr)
	{
		entityPtr = *(Entity**)aPtr;
	}

	return entityPtr;
}

int Entity::Update(lua_State * L)
{
	Entity* aPtr = nullptr;
	aPtr = CheckEntity(L, 1);

	if (aPtr != nullptr)
	{
		aPtr->move();
	}
	return 0;
}

int Entity::New(lua_State * L)
{
	float x = lua_tonumber(L, 1);
	float y = lua_tonumber(L, 2);

	float xDir = lua_tonumber(L, 3);
	float yDir = lua_tonumber(L, 4);

	Entity** entity = reinterpret_cast<Entity**>(lua_newuserdata(L, sizeof(Entity*)));

	*entity = new Entity(x, y, xDir, yDir);
	//std::cout
		//<< "xValue" << x << std::endl
		//<< "yValue" << y << std::endl;


	luaL_getmetatable(L, "MetaEntity");
	lua_setmetatable(L, -2);

	return 1;
}

int Entity::UpdatePlayer(lua_State *L)
{
	Entity* aPtr = nullptr;
	aPtr = CheckEntity(L, 1);
	float xSpeed = 0.01f;
	float ySpeed = 0.01f;
	if (aPtr != nullptr)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			aPtr->move(-xSpeed, 0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			aPtr->move(0.0f, -ySpeed);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			aPtr->move(xSpeed, 0.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			aPtr->move(0.0f, ySpeed);
		}

	}

	return 0;
}

int Entity::getPos(lua_State *L)
{
	Entity* aPtr = CheckEntity(L, 1);
	if (aPtr != nullptr)
	{
		int xPos = aPtr->getXPos();
		int yPos = aPtr->getYPos();

		lua_pushinteger(L, xPos);
		lua_setglobal(L, "xPos");

		lua_pushinteger(L, yPos);
		lua_setglobal(L, "yPos");
	}


	return 1;
}

