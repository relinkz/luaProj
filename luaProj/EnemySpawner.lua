math.randomseed (os.time () - os.clock () * 1000)

ARENA_WIDTH = 640
ARENA_HEIGHT = 480

function test ()
	print("it works!")
end

function speedx (toSpawnX)
	speedX = math.random (20, 50)
	speedX = speedX * 0.01
	if (ARENA_WIDTH - toSpawnX < toSpawnX) then
		speedX = speedX * - 1
	end

	return speedX
end

function speedy (toSpawnY)
	speedY = math.random (20, 50)
	speedY = speedY * 0.01;

	if(ARENA_HEIGHT - toSpawnY < toSpawnY) then
		speedY = speedY * - 1
	end

	return speedY
end

function getWidth()
	maxWidth = 50
	width = math.random (10, maxWidth);
	return width

end

function getHeight()
	maxHeight = 50
	height = math.random (10, maxHeight);
	return height
end

function findOutClosestSide (xPos, yPos, xMax, yMax)
	finalXPos = 0;
	finalYPos = 0;

	distanceX = xMax - xPos;
	distanceY = yMax - yPos;

	finalSpeedX = 0;
	finalSpeedY = 0;

	sideToSpawn = math.random(0,1);

	if(sideToSpawn == 0) then

		if(distanceX < xPos) then --left or right
			-- right side
			finalYPos = yPos;
			finalXPos = xMax;
		else
			--right
			finalXPos = 0- 50;
			finalYPos = yPos;
		end
		finalSpeedX = speedx (finalXPos);
	else	--top or bot
		if(distanceY < yPos) then
			finalXPos = xPos;
			finalYPos = yMax;
		else
			finalYPos = 0 - 50;
			finalXPos = xPos;
		end
		finalSpeedY = speedy (finalYPos)
	end
		return finalXPos, finalYPos, finalSpeedX, finalSpeedY;
end

function spawnPos()
	windowHeight = ARENA_HEIGHT;
	halfWindowHeight = (windowHeight / 2);

	windowWidth = ARENA_WIDTH;
	halfWindowHeight = (windowWidth / 2);

	spawnY = math.random (10, windowWidth);
	spawnX = math.random (10, windowHeight);

	spawnX, spawnY, leSpeedX, leSpeedY = findOutClosestSide (spawnX, spawnY, windowWidth, windowHeight);

	return spawnX,spawnY, leSpeedX, leSpeedY;
end