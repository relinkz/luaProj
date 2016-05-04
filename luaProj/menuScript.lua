print(package.path)

Player = Entity.New(150, 50, 0.5, 0.5, 10, 10)
Buttons = {}
engi = Engine
SWAG_SCORE = 0
buttonPressed = -1
intersectiontest = -1
input = -1
gameTime = 0;

table.insert(Buttons, Entity.New(150, 0,  0.0, 0.0, 200, 140))
table.insert(Buttons, Entity.New(150, 150, 0.0, 0.0, 200, 140))
table.insert(Buttons, Entity.New(150, 300, 0.0, 0.0, 200, 140))

function menuRender()
	engi.windowClear()

	for y=1, #Buttons
	do
		engi.renderMenu(Buttons[y], y)
	end
	
	engi.renderPlayer(Player)
	engi.windowDisplay()
end


while(buttonPressed == -1)
do
	engi.getGameTime();
	Player:UpdatePlayer(gameTime)

	engi.getInput()
	if input == 1 then
		for y=1, #Buttons
		do
			engi.buttonIntersectionTest(Player, Buttons[y], y)
			if intersectionTest == -1 then
				buttonPressed = 1
			end

		end
	end

	menuRender()
	if buttonPressed == 1 then
		return
	end
end

