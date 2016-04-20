Player = Entity.New(0, 0, 0.05, 0.05)
Enemies = {Entity.New(0, 0, 0.01, 0.0), Entity.New(0, 50, 0.01, 0.0)}
engi = Engine
nrOfEnemies = 2

function render()
	engi.windowClear()
	engi.renderPlayer(Player)
	
	for y=1, #Enemies
	do
		engi.renderEnemy(Enemies[y])
	end

	engi.windowDisplay()
end

function spawnEnemy()
	table.insert(Enemies ,Entity.New(0, 20, 0.02, 0.0))
	print("enemy created")
end

while(true)
do
	Player:Update()
	--spawnEnemy()

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
