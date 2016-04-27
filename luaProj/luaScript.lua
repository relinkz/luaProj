print(package.path)
require("EnemySpawner")

Player = Entity.New(0, 0, 0.5, 0.5, 10, 10)
Enemies = {}
Walls = {}
engi = Engine
nrOfEnemies = 3



function render()
	engi.windowClear()
	engi.renderPlayer(Player)
	engi.renderEnemy(Enemy)
	
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

function spawnBullet(var1, var2, var3, var4, width, height)
	table.insert(Enemies ,Entity.New(var1, var2, var3, var4, width, height))
    --print("enemy created")
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
	--print(io.read())

	io.close(file)

	--print("hello? again")
end

readFile()
while(true)
do
	for y=1, #Walls
	do
		engi.wallIntersectionTest(Player, Walls[y], 1) 
	end

	--Player:Update()
	Player:UpdatePlayer()

	if nrOfEnemies < 100 then
		spawnX, spawnY, spawnSpeedX, spawnSpeedY = spawnPos();

		width = getWidth()
		height = getHeight()

		spawnBullet (spawnX, spawnY, spawnSpeedX, spawnSpeedY, width, height)

		nrOfEnemies = nrOfEnemies + 1
	end
	for y=1, #Enemies
	do
		Enemies[y]:Update ()
		engi.intersectionTest (Player, Enemies[y])

	end
	

	render()
end
