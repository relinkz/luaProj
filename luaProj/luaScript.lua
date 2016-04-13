local seb = Entity.New(0, 0, 0.2, 0.2)
local axel = Entity.New(15,15, 0.1, 0.1)
local engi = Engine

while(true)
do
	engi.windowClear()

	seb:Update()
	axel:Update()

	engi.renderPlayer(seb)
	engi.renderEnemy(axel)

	engi.windowDisplay()
end
