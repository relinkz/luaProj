#ifndef ENTITY_H
#define ENTITY_H
#include "lua.hpp"
#include <iostream>

void initializeEntityMetadata(lua_State * L);

class Entity
{
private:
	float xPos;
	float yPos;
public:
	Entity();
	Entity(float x, float y);
	virtual ~Entity();

	float getXPos() const;
	float getYPos() const;

	void setXPos(const float &newPos);
	void setYPos(const float &newPos);

	static int New(lua_State *L);
};

#endif

