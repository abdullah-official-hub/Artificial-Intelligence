
	Best First Search (without Priority Queue)

	Prerequisite :

	1. Random Grid (Source Grid) must be given in 'GivenGrid.txt' for which you want optimal Solution. '-1' for blank.
	2. Ideal Grid (Target Grid) must be given in 'IdealGrid.txt'.

	Working of Code :

  -->	Simple Search Heuristic :
	
	1. Choose the Only One Node with High Heuristic.
	2. Ask for manual Depth to user. And starting searching till given depth.
	3. If the solution is found, then Graphically represent the moves one by one and save them also in file 'BlankMovement.txt'.
	4. Display the Total Numbers of explored Nodes.
	5. Delete the useless nodes time by time to make the Time Complexity Better.
	
  -->	Deep Search Heuristic :

	1. Track of all Heuristics which have same values.
	2. If Deep Heuristic find the answer, it is search for more optimal.
	3. Ask for manual Depth to user. And starting searching till given depth.
	4. If the solution is found, then Graphically represent the moves one by one and save them also in file 'BlankMovement.txt'.
	5. Display the Total Numbers of explored Nodes.
	6. Delete the useless nodes time by time to make the Time Complexity Better.

	Heuristic Use :
	
	1. Right Tiles Placement
	2. Minimum Distance of Tiles with their original Placements
	3. Depth
	4. Parent Checking



	Note : It is highly recommended to close application by entering '-1' to clear the used space.