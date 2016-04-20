print(package.path)
require("EnemySpawner")

Player = Entity.New(0, 0, 0.0, 0.0)
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

function spawnBullet(var1, var2, var3, var4)
	table.insert(Enemies ,Entity.New(var1, var2, var3, var4))
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
	Player:Update()
	Player:UpdatePlayer()

	if nrOfEnemies < 100 then
		spawnX = spawnXPos()
		spawnY = spawnYPos()

		spawnSpeedX = speedx(spawnX)
		spawnSpeedY = speedy(spawnY)

		spawnBullet (spawnX, spawnY, spawnSpeedX, spawnSpeedY)

		nrOfEnemies = nrOfEnemies + 1
	end
	for y=1, #Enemies
	do
		Enemies[y]:Update ()
		engi.intersectionTest (Player, Enemies[y])

	end
	

	render()
end
