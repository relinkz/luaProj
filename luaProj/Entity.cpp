#include "Entity.h"

void initializeEntityMetadata(lua_State * L)
{
	//här inititierar metedata, such meta such wow
	luaL_newmetatable(L, "MetaEntity");

	luaL_Reg EntityTable[] =
	{
		{"New",   Entity::New },
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
}

Entity::Entity(float x, float y)
{
	this->xPos = x;
	this->yPos = y;
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

int Entity::New(lua_State * L)
{
	float x = lua_tonumber(L, 1);
	float y = lua_tonumber(L, 2);

	Entity** entity = reinterpret_cast<Entity**>(lua_newuserdata(L, sizeof(Entity)));

	*entity = new Entity(x, y);
	std::cout
		<< "xValue" << x << std::endl
		<< "yValue" << y << std::endl;


	return 0;
}
