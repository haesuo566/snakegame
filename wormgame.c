#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define MAX_WORM 30
#define Width 25
#define Height 25 

#define Left 'a'
#define Right 'd'
#define Up 'w'
#define Down 's'

char front_map[Height][Width]; //����Ұ��� ���� ���� 
char back_map[Height][Width]; //����ϴ� ���� 

//��ǥ ����ü 
typedef struct POSITION {
	short x;
	short y;
}position, *Pposition;

void output(); //����Լ� 
void xy(short x, short y); //��ǥ�Լ� 
void Cursor(); //Ŀ�� ���ִ� �Լ� 
void Initialize(Pposition worm[], int tails); //�ʱ�ȭ �Լ� 
void Move(char dir, Pposition worm[], int tails, position **feedpos); //�����̸� �����̴� �Լ� 
void KeyBoard(char Key, char *dir);  //Ű���� �Է� �Լ� 
void draw(Pposition worm[], int tails, position **feedpos); //��� �繰�� ���ۿ� �׸��� �Լ� 
void Color(int cnum);
void feed(position **feedpos, Pposition worm[]);
void endgame();
void Delete(Pposition worm[], position **feedpos, int *tails); //���� ����� �Լ� 

int main() {
	srand((unsigned int)time(NULL));
	
	Pposition worm[MAX_WORM];
	Pposition feedpos = NULL;
	char Key;
	char dir = Left ;
	int tails = 3; //�������� �Ӹ������� ���� 
	
	Initialize(worm, tails);
	
	while(TRUE) { 
	  	
	  	//�ӵ� ���� 
		Sleep(100);
		
		output(); //���
		KeyBoard(Key, &dir);
		feed(&feedpos, worm);
		 
		Move(dir, worm, tails, &feedpos); //������ ������ 
		
		Delete(worm, &feedpos, &tails);
		
		draw(worm, tails, &feedpos);
	}
	
	int i;
	
	for(i = 0;i < tails;i++) 	
		free(worm[i]);
	
	if(feedpos != NULL)
		free(feedpos); 
	
	system("pause");
	return 0;
}

void Delete(Pposition worm[], position **feedpos, int *tails) {

	if((worm[0] -> x == (**feedpos).x) && (worm[0] -> y == (**feedpos).y)) {
		free(*feedpos);
		(*feedpos) = NULL;
		
		if((*tails) <= MAX_WORM) 
			(*tails)++;
	}
	
	else 
		return;
}

void feed(position **feedpos, Pposition worm[]) {	
	
	if(*feedpos != NULL)
		return;
	
	else if(*feedpos == NULL) {
		*feedpos = (position *)malloc(sizeof(position));
	
		(**feedpos).x = rand() % (Width - 1);	
		(**feedpos).y = rand() % Height;
	}
}

void KeyBoard(char Key, char *dir) {
	
	if(kbhit()) {
		Key = getch();
		
		switch(Key) {
			
			case Right:
			case 'D':
				
				if(*dir == Left) {					
					*dir = Left;
					break;
				}
				
				*dir = Right;
				break;
				
			case Left:
			case 'A': 
				
				if(*dir == Right) {	
					*dir = Right;
					break;
				}
				
				*dir = Left;
				break;
				
			case Up:
			case 'W':
				
				if(*dir == Down) {	
					*dir = Down;
					break;
				}
				
				*dir = Up;
				break;
			
			case Down:
			case 'S':	
				
				if(*dir == Up) {	
					*dir = Up;
					break;
				}
				
				*dir = Down;
				break;	 	 	
		}
	}
}

void Move(char dir, Pposition worm[], int tails, position **feedpos) {
	int i;
	
	//������ ������ �޸𸮸� �Ҵ��� 
	if(worm[tails - 1] == NULL)
		worm[tails - 1] = (Pposition)malloc(sizeof(Pposition));
	
	//������ ����κ��� ���� 
	for(i = (tails - 1);i > 0;i--) {
		
		if(worm[i] != NULL) {
			worm[i] -> x = worm[i - 1] -> x;
			worm[i] -> y = worm[i - 1] -> y; 
		}
	}
	
	switch(dir) {
		
		case Right:
			
			(worm[0] -> x)++;
			
			if((worm[0] -> x) > Width - 1)
				(worm[0] -> x) = Width - 1;
				
			break;
			
		case Left:
			
			(worm[0] -> x)--;
			
			if(worm[0] -> x < 0)
				(worm[0] -> x) = 0;
			
			break;
			
		case Up:
			
			(worm[0] -> y)--;
			
			if(worm[0]->y < 0)
				worm[0]->y = 0;
				
			break;
			
		case Down:
			
			(worm[0]->y)++;
			
			if(worm[0]->y > Height - 1)
				(worm[0]->y) = Height - 1;
			
			break;
			
		default:
			return;				
	}
}

void draw(Pposition worm[], int tail, position **feedpos) {
	
	int i;
	
	//���̸� �׸� 
	if(*feedpos != NULL)  
		back_map[(**feedpos).y][(**feedpos).x] = '3';
	
	//������ �׸� 
	for(i = 1;i < tail;i++) {
		
		if(worm[i] != NULL) 
			back_map[worm[i]->y][worm[i]->x] = '2';
			
		else 
			continue;	
	}
	back_map[worm[0]->y][worm[0]->x] = '1';	
}

void Initialize(Pposition worm[], int tails) {
	
	int i,j;
	
	//���ʱ�ȭ 
	for(i = 0;i < Width;i++) 
		for(j = 0;j < Height;j++) {
			back_map[j][i] = '0';
		}
		
	for(i = 0;i < MAX_WORM;i++)
		worm[i] = NULL;	
	
	//�������� �ʹ� ��ġ 
	for(i = 0;i < tails;i++) {
		worm[i] = (Pposition)malloc(sizeof(Pposition));	
		worm[i] -> x = i + 20;
		worm[i] -> y = 23;
	}
	
	system("mode con cols=50 lines=25");
	Cursor();
	
}

void output() {
	int i,j;
	
	for(i = 0;i < Width;i++) 
		for(j = 0;j < Height;j++) {
			
			if(back_map[j][i] != front_map[j][i]) {	
				xy(i * 2, j);	
				//��ĭ	
				if(back_map[j][i] == '0') {
					printf("%s","  ");
				} else if(back_map[j][i] == '1') {			
					Color(3);
					printf("%s","��");
				} else if(back_map[j][i] == '2') {				
					Color(4);
					printf("%s","��");
		        } else if(back_map[j][i] == '3') {  	
		        	Color(5);
		        	printf("%s","��");
				}
	        }
	        else 
	        	continue;
	    }
	
	//����Ʈ���ۿ� ����� ������ �Ű� ��� ����۴� 0���� �ٽ� �ʱ�ȭ        
	for(i = 0;i < Width;i++) 
		for(j = 0;j < Height;j++) {
			front_map[j][i] = back_map[j][i];
			back_map[j][i] = '0';
		}
}

void Cursor() {
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}


void Color(int cnum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cnum);
}

void xy(short x, short y) {
	COORD pos = {x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
