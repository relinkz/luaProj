#include "Entity.h"

void registerEntityFunctions(lua_State * L)
{
	//här inititierar metedata, such meta such wow
	luaL_newmetatable(L, "MetaEntity");

	luaL_Reg EntityTable[] =
	{
		{"New",				Entity::New },
		{"Update",			Entity::Update},
		{"UpdatePlayer",	Entity::UpdatePlayer },
		{"getPos",			Entity::getPos },
		{"remove",			Entity::remove },
		{ NULL, NULL}
	};

	//sätt functions
	luaL_setfuncs(L, EntityTable, 0);
	
	//pusha upp metatable
	lua_pushvalue(L, -1);

	//sätter metadatan till __index
	lua_setfield(L, -1, "__index");

	lua_setglobal(L, "Entity");
}

Entity::Entity()
{
	this->xPos = 0;
	this->yPos = 0;

	this->width = 10;
	this->height = 10;
}

Entity::Entity(float x, float y, float xDir, float yDir, int width, int height)
{
	this->xPos = x;
	this->yPos = y;

	this->xDir = xDir;
	this->yDir = yDir;

	this->width = width;
	this->height = height;
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

void Entity::setXDir(const float & newXDir)
{
	this->xDir = newXDir;
}
void Entity::setYDir(const float & newYDir)
{
	this->yDir = newYDir;
}

void Entity::move()
{
	this->xPos += this->xDir;
	this->yPos += this->yDir;
}

void Entity::move(const float & dt)
{
	this->xPos += (this->xDir * BASE_ENTITY_SPEED) * dt;
	this->yPos += (this->yDir * BASE_ENTITY_SPEED) * dt;
}

void Entity::move(float xDir,float  yDir)
{
	this->xPos += xDir;
	this->yPos += yDir;
}

float Entity::getXDir() const
{
	return this->yDir;
}

float Entity::getYDir() const
{
	return this->xDir;
}

int Entity::getWidth() const
{
	return this->width;
}
int Entity::getHeight() const
{
	return this->height;
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

	float dt = lua_tonumber(L, 2);

	float xSpeed = dt *10000.0f;
	float ySpeed = dt *10000.0f;

	if (aPtr != nullptr)
	{
		aPtr->move(dt);
	}

	return 0;
}

int Entity::New(lua_State * L)
{
	float x = lua_tonumber(L, 1);
	float y = lua_tonumber(L, 2);

	float xDir = lua_tonumber(L, 3);
	float yDir = lua_tonumber(L, 4);

	int width = lua_tonumber(L, 5);
	int height = lua_tonumber(L, 6);

	Entity** entity = reinterpret_cast<Entity**>(lua_newuserdata(L, sizeof(Entity*)));

	*entity = new Entity(x, y, xDir, yDir, width, height);
	//delete entity;
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

	float dt = lua_tonumber(L, 2);


	if (aPtr != nullptr)
	{
		float xSpeed = 0;
		float ySpeed = 0;
		float speedMultplier = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			speedMultplier = 0.25f;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			speedMultplier = 2.0f;
		}


		xSpeed += BASE_ENTITY_SPEED * dt * speedMultplier;
		ySpeed += BASE_ENTITY_SPEED * dt * speedMultplier;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && aPtr->getXPos() > 0)
		{
			aPtr->move(-xSpeed, 0.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && aPtr ->getYPos () > 0)
		{
			aPtr->move(0.0f, -ySpeed);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && aPtr->getXPos() < (640 - aPtr->getWidth()))
		{
			aPtr->move(xSpeed, 0.0f);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && aPtr->getYPos() < (480 - aPtr->getHeight()))
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

int Entity::remove(lua_State * L)
{
	Entity* ptr = CheckEntity(L, 1);
	delete ptr;
	return 0;
}
