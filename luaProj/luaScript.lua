print(package.path)
require("EnemySpawner")

Player = Entity.New(250, 250, 0.5, 0.5, 10,10)
Enemies = {}
Walls = {}
Particles = {}
engi = Engine
nrOfEnemies = 3
SWAG_SCORE = 0
bonusScoreCounter = 0
enemiesClose = 0
xPos = 0
yPos = 0
counter = 0.0
nrOfEnemies = 0
maxNrOfEnemies = 50
enemySpeedMulti = 1.0
enemyIntersectionResult = 1
gameTime = 0;
musicTimer = 216.0
timer = 0

LevelSelected = 0

FPS = 0

function saveSwagToFile(score)
	local file = io.open("highScore.txt", "r")
	io.input(file)
	local highestSwag = 1
	highestSwag = tonumber(io.read())

	io.close(file)

	file = io.open("highScore.txt", "w+")
	io.input(file)
	if SWAG_SCORE > highestSwag then
		file:write(SWAG_SCORE, "\n")
	else
		file:write(highestSwag, "\n")
	end
	io.close(file)
end

function gameRender()
	engi.windowClear()

	engi.renderArena()
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
	if LevelSelected == 2 then
		engi.printScore(SWAG_SCORE, "RICK ROLLED ", 550, 250)
	end
	if LevelSelected == 4 then
		engi.printScore(SWAG_SCORE, "U MAD LOL? ", 550, 250)
	end
	engi.printScore(SWAG_SCORE, "SWAGSCORE: ", 0, 0)
	engi.printScore(FPS, "FPS: ", 0, 50)
	for y=1, #Particles
	do
		engi.renderParticle(Particles[y], enemiesClose)
	end
	engi.windowDisplay()
end

function spawnBullet(var1, var2, var3, var4, width, height)
	table.insert(Enemies ,Entity.New(var1, var2, var3, var4, width, height))
end
function release()
	for y=1, #Enemies
	do
		Enemies[y]:remove()
	end

	for y=1, #Walls 
	do
		Walls[y]:remove()
	end

	for y=1, #Particles
	do
		Particles[y]:remove()
	end
	Player:remove()
	--collectgarbage() 
end
function destroyOutsideBullets()

	for i=1, #Enemies
	do
		Enemies[i]:getPos();
		
		if xPos > 700 then
			--destroy that shit
			Enemies[i]:remove()
			test = table.remove(Enemies,i)
			nrOfEnemies = nrOfEnemies - 1
			break
		end

		if yPos > 500 then
			--destroy that shit
			Enemies[i]:remove()
			table.remove(Enemies,i)
			nrOfEnemies = nrOfEnemies - 1
			break
		end
		if xPos < -50 then
			Enemies[i]:remove()
			table.remove(Enemies,i)
			nrOfEnemies = nrOfEnemies - 1
			break
		end

		if (yPos < -50) then
			Enemies[i]:remove()
			table.remove(Enemies,i)
			nrOfEnemies = nrOfEnemies - 1
			break
		end
	end

end

function spawnEnemy()
	if nrOfEnemies < maxNrOfEnemies and LevelSelected ~= 2 and LevelSelected ~= 4  then
		spawnX, spawnY, spawnSpeedX, spawnSpeedY = spawnPos();

		width = getWidth()
		height = getHeight()

		spawnSpeedX = spawnSpeedX * enemySpeedMulti
		spawnSpeedY = spawnSpeedY * enemySpeedMulti

		spawnBullet (spawnX, spawnY, spawnSpeedX, spawnSpeedY, width, height)

		nrOfEnemies = nrOfEnemies + 1
	end	
end

function spawnWalls()
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

		table.insert(Walls,Entity.New(numberX,numberY,0.0,0.0,10,10))

		contentX = io.read()
		contentY = io.read()
		
	end
	io.close(file)
end

spawnWalls()
engi.getLevelSelected()
if LevelSelected == 4 then
table.insert(Enemies, Entity.New(0, 0,   0.0, 0.01, 640, 100))
table.insert(Enemies, Entity.New(0, 480, 0.0, -0.01, 740, 100))
table.insert(Enemies, Entity.New(0, 0,   0.01, 0.0, 100, 480))
table.insert(Enemies, Entity.New(640, 0, -0.01, 0.0, 100, 480))
end
while(true)
	do
		enemiesClose = -1
		for y=1, #Walls
		do
			engi.wallIntersectionTest(Player, Walls[y], 1) 
		end

		engi.getGameTime();
		musicTimer = musicTimer - gameTime
		if musicTimer < 0 then
		if LevelSelected == 1 then
			musicTimer = 216
			end
			if LevelSelected == 2 then
			musicTimer = 213
			os.exit()
			end
			engi.startMusic()		
		end
		Player:UpdatePlayer(gameTime)

		for y=1, #Particles
		do
			Particles[y]:Update(gameTime)
		end

		spawnEnemy()
		for y=1, #Enemies
		do
			Enemies[y]:Update (gameTime)
			engi.intersectionTest (Player, Enemies[y], bonusScoreCounter, enemiesClose)

	if enemyIntersectionResult == -1 then
			
			saveSwagToFile(SWAG_SCORE)
			release()
			return
			end
		end

		destroyOutsideBullets()


		if #Particles >= 1 then
			counter = counter + gameTime
		end
		if counter > 0.5 then
			Particles[1]:remove()
			table.remove(Particles, 1)
			counter = 0
		end
		if bonusScoreCounter >= 0.5 then
			bonusScoreCounter = 0
			SWAG_SCORE = SWAG_SCORE + 50 * (FPS / 100)
			Player:getPos();
			table.insert(Particles ,Entity.New(xPos, yPos - 10, 0.0, -3.0, 100, 100))
		end
		timer = timer + gameTime
		if timer >= 1 then
		 SWAG_SCORE = SWAG_SCORE + 1 * (FPS / 100)
		 timer = 0
		 enemySpeedMulti = enemySpeedMulti + 0.1
		 maxNrOfEnemies = maxNrOfEnemies + 1
		end
		gameRender()
	end



