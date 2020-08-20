#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <map>
#include <iterator>
#include <string.h>
using namespace std;
#define GRIDSIZE 3
string backTrackPath;
map <string,bool> m1;
HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
void gotoxy(int Xaxis, int Yaxis){
	COORD coord;
	coord.X = Xaxis;
	coord.Y = Yaxis;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
}
class GUI_track{
	public:
		int val,x,y;
		GUI_track(){
			val=x=y=0;
		}
		void setData(int val,int x,int y){
			this->x=x;
			this->y=y;
			this->val=val;
		}
};
class node{
	public:
		int **data,priority,depth;
		string moves;
		node *next;
};
node *front = NULL;
void insertPQ(int **data,int priority,int depth,string move){
	node *temp, *ptr;
	temp = new node;
	temp->data=new int*[GRIDSIZE];
	for (int i=0;i<GRIDSIZE;i++)
		temp->data[i]=new int[GRIDSIZE];
	for (int i=0;i<GRIDSIZE;i++){
		for (int j=0;j<GRIDSIZE;j++)
			temp->data[i][j]=data[i][j];
	}
	temp->priority = priority;
	temp->depth=depth;
	temp->moves=move;
	if (front == NULL || priority < front->priority) {
    	temp->next = front;
    	front = temp;
	}
	else{
    	ptr = front;
    	while (ptr->next != NULL && ptr->next->priority <= priority)
    	ptr=ptr->next;
    	temp->next = ptr->next;
    	ptr->next = temp;
   }
}
void deletePQ(){
	node *temp;
	if(front == NULL){
	cout<<"Priority Queue Underflow1\n";
	system("pause");
	}
	else{
		temp = front;
		front = front->next;
		for (int i=0;i<GRIDSIZE;i++)
			delete temp->data[i];
		delete temp;
   }
}
bool isEmpty(){
	if (front==NULL)
		return true;
	return false;
}
void deleteAllQueue(){
	while(!isEmpty())
		deletePQ();
}
node* getTop(){
	node *temp;
	if(front == NULL){
	cout<<"Priority Queue Underflow2\n";
	system("pause");
	}
	else{
		temp = front;
		front = front->next;
		return temp;
   }
}
int getTopPriority(){
	if(front != NULL)
		return front->depth;
}
void displayPQ(){
	node *ptr;
	ptr = front;
	if (front == NULL)
		cout<<"Priority Queue is empty\n";
	else{
    	cout<<"Queue elements in decreasing priority are :\n";
    	while(ptr != NULL) {
        	cout<<ptr->data<<" ";
        	ptr = ptr->next;
    	}
    	cout<<endl;
	}
}
void printBoard(bool flag){
	if (flag){
		SetConsoleTitle("8 Puzzle - Best First Search (Pure Heuristic) - Priority Queue");
		system("cls");
		system("color f0");
		SetConsoleTextAttribute(hstdout,752);
		gotoxy(6,1);
		cout << "Source Given in File";
		gotoxy(36,1);
		cout << "Target Given in File";	
	}
	int num,diff=0;
	SetConsoleTextAttribute(hstdout,768);
	num=3;
	if (!flag)
		diff=30;
	for (int i=1;i<=4;i++){					// Rows
		for (int j=6+diff;j<=30+diff;j++){
			gotoxy(j,num);
			cout << ' ';
		}
	num+=6;
	}
	num=6+diff;
	for (int i=1;i<=4;i++){					// Columns
		for (int j=3;j<21;j++){
			gotoxy(num,j);
			cout << ' ';
		}
		num+=8;
	}
}
void MakeBox(int num,int x,int y){
	gotoxy(x,y);
	SetConsoleTextAttribute(hstdout,736);
	if (num<0)
		SetConsoleTextAttribute(hstdout,768);
	for (int i=1;i<6;i++){
		gotoxy(x,y++);
		cout << "   " << ((i==3)?char(num+48):' ') << "   ";
	}
	
}
void fillBoards(int **randomArray,int **idealArray,GUI_track *obj){
	int x,y=4,diff=0,counter=0;
	for (int k=1;k<=2;k++){
		y=4;
		if (k==2)
			diff=30;
		for (int i=0;i<GRIDSIZE;i++){
			x=7+diff;
			for (int j=0;j<GRIDSIZE;j++){
				if (k==2)
					MakeBox(idealArray[i][j],x,y);
				else{
					MakeBox(randomArray[i][j],x,y);
					obj[counter++].setData(randomArray[i][j],x,y);
				}
				x=x+8;
			}
			y=y+6;
		}
	}
}
void sys(string str){
	cout << str << ' ';
	system("pause");
}
long long int ExploredNodes=0;
bool Match(int **randomGrid,int **idealGrid){
	for (int a1=0;a1<GRIDSIZE;a1++)
		for (int a2=0;a2<GRIDSIZE;a2++)
			if (randomGrid[a1][a2]!=idealGrid[a1][a2])
				return false;
	return true;
}
string Traversal(int **randomArray){
	string str;
	for (int i=0;i<GRIDSIZE;i++)
		for (int j=0;j<GRIDSIZE;j++)
			if (randomArray[i][j]==-1){
				str=((i-1)>=0?"U":"X");				// UP
				str=str+((i+1)<GRIDSIZE?"D":"X");	// DOWN
				str=str+((j-1)>=0?"L":"X"); 		// LEFT
				str=str+((j+1)<GRIDSIZE?"R":"X");	// RIGHT
			}
	return str;
}
void CopyData(int **randomArray,int **arrayCopy){
	for (int i=0;i<GRIDSIZE;i++)
		for (int j=0;j<GRIDSIZE;j++)
			arrayCopy[i][j]=randomArray[i][j];
}
void printArray(int **array){
	for (int a1=0;a1<GRIDSIZE;a1++){
		for (int a2=0;a2<GRIDSIZE;a2++)
			cout << array[a1][a2] << ' ';
		cout << endl;
	}
}
void DeleteArray(int **array){
	for (int i=0;i<GRIDSIZE;i++)
		delete array[i];
	delete array;
}
void swap(int &a1,int &a2){
	a1=a1+a2;
	a2=a1-a2;
	a1=a1-a2;
}
void looop(int val,char ch){
	for (int i=1;i<=val;i++)
		cout << ch;
}
string Filter(string str){
	string str1;
	for(int i=0;i<str.length();i++){
		if(str[i]!='X')
			str1=str1+str[i];
	}
	str.clear();
	str=str1;
	return str;
}
int RightTilesPlacement(int **source,int **target){
	int RTP=0;
	for (int i=0;i<GRIDSIZE;i++)
		for (int j=0;j<GRIDSIZE;j++)
			if (source[i][j]==target[i][j] && source[i][j]!=-1)
				++RTP;
	return RTP;
}
int PositiveThenSum(int val1,int val2){
	if (val1<0)
		val1=val1*(-1);
	if (val2<0)
		val2=val2*(-1);
	return val1+val2;
}
int MinimumDistance(int **source,int **target){
	int row=2,col=9;
	int **a1,**a2;
	a1=new int*[row];
	a2=new int*[row];
	for (int i=0;i<row;i++){
		a1[i]=new int[col];
		a2[i]=new int[col];
	}
	for (int i=0;i<GRIDSIZE;i++){
		for (int j=0;j<GRIDSIZE;j++){
			if(source[i][j]!=-1){
				a1[0][source[i][j]]=i;
				a1[1][source[i][j]]=j;
			}
			if(target[i][j]!=-1){
				a2[0][target[i][j]]=i;
				a2[1][target[i][j]]=j;
			}
		}
	}
	int sum=0;
	for (int i=1;i<col;i++)
		sum=sum+PositiveThenSum(a1[0][i]-a2[0][i],a1[1][i]-a2[1][i]);
	for (int i=0;i<row;i++){
		delete a1[i];
		delete a2[i];
	}
	delete a1;
	delete a2;
	return sum;
}
bool solutionFound=false;
bool Yes(string str){
	if (m1.find(str)==m1.end())
		return false;
	return true;
}
string MakeKey(int **array){
	string key;
	for (int i=0;i<GRIDSIZE;i++){
		for (int j=0;j<GRIDSIZE;j++){
			if (array[i][j]!=-1)
				key=key+char((array[i][j])+48);
			else
				key=key+"0";
		}
	}
	return key;
}
void CompleteMove1(int **arrayCopy1,int **idealArray,char signal,int depth,string status){
	int **arrayCopy;
	arrayCopy=new int*[GRIDSIZE];
	for(int i=0;i<GRIDSIZE;i++)
		arrayCopy[i]=new int[GRIDSIZE];
	CopyData(arrayCopy1,arrayCopy);
	int row,column;
	bool flag=false;
	for (row=0;row<GRIDSIZE;++row){
		for (column=0;column<GRIDSIZE;++column){
			flag=false;
			if (arrayCopy[row][column]==-1){
				flag=true;
				break;
			}
		}
		if(flag)
			break;
	}
	if (signal=='U')
		swap(arrayCopy[row][column],arrayCopy[row-1][column]);
	else if (signal=='D')
		swap(arrayCopy[row][column],arrayCopy[row+1][column]);
	else if (signal=='L')
		swap(arrayCopy[row][column],arrayCopy[row][column-1]);
	else if (signal=='R')
		swap(arrayCopy[row][column],arrayCopy[row][column+1]);
	int Priority;
	string str=MakeKey(arrayCopy);
	if (!Yes(str)){
		ExploredNodes++;
		Priority=(8-RightTilesPlacement(arrayCopy,idealArray))+MinimumDistance(arrayCopy,idealArray);
	//	status=status+signal;
		insertPQ(arrayCopy,Priority,depth,status+signal);
	}
	DeleteArray(arrayCopy);
}
bool Priority_Heuristic(int **idealArray,int givenDepth,int &goalDepth,int runningDepth=0){
	while(true){
		if(runningDepth>givenDepth || isEmpty())
			return false;
		node *temp=getTop();
		string str=MakeKey(temp->data);
		if (!Yes(str))
			m1.insert(pair<string,bool>(MakeKey(temp->data),true));
		if(Match(temp->data,idealArray)){
			backTrackPath=temp->moves;
			goalDepth=temp->depth;
			return true;
		}
		string moves=Filter(Traversal(temp->data));	
		for (int i=0;i<moves.length() && runningDepth<givenDepth;i++){
			CompleteMove1(temp->data,idealArray,moves[i],temp->depth+1,temp->moves);
		}
		DeleteArray(temp->data);
		runningDepth=getTopPriority();
	}
}
bool BFS_PQ(int **randomArray,int **idealArray,int givenDepth,int &goalDepth){
	int Priority=(8-RightTilesPlacement(randomArray,idealArray))+MinimumDistance(randomArray,idealArray);
	insertPQ(randomArray,Priority,0,"S");
	return Priority_Heuristic(idealArray,givenDepth,goalDepth);
}
void swapGT(int val1,int val2,GUI_track *GT,int &index1,int &index2){
	for (int i=0;i<GRIDSIZE*GRIDSIZE;i++){
		if (GT[i].val==val1)
			index1=i;
		else if (GT[i].val==val2)
			index2=i;
	}
	GT[index1].val=GT[index1].val+GT[index2].val;
	GT[index2].val=GT[index1].val-GT[index2].val;
	GT[index1].val=GT[index1].val-GT[index2].val;
}
void Animation(int **randomArray,GUI_track *GT){
	fstream file;
	file.open("BlankMovement.txt",ios::out); 
	int blankIndexRow=-1,blankIndexColumn;
	for (int i=0;i<GRIDSIZE;i++){
		for (int j=0;j<GRIDSIZE;j++)
			if (randomArray[i][j]==-1){
				blankIndexRow=i;
				blankIndexColumn=j;
				break;
			}
		if(blankIndexRow!=-1)
			break;
	}
	int i1,i2;
	gotoxy(63,16);
	cout << "Blank Movement";
	for (int i=0;i<backTrackPath.length();i++){
		Sleep(1000);
		SetConsoleTextAttribute(hstdout,767);
		gotoxy(65,18);
		cout << "     ";
		gotoxy(65,18);
		SetConsoleTextAttribute(hstdout,764);
		if (backTrackPath[i]=='U'){
			cout << "UP";
			file << "UP" << endl;
			swap(randomArray[blankIndexRow][blankIndexColumn],randomArray[blankIndexRow-1][blankIndexColumn]);
			swapGT(randomArray[blankIndexRow][blankIndexColumn],randomArray[blankIndexRow-1][blankIndexColumn],GT,i1,i2);
			MakeBox(GT[i2].val,GT[i2].x,GT[i2].y);
			MakeBox(GT[i1].val,GT[i1].x,GT[i1].y);
			blankIndexRow=blankIndexRow-1;
		}
		else if (backTrackPath[i]=='D'){
			cout << "DOWN";
			file << "DOWN" << endl;
			swap(randomArray[blankIndexRow][blankIndexColumn],randomArray[blankIndexRow+1][blankIndexColumn]);
			swapGT(randomArray[blankIndexRow][blankIndexColumn],randomArray[blankIndexRow+1][blankIndexColumn],GT,i1,i2);
			MakeBox(GT[i2].val,GT[i2].x,GT[i2].y);
			MakeBox(GT[i1].val,GT[i1].x,GT[i1].y);
			blankIndexRow=blankIndexRow+1;
		}
		else if (backTrackPath[i]=='L'){
			cout << "LEFT";
			file << "LEFT" << endl;
			swap(randomArray[blankIndexRow][blankIndexColumn],randomArray[blankIndexRow][blankIndexColumn-1]);
			swapGT(randomArray[blankIndexRow][blankIndexColumn],randomArray[blankIndexRow][blankIndexColumn-1],GT,i1,i2);
			MakeBox(GT[i2].val,GT[i2].x,GT[i2].y);
			MakeBox(GT[i1].val,GT[i1].x,GT[i1].y);
			blankIndexColumn=blankIndexColumn-1;			
		}
		else if (backTrackPath[i]=='R'){
			cout << "RIGHT";
			file << "RIGHT" << endl;
			swap(randomArray[blankIndexRow][blankIndexColumn],randomArray[blankIndexRow][blankIndexColumn+1]);
			swapGT(randomArray[blankIndexRow][blankIndexColumn],randomArray[blankIndexRow][blankIndexColumn+1],GT,i1,i2);
			MakeBox(GT[i2].val,GT[i2].x,GT[i2].y);
			MakeBox(GT[i1].val,GT[i1].x,GT[i1].y);	
			blankIndexColumn=blankIndexColumn+1;		
		}
	}
	file.close();
	Sleep(1000);
	SetConsoleTextAttribute(hstdout,764);
	gotoxy(63,18);
	cout << "Movements Record";
	gotoxy(63,20);
	cout << "in text file";
}
void fillIdealArrayFromFile(fstream &file,int **array){
	for (int i=0;i<GRIDSIZE;i++)
		for (int j=0;j<GRIDSIZE;j++)
			file >> array[i][j];
}
void fillGivenArrayFromFile(fstream &file,int **array){
	for (int i=0;i<GRIDSIZE;i++)
		for (int j=0;j<GRIDSIZE;j++)
			file >> array[i][j];
}
int main(){
	srand(time(0));
	ExploredNodes=0;
	backTrackPath.clear();
	solutionFound=false;
	printBoard(true);
	printBoard(false);
	SetConsoleTextAttribute(hstdout,736);
	fstream file;
	file.open("GivenGrid.txt",ios::in);
	if (!file){
		sys("\n GivenGrid.txt File not Exists\n");
		exit(0);
	}
	int **idealArray,**randomArray;
	idealArray=new int*[GRIDSIZE];
	randomArray=new int*[GRIDSIZE];
	for (int i=0;i<GRIDSIZE;i++){
		idealArray[i]=new int[GRIDSIZE];
		randomArray[i]=new int[GRIDSIZE];
	}
	fillGivenArrayFromFile(file,randomArray);
	file.close();
	file.open("IdealGrid.txt",ios::in);
	if (!file){
		sys("\n IdealGrid.txt File not Exists\n");
		exit(0);
	}
	fillIdealArrayFromFile(file,idealArray);
	file.close();
	GUI_track *GT;
	GT=new GUI_track[9];
	fillBoards(randomArray,idealArray,GT);
	int max_depth,goalDepth,ch;
	SetConsoleTextAttribute(hstdout,764);
	gotoxy(63,3);
	cout << "'-1' to Exit";
	gotoxy(6,23);
	SetConsoleTextAttribute(hstdout,752);
	cout << "Enter '1' to Use Depth Bound / Any other key for without Depth Bound : ";
	cin >> ch;
	gotoxy(6,23);
	looop(100,' ');
	bool f1=false;
	if (ch==1){
		gotoxy(6,23);
		gotoxy(6,23);
		cout << "Enter Depth : ";
		cin >> max_depth;
	}
	else{
		max_depth=1000;
		f1=true;
	}
	if (max_depth==-1 || ch==-1){
		gotoxy(6,23);
		looop(100,' ');
		gotoxy(6,23);
		SetConsoleTextAttribute(hstdout,761);
		DeleteArray(randomArray);
		DeleteArray(idealArray);
		cout << "Your Choice is 'Exit'. ";
		system("pause");
		exit(0);
	}
	gotoxy(63,3);
	cout << "Given Depth ";
	gotoxy(63,5);
	if (!f1)
		cout  << max_depth;
	else
		cout  << max_depth << " (Default)";
	ExploredNodes=ExploredNodes+1;
	solutionFound=BFS_PQ(randomArray,idealArray,max_depth,goalDepth);
	gotoxy(63,7);
	cout << "Explored Nodes";
	gotoxy(63,9);
	cout << ExploredNodes;
	if (solutionFound){
		gotoxy(63,11);
		cout << "Goal Depth";
		gotoxy(63,13);
		cout  << goalDepth;
		Animation(randomArray,GT);
	}
	else{
		SetConsoleTextAttribute(hstdout,764);
		gotoxy(63,16);
		cout << "No Solution";
		gotoxy(63,18);
		cout << "Found under";
		gotoxy(63,20);
		cout << "Depth " << max_depth;
	}
	gotoxy(36,23);
	SetConsoleTextAttribute(hstdout,761);
	DeleteArray(randomArray);
	DeleteArray(idealArray);
	deleteAllQueue();
	m1.clear();
	system("pause");
	main();
	return 0;
}
