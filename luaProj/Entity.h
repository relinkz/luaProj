#ifndef ENTITY_H
#define ENTITY_H
#include "lua.hpp"
#include "SFML\Graphics.hpp"
#include <iostream>

void registerEntityFunctions(lua_State * L);

class Entity
{
private:
	float xPos;
	float yPos;

	float xDir;
	float yDir;

	float sideLength;
public:
	Entity();
	Entity(float x, float y, float xDir, float yDir);
	virtual ~Entity();

	float getXPos() const;
	float getYPos() const;

	void setXPos(const float &newPos);
	void setYPos(const float &newPos);

	void move();
	void move(float xDif,float yDir);

	float getSideLength() const;
	
	static Entity* CheckEntity(lua_State * L, int i);

	static int Update(lua_State * L);
	static int New(lua_State *L);
	static int UpdatePlayer(lua_State *L);
	static int RemoveEntity(lua_State *L);

};

#endif

