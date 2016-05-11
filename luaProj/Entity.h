#ifndef ENTITY_H
#define ENTITY_H
#include "lua.hpp"
#include "SFML\Graphics.hpp"
#include <iostream>

static float BASE_ENTITY_SPEED = 100.0f;

void registerEntityFunctions(lua_State * L);

class Entity
{
private:
	float xPos;
	float yPos;

	float xDir;
	float yDir;

	int width;
	int height;

	int spriteStage;
	float animationTimer;

	//std::vector<sf::IntRect> spriteSheetLevel;
	std::vector<sf::IntRect> spriteSheetLevel;
	sf::Texture* textureSheet;
	sf::Sprite* playerSprite;

public:
	Entity();
	Entity(float x, float y, float xDir, float yDir,int width, int height);
	virtual ~Entity();

	float getXPos() const;
	float getYPos() const;

	float getXDir() const;
	float getYDir() const;

	int getWidth() const;
	int getHeight() const;

	void setTextureSheet(const std::string &sourceName);
	void setSprite();

	sf::Sprite* getPlayerSprite() const;

	void setXPos(const float &newPos);
	void setYPos(const float &newPos);

	void setXDir(const float &newXDir);
	void setYDir(const float &newYDir);

	void move();
	void move(const float &dt);
	void move(float xDif,float yDir);

	
	static Entity* CheckEntity(lua_State * L, int i);

	static int Update(lua_State * L);
	static int New(lua_State *L);
	static int UpdatePlayer(lua_State *L);
	static int getPos(lua_State *L);
	static int remove(lua_State *L);
};

#endif

