Player = Entity.New(0, 0, 0.0, 0.0)
Enemies = {Entity.New(0, 0, 0.01, 0.0), Entity.New(0, 50, 0.01, 0.0)}
Walls = {}
engi = Engine
nrOfEnemies = 2

function render()
	engi.windowClear()
	engi.renderPlayer(Player)
	
	for y=1, #Enemies
	do
		engi.renderEnemy(Enemies[y])
	end
		for y=1, #Walls
	do
		engi.renderWall(Walls[y])
	end

	engi.windowDisplay()
end

function spawnEnemy()
	table.insert(Enemies ,Entity.New(0, 20, 0.02, 0.0))
	--print("enemy created")
end

function readFile()
	--print("hello?")  

	local file = io.open("filename.txt", "r")
	local contentX = "-1"
	local contenty = "-1"
	local numberX = -1
	local numberY = -1

	io.input(file)
	
	contentX = io.read()
	contentY = io.read()

	while(contentX ~= "")
	do

		numberX = tonumber(contentX)
		numberY = tonumber(contentY)

		print(numberX)
		print(numberY)

		table.insert(Walls,Entity.New(numberX,numberY,0.0,0.0))

		contentX = io.read()
		contentY = io.read()
		
	end
	--print(io.read())

	io.close(file)

	--print("hello? again")
end

readFile()
while(true)
do
	Player:Update()
	--spawnEnemy()
	Player:UpdatePlayer()

	if nrOfEnemies < 3 then
		spawnEnemy()
		nrOfEnemies = nrOfEnemies + 1
	end
	for y=1, #Enemies
	do
		Enemies[y]:Update()
	end

	render()
end
