#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "defines.h"

int array[FIELD_H][FIELD_W];	//field array
int pieceCoords[2];				//current piece coordinates
int x, y;						//looping variables
int thisPiece, nextPiece;		//current pieces
double score;					//the players current score
int newpiece=1;					//the newpiece flag
int init_h=1;					/*starting coordinates of piece*/
int init_w=4;					/*these are obsolete variables*/
int h_offset, w_offset;			//piece position offsets from start
int rtTop, rtBot, rtLeft, rtRight;	//rotation thresholds for collision
int b1h, b1w, b2h, b2w, b3h, b3w;	//block draw offsets
int rotation;					/*0=normal view
								  1=90 degrees clockwise
								  2=upside-down
								  3=270 degrees clockwise */

clock_t speed;			//time before piece falls (in milliseconds)
clock_t gravity;		//temp variable for speed

//clock_t fps;
int piececount;			//number of pieces that have fallen
int oldpiececount;
int level;


void GameInit(void);
int GameStart(void);
int GameMain(void);
void newPiece(void);

void scrollbar(void);			//scrolling marquee bar
void drawField(void);			//graphics rendering function
void setPiece(int piece);		//this sets the individual piece offsets
void drawPiece(void);			//this sets the blocks in the array
void initArray(void);			//this initializes the array
void getMoves(char move);		//input function
void rotate(int coll);			//loops from 1 to 4
int collisionCheck(int side);	//checks to see if piece will collide
int loseGame(void);				//checks to see if game has ended
void eraseLastMove(void);		//clears the array of the last move
void setLastMove(void);			//sets the piece in place for the next piece
void lineClear(void);			//clears filled lines and tabulates the score
void predict(int undo);			//predicts the next piece rotation for collision
//void delay(int duration);

int main()
{
restart:
	GameInit();
	//some test blocks
	/*for(x=FIELD_H;x>3;x--)		//quick lose pattern
		array[x][4]=1;*/

	nextPiece=rand()%7;		//generate a piece to use
	drawField();			//draw a blank field. should be a title screen
	printf("                             / Welcome to TETRIS! \\");	//29 spaces and title

	getch();			//wait to start the game

	while(1)	//main game loop
	{
		if(newpiece==1)			//check to see if a new piece is required
			newPiece();

		//if(gravity<=clock())
			gravity=speed+clock();	//get the end time
		while(gravity>clock())		//count to the end time
		{
			if(kbhit())
			{
				getMoves(getch());	//get input
				if(newpiece==1)		//if piece reaches bottom...
					break;			//...skip by redraw to save some cycles
				else
				{
					eraseLastMove();	//clear the last move from the array
					drawPiece();		//set the piece in the array
					drawField();		//refresh the screen
					scrollbar();		//draw the scrollbar
				}
			}
		}
		if(newpiece==0)		/*minor speed optimization*/
			getMoves('s');	//force piece down a line
		eraseLastMove();	//clear the last move from the array
		drawPiece();		//set the piece in the array
		drawField();		//refresh the screen
		scrollbar();		//draw the scrollbar
	}
	return 0;
}

void GameInit()
{
	srand((unsigned)time(NULL));	//seed the random number genreator...
	initArray();					//...and initialize the array
	//reset some variables
	score=0;
	piececount=-1;		//set to -1 because of newpiece loop on start
	oldpiececount=0;
	speed=1000;			//starting speed of piece
	level=1;
}

void newPiece()
{
	piececount++;		//another piece has fallen
	setLastMove();		//set the last move in place
	lineClear();		//clear any full lines

	newpiece=0;			//reset the newpiece flag
	rotation=0;			//reset the piece rotation
	h_offset=0;			//reset the...
	w_offset=0;			//...piece coordinates
	gravity=0;			//reset the gravity

	if(piececount-oldpiececount==10)	//if 10 more pieces have fallen
		if(speed!=200)					//200 is the lowest speed it can go
		{
			oldpiececount=piececount;	//set current number of fallen pieces
			speed-=100;					//make the speed faster
			level++;					//increment the level counter
		}

	thisPiece=nextPiece;//get the current piece
	nextPiece=rand()%7;	//pick the next piece

	setPiece(thisPiece);//get piece data
	if(loseGame())		//did player lose the game?
	{
		drawField();	//refresh the screen
		printf("You lose! Final score: %.0f. Press any key to begin a new game.",score);
		getch();
		newpiece=1;		//reset the newpiece flag /*add more initialization here*/
		//goto restart;	//return to start of the program
		exit(0);		//quit the game for now
	}
	drawPiece();		//set the piece in the array
	drawField();		//refresh the screen
	scrollbar();		//draw the scrollbar
}

void scrollbar()
{
	//printf("================================================================================");

	printf("Score: %.0f\t\t      ",score);	//print the score (6 spaces)
	if(nextPiece==5||nextPiece==6)			//print a space if the piece name is short
		printf(" ");

	printf("Next piece: ");
	if(nextPiece==0)			//print the name of the next piece
		printf("straight");
	else if(nextPiece==1)
		printf("t-shaped");
	else if(nextPiece==2)
		printf("s-shaped");
	else if(nextPiece==3)
		printf("z-shaped");
	else if(nextPiece==4)
		printf("l-shaped");
	else if(nextPiece==5)
		printf("back-l");
	else if(nextPiece==6)
		printf("square");

	printf("\t\t\tLevel %i",level);	//print the level
}

void drawField()
{
	system("cls");		//clear the screen

	for(x=0;x<FIELD_H;x++)
	{
		printf("                              |");	//thirty spaces

		for(y=0;y<FIELD_W;y++)
			if(array[x][y]==1||array[x][y]==2)
				printf("[]");
			else
				printf("  ");

		printf("|                              ");	//thirty spaces
	}
	
	//bottom message bar
	//scrollbar();
}

void drawPiece()
{
	/* This sets the initial location of the piece with
	   movement offset. Coordinates are based on the
	   center of the piece. */
	pieceCoords[0]=init_h+h_offset;	//set height position
	pieceCoords[1]=init_w+w_offset;	//set width position

	//mark the coordinates to draw the piece
	array[(pieceCoords[0])][(pieceCoords[1])]=2;		//center piece
	array[(pieceCoords[0]+b1h)][(pieceCoords[1]+b1w)]=2;	//other...
	array[(pieceCoords[0]+b2h)][(pieceCoords[1]+b2w)]=2;	//...three...
	array[(pieceCoords[0]+b3h)][(pieceCoords[1]+b3w)]=2;	//...pieces
}

void initArray()
{
	for(x=0;x<FIELD_H;x++)
		for(y=0;y<FIELD_W;y++)
			array[x][y]=0;
		//array equals: 0 for empty
		//				1 for full
		//				2 for moving piece
}

void getMoves(char move)
{
	switch(move)
	{
		case 'a':
		case 'A':
			if(collisionCheck(1)==0)	//if the piece doesnt hit something...
				w_offset--;				//...move the piece
			break;
		case 'd':
		case 'D':
			if(collisionCheck(2)==0)
				w_offset++;
			break;
		case 'w':
		case 'W':
			if(collisionCheck(0)==0)
			{
				rotate(0);				//rotate the piece and...
				setPiece(thisPiece);	//...get new piece data
			}
			break;
		case 's':
		case 'S':
			if(collisionCheck(3)==0)
				h_offset++;
			else						//if the piece hits the bottom...
				newpiece=1;				//...set the newpiece flag
			break;
		default:
			break;
	}
}

void rotate(int coll)
{
	if(coll==0)		//rotate clockwise
	{
		if(rotation==3)
			rotation=0;
		else
			rotation++;
	}

	if(coll==1)		//rotate counterclockwise
	{
		if(rotation==0)
			rotation=3;
		else
			rotation--;
	}
}

int collisionCheck(int side)
{
	if(side==0)	//check rotation collision
	{
		predict(0);

		/* For the first part, we check to see if the
		   rotation will collide with the sides. */
		for(x=0;x<rtTop;x++)	//check top side collision
		{
			if(pieceCoords[0]-x==0)	//if piece minus top offset hits the top
			{
				predict(1);
				return 1;	//collision detected!
			}
		}

		for(x=0;x<rtLeft;x++)	//check left side collision
		{
			if(pieceCoords[1]-x==0)
			{
				predict(1);
				return 1;
			}
		}

		for(x=0;x<rtRight;x++)	//check right side collision
		{
			if(pieceCoords[1]+x==(FIELD_W-1))
			{
				predict(1);
				return 1;
			}
		}

		for(x=0;x<rtBot;x++)	//check bottom side collision
		{
			if(pieceCoords[0]+x==(FIELD_H-1))
			{
				predict(1);
				return 1;
			}
		}

		/* Then we check to see if any of the
		   blocks will hit other blocks on the field. */
		if(array[(pieceCoords[0]+b1h)][(pieceCoords[1]+b1w)]==1
	||array[(pieceCoords[0]+b2h)][(pieceCoords[1]+b2w)]==1
	||array[(pieceCoords[0]+b3h)][(pieceCoords[1]+b3w)]==1)
		{
			predict(1);
			return 1;
		}

		predict(1);
		return 0;
	}

	/* Here, we scan the array and check to see whether
	   any of the blocks in the current piece are
	   adjacent to the field sides or another block. */
	for(x=0;x<FIELD_H;x++)			//scan the array
		for(y=0;y<FIELD_W;y++)
			if(array[x][y]==2)		//if array location contains a block
			{
				if(side==1)	//check left side
				{
					if(y==0||array[x][y-1]==1)	//if piece is next to border or block...
						return 1;	//collision detected!
				}

				else if(side==2)	//check right side
				{
					if(y==FIELD_W-1||array[x][y+1]==1)
						return 1;
				}

				else if(side==3)	//check bottom
				{
					if(x==FIELD_H-1||array[x+1][y]==1)
						return 1;
				}
			}

	return 0;
}

int loseGame()		/*need to modify the conditions of losing*/
{
	if(array[init_h][init_w]==1
||array[(init_h+b1h)][(init_w+b1w)]==1
||array[(init_h+b2h)][(init_w+b2w)]==1
||array[(init_h+b3h)][(init_w+b3w)]==1)
		return 1;

	else
		return 0;
}

void eraseLastMove()	/*need to optimize this*/
{
	for(x=0;x<FIELD_H;x++)		//scan the array
		for(y=0;y<FIELD_W;y++)
			if(array[x][y]==2)		//if the spot is filled...
				array[x][y]=0;		//...clear the block
}

void setLastMove()		/*also need to optimize this*/
{
	for(x=0;x<FIELD_H;x++)		//scan the array
		for(y=0;y<FIELD_W;y++)
			if(array[x][y]==2)		//if the spot is filled...
				array[x][y]=1;		//...set the block
}

void lineClear()
{
	int x2, y2;	//new looping variables
	int empty;	//is the row empty?
	int lines=0;//number of lines cleared

	for(x=0;x<FIELD_H;x++)		//scan the rows
	{
		empty=0;				//reset the empty flag on each row

		for(y=0;y<FIELD_W;y++)	//scan the blocks in each row
			if(array[x][y]==0)	//if the row isn't full...
			{
				empty=1;		//...set the empty flag and...
				break;			//...scan the next row
			}

		if(!empty)
		{
			for(y=0;y<FIELD_W;y++)		//rescan the filled row...
				array[x][y]=0;			//...and clear the blocks

			for(x2=x-1;x2>=0;x2--)		//scan rows from the bottom up starting above cleared row
				for(y2=0;y2<FIELD_W;y2++)	//scan each block in the row
					if(array[x2][y2]==1)
					{
						array[x2][y2]=0;	//clear the block
						array[x2+1][y2]=1;	//fill in the block below it
					}

			lines++;			//a line has been cleared
		}
	}

	//tabulate the scores
	if(lines==1)
		score+=10;
	else if(lines==2)
		score+=25;
	else if(lines==3)
		score+=50;
	else if(lines==4)	/* TETRIS! */
		score+=100;
}

void predict(int undo)
{
	if(undo==0)
	{
		rotate(0);				//get next rotation...
		setPiece(thisPiece);	//...and next piece data
	}

	if(undo==1)
	{
		rotate(1);				//un-rotate
		setPiece(thisPiece);	//reset the piece
	}
}

/*void delay(int duration)
{
	clock_t done;

	done = duration + clock();
	while(done>clock())
		;	//sit and wait
}*/
