
	Best First Search (Priority Queue)

	Prerequisite :

	1. Random Grid (Source Grid) must be given in 'GivenGrid.txt' for which you want optimal Solution. '-1' for blank.
	2. Ideal Grid (Target Grid) must be given in 'IdealGrid.txt'.

	Working of Code :

	1. Choose the Node with high priority from Priority Queue.
	2. Track the Visited Nodes by using maps.
	3. Return the first answer which found
	2. Ask for manual Depth to user. And starting searching till given depth.
	3. If Depth is not Given. Bydefault set the depth to 1000.
	3. If the solution is found, then Graphically represent the moves one by one and save them also in file 'BlankMovement.txt'.
	4. Display the Total Numbers of explored Nodes.
	5. Delete the useless nodes time by time to make the Time Complexity Better.



	Heuristic Use :
	
	1. Right Tiles Placement
	2. Minimum Distance of Tiles with their original Placements
	3. Depth
	4. Avoid Nodes found in Maps



	Note : It is highly recommended to close application by entering '-1' to clear the used space.