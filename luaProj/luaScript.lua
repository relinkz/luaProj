print(package.path)
require("EnemySpawner")

Player = Entity.New(320, 240, 0.5, 0.5, 10, 10)
Enemies = {}
Walls = {}
engi = Engine
nrOfEnemies = 0

xPos = 0;   -- temp values
yPos = 0;	-- temp values



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

function destroyOutsideBullets()

	for i=1, #Enemies
	do
		Enemies[i]:getPos();
		
		if xPos > 700 then
			--destroy that shit
			test = table.remove(Enemies,i)
			nrOfEnemies = nrOfEnemies - 1
			break
		end

		if yPos > 500 then
			--destroy that shit
			table.remove(Enemies,i)
			nrOfEnemies = nrOfEnemies - 1
			break
		end
		if xPos < -50 then
			table.remove(Enemies,i)
			nrOfEnemies = nrOfEnemies - 1
			break
		end

		if (yPos < -50) then
			table.remove(Enemies,i)
			nrOfEnemies = nrOfEnemies - 1
			break
		end
	end

end

function spawnEnemy()
	print(nrOfEnemies)
	if nrOfEnemies < 50 then
		spawnX, spawnY, spawnSpeedX, spawnSpeedY = spawnPos();

		width = getWidth()
		height = getHeight()

		spawnBullet (spawnX, spawnY, spawnSpeedX, spawnSpeedY, width, height)

		nrOfEnemies = nrOfEnemies + 1
		print("spawed an enemy")
	end	
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

readFile()
while(true)
do
	for y=1, #Walls
	do
		engi.wallIntersectionTest(Player, Walls[y], 1) 
	end

	--Player:Update()
	Player:UpdatePlayer()
	spawnEnemy()

	for y=1, #Enemies
	do
		Enemies[y]:Update ()
		engi.intersectionTest (Player, Enemies[y])
	end
	destroyOutsideBullets()

	render()
end
