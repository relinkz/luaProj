math.randomseed(os.time() * 235262)

function test ()
	print("it works!")
end

function spawnYPos()
	windowWidth = 480
	spawnY = math.random (10, windowWidth);
	print (spawnY)
	return spawnY
end

function spawnXPos()
	windowWidth = 640
	spawnX = math.random (10, windowWidth);
	print (spawnX)
	return spawnX

end

function speedx (toSpawnX)
	speedX = -0.02
	if (640 - toSpawnX > toSpawnX) then
		speedX = speedX * - 1
	end

	return speedX
end

function speedy (toSpawnY)
	speedY = -0.02
	if(480 - toSpawnY > toSpawnY) then
		speedY = speedY * - 1
	end

	return speedY
end