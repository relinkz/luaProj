#include "SFML\Graphics.hpp"
#include "lua.hpp"

//lua funktion som skapar spelare

sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");



void renderPlayer(float x, float y);
void renderEnemy(float x, float y);

void windowDisplay();
void windowClear();

int main()
{
	
	lua_State *L = luaL_newstate();
		
	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			//anropa lua update();

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		renderPlayer(0, 0);
		renderEnemy(10, 10);
		windowDisplay();
	}

	return 0;
}

void renderPlayer(float x, float y)
{
	sf::RectangleShape shape(sf::Vector2f(10, 10));
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(x, y);
	window.draw(shape);
}

void renderEnemy(float x, float y)
{
	sf::RectangleShape shape(sf::Vector2f(10, 10));
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(x, y);
	window.draw(shape);
}

void windowClear()
{
	window.clear();
}

void windowDisplay()
{
	window.display();
}