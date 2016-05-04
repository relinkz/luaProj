Player = Entity.New(98, 98, 0.0, 0.0,10,10)
Walls = {}
xPos = -1
yPos = -1
input = -1
inputDelay = 0
intersectionTest = 1
engi = Engine
done = -1
gameTime = 0

function levleEditorRender()
	engi.windowClear()

	engi.renderPlayer(Player)
	
	for y=1, #Walls
	do
		engi.renderWall(Walls[y])
	end

	engi.windowDisplay()
end

function readFile()

	local file = io.open("filename.txt", "r")

	local contentX = "-1"
	local contenty = "-1"
	local numberX = -1
	local numberY = -1

	io.input(file)
	
	contentX = io.read()
	contentY = io.read()

	while(contentX ~= nil)
	do

		numberX = tonumber(contentX)
		numberY = tonumber(contentY)

		table.insert(Walls,Entity.New(numberX,numberY,0.0,0.0))

		contentX = io.read()
		contentY = io.read()
		
	end

	io.close(file)

end

function saveToFile()

	local file = io.open("filename.txt", "w+")
	
	io.input(file)
	for y=1, #Walls
	do
		Walls[y]:getPos()
		file:write(xPos,"\n")
		file:write(yPos,"\n")
	end

	io.close(file)
	--os.exit()
	done = 1

end
function handleInput()

	engi.getInput()

	if input == 1 and inputDelay > 50 then
		for y=1, #Walls
		do
			engi.wallIntersectionTest(Player, Walls[y], 0)
			if intersectionTest == -1 then
				break
			end
		end
		if intersectionTest == 1 then
			Player:getPos()
			table.insert(Walls,Entity.New(xPos, yPos, 0.0, 0.0))
		end
		input = -1
		inputDelay = 0
	end
	if input == 2 and inputDelay > 50 then
		saveToFile()
		input = -1
		inputDelay = 0
	end
	if input == 3 and inputDelay > 50 then
		for y=1, #Walls
		do
			engi.wallIntersectionTest(Player, Walls[y], 0)
			if intersectionTest == -1 then
				table.remove(Walls,y);
				input = -1
				inputDelay = 0
				break
			end
		end
	end


end

readFile()
while(done == -1)
	do
		engi.getGameTime();
		Player:UpdatePlayer(gameTime)

		handleInput()
		inputDelay = inputDelay + 1
		levleEditorRender()

	end
count = #Walls
for i=0, count do Walls[i]=nil end

