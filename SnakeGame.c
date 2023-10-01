#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#define HASHMARK '#'
#define SNAKE '@'
#define FOOD '$'
#define SPACE ' '
#define ROWS 25 
#define COLS 75 
#define MINSIZE 10 // starting size for the snake
#define MAXSIZE 20 // the maximum size of the snake
#define UP 72     
#define DOWN 80    
#define LEFT 75    
#define RIGHT 77   
#define ESC 27   
typedef struct {
	int x;
	int y;
}TheSnake;
void firstMenu();  //Starting Game Menu
void PrintstheStartingMenu();//Prints menu list
void Start(); // Starting the game
void print_border(int row_size, int col_size);
void gotoxy(int y, int x);//function that takes the cursor to a chosen location
void goto_char(int x, int y, char c);
void freeArr(TheSnake** head, int snakesize);
int getKey();// Get the players pick
TheSnake move_snake(TheSnake** head, int snake_size, TheSnake* currentlocation);
TheSnake* newlocation(int x, int y);
TheSnake create_food(TheSnake* foodllocation, TheSnake** head, int snakesize);
TheSnake** SnakeGetsLarger(TheSnake** head, int* snakesize, TheSnake* currentlocation);

void main() {
	firstMenu();
			}

void firstMenu() {
	int pick;
	do {
		PrintstheStartingMenu();
		scanf("%d", &pick);
		if (pick == 1) {
			Start(ROWS, COLS);
		}
	} while (pick != 0);
	exit(1);
}
void PrintstheStartingMenu() {
	int i;
	for (i = 0; i < 55; i++)
		printf("*");
	printf("\nWelcome! ");
	printf("\n* Pick your level,to exit press 0");
	printf("\n* 0 - Exit");
	printf("\n* 1 - Basic Level");
	printf("\n* 2 - Intermidiate Level");
	printf("\n* 3 - Advanced Level\n");
	for (i = 0; i < 55; i++)
		printf("*");
}
void Start() {
	system("cls");
	print_border(ROWS, COLS);
	TheSnake** head = (TheSnake**)malloc(sizeof(TheSnake*) * 10);
	int x = ROWS / 2, y = COLS / 2, i, snakesize = MINSIZE;
	char move = RIGHT, temp;
	TheSnake* momentarylocation = newlocation(x, y);
	for (i = 0; i < 10; i++)
		head[i] = NULL;
	move_snake(head, snakesize, momentarylocation);
	TheSnake foodlocation = create_food(&foodlocation, head, snakesize);
	goto_char(foodlocation.x, foodlocation.y, FOOD);
	temp = getKey();
	switch (temp)
	{
	case RIGHT:
	case DOWN:
	case LEFT:
	case UP:
		move = temp;
		break;
	}
	while (move != ESC)
	{
		Sleep(30);
		// reads the chosen key if pressed
		if (_kbhit()) // if any key was hit
			move = getKey();  // change the snakes  direction
		// if not any changes made - snake runs the same direction
		switch (move)
		{
		case RIGHT:
			y++;
			break;
		case DOWN:
			x++;
			break;
		case LEFT:
			y--;
			break;
		case UP:
			x--;
			break;
		}
		// checks for a valid pick
		if (x == 0 || y == 0 || x == ROWS || y == COLS) // snake gets into the side walls
			goto gameover;
		for (i = 0; i < snakesize - 1; i++)
		{
			if (head[i] != NULL && head[i]->x == x && head[i]->y == y)
			{
				goto gameover;
			}
		}
		momentarylocation = newlocation(x, y);
		if (foodlocation.x == momentarylocation->x && foodlocation.y == momentarylocation->y)
		{
			head = SnakeGetsLarger(head, &snakesize, momentarylocation);
			foodlocation = create_food(&foodlocation, head, snakesize);
			goto_char(foodlocation.x, foodlocation.y, FOOD);
		}
		else
		{
			// snake move when snake not eaten
			TheSnake tail = move_snake(head, snakesize, momentarylocation);
			if (tail.x != -1 && tail.y != -1)
			{
				goto_char(tail.x, tail.y, SPACE);
			}
		}
		goto_char(momentarylocation->x, momentarylocation->y, SNAKE);
		gotoxy(26, 81);
		if (snakesize >= MAXSIZE)
		{
			goto gameover_win;
		}
	}
gameover:;
	system("CLS");
	freeArr(head, snakesize);
	printf("############################################\n");
	printf("\t\tGame Over\n");
	printf("Better Luck Next Time !\n");
	printf("############################################\n");
	return;
gameover_win:;
	system("CLS");
	freeArr(head, snakesize);
	printf("############################################\n");
	printf("\t\tYou Won!\n");
	printf("Congratulations !! \n");
	printf("############################################\n");
	return;
}
void print_border(int row_size, int col_size)
{
	int i;
	for (i = 0; i < col_size; i++)
	{
		goto_char(0, i, HASHMARK);
	}
	for (i = 0; i < col_size; i++)
	{
		goto_char(row_size, i, HASHMARK);
	}
	for (i = 0; i < row_size; i++)
	{
		goto_char(i, 0, HASHMARK);
	}
	for (i = 0; i <= row_size; i++)
	{
		goto_char(i, col_size, HASHMARK);
	}
	printf("\n");
}
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}
void goto_char(int x, int y, char c) {
	gotoxy(x, y);
	printf("%c", c);
}
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}
TheSnake move_snake(TheSnake** head, int snake_size, TheSnake* currentlocation)
{
	int i;
	TheSnake point = {
		.x = -1,
		.y = -1
	};
	if (head[snake_size - 1] != NULL)
	{
		point.x = head[snake_size - 1]->x;
		point.y = head[snake_size - 1]->y;
		free(head[snake_size - 1]);
		head[snake_size - 1] = NULL;
	}
	for (i = snake_size - 1; i > 0; i--)
	{
		head[i] = head[i - 1];
	}
	head[0] = currentlocation;
	return point;
}
TheSnake* newlocation(int x, int y)
{
	TheSnake* location = (TheSnake*)malloc(sizeof(TheSnake));
	if (location == NULL)
	{
		printf("There has been an error\n");
		exit(-2);
	}
	location->x = x;
	location->y = y;
	return location;
}
TheSnake create_food(TheSnake* foodlocation, TheSnake** head, int snakesize)
{
	srand((unsigned int)time(NULL));
	int x, y, i;
	bool flag = false;
	do {
		x = 2 + (rand() % (ROWS - 2));
		y = 2 + (rand() % (COLS - 2));
		for (i = 0; i < snakesize; i++)
		{
			TheSnake* point = head[i];
			if (point != NULL && point->x == x && point->y == y)
			{
				flag = true;
			}
		}
	} while (flag);
	TheSnake point = {
		.x = x,
		.y = y
	};
	return point;
}
TheSnake** SnakeGetsLarger(TheSnake** head, int* snakesize, TheSnake* currentlocation)
{
	if (head[(*snakesize) - 1] != NULL && (*snakesize) < MAXSIZE)
	{
		int size = (*snakesize);
		TheSnake** temp = (TheSnake**)realloc(head, (size + 1) * sizeof(TheSnake*));
		if (temp == NULL)
		{
			printf("Memmory failure\n");
			exit(-1);
		}
		temp[size] = NULL;
		head = temp;
		(*snakesize)++;
	}
	move_snake(head, (*snakesize), currentlocation);
	return head;
}
void freeArr(TheSnake** head, int snakesize)
{
	int i;
	if (head != NULL)
	{
		for (i = 0; i < snakesize; i++)
		{
			if (head[i] != NULL)
			{
				head[i] = NULL;
				free(head[i]);
			}
		}
		free(head);
	}
}
