Player = Entity.New(200, 200, 0.0, 0.0)
Enemies = {Entity.New(0, 0, 0.01, 0.0), Entity.New(0, 50, 0.01, 0.0)}
Enemy = Entity.New(15,0,0.01,0.0)

engi = Engine
nrOfEnemies = 2

function render()
	engi.windowClear()
	engi.renderPlayer(Player)
	engi.renderEnemy(Enemy)
	
	for y=1, #Enemies
	do
		engi.renderEnemy(Enemies[y])
	end

	engi.windowDisplay()
end

function spawnEnemy(var1, var2)
	table.insert(Enemies ,Entity.New(var1, var2, 0.02, 0.0))
	print("enemy created")
end

while(true)
do
	Player:Update()
	Player:UpdatePlayer()

	if nrOfEnemies < 3 then
		spawnEnemy (20, 20)
		nrOfEnemies = nrOfEnemies + 1
	end
	for y=1, #Enemies
	do
		Enemies[y]:Update ()
		engi.intersectionTest (Player, Enemies[y])
	end
	

	render()
end
