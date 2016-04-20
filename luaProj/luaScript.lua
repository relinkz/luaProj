Player = Entity.New(0, 0, 0.0, 0.0)
Enemies = {Entity.New(0, 0, 0.01, 0.0), Entity.New(0, 50, 0.01, 0.0)}
engi = Engine

function render()
	engi.windowClear()
	engi.renderPlayer(Player)
	
	for y=1, #Enemies
	do
		engi.renderEnemy(Enemies[y])
	end

	engi.windowDisplay()
end

while(true)
do
	Player:Update()

	Player:UpdatePlayer()

	table.insert(Enemies ,Entity.New(0, 70, 0.01, 0.0))

	for y=1, #Enemies
	do
		Enemies[y]:Update()
	end

	render()
end
