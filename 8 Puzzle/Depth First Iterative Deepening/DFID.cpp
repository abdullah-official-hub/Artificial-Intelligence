#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <string.h>
#include <map>
#include <iterator>
using namespace std;
#define GRIDSIZE 3
string backTrackPath;
map <string,int> m1;
HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
void gotoxy(int Xaxis, int Yaxis){
	COORD coord;
	coord.X = Xaxis;
	coord.Y = Yaxis;
	SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
}
bool flagi=false;
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
void printArray(int **array){
	for (int a1=0;a1<GRIDSIZE;a1++){
		for (int a2=0;a2<GRIDSIZE;a2++)
			cout << array[a1][a2] << ' ';
		cout << endl;
	}
}
void printBoard(bool flag){
	if (flag){
		SetConsoleTitle("8 Puzzle - Iterative Deepening Search");
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
bool Yes(string str,int depth){
	map<string,int>::iterator itr;
	itr=m1.find(str);
	if (itr==m1.end()){
		m1[str]=depth;
		return false;
	}
	else{
		if (itr->second<=depth){
		//	m1[str]=depth;
			itr->second=depth;
			return false;
		}
	}
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
void SpecialSwap(string &str,int i1,int i2){
	string Yahoo;
	Yahoo=str[i1];
	str[i1]=str[i2];
	str[i2]=Yahoo[0];
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
void CompleteMove(int **arrayCopy,char signal){
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
bool AllPossibleChecks(int **randomArray,int **idealArray,int depth,char signal){
	if (signal=='X' || depth<0 || Yes(MakeKey(randomArray),depth))
		return false;
	int **arrayCopy;
	arrayCopy=new int*[GRIDSIZE];
	for (int i=0;i<GRIDSIZE;i++)
		arrayCopy[i]=new int[GRIDSIZE];
	CopyData(randomArray,arrayCopy);
	if (depth==0){
		ExploredNodes++;
		if (Match(arrayCopy,idealArray)){
			if (signal!='S')
				backTrackPath=signal+backTrackPath;
			DeleteArray(arrayCopy);
			return true;	
		}
		DeleteArray(arrayCopy);
		return false;
	}
	string moves=Filter(Traversal(arrayCopy));
	for (int i=0;i<moves.length();i++){
		CompleteMove(arrayCopy,moves[i]);
	    if	(AllPossibleChecks(arrayCopy,idealArray,depth-1,moves[i])){
	    	if (signal!='S')
		    	backTrackPath=signal+backTrackPath;
	    	return true;
		}
		CopyData(randomArray,arrayCopy);
	}
	return false;
}
bool IterativeDeepening(int **randomArray,int **idealArray,int depth,int &GoalDepth){
	for (int i=0;i<=depth;i++){
		if (AllPossibleChecks(randomArray,idealArray,i,'S')){
			GoalDepth=i;
			return true;
		}
		m1.clear();
	}
	return false;
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
	printBoard(true);
	printBoard(false);
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
	int max_depth,goalDepth;
	SetConsoleTextAttribute(hstdout,764);
	gotoxy(63,3);
	cout << "'-1' to Exit";
	gotoxy(6,23);
	SetConsoleTextAttribute(hstdout,752);
	cout << "Enter Depth : ";
	cin >> max_depth;
	if (max_depth==-1){
		gotoxy(36,23);
		SetConsoleTextAttribute(hstdout,761);
		DeleteArray(randomArray);
		DeleteArray(idealArray);
		system("pause");
		exit(0);
	}
	gotoxy(63,3);
	cout << "Given Depth ";
	gotoxy(63,5);
	cout  << max_depth;
	bool Animate=IterativeDeepening(randomArray,idealArray,max_depth,goalDepth);
	gotoxy(63,7);
	cout << "Explored Nodes";
	gotoxy(63,9);
	cout << ExploredNodes;
	if (Animate){
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
	system("pause");
	main();
	return 0;
}
