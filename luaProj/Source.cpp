#include "SFML\Graphics.hpp"
#include "lua.hpp"

sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
sf::CircleShape shape(100.f);

void renderBox(float x, float y);
int main()
{
	lua_State *L = luaL_newstate();


	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		renderBox(0,0);
		window.display();
	}

	return 0;
}
void renderBox(float x, float y)
{
	window.draw(shape);

}