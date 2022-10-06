--How to run
	- Clown Repo
	- Run Debug/OpenGLSetup.exe
	- Play

--Help Button
F1 Key
Outputs:		---------Difficulty------------

				1-9 - Select number of bots

				---------Controls------------

				Left Arrow - Move Turret Left
				Right Arrow - Move Turret Right
				Spacebar - Shoot Turret Laser
				A - Start Game
				R - Reset Game

				---------View------------

				V - Switch between 1st person and 3rd person
				Scrollwheel - Zoom while in 3rd person
				Rightclick + drag - Zoom while in 3rd person


--Robots (1-9 robots based on difficulty/level)
	Robot model from assignment 1
	Robots move along a randomly generated animation path from assignment 2. 
	The robot's gun/canon and head will rotate seperately from the body to always aim at the Defensive cannon/turret
	The robots will randomly fire projectiles independantly of eachother
		The trajectory of the bullet is calculated when fired, such that the bot can continue to move along its own animation path independant of the bullet
			Starting where the bot was when bullet was fired
			Ends where the defensive cannon/turret was when bullet was fired



--Canon
	Cannon/turret can move left or right using arrow keys
	Canon shoots a laser triggered by spacebar
	Canon can move while shooting laser



--Collision/Death animation
	There is collision detection for every bullet shot from a robot against the turret
		Collision is detected by measuring the distance from bullet to turret centerpoint and seeing if its close enough

	There is collision detection for the laser shot by the defensive turret against every robot
		Collision is detected by measuring the x coor distance between laser to robot

	When a robot is hit it will:
		stop moving
		start to shrink until it disapears

	When the defensive cannon/turret is hit it will:
		stop moving
		turn red
		animate to a stop working state (canon barrel will aim downwards to the left)



--Camera
	Switch between normal view and first person view using 'V' key



--Additional
	1) Canon shoots Laser instead of projectile
		Laser will grow quickly from the cannon/turret barrel, then dissapear the same way
	
	2) Levels
		Can change the difficulty of the game by changing the number of bots 
			Select number of bots using 1-9 keys

		More bots = more to kill and more bullets to dodge thus increasing the difficulty
	
		Stats are displayed on the top right. It will display:
			Current difficulty/level
			Bots left on the field
			Score
				Gain 1 score per bot killed, score is saved even after resetting with 'r' key
			
