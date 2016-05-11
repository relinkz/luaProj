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
	if (this->textureSheet != nullptr)
	{
		delete this->textureSheet;
	}

	if (this->playerSprite != nullptr)
	{
		delete this->playerSprite;
	}
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


sf::Sprite* Entity::getPlayerSprite() const
{
	return this->playerSprite;
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

	if (aPtr->playerSprite == nullptr)
	{
		aPtr->textureSheet = new sf::Texture();
		aPtr->playerSprite = new sf::Sprite();
		
		aPtr->textureSheet->loadFromFile("Doge_Swag.png");
		aPtr->playerSprite->setTexture(*(aPtr->textureSheet));

		sf::IntRect animationFrame1(0, 0, 38, 30);
		sf::IntRect animationFrame2(38, 0, 38, 30);
		sf::IntRect animationFrame3(38 * 2, 0, 38, 30);
		sf::IntRect animationFrame4(38 * 3, 0, 38, 30);
		sf::IntRect animationFrame5(38 * 4, 0, 38, 30);

		sf::IntRect walkingAnimationFrame1(0, 35, 38, 30);
		sf::IntRect walkingAnimationFrame2(38, 35, 38, 30);

		aPtr->playerSprite->setTextureRect(animationFrame4);

		aPtr->spriteSheetLevel.push_back(animationFrame1);
		aPtr->spriteSheetLevel.push_back(animationFrame2);
		aPtr->spriteSheetLevel.push_back(animationFrame3);
		aPtr->spriteSheetLevel.push_back(animationFrame4);
		aPtr->spriteSheetLevel.push_back(animationFrame5);

		aPtr->spriteSheetLevel.push_back(walkingAnimationFrame1);
		aPtr->spriteSheetLevel.push_back(walkingAnimationFrame2);

		aPtr->spriteStage = 0;
		aPtr->animationTimer = 0.0f;
	}

	if (aPtr != nullptr)
	{
		float xSpeed = 0;
		float ySpeed = 0;
		float speedMultplier = 1;
		bool isIdle = true;

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

		aPtr->animationTimer += dt;
		bool walkLeft = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && aPtr->getXPos() > 0)
		{
			walkLeft = true;
			aPtr->move(-xSpeed, 0.0f);
			if (aPtr->spriteStage < 5)
			{
				//if player just started walking
				aPtr->spriteStage = 5;
				aPtr->playerSprite->setTextureRect(aPtr->spriteSheetLevel.at(aPtr->spriteStage));
			}
			isIdle = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && aPtr ->getYPos () > 0)
		{
			aPtr->move(0.0f, -ySpeed);
			isIdle = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && aPtr->getXPos() < (640 - aPtr->getWidth()))
		{
			aPtr->move(xSpeed, 0.0f);
			isIdle = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && aPtr->getYPos() < (480 - aPtr->getHeight()))
		{
			aPtr->move(0.0f, ySpeed);
			isIdle = false;
		}
		
		if (isIdle == true)
		{
			if (aPtr->animationTimer > 1.0f)
			{
				if (aPtr->spriteStage > 5)
				{
					//player just went Idle
					aPtr->spriteStage = 0;
				}
				aPtr->animationTimer = 0.0f;
				aPtr->spriteStage++;
			}
			if (aPtr->spriteStage == 5)
			{
				aPtr->spriteStage %= 5;
			}
		}
		else
		{
			//hoppa mellan 2 steg, 5 och 6
			if (aPtr->animationTimer > 1.0f)
			{
				aPtr->animationTimer = 0.0f;
				if(aPtr->spriteStage == 5)
				{
					aPtr->spriteStage++;
				}
				else
				{
					aPtr->spriteStage = 5;
				}
				
			}

		}
		if (walkLeft == true)
		{
			sf::Sprite* temp = nullptr;
			temp = aPtr->getPlayerSprite();
			temp->setScale(-1, 1);
			aPtr->playerSprite->setTextureRect(aPtr->spriteSheetLevel.at(aPtr->spriteStage));
			
		}
		else
		{
			sf::Sprite* temp = nullptr;
			temp = aPtr->getPlayerSprite();
			temp->setScale(1, 1);
			aPtr->playerSprite->setTextureRect(aPtr->spriteSheetLevel.at(aPtr->spriteStage));

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
