#include "defines.h"

extern int rtTop, rtBot, rtLeft, rtRight;	//rotation thresholds for collision
extern int b1h, b1w, b2h, b2w, b3h, b3w;	//block draw offsets
extern int rotation;					/*0=normal view
										  1=90 degrees clockwise
										  2=upside-down
										  3=270 degrees clockwise */

void setPiece(int piece)
{
	if(piece==0)
	{
		switch(rotation)
		{
			case 0:		//piece looks identical rotated upside-down
			case 2:
				/* The rotation thresholds dictate how far away the piece
				   needs to be from the field edge for the next rotation. */
				rtTop=1;
				rtBot=2;
				rtLeft=0;
				rtRight=0;

				/* These are the coordinates of the
				   blocks relative to the center block. */
				b1h=-1;
				b1w=0;
				b2h=1;
				b2w=0;
				b3h=2;
				b3w=0;
				break;
			case 1:		//piece looks identical side to side
			case 3:
				rtTop=0;
				rtBot=0;
				rtLeft=1;
				rtRight=2;

				b1h=0;
				b1w=-1;
				b2h=0;
				b2w=1;
				b3h=0;
				b3w=2;
				break;
		}
	}
		//draw blue straight piece
	else if(piece==1)
	{
		switch(rotation)
		{
			case 0:
				rtTop=1;
				rtBot=0;
				rtLeft=1;
				rtRight=1;

				b1h=0;
				b1w=-1;
				b2h=-1;
				b2w=0;
				b3h=0;
				b3w=1;
				break;
			case 1:
				rtTop=1;
				rtBot=1;
				rtLeft=0;
				rtRight=1;

				b1h=-1;
				b1w=0;
				b2h=0;
				b2w=1;
				b3h=1;
				b3w=0;
				break;
			case 2:
				rtTop=0;
				rtBot=1;
				rtLeft=1;
				rtRight=1;

				b1h=0;
				b1w=-1;
				b2h=1;
				b2w=0;
				b3h=0;
				b3w=1;
				break;
			case 3:
				rtTop=1;
				rtBot=1;
				rtLeft=1;
				rtRight=0;

				b1h=-1;
				b1w=0;
				b2h=0;
				b2w=-1;
				b3h=1;
				b3w=0;
				break;
		}
	}
		//draw t-shaped piece
	else if(piece==2)
	{
		switch(rotation)
		{
			case 0:
			case 2:
				rtTop=1;
				rtBot=0;
				rtLeft=1;
				rtRight=1;

				b1h=0;
				b1w=-1;
				b2h=-1;
				b2w=0;
				b3h=-1;
				b3w=1;
				break;
			case 1:
			case 3:
				rtTop=1;
				rtBot=1;
				rtLeft=1;
				rtRight=0;

				b1h=-1;
				b1w=-1;
				b2h=0;
				b2w=-1;
				b3h=1;
				b3w=0;
				break;
		}
	}
		//draw s-piece
	else if(piece==3)
	{
		switch(rotation)
		{
			case 0:
			case 2:
				rtTop=1;
				rtBot=0;
				rtLeft=1;
				rtRight=1;

				b1h=-1;
				b1w=-1;
				b2h=-1;
				b2w=0;
				b3h=0;
				b3w=1;
				break;
			case 1:
			case 3:
				rtTop=1;
				rtBot=1;
				rtLeft=0;
				rtRight=1;

				b1h=-1;
				b1w=1;
				b2h=0;
				b2w=1;
				b3h=1;
				b3w=0;
				break;
		}
	}
		//draw z-piece
	else if(piece==4)
	{
		switch(rotation)
		{
			case 0:
				rtTop=1;
				rtBot=1;
				rtLeft=0;
				rtRight=1;

				b1h=-1;
				b1w=0;
				b2h=1;
				b2w=0;
				b3h=1;
				b3w=1;
				break;
			case 1:
				rtTop=0;
				rtBot=1;
				rtLeft=1;
				rtRight=1;

				b1h=0;
				b1w=-1;
				b2h=0;
				b2w=1;
				b3h=1;
				b3w=-1;
				break;
			case 2:
				rtTop=1;
				rtBot=1;
				rtLeft=1;
				rtRight=0;

				b1h=-1;
				b1w=-1;
				b2h=-1;
				b2w=0;
				b3h=1;
				b3w=0;
				break;
			case 3:
				rtTop=1;
				rtBot=0;
				rtLeft=1;
				rtRight=1;

				b1h=-1;
				b1w=1;
				b2h=0;
				b2w=1;
				b3h=0;
				b3w=-1;
				break;
		}
	}
		//draw L-piece
	else if(piece==5)
	{
		switch(rotation)
		{
			case 0:
				rtTop=1;
				rtBot=1;
				rtLeft=1;
				rtRight=0;

				b1h=-1;
				b1w=0;
				b2h=1;
				b2w=0;
				b3h=1;
				b3w=-1;
				break;
			case 1:
				rtTop=1;
				rtBot=0;
				rtLeft=1;
				rtRight=1;

				b1h=-1;
				b1w=-1;
				b2h=0;
				b2w=-1;
				b3h=0;
				b3w=1;
				break;
			case 2:
				rtTop=1;
				rtBot=1;
				rtLeft=0;
				rtRight=1;

				b1h=-1;
				b1w=0;
				b2h=-1;
				b2w=1;
				b3h=1;
				b3w=0;
				break;
			case 3:
				rtTop=0;
				rtBot=1;
				rtLeft=1;
				rtRight=1;

				b1h=0;
				b1w=-1;
				b2h=0;
				b2w=1;
				b3h=1;
				b3w=1;
				break;
		}
	}
		//draw backwards L-piece
	else if(piece==6)
	{
		switch(rotation)
		{
			case 0:		//a cube looks identical when rotated 90 degrees
			case 1:
			case 2:
			case 3:
				rtTop=0;	//piece doesn't rotate so no thresholds needed
				rtBot=0;
				rtLeft=0;
				rtRight=0;

				b1h=-1;
				b1w=0;
				b2h=-1;
				b2w=1;
				b3h=0;
				b3w=1;
				break;
		}
	}
		//draw square piece
}
