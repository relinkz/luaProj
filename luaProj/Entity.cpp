#include "Entity.h"



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
