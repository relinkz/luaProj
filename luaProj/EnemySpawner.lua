math.randomseed (os.time () - os.clock () * 1000)

function test ()
	print("it works!")
end

function spawnYPos()
	windowWidth = 480
	spawnY = math.random (10, windowWidth);
	return spawnY
end

function spawnXPos()
	windowWidth = 640
	spawnX = math.random (10, windowWidth);
	return spawnX

end

function speedx (toSpawnX)
	speedX = math.random (1, 100)
	speedX = speedX * 0.01
	if (640 - toSpawnX < toSpawnX) then
		speedX = speedX *  -1
	end

	return speedX
end

function speedy (toSpawnY)
	speedY = 0.02
	if(480 - toSpawnY < toSpawnY) then
		speedY = speedY * - 1
	end

	return speedY
end