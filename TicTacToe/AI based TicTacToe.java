import java.util.Random;
import javax.swing.*;
import java.util.*;

class Provider{
	public static int nullVal=-1;
	public static int GivenDepth=5;
}
class Node{
	int Grid[],GridSize,status;
	Node(){
		this.status=11;
		this.GridSize=9;
		this.Grid=new int[this.GridSize];
		setDefault();
	}
	Node(Node node){
		this.status=11;
//		this.status=node.status;
		this.GridSize=node.GridSize;
		this.Grid=new int[this.GridSize];
		SetGrid(node);
	}
	void newLine(){
		System.out.println();
	}
	void setDefault(){
		for (int i=0;i<this.GridSize;i++)
			Grid[i]=Provider.nullVal;
	}
	void setGridValue(int boxNo,int value){
		if (boxNo>0 && boxNo<=9)
			this.Grid[boxNo-1]=value;
	}
	int getGridValue(int boxNo){
		if (boxNo>0 && boxNo<=9)
			return this.Grid[boxNo-1];
		else
			return 0;
	}
	void SetGrid(Node node){
		for (int i=0;i<this.GridSize;i++)
			this.Grid[i]=node.Grid[i];
	}
	void PrintGrid(){
		newLine();
		for (int i=0;i<GridSize;i++){
			System.out.print(" "+this.Grid[i]);
			if ((i+1)%3==0)
				newLine();
		}
	}
	boolean isSame(Node node){
		for (int i=0;i<this.GridSize;i++)
			if (node.Grid[i]!=this.Grid[i])
				return false;
		return true;
	}
	boolean isSetAll(){
		for (int i=0;i<this.GridSize;i++)
			if (this.Grid[i]==Provider.nullVal)
				return false;
		return true;
	}
	boolean isMatch(int index1,int index2,int index3,int val){
		if ((this.Grid[index1]==this.Grid[index2] && this.Grid[index1]==this.Grid[index3]) && this.Grid[index1]!=-1 && this.Grid[index1]==val)
			return true;
		return false;
	}
	int isWin(){
		int status=0;
		if (isMatch(0,1,2,5) || isMatch(3,4,5,5) || isMatch(6,7,8,5) || isMatch(0,3,6,5) || isMatch(1,4,7,5) ||
			isMatch(2,5,8,5) || isMatch(0,4,8,5) || isMatch(2,4,6,5))
			status=1;
		if (isMatch(0,1,2,6) || isMatch(3,4,5,6) || isMatch(6,7,8,6) || isMatch(0,3,6,6) || isMatch(1,4,7,6) ||
			isMatch(2,5,8,6) || isMatch(0,4,8,6) || isMatch(2,4,6,6))
			if (status==1)
				status=3;
			else
				status=2;
		return status;
	}
}

class AlphaBeta{
	int Alpha,Beta;
	AlphaBeta(){
		this.Alpha=-1000;
		this.Beta=1000;
	}
	AlphaBeta(AlphaBeta obj){
		this.Alpha=obj.Alpha;
		this.Beta=obj.Beta;
	}
	void setAplphaBeta(int Alpha,int Beta){
		this.Alpha=Alpha;
		this.Beta=Beta;
	}
}

class SpecialNode{
	int heuristic,box;
	void setSpecialNode(int heuristic,int box){
		this.heuristic=heuristic;
		this.box=box;
	}
}

class TicTacToe{
	Node node;
	int comVal,humanVal;
	ArrayList <SpecialNode> specialNode;
	TicTacToe(){
		comVal=5;
		humanVal=6;
		node=new Node();
		specialNode=new ArrayList<SpecialNode>();
	}
	String CheckGameStatus(){
		if (node.isWin()==1)
			return "ComWin";
		else if (node.isWin()==2)
			return "HumanWin";
		else if(node.isSetAll())
			return "Draw";
		else
			return "Continue";
	}
	ArrayList <Integer> maxMoves(Node workingNode) throws Exception{
		ArrayList <Integer> moves=new ArrayList<Integer>();
		for (int i=0;i<workingNode.GridSize;i++)
			if (workingNode.Grid[i]==Provider.nullVal)
				moves.add(new Integer(i+1));
		return moves;
	}
	boolean compareNodes(Node node1,Node node2){
		return node1.isSame(node2);
	}
	ArrayList <Node> getManipulateNodes(Node possibleMove){
		Node original=new Node(possibleMove);
		Node change=new Node();
		Node temp=new Node();
		ArrayList <Node> ManipulateMoves=new ArrayList<Node>();
		int counter=0;
		for (int i1=2;i1>=0;i1--)
			for (int i2=i1;i2<original.GridSize;i2+=3)
				change.Grid[counter++]=original.Grid[i2];
		ManipulateMoves.add(new Node(change));
		temp.SetGrid(change);
		for (int i=0,j=temp.GridSize-1;i<change.GridSize;i++,j--)
			change.Grid[i]=temp.Grid[j];
		ManipulateMoves.add(new Node(change));
		temp.SetGrid(original);
		for (int i=0,j=temp.GridSize-1;i<change.GridSize;i++,j--)
			change.Grid[i]=temp.Grid[j];
		ManipulateMoves.add(new Node(change));
		temp.SetGrid(original);
		counter=0;
		for (int i1=0;i1<=2;i1++)
			for (int i2=i1;i2<possibleMove.GridSize;i2+=3)
				change.Grid[counter++]=temp.Grid[i2];
		ManipulateMoves.add(new Node(change));
		temp.SetGrid(original);
		counter=0;
		for (int i1=6;i1>=0;i1-=3)
			for (int i2=i1;i2<i1+3;i2++)
				change.Grid[counter++]=temp.Grid[i2];
		ManipulateMoves.add(new Node(change));
		temp.SetGrid(original);
		counter=0;
		for (int i1=2;i1<temp.GridSize;i1+=3)
			for (int count=1,i2=i1;count<=3;i2--,count++)
				change.Grid[counter++]=temp.Grid[i2];
		ManipulateMoves.add(new Node(change));

		return ManipulateMoves;
	}
	void stop(String str){
		System.out.println(str);
		JOptionPane.showInputDialog("Pass");
	}
	ArrayList <Node> PolishMoves(ArrayList <Node> possibleMoves){
		for (int i=0;i<possibleMoves.size();i++){
			if (possibleMoves.get(i).status!=12){
				ArrayList <Node> ManipulateNodes=getManipulateNodes(possibleMoves.get(i));
				for (int i2=i+1;i2<possibleMoves.size();i2++){
					if (i2>=possibleMoves.size())
						break;
					for (int i1=0;i1<ManipulateNodes.size();i1++){
						if (possibleMoves.get(i).status!=12){
							if (compareNodes(ManipulateNodes.get(i1),possibleMoves.get(i2)))
								possibleMoves.get(i).status=12;
						}
					}
				}
			}
		}
		ArrayList <Node> MinimizeMoves=new ArrayList <Node>();
		for (int i=0;i<possibleMoves.size();i++)
			if (possibleMoves.get(i).status!=12)
				MinimizeMoves.add(possibleMoves.get(i));
		return MinimizeMoves;
	}
	int diff(Node n1,Node n2){
		for (int i=0;i<n1.GridSize;i++)
			if (n1.Grid[i]!=n2.Grid[i])
				return i+1;
		return -1;
	}
	int fetchBox(int heuristicVal){
		for (int i=0;i<specialNode.size();i++)
			if (specialNode.get(i).heuristic==heuristicVal)
				return specialNode.get(i).box;
		return -1;
	}
	AlphaBeta AlphaBetaPruning(boolean turn,Node workingNode,int Depth,AlphaBeta alphabeta,int comWin,int humanWin)throws Exception{

		int heuristic;
		if (workingNode.isWin()==1)
			comWin++;
		else if (workingNode.isWin()==2)
			humanWin++;
		else if (workingNode.isWin()==3){
			comWin++;
			humanWin++;
		}

		if(workingNode.isSetAll() || Depth==Provider.GivenDepth){

			if (comWin>=humanWin && comWin!=0)
				heuristic=comWin;
			else if (comWin==humanWin && comWin==0)
				heuristic=0;
			else
				heuristic=-1;
			if (!turn)
				alphabeta.Alpha=heuristic;
			else
				alphabeta.Beta=heuristic;
			return alphabeta;
		}

		ArrayList <Integer> moves=maxMoves(workingNode);
		ArrayList <Node> possibleMoves=new ArrayList<Node>();
		Node obj=new Node();
		obj.SetGrid(workingNode);
		for(int i=0;i<moves.size();i++){
			if(turn==false){
				obj.setGridValue((int)moves.get(i),comVal);
				possibleMoves.add(new Node(obj));
				obj.setGridValue((int)moves.get(i),Provider.nullVal);
			}
			else{
				obj.setGridValue((int)moves.get(i),humanVal);
				possibleMoves.add(new Node(obj));
				obj.setGridValue((int)moves.get(i),Provider.nullVal);
			}
		}
		ArrayList <Node> MinimizeMoves=PolishMoves(possibleMoves);
		AlphaBeta object;
		int freshChange;
		for (int i=0;i<MinimizeMoves.size();i++){

			if (alphabeta.Alpha<alphabeta.Beta){
				object=AlphaBetaPruning(!turn,MinimizeMoves.get(i),Depth+1,new AlphaBeta(alphabeta),comWin,humanWin);
				if (!turn){
					if (alphabeta.Alpha<object.Beta)
						alphabeta.Alpha=object.Beta;
					freshChange=alphabeta.Alpha;
				}
				else{
					if (alphabeta.Beta>object.Alpha)
						alphabeta.Beta=object.Alpha;	
					freshChange=alphabeta.Beta;
				}
				if (Depth==0){
					SpecialNode temp=new SpecialNode();
					temp.setSpecialNode(freshChange,diff(MinimizeMoves.get(i),this.node));
					specialNode.add(temp);
				}
			}
		}

		return alphabeta;
	}

	void StartGame() throws Exception{
		Random rand=new Random();
		// value '5' in Grid is for Computer and '6' for Human
		System.out.println(" Computer Turn");
//		node.setGridValue(rand.nextInt(9)+1,comVal);
		node.setGridValue(2,comVal);
		node.PrintGrid();
		int box=0;
		String status;
		boolean turn=true; // true for Human
		AlphaBeta YumPumVal;
		while(true){
			if (turn){
				box=Integer.parseInt(JOptionPane.showInputDialog("Enter Box Number"));
				while(true){
					if (box>0 && box<=9 && node.getGridValue(box)<0)
						break;
					box=Integer.parseInt(JOptionPane.showInputDialog("Wrong !!! Enter Box Number (1-9)"));
				}
				node.setGridValue(box,humanVal);
				System.out.println(" Human Turn");
				node.PrintGrid();
			}
			else{
				YumPumVal=AlphaBetaPruning(turn,this.node,0,new AlphaBeta(),0,0);
			//	for (int i=0;i<specialNode.size();i++){
				//	System.out.println("Box : "+specialNode.get(i).box+" Heuristic : "+specialNode.get(i).heuristic);
			//	}
				box=fetchBox(YumPumVal.Alpha);
				specialNode.clear();
				node.setGridValue(box,comVal);
				System.out.println(" Computer Turn");
				node.PrintGrid();
			}
			status=CheckGameStatus();
			if (!(status.equals("Continue")))
				break;
			turn=!turn;
		}
	}
}

class Driver{
	public static void main(String[] args) throws Exception
	{
		TicTacToe game=new TicTacToe();
		game.StartGame();
	}
}