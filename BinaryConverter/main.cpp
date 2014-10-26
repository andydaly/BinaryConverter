/* Andrew Daly, C11710699
Binary Converter
DT211/1 Programming Assignment */

// Header files
#include <allegro.h>
#include <math.h>
#include <time.h>

#define GATESPEED 1.7f
// High Speed Computer: 0.3f
#define BULLETSPEED 3.9f
// High Speed Computer: 0.6f
#define ENEMYY 0.1f
// High Speed Computer: 0.01f
#define ENEMYX0 0.9f
// High Speed Computer: 0.1f
#define ENEMYX1 1.3f
// High Speed Computer: 0.2f
#define ENEMYX2 1.7f
// High Speed Computer: 0.3f
#define ENEMYX3 2.1f
// High Speed Computer: 0.4f

// Defining, make writing code easier,
// This was used for defining Colours in the program,
// Also if changing colour, all that has to be done is to change code here
#define BACKGROUND makecol(130,130,130)
#define TEXTCOLOUR makecol(10,120,10)
#define SCREENCOLOUR makecol(35,35,35)
#define BUTTONCOL makecol(255,0,0)
#define BUTTONPRESS makecol(100,11,11)

// Declaring Global Variables
BITMAP * backBuffer;
FONT *outputscreen;
FONT *button;
FONT *signs;
volatile bool done = false;
bool stat = true;
char * fullbits = "Fill";
bool fill = true;
long long int decimal = 0;
int rights = 36;
int lefts = 35;

// Game 1 variables
char * activegame1 = "Game1";
bool Game1 = false;
long long int randomnum;
long long int answer;
char * choice = "";

// Game 2 variables
char * activegame2 = "Game2";
bool Game2 = false;
int GuessCounter = 0;

// Calculator variables
char * Calc = "+/-/x";
bool Calculator = false;
bool Equations = false;
bool First = false;
bool Second = false;
bool Equals = false;
char * equat;
int fnum1;
int fnum2;
float Result;

// Extreme binary variables
bool BinaryGame = false;
int iCounter = -1;
int pressed = 1;
int shoot = 1;
int activebullets = 0;
int playerhp;
int enemyskilled = 0;
int activeenemies;
int enemynum;
bool spawn = false;
bool gameover = false;
bool wave[4];
int round = 0;
bool next = false;
long long int results[4];
int startenemies;
// Struct used for bullets
typedef struct BULLETPOS
{
	float bx;
	float by;
	bool be;
};
BULLETPOS bullet[100];
// Struct used for enemies
typedef struct ENEMYSTAT
{
	bool direction;
	float ex;
	float ey;
	bool enemyalive;
	bool enemybit;
	int enemyhp;
};
ENEMYSTAT enemy[32];


// Handler for exiting program using top right close button
void Handler() 
{
	done = true;
}

// Function for basic binary conversion
long long int toDecimal(bool * bits, int size)
{
	int power = 0;
	long long int number = 0;
	for (int i = 0; i < size; i++)
	{
		if (bits[i] == true)
		{
			number += pow(2.0, power);
		}
		power += 1;
	}
	return number;
}

// Function for 2's complement binary conversion
long long int complement2(bool * bits, int size)
{
	int power = 0;
	long long int number = 0;
	for (int i = 0; i < size-1; i++)
	{
		if (bits[i] == true)
		{
			number += pow(2.0, power);
		}
		power += 1;
	}
	if (bits[size-1] == true)
	{
		number -= pow(2.0, size-1);
	}
	return number;
}

// Code for splitting up the bits, into 8 bits separatly
void Split(bool * bits, int * set1, int * set2, int * set3, int * set4, int size)
{
	int power = 0;
	int i;
	if (size >= 8)
	{
		for (i = 0; i < 8; i++)
		{
			if (bits[i] == true)
			{
				* set1 += pow(2.0, power);
			}
			power += 1;
		}
		power = 0;
	}

	if (size >= 16)
	{
		for (i; i < 16; i++)
		{
			if (bits[i] == true)
			{
				* set2 += pow(2.0, power);
			}
			power += 1;
		}
		power = 0;
	}

	if (size >= 24)
	{
		for (i; i < 24; i++)
		{
			if (bits[i] == true)
			{
				* set3 += pow(2.0, power);
			}
			power += 1;
		}
		power = 0;
	}

	if (size >= 32)
	{
		for (i; i < 32; i++)
		{
			if (bits[i] == true)
			{
				* set4 += pow(2.0, power);
			}
			power += 1;
		}
		power = 0;
	}
}

// Code used for the buttons and their functions in the program
void Buttons(bool *bits, int * size, bool * bit8, bool * bit16, bool * bit24, bool * bit32, float * xstatic)
{
	int x1 = 565;
	int x2 = x1+60;
	int y1 = 75;
	int y2 = y1+40;

	// 8 bit button
	// Changes bits to 8 bit
	// For Extreme binary, changes enemy number (Difficulty Easy)
	rectfill(backBuffer, x1, y1, x2, y2, BUTTONCOL);
	rect(backBuffer, x1, y1, x2, y2, makecol(0, 0, 0));
	rect(backBuffer, x1, y1, x2-1, y2-1, makecol(0, 0, 0));
	rect(backBuffer, x1, y1, x2-2, y2-2, makecol(0, 0, 0));
	textprintf_ex(backBuffer, button, x1+5, y1+5, makecol(10, 10, 10), BUTTONCOL, "8 bit");
	if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
	{
		if (mouse_b & 1)
		{
			pressed = 0;
			rectfill(backBuffer, x1, y1, x2, y2, BUTTONPRESS);
			rect(backBuffer, x1, y1, x2, y2, makecol(0, 0, 0));
			rect(backBuffer, x1+1, y1+1, x2, y2, makecol(0, 0, 0));
			textprintf_ex(backBuffer, button, x1+6, y1+6, makecol(10, 10, 10), BUTTONPRESS, "8 bit");
			* bit8 = true;
			* bit16 = false;
			* bit24 = false;
			* bit32 = false;
			* size = 8;
			enemyskilled = 0;
			activeenemies = * size;
			startenemies = 2;
			enemynum = startenemies;
			playerhp = 100;
			gameover = false;
			for (int i = 0; i < 32; i++)
			{
				enemy[i].enemyhp = 100;
			}
			for (int i = 0; i < 32; i++)
			{
				enemy[i].enemybit = rand()%2;
			}		
			for (int i = 0; i < enemynum; i++)
			{
				enemy[i].enemyalive = true;
			}
			for (int i = 0; i < 4; i++)
			{
				results[i] = 0;
			}
			for (int i = 0; i < 4; i++)
			{
				wave[i] = false;
			}
			round = 0;
			wave[round] = true;
			rest(150);
		}
		else
		{
			pressed = 1;
		}
	}

	// 16 bit button
	// Changes bits to 16 bit
	// For Extreme binary, changes enemy number (Difficulty Medium)
	rectfill(backBuffer, x1, y1+50, x2, y2+50, BUTTONCOL);
	rect(backBuffer, x1, y1+50, x2, y2+50, makecol(0, 0, 0));
	rect(backBuffer, x1, y1+50, x2-1, y2+50-1, makecol(0, 0, 0));
	rect(backBuffer, x1, y1+50, x2-2, y2+50-2, makecol(0, 0, 0));
	textprintf_ex(backBuffer, button, x1+5, y1+50+5, makecol(10, 10, 10), BUTTONCOL, "16 bit");
	if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1+50) && (mouse_y < y2+50))
	{
		if (mouse_b & 1)
		{
			pressed = 0;
			rectfill(backBuffer, x1, y1+50, x2, y2+50, BUTTONPRESS);
			rect(backBuffer, x1, y1+50, x2, y2+50, makecol(0, 0, 0));
			rect(backBuffer, x1+1, y1+50+1, x2, y2+50, makecol(0, 0, 0));
			textprintf_ex(backBuffer, button, x1+6, y1+50+6, makecol(10, 10, 10), BUTTONPRESS, "16 bit");
			* bit8 = false;
			* bit16 = true;
			* bit24 = false;
			* bit32 = false;
			* size = 16;
			enemyskilled = 0;
			activeenemies = * size;
			startenemies = 4;
			enemynum = startenemies;
			playerhp = 100;
			gameover = false;
			for (int i = 0; i < 32; i++)
			{
				enemy[i].enemyhp = 100;
			}
			for (int i = 0; i < 32; i++)
			{
				enemy[i].enemybit = rand()%2;
			}		
			for (int i = 0; i < enemynum; i++)
			{
				enemy[i].enemyalive = true;
			}
			for (int i = 0; i < 4; i++)
			{
				results[i] = 0;
			}
			for (int i = 0; i < 4; i++)
			{
				wave[i] = false;
			}
			round = 0;
			wave[round] = true;
			rest(150);
		}
		else
		{
			pressed = 1;
		}
	}

	// 24 bit button
	// Changes bits to 24 bit
	// For Extreme binary, changes enemy number (Difficulty Hard)
	rectfill(backBuffer, x1, y1+100, x2, y2+100, BUTTONCOL);
	rect(backBuffer, x1, y1+100, x2, y2+100, makecol(0, 0, 0));
	rect(backBuffer, x1, y1+100, x2-1, y2+100-1, makecol(0, 0, 0));
	rect(backBuffer, x1, y1+100, x2-2, y2+100-2, makecol(0, 0, 0));
	textprintf_ex(backBuffer, button, x1+5, y1+100+5, makecol(10, 10, 10), BUTTONCOL, "24 bit");
	if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1+100) && (mouse_y < y2+100))
	{
		if (mouse_b & 1)
		{
			pressed = 0;
			rectfill(backBuffer, x1, y1+100, x2, y2+100, BUTTONPRESS);
			rect(backBuffer, x1, y1+100, x2, y2+100, makecol(0, 0, 0));
			rect(backBuffer, x1+1, y1+100+1, x2, y2+100, makecol(0, 0, 0));
			textprintf_ex(backBuffer, button, x1+6, y1+100+6, makecol(10, 10, 10), BUTTONPRESS, "24 bit");
			* bit8 = false;
			* bit16 = false;
			* bit24 = true;
			* bit32 = false;
			* size = 24;
			enemyskilled = 0;
			activeenemies = * size;
			startenemies = 6;
			enemynum = startenemies;
			playerhp = 100;
			gameover = false;
			for (int i = 0; i < 32; i++)
			{
				enemy[i].enemyhp = 100;
			}
			for (int i = 0; i < 32; i++)
			{
				enemy[i].enemybit = rand()%2;
			}		
			for (int i = 0; i < enemynum; i++)
			{
				enemy[i].enemyalive = true;
			}
			for (int i = 0; i < 4; i++)
			{
				results[i] = 0;
			}
			for (int i = 0; i < 4; i++)
			{
				wave[i] = false;
			}
			round = 0;
			wave[round] = true;
			rest(150);

		}
		else
		{
			pressed = 1;
		}
	}

	// 32 bit button
	// Changes bits to 32 bit
	// For Extreme binary, changes enemy number (Difficulty Very Hard)
	rectfill(backBuffer, x1, y1+150, x2, y2+150, BUTTONCOL);
	rect(backBuffer, x1, y1+150, x2, y2+150, makecol(0, 0, 0));
	rect(backBuffer, x1, y1+150, x2-1, y2+150-1, makecol(0, 0, 0));
	rect(backBuffer, x1, y1+150, x2-2, y2+150-2, makecol(0, 0, 0));
	textprintf_ex(backBuffer, button, x1+5, y1+150+5, makecol(10, 10, 10), BUTTONCOL, "32 bit");
	if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1+150) && (mouse_y < y2+150))
	{
		if (mouse_b & 1)
		{	
			pressed = 0;
			rectfill(backBuffer, x1, y1+150, x2, y2+150, BUTTONPRESS);
			rect(backBuffer, x1, y1+150, x2, y2+150, makecol(0, 0, 0));
			rect(backBuffer, x1+1, y1+150+1, x2, y2+150, makecol(0, 0, 0));
			textprintf_ex(backBuffer, button, x1+6, y1+150+6, makecol(10, 10, 10), BUTTONPRESS, "32 bit");
			* bit8 = false;
			* bit16 = false;
			* bit24 = false;
			* bit32 = true;
			* size = 32;
			enemyskilled = 0;
			activeenemies = * size;
			startenemies = 8;
			enemynum = startenemies;
			playerhp = 100;
			gameover = false;
			for (int i = 0; i < 32; i++)
			{
				enemy[i].enemyhp = 100;
			}
			for (int i = 0; i < 32; i++)
			{
				enemy[i].enemybit = rand()%2;
			}		
			for (int i = 0; i < enemynum; i++)
			{
				enemy[i].enemyalive = true;
			}
			for (int i = 0; i < 4; i++)
			{
				results[i] = 0;
			}
			for (int i = 0; i < 4; i++)
			{
				wave[i] = false;
			}
			round = 0;
			wave[round] = true;
			rest(150);
		}
		else
		{
			pressed = 1;
		}
	}
	
	// Commence Game 1, Guess that byte
	// Clear
	rectfill(backBuffer, x1-70, y1, x2-70, y2, BUTTONCOL);
	rect(backBuffer, x1-70, y1, x2-70, y2, makecol(0, 0, 0));
	rect(backBuffer, x1-70, y1, x2-1-70, y2-1, makecol(0, 0, 0));
	rect(backBuffer, x1-70, y1, x2-2-70, y2-2, makecol(0, 0, 0));
	textprintf_ex(backBuffer, button, x1+3-70, y1+5, makecol(10, 10, 10), BUTTONCOL, "%s", activegame1);
	if ((mouse_x > x1-70) && (mouse_x < x2-70) && (mouse_y > y1) && (mouse_y < y2))
	{
		if (mouse_b & 1)
		{
			rectfill(backBuffer, x1-70, y1, x2-70, y2, BUTTONPRESS);
			rect(backBuffer, x1-70, y1, x2-70, y2, makecol(0, 0, 0));
			rect(backBuffer, x1+1-70, y1+1, x2-70, y2, makecol(0, 0, 0));
			textprintf_ex(backBuffer, button, x1+4-70, y1+6, makecol(10, 10, 10), BUTTONPRESS, "%s", activegame1);
			BinaryGame = false;
			* xstatic = 35;
			lefts = 35;
			rights = 36;
			if (Game1 == true)
			{
				// Restarts Game 1
				for (int i = 0; i < 32; i++)
				{
					bits[i] = false;
				}
				choice = "";
				if (* bit8 == true)
				{
					randomnum = rand()%255+1;
					rest(150);
				}
				if (* bit16 == true)
				{
					randomnum = rand()%65535+1;
					rest(150);
				}
				if (* bit24 == true)
				{
					randomnum = rand()%1677215+1;
					rest(150);
				}
				if (* bit32 == true)
				{
					randomnum = rand()%4294967295+1;
					rest(150);
				}
			}
			else if (Game2 == true)
			{
				for (int i = 0; i < 32; i++)
				{
					bits[i] = false;
				}
				choice = "";
				GuessCounter = 0;
				if (* bit8 == true)
				{
					randomnum = rand()%256+1;
					rest(150);
				}
				if (* bit16 == true)
				{
					randomnum = rand()%65535+1;
					rest(150);
				}
				if (* bit24 == true)
				{
					randomnum = rand()%1677215+1;
					rest(150);
				}
				if (* bit32 == true)
				{
					randomnum = rand()%4294967295+1;
					rest(150);
				}
			}
			else if (Calculator == true)
			{
				// Clears current equation
				First = true;
				Second = false;
				Equals = false;
				equat = "";
				Equations = false;
				fnum1 = 0;
				fnum2 = 0;
				Result = 0.0;
				activegame2 = "Enter";
				for (int i = 0; i < 32; i++)
				{
					bits[i] = false;
				}
				rest(150);
			}
			else
			{
				// Begins Game 1
				Game1 = true;
				activegame1 = "Restart";
				activegame2 = "Submit";
				Calc = "Exit";
				if (* bit8 == true)
				{
					randomnum = rand()%256+1;
					rest(150);
				}
				if (* bit16 == true)
				{
					randomnum = rand()%65535+1;
					rest(150);
				}
				if (* bit24 == true)
				{
					randomnum = rand()%1677215+1;
					rest(150);
				}
				if (* bit32 == true)
				{
					randomnum = rand()%4294967295+1;
					rest(150);
				}
			}
		}
	}

	// Game 2, Higher or Lower
	rectfill(backBuffer, x1-70, y1+50, x2-70, y2+50, BUTTONCOL);
	rect(backBuffer, x1-70, y1+50, x2-70, y2+50, makecol(0, 0, 0));
	rect(backBuffer, x1-70, y1+50, x2-1-70, y2-1+50, makecol(0, 0, 0));
	rect(backBuffer, x1-70, y1+50, x2-2-70, y2-2+50, makecol(0, 0, 0));
	textprintf_ex(backBuffer, button, x1+3-70, y1+5+50, makecol(10, 10, 10), BUTTONCOL, "%s", activegame2);
	if ((mouse_x > x1-70) && (mouse_x < x2-70) && (mouse_y > y1+50) && (mouse_y < y2+50))
	{
		if (mouse_b & 1)
		{
			rectfill(backBuffer, x1-70, y1+50, x2-70, y2+50, BUTTONPRESS);
			rect(backBuffer, x1-70, y1+50, x2-70, y2+50, makecol(0, 0, 0));
			rect(backBuffer, x1+1-70, y1+1+50, x2-70, y2+50, makecol(0, 0, 0));
			textprintf_ex(backBuffer, button, x1+4-70, y1+6+50, makecol(10, 10, 10), BUTTONPRESS, "%s", activegame2);
			BinaryGame = false;
			* xstatic = 35;
			lefts = 35;
			rights = 36;
			if (Game1 == true)
			{
				// Submits Game 1 answer
				answer = toDecimal(bits, *size);
				if (answer == randomnum)
				{
					choice = "Correct!";
					rest(150);
				}
				else
				{
					choice = "Incorrect!";
					rest(150);
				}
			}
			else if (Game2 == true)
			{
				answer = toDecimal(bits, *size);
				
				if (answer == randomnum)
				{
					choice = "Correct!";
					GuessCounter++;
					rest(150);
				}	
				else if (answer > randomnum)
				{
					choice = "Too High!";
					GuessCounter++;
					rest(150);
				}
				else if (answer < randomnum)
				{
					choice = "Too Low!";
					GuessCounter++;
					rest(150);
				}
			}
			else if (Calculator == true)
			{
				// Begins Equations Choice
				if (First == true)
				{
					First = false;
					Second = true;
					for (int i = 0; i < 32; i++)
					{
						bits[i] = false;
					}
					rest(150);
				}
				else if (Second == true)
				{
					Equations = true;
					Second = false;
					rest(150);
				}
			}
			else
			{
				Game2 = true;
				activegame1 = "Restart";
				activegame2 = "Submit";
				Calc = "Exit";
				choice = "";
				GuessCounter = 0;
				if (* bit8 == true)
				{
					randomnum = rand()%256+1;
					rest(150);
				}
				if (* bit16 == true)
				{
					randomnum = rand()%65535+1;
					rest(150);
				}
				if (* bit24 == true)
				{
					randomnum = rand()%1677215+1;
					rest(150);
				}
				if (* bit32 == true)
				{
					randomnum = rand()%4294967295+1;
					rest(150);
				}
			}
		}
	}

	// Calculator
	rectfill(backBuffer, x1-70, y1+100, x2-70, y2+100, BUTTONCOL);
	rect(backBuffer, x1-70, y1+100, x2-70, y2+100, makecol(0, 0, 0));
	rect(backBuffer, x1-70, y1+100, x2-1-70, y2-1+100, makecol(0, 0, 0));
	rect(backBuffer, x1-70, y1+100, x2-2-70, y2-2+100, makecol(0, 0, 0));
	textprintf_ex(backBuffer, button, x1+4-70, y1+5+100, makecol(10, 10, 10), BUTTONCOL, "%s", Calc);
	if ((mouse_x > x1-70) && (mouse_x < x2-70) && (mouse_y > y1+100) && (mouse_y < y2+100))
	{
		if (mouse_b & 1)
		{
			rectfill(backBuffer, x1-70, y1+100, x2-70, y2+100, BUTTONPRESS);
			rect(backBuffer, x1-70, y1+100, x2-70, y2+100, makecol(0, 0, 0));
			rect(backBuffer, x1+1-70, y1+1+100, x2-70, y2+100, makecol(0, 0, 0));
			textprintf_ex(backBuffer, button, x1+5-70, y1+6+100, makecol(10, 10, 10), BUTTONPRESS, "%s", Calc);
			BinaryGame = false;
			* xstatic = 35;
			lefts = 35;
			rights = 36;
			if (Game1 == true)
			{
				// Exits Game 1
				for (int i = 0; i < 32; i++)
				{
					bits[i] = false;
				}
				Game1 = false;
				choice = "";
				activegame1 = "Game1";
				activegame2 = "Game2";
				Calc = "+/-/x";
				rest(150);
			}
			else if (Game2 == true)
			{
				for (int i = 0; i < 32; i++)
				{
					bits[i] = false;
				}
				Game2 = false;
				choice = "";
				activegame1 = "Game1";
				activegame2 = "Game2";
				Calc = "+/-/x";
				rest(150);
			}
			else if (Calculator == true)
			{
				// Exits Calculator
				Calculator = false;
				First = false;
				Second = false;
				Equals = false;
				equat = "";
				Equations = false;
				fnum1 = 0;
				fnum2 = 0;
				Result = 0.0;
				choice = "";
				activegame1 = "Game1";
				activegame2 = "Game2";
				Calc = "+/-/x";
				rest(150);
			}
			else
			{
				//Begins Calculator
				Calculator = true;
				First = true;
				activegame1 = "Clear";
				activegame2 = "Enter";
				Calc = "Exit";
				rest(150);
			}
		}
	}

	// fill button
	rectfill(backBuffer, x1-70, y1+150, x2-70, y2+150, BUTTONCOL);
	rect(backBuffer, x1-70, y1+150, x2-70, y2+150, makecol(0, 0, 0));
	rect(backBuffer, x1-70, y1+150, x2-1-70, y2+150-1, makecol(0, 0, 0));
	rect(backBuffer, x1-70, y1+150, x2-2-70, y2+150-2, makecol(0, 0, 0));
	textprintf_ex(backBuffer, button, x1+5-70, y1+150+5, makecol(10, 10, 10), BUTTONCOL, "%s", fullbits);
	if ((mouse_x > x1-70) && (mouse_x < x2-70) && (mouse_y > y1+150) && (mouse_y < y2+150))
	{
		if (mouse_b & 1)
		{
			rectfill(backBuffer, x1-70, y1+150, x2-70, y2+150, BUTTONPRESS);
			rect(backBuffer, x1-70, y1+150, x2-70, y2+150, makecol(0, 0, 0));
			rect(backBuffer, x1+1-70, y1+150+1, x2-70, y2+150, makecol(0, 0, 0));
			textprintf_ex(backBuffer, button, x1+6-70, y1+150+6, makecol(10, 10, 10), BUTTONPRESS, "%s", fullbits);
			BinaryGame = false;
			* xstatic = 35;
			lefts = 35;
			rights = 36;
			if (fill == true)
			{
				for (int i = 0; i < 32; i++)
				{
					bits[i] = true;
				}
				fill = false;
				fullbits = "Empty";
				rest(150);
			}
			else if (fill == false)
			{
				for (int i = 0; i < 32; i++)
				{
					bits[i] = false;
				}
				fill = true;
				fullbits = "Fill";
				rest(150);
			}
		}
	}

	// Extreme Binary Game
	rectfill(backBuffer, x1-40, y1+200, x2-20, y2+200, BUTTONCOL);
	rect(backBuffer, x1-40, y1+200, x2-20, y2+200, makecol(0, 0, 0));
	rect(backBuffer, x1-40, y1+200, x2-1-20, y2+200-1, makecol(0, 0, 0));
	rect(backBuffer, x1-40, y1+200, x2-2-20, y2+200-2, makecol(0, 0, 0));
	textprintf_ex(backBuffer, button, x1+5-40, y1+200+5, makecol(10, 10, 10), BUTTONCOL, "Extreme");
	if ((mouse_x > x1-40) && (mouse_x < x2-20) && (mouse_y > y1+200) && (mouse_y < y2+200))
	{
		if (mouse_b & 1)
		{
			pressed = 0;
			rectfill(backBuffer, x1-40, y1+200, x2-20, y2+200, BUTTONPRESS);
			rect(backBuffer, x1-40, y1+200, x2-20, y2+200, makecol(0, 0, 0));
			rect(backBuffer, x1+1-40, y1+200+1, x2-20, y2+200, makecol(0, 0, 0));
			textprintf_ex(backBuffer, button, x1+6-40, y1+200+6, makecol(10, 10, 10), BUTTONPRESS, "Extreme");
			if (BinaryGame == false)
			{
				Game1 = false;
				Game2 = false;
				Calculator = false;
				Equations = false;
				First = false;
				Second = false;
				Equals = false;
				BinaryGame = true;
				playerhp = 100;
				gameover = false;
				for (int i = 0; i < 4; i++)
				{
					wave[i] = false;
				}
				for (int i = 0; i < 4; i++)
				{
					results[i] = 0;
				}
				round = 0;
				wave[round] = true;
				for (int i = 0; i < 32; i++)
				{
					enemy[i].enemyhp = 100;
				}
				for (int i = 0; i < 32; i++)
				{
					enemy[i].enemybit = rand()%2;
				}
				enemyskilled = 0;
				activeenemies = * size;
				if (* bit8 == true)
				{
					startenemies = 2;
				}
				enemynum = startenemies;
				for (int i = 0; i < enemynum; i++)
				{
					enemy[i].enemyalive = true;
				}
			}
			else if (BinaryGame == true)
			{
				BinaryGame = false;
				* xstatic = 35;
				lefts = 35;
				rights = 36;
				gameover = false;
				for (int i = 0; i < 32; i++)
				{
					bits[i] = false;
				}
				for (int i = 0; i < 32; i++)
				{
					enemy[i].enemyalive = false;
				}
			}
			rest(150);
		}
		else
		{
			pressed = 1;
		}
	}
}

// Controls used for Extreme Binary
void Arrows(bool *bits, int * size, bool * bit8, bool * bit16, bool * bit24, bool * bit32, float * player)
{
	int x = 300;
	int y = 380;

	// Left Arrow
	rectfill(backBuffer, x-75, y-9, x-15, y+9, makecol(0,0,0));
	for (int i = 0; i <= 25; i++)
	{
		line(backBuffer, x-75+i, y-10, x-50+i, y-25, makecol(0,0,0)); 
	}
	for (int i = 0; i <= 25; i++)
	{
		line(backBuffer, x-75+i, y+10, x-50+i, y+25, makecol(0,0,0)); 
	}
	rectfill(backBuffer, x-74, y-8, x-16, y+8, BUTTONCOL);
	for (int i = 0; i <= 21; i++)
	{
		line(backBuffer, x-74+i, y-9, x-49+i, y-24, BUTTONCOL); 
	}
	for (int i = 0; i <= 21; i++)
	{
		line(backBuffer, x-74+i, y+9, x-49+i, y+24, BUTTONCOL); 
	}

	// Right Arrow
	rectfill(backBuffer, x+75, y-9, x+15, y+9, makecol(0,0,0));
	for (int i = 0; i <= 25; i++)
	{
		line(backBuffer, x+75-i, y-10, x+50-i, y-25, makecol(0,0,0)); 
	}
	for (int i = 0; i <= 25; i++)
	{
		line(backBuffer, x+75-i, y+10, x+50-i, y+25, makecol(0,0,0)); 
	}
	rectfill(backBuffer, x+74, y-8, x+16, y+8, BUTTONCOL);
	for (int i = 0; i <= 21; i++)
	{
		line(backBuffer, x+74-i, y-9, x+49-i, y-24, BUTTONCOL); 
	}
	for (int i = 0; i <= 21; i++)
	{
		line(backBuffer, x+74-i, y+9, x+49-i, y+24, BUTTONCOL); 
	}


	if (((((mouse_x < x-25) && (mouse_x > x-76) && 
		(mouse_y < y+26) && (mouse_y > y-26)) ||
		((mouse_x > x-25) && (mouse_x < x-15) &&
		(mouse_y < y+10) && (mouse_y > y-10))) &&
		(mouse_b & 1)) || (key[KEY_LEFT])) 
	{
		rectfill(backBuffer, x-76, y-10, x-14, y+10, makecol(0,0,0));
		for (int i = 0; i <= 27; i++)
		{
			line(backBuffer, x-76+i, y-11, x-51+i, y-26, makecol(0,0,0)); 
		}
		for (int i = 0; i <= 27; i++)
		{
			line(backBuffer, x-76+i, y+11, x-51+i, y+26, makecol(0,0,0)); 
		}
		rectfill(backBuffer, x-74, y-8, x-16, y+8, BUTTONPRESS);
		for (int i = 0; i <= 18; i++)
		{
			line(backBuffer, x-74+i, y-9, x-49+i, y-24, BUTTONPRESS); 
		}
		for (int i = 0; i <= 18; i++)
		{
			line(backBuffer, x-74+i, y+9, x-49+i, y+24, BUTTONPRESS); 
		}
		if (* player > -145)
		{
			* player-=1;
			rights -= 1;
			lefts -= 1;
		}
	}
	if (((((mouse_x > x+25) && (mouse_x < x+76) && 
		(mouse_y < y+26) && (mouse_y > y-26)) ||
		((mouse_x < x+25) && (mouse_x > x+14) &&
		(mouse_y < y+10) && (mouse_y > y-10))) &&
		(mouse_b & 1)) || (key[KEY_RIGHT])) 
	{
		
		rectfill(backBuffer, x+76, y-10, x+14, y+10, makecol(0,0,0));
		for (int i = 0; i <= 27; i++)
		{
			line(backBuffer, x+76-i, y-11, x+51-i, y-26, makecol(0,0,0)); 
		}
		for (int i = 0; i <= 27; i++)
		{
			line(backBuffer, x+76-i, y+11, x+51-i, y+26, makecol(0,0,0)); 
		}
		rectfill(backBuffer, x+74, y-8, x+16, y+8, BUTTONPRESS);
		for (int i = 0; i <= 18; i++)
		{
			line(backBuffer, x+74-i, y-9, x+49-i, y-24, BUTTONPRESS); 
		}
		for (int i = 0; i <= 18; i++)
		{
			line(backBuffer, x+74-i, y+9, x+49-i, y+24, BUTTONPRESS); 
		}

		if (*player < 250)
		{
			* player+=1;
			rights += 1;
			lefts += 1;	
		}
	}

	circlefill(backBuffer, x-118, y+2, 26, makecol(10, 10, 10));
	circlefill(backBuffer, x-119, y+1, 26, makecol(10, 10, 50));
	circlefill(backBuffer, x-120, y, 26, makecol(5, 5, 5));
	circlefill(backBuffer, x-120, y, 26-2, makecol(255, 0, 0));
	textprintf_centre_ex(backBuffer, button, x-120-1, y-12, makecol(0, 0, 0), makecol(255, 0, 0), "Fire");
			
	if ((((mouse_x < x-120+26) && (mouse_x > x-120-26) && (mouse_y < y+26) && (mouse_y > y-26)) && (mouse_b & 1)) || (key[KEY_SPACE]))
	{			
		circlefill(backBuffer, x-120, y, 26, makecol(5, 5, 5));
		circlefill(backBuffer, x-120, y, 26-2, BUTTONPRESS);
		textprintf_centre_ex(backBuffer, button, x-120, y-13, makecol(0, 0, 0), BUTTONPRESS, "Fire");
		shoot = 0;
	}
	else if ((!mouse_b & 1) && (!key[KEY_SPACE]))
	{
		if (shoot == 0)
        {
			if (bullet[iCounter].be == false)
			{
				iCounter++;
				bullet[iCounter].be = true;
				bullet[iCounter].bx = * player+200;
				bullet[iCounter].by =250; 
					
				shoot = 1;
			}
        }
	}

	if ((activeenemies == 0) && (enemyskilled == * size) && (gameover == false) && (round < 3))
	{
		circlefill(backBuffer, x-218, y+2, 26, makecol(10, 10, 10));
		circlefill(backBuffer, x-219, y+1, 26, makecol(10, 10, 50));
		circlefill(backBuffer, x-220, y, 26, makecol(5, 5, 5));
		circlefill(backBuffer, x-220, y, 26-2, makecol(255, 0, 0));
		textprintf_centre_ex(backBuffer, button, x-220-1, y-12, makecol(0, 0, 0), makecol(255, 0, 0), "Next");
			
		if ((((mouse_x < x-220+26) && (mouse_x > x-220-26) && (mouse_y < y+26) && (mouse_y > y-26)) && (mouse_b & 1)) || (key[KEY_N]))
		{			
			circlefill(backBuffer, x-220, y, 26, makecol(5, 5, 5));
			circlefill(backBuffer, x-220, y, 26-2, BUTTONPRESS);
			textprintf_centre_ex(backBuffer, button, x-220, y-13, makecol(0, 0, 0), BUTTONPRESS, "Next");
			next = true;
			wave[round] = false;
			if (round < 4)
				round++;
			wave[round] = true;
			for (int i = 0; i < 32; i++)
			{
				enemy[i].enemybit = rand()%2;
			}
			enemyskilled = 0;
			activeenemies = * size;
			enemynum = startenemies;
			for (int i = 0; i < enemynum; i++)
			{
				enemy[i].enemyalive = true;
			}
			if (wave[1] == true)
			{		
				for (int i = 0; i < 32; i++)
				{
					enemy[i].enemyhp = 150;
				}
			}
			else if  (wave[2] == true)
			{
				for (int i = 0; i < 32; i++)
				{
					enemy[i].enemyhp = 200;
				}
			}
			else if (wave[3] == true)
			{
				for (int i = 0; i < 32; i++)
				{
					enemy[i].enemyhp = 250;
				}
			}
		}
	}
}

// Draws Bits
// Also Calculator symbols
void drawBits(bool * bits, int size, bool  bit8, bool bit16, bool bit24, bool bit32)
{	
	int startX,
		gap,
		radius, 
		posy;
	double y = 360;
	
	if (bit8 == true)
	{
		startX = 450;
		gap = 53;
		radius = 25;
		posy = 0;
	}
	else if (bit16 == true)
	{
		startX = 600;
		gap = 38;
		radius = 18;
		posy = 0;
	}
	else
	{
		startX = 600;
		gap = 38;
		radius = 18;
		posy = 50;
	}
	
	if (Equations == true)
	{
		char * sign;
		startX = 200;
		gap = 68;
		radius = 32;
		y = 350;
		for (int i = 0; i < 4; i++)
		{		
			if (i == 0)
			{
				sign = "+";
			}
			else if (i == 1)
			{
				sign = "-";
			}
			else if (i == 2)
			{
				sign = "x";
			}
			else if (i == 3)
			{
				sign = "/";
			}
			double x = startX + (i * gap);
			circlefill(backBuffer, x+2, y+2, radius, makecol(10, 10, 10));
			circlefill(backBuffer, x+1, y+1, radius, makecol(10, 10, 50));
			circlefill(backBuffer, x, y, radius, makecol(5, 5, 5));
			circlefill(backBuffer, x, y, radius-2, makecol(255, 0, 0));
			textprintf_centre_ex(backBuffer, signs, x-1, y-18, makecol(0, 0, 0), makecol(255, 0, 0), "%s", sign);
			
			if ((mouse_x < startX+radius+(0*gap)) && (mouse_x > startX-radius+(0*gap)) && (mouse_y < y+radius) && (mouse_y > y-radius))
			{
				if (mouse_b & 1)
				{
					circlefill(backBuffer, x, y, radius, makecol(5, 5, 5));
					circlefill(backBuffer, x, y, radius-2, BUTTONPRESS);
					textprintf_centre_ex(backBuffer, signs, x, y-17, makecol(0, 0, 0), BUTTONPRESS, "%s", sign);		
					Equations = false;
					Result = fnum1 + fnum2;
					equat = "+";
					Equals = true;
					for (int i = 0; i < 32; i++)
					{
						bits[i] = false;
					}
					rest(150);
				}
			}
			else if ((mouse_x < startX+radius+(1*gap)) && (mouse_x > startX-radius+(1*gap)) && (mouse_y < y+radius) && (mouse_y > y-radius))
			{
				if (mouse_b & 1)
				{
					circlefill(backBuffer, x, y, radius, makecol(5, 5, 5));
					circlefill(backBuffer, x, y, radius-2, BUTTONPRESS);
					textprintf_centre_ex(backBuffer, signs, x, y-17, makecol(0, 0, 0), BUTTONPRESS, "%s", sign);
					
					Equations = false;
					Result = fnum1 - fnum2;
					equat = "-";
					Equals = true;
					for (int i = 0; i < 32; i++)
					{
						bits[i] = false;
					}
					rest(150);
				}
			}
			else if ((mouse_x < startX+radius+(2*gap)) && (mouse_x > startX-radius+(2*gap)) && (mouse_y < y+radius) && (mouse_y > y-radius))
			{
				if (mouse_b & 1)
				{
					circlefill(backBuffer, x, y, radius, makecol(5, 5, 5));
					circlefill(backBuffer, x, y, radius-2, BUTTONPRESS);
					textprintf_centre_ex(backBuffer, signs, x, y-17, makecol(0, 0, 0), BUTTONPRESS, "%s", sign);
				
					Equations = false;
					Result = fnum1 * fnum2;
					equat = "x";
					Equals = true;
					for (int i = 0; i < 32; i++)
					{
						bits[i] = false;
					}
					rest(150);
				}
			}
			else if ((mouse_x < startX+radius+(3*gap)) && (mouse_x > startX-radius+(3*gap)) && (mouse_y < y+radius) && (mouse_y > y-radius))
			{
				if (mouse_b & 1)
				{
					circlefill(backBuffer, x, y, radius, makecol(5, 5, 5));
					circlefill(backBuffer, x, y, radius-2, BUTTONPRESS);
					textprintf_centre_ex(backBuffer, signs, x, y-17, makecol(0, 0, 0), BUTTONPRESS, "%s", sign);
					if (fnum2 == 0)
					{
						Equations = false;
						First = true;
						Second = false;
						fnum1 = 0;
						allegro_message("Cannot divide by zero, Restarting");
						break;
					}
					else
					{
						Equations = false;
						Result = fnum1 / fnum2;
						equat = "/";
						Equals = true;
						for (int i = 0; i < 32; i++)
						{
							bits[i] = false;
						}
					}
					rest(150);
				}
			}
		}
	}
	else
	{
		for (int i = 0, j = 0; i < size; i++, j++)
		{		
			double x = startX - (j * gap);
						
			if (bits[i] == true) 
			{
				circlefill(backBuffer, x+2, y+2+posy, radius, makecol(10, 10, 10));
				circlefill(backBuffer, x+1, y+1+posy, radius, makecol(10, 10, 50));
				circlefill(backBuffer, x, y+posy, radius, makecol(5, 5, 5));
				circlefill(backBuffer, x, y+posy, radius-2, makecol(255, 0, 0));
				textprintf_centre_ex(backBuffer, button, x-1, y-14+posy, makecol(0, 0, 0), makecol(255, 0, 0), "%d", bits[i]);
			}
			else
			{	
				circlefill(backBuffer, x, y+posy, radius, makecol(5, 5, 5));
				circlefill(backBuffer, x, y+posy, radius-2, BUTTONPRESS);
				textprintf_centre_ex(backBuffer, button, x-1, y-14+posy, makecol(0, 0, 0), BUTTONPRESS, "%d", bits[i]);
			}
		
			if ((mouse_x < x+radius) && (mouse_x > x-radius) && (mouse_y < y+posy+radius) && (mouse_y > y+posy-radius))
			{
				if (mouse_b & 1)
				{
					bits[i] = !bits[i];
					rest(150);
				}
			}
			if (i == 15)
			{
				posy = 0;
				j = -1;
				x = 0;
			}	
		}
	}
}

void DisplayOutput(bool * bits, int size, float * x, float * move, bool bit8, bool bit16, bool bit24, bool bit32)
{
	int p1=0, p2=0, p3=0, p4=0;
	* x+=* move;
		
	if (* x > rights)
	{
		* move *= -1;
	}
	else if (* x < lefts)
	{
		* move *= -1;
	}

	// Screen
	rectfill(backBuffer, 24, 14, 481, 301, makecol(5, 5, 5));
	rectfill(backBuffer, 25, 15, 480, 300, makecol(150, 150, 150));
	rectfill(backBuffer, 30, 20, 475, 295, makecol(5, 5, 5));
	rectfill(backBuffer, 31, 21, 474, 294, SCREENCOLOUR);

	if (BinaryGame == true)
	{
		// Extreme Binary game
		int y;
		// Displays current enemy wave
		textprintf_ex(backBuffer, outputscreen, 38, 20, TEXTCOLOUR, SCREENCOLOUR, "Wave %d", round+1);
		
		// If All enemies destroyed, wave complete
		if ((activeenemies == 0) && (enemyskilled == size) && (gameover == false))
		{
			results[round] = toDecimal(bits, size);
			textprintf_ex(backBuffer, outputscreen, 58, 45, TEXTCOLOUR, SCREENCOLOUR, "Enemies killed (right to left):");
			int j = 0;
			for (int i = size-1; i >= 0; i--)
			{
				textprintf_ex(backBuffer, outputscreen, 90+j, 75, TEXTCOLOUR, SCREENCOLOUR, "%d", bits[i]);
				j+=11;
			}

			if (round < 3)
			{
				textprintf_ex(backBuffer, outputscreen, 90, 105, TEXTCOLOUR, SCREENCOLOUR, "Your score: %u", results[round]);
				textprintf_ex(backBuffer, outputscreen, 90, 135, TEXTCOLOUR, SCREENCOLOUR, "Total score: %u", results[0]+results[1]+results[2]+results[3]);
				textprintf_ex(backBuffer, outputscreen, 58, 165, TEXTCOLOUR, SCREENCOLOUR, "Press (N)ext to continue...");
			}
			// Game complete
			else
			{
				textprintf_ex(backBuffer, outputscreen, 90, 105, TEXTCOLOUR, SCREENCOLOUR, "You bet the Game!");
				textprintf_ex(backBuffer, outputscreen, 90, 135, TEXTCOLOUR, SCREENCOLOUR, "Total score: %u", results[0]+results[1]+results[2]+results[3]);
				textprintf_ex(backBuffer, outputscreen, 90, 165, TEXTCOLOUR, SCREENCOLOUR, "Round 1: %u", results[0]);
				textprintf_ex(backBuffer, outputscreen, 90, 195, TEXTCOLOUR, SCREENCOLOUR, "Round 2: %u", results[1]);
				textprintf_ex(backBuffer, outputscreen, 90, 225, TEXTCOLOUR, SCREENCOLOUR, "Round 3: %u", results[2]);
				textprintf_ex(backBuffer, outputscreen, 90, 255, TEXTCOLOUR, SCREENCOLOUR, "Round 4: %u", results[3]);
			}
		}
		else if (gameover == true)
		{
			textprintf_ex(backBuffer, outputscreen, 88, 70, TEXTCOLOUR, SCREENCOLOUR, "Game Over");
			textprintf_ex(backBuffer, outputscreen, 48, 105, TEXTCOLOUR, SCREENCOLOUR, "The binary aliens are now in control");
			textprintf_ex(backBuffer, outputscreen, 58, 135, TEXTCOLOUR, SCREENCOLOUR, "All hope is lost...");
		}

		// Takes player off screen when dead
		if (playerhp <= 0)
		{
			y = -500;
			* x = -500;
			playerhp = 0;
		}
		else if (playerhp >= 1)
		{
			y = 262;
		}
		textprintf_ex(backBuffer, outputscreen, 325, 265, TEXTCOLOUR, SCREENCOLOUR, "Player: %d%%",  playerhp);
		
		// bullets
		if (bullet[iCounter].be == true)
		{
			bullet[iCounter].by -= BULLETSPEED;
			circlefill(backBuffer, bullet[iCounter].bx, bullet[iCounter].by, 2, TEXTCOLOUR);
			if (bullet[iCounter].by < 27)
			{
				// Bullet disapears
				bullet[iCounter].be = false;
			}	
		}
		//Bullet Counter, Unlimited Ammo
		if (iCounter >= 99)
		{
			iCounter= -1;
		}

		for (int i = 0; i < enemynum; i++)
		{
			// spawns starting enemies
			if ((pressed == 0) || (next == true))
			{
				enemy[i].enemyalive = true;
				enemy[i].ex = rand()%395;
				enemy[i].ex += 55;
				enemy[i].ey = 47;
				enemy[i].direction = rand()%2;
				if (i == startenemies-1)
					next = false;
			}
			
			// New enemy appears
			if (spawn == true)
			{
				enemy[enemynum-1].enemyalive = true;
				enemy[enemynum-1].ex = rand()%410;
				enemy[enemynum-1].ex += 55;
				enemy[enemynum-1].ey = 47;
				enemy[enemynum-1].direction = rand()%2;
				spawn = false;
			}
			
			// Once this enemy reaches this point, a new enemy appears, or if this enemy dies, spawns a new enemy
			if ((((enemy[enemynum-1].ey >= 60.1) && (enemy[enemynum-1].ey <= 60.2)) || (enemy[enemynum-1].enemyalive == false)) && (enemynum < size))
			{
 				enemynum += 1;
				spawn = true;
			}

			// Change of enemies current direction
			if (enemy[i].direction == true)
			{
				if (wave[0] == true)
				{
					enemy[i].ex += ENEMYX0;
					enemy[i].ey += ENEMYY;
				}
				else if (wave[1] == true)
				{
					enemy[i].ex += ENEMYX1;
					enemy[i].ey += ENEMYY;
				}
				else if (wave[2] == true)
				{
					enemy[i].ex += ENEMYX2;
					enemy[i].ey += ENEMYY;
				}
				else if (wave[3] == true)
				{
					enemy[i].ex += ENEMYX3;
					enemy[i].ey += ENEMYY;
				}

			}
			// Change of enemies current direction
			else if (enemy[i].direction == false)
			{
				if (wave[0] == true)
				{
					enemy[i].ex -= ENEMYX0;
					enemy[i].ey += ENEMYY;
				}
				else if (wave[1] == true)
				{
					enemy[i].ex -= ENEMYX1;
					enemy[i].ey += ENEMYY;
				}
				else if (wave[2] == true)
				{
					enemy[i].ex -= ENEMYX2;
					enemy[i].ey += ENEMYY;
				}
				else if (wave[3] == true)
				{
					enemy[i].ex -= ENEMYX3;
					enemy[i].ey += ENEMYY;
				}
			}

			// if enemy hits wall, change direction
			if (enemy[i].ex >= 450)
			{
				enemy[i].direction = false;
			}

			if (enemy[i].ex <= 55)
			{
				enemy[i].direction = true;
			}
			
			// enemy and player collision detection
			if (((enemy[i].ex+16 >= * x+190) && (enemy[i].ey+18 >= y-3) &&
				(enemy[i].ex-16 <= * x+210) && (enemy[i].ey-18 <= y+32)) ||
				((enemy[i].ex-15-8 <= * x+210) && (enemy[i].ey+12+20 >= y-3) &&
				(enemy[i].ex-15+8 >= * x+190) && (enemy[i].ey+12-8 <= y+32)) ||
				((enemy[i].ex+15-8 <= * x+210) && (enemy[i].ey+12+20 >= y-3) &&
				(enemy[i].ex+15+8 >= * x+190) && (enemy[i].ey+12-8 <= y+32)))
			{
				enemy[i].enemyalive = false;
				playerhp -= 15;
				enemy[i].enemybit = 0;
				bits[enemyskilled] = enemy[i].enemybit;
				enemyskilled++;
				activeenemies--;
				if (playerhp <= 0)
				{
					y = -500;
					* x = -500;
					playerhp = 0;
					gameover = true;
				}
				textprintf_ex(backBuffer, outputscreen, 325, 265, TEXTCOLOUR, SCREENCOLOUR, "Player: %d%%",  playerhp);
			}

			if (enemy[i].ey+12+20 >= 300)
			{
				enemy[i].enemyalive = false;
				enemy[i].enemybit = 0;
				bits[enemyskilled] = enemy[i].enemybit;
				enemyskilled++;
				activeenemies--;
			}

			if (enemy[i].enemyalive == false)
			{
				enemy[i].ex = 0;
				enemy[i].ey = 0;
			}

			if (enemy[i].enemyalive == true)
			{
				// enemies outline
				ellipsefill(backBuffer, enemy[i].ex, enemy[i].ey, 16, 18, SCREENCOLOUR);
				triangle(backBuffer, enemy[i].ex-10, enemy[i].ey-16, enemy[i].ex-3, enemy[i].ey-16, enemy[i].ex-6, enemy[i].ey-26, SCREENCOLOUR); 
				triangle(backBuffer, enemy[i].ex+10, enemy[i].ey-16, enemy[i].ex+3, enemy[i].ey-16, enemy[i].ex+6, enemy[i].ey-26, SCREENCOLOUR);
				circlefill(backBuffer, enemy[i].ex-15, enemy[i].ey+12, 8, SCREENCOLOUR);
				triangle(backBuffer, enemy[i].ex-23, enemy[i].ey+13, enemy[i].ex-16, enemy[i].ey+13, enemy[i].ex-18, enemy[i].ey+26, SCREENCOLOUR);
				triangle(backBuffer, enemy[i].ex-17, enemy[i].ey+13, enemy[i].ex-10, enemy[i].ey+13, enemy[i].ex-14, enemy[i].ey+26, SCREENCOLOUR);
				circlefill(backBuffer, enemy[i].ex+15, enemy[i].ey+12, 8, SCREENCOLOUR);
				triangle(backBuffer, enemy[i].ex+23, enemy[i].ey+13, enemy[i].ex+16, enemy[i].ey+13, enemy[i].ex+18, enemy[i].ey+26, SCREENCOLOUR);
				triangle(backBuffer, enemy[i].ex+17, enemy[i].ey+13, enemy[i].ex+10, enemy[i].ey+13, enemy[i].ex+14, enemy[i].ey+26, SCREENCOLOUR);
				
				// enemies
				ellipsefill(backBuffer, enemy[i].ex, enemy[i].ey, 15, 17, TEXTCOLOUR);
				triangle(backBuffer, enemy[i].ex-9, enemy[i].ey-15, enemy[i].ex-2, enemy[i].ey-15, enemy[i].ex-5, enemy[i].ey-25, TEXTCOLOUR); 
				triangle(backBuffer, enemy[i].ex+9, enemy[i].ey-15, enemy[i].ex+2, enemy[i].ey-15, enemy[i].ex+5, enemy[i].ey-25, TEXTCOLOUR);
				circlefill(backBuffer, enemy[i].ex-15, enemy[i].ey+12, 7, TEXTCOLOUR);
				triangle(backBuffer, enemy[i].ex-22, enemy[i].ey+12, enemy[i].ex-15, enemy[i].ey+12, enemy[i].ex-17, enemy[i].ey+25, TEXTCOLOUR);
				triangle(backBuffer, enemy[i].ex-16, enemy[i].ey+12, enemy[i].ex-9, enemy[i].ey+12, enemy[i].ex-13, enemy[i].ey+25, TEXTCOLOUR);
				circlefill(backBuffer, enemy[i].ex+15, enemy[i].ey+12, 7, TEXTCOLOUR);
				triangle(backBuffer, enemy[i].ex+22, enemy[i].ey+12, enemy[i].ex+15, enemy[i].ey+12, enemy[i].ex+17, enemy[i].ey+25, TEXTCOLOUR);
				triangle(backBuffer, enemy[i].ex+16, enemy[i].ey+12, enemy[i].ex+9, enemy[i].ey+12, enemy[i].ex+13, enemy[i].ey+25, TEXTCOLOUR);
				textprintf_ex(backBuffer, outputscreen, enemy[i].ex-6, enemy[i].ey-13, SCREENCOLOUR,  TEXTCOLOUR, "%d", enemy[i].enemybit);
				
				if ((bullet[iCounter].bx >= enemy[i].ex-16) && (bullet[iCounter].by <= enemy[i].ey+18) && 
				(bullet[iCounter].bx <= enemy[i].ex+16) && (bullet[iCounter].by >= enemy[i].ey-18) ||
				(bullet[iCounter].bx >= enemy[i].ex-15-8) && (bullet[iCounter].by <= enemy[i].ey+12+20) &&
				(bullet[iCounter].bx <= enemy[i].ex-15+8) && (bullet[iCounter].by >= enemy[i].ey+12-8) ||
				(bullet[iCounter].bx >= enemy[i].ex+15-8) && (bullet[iCounter].by <= enemy[i].ey+12+20) &&
				(bullet[iCounter].bx <= enemy[i].ex+15+8) && (bullet[iCounter].by >= enemy[i].ey+12-8))
			 	{
 					enemy[i].enemyhp -= 50;
					bullet[iCounter].be = false;
					bullet[iCounter].bx = * x+200;
					bullet[iCounter].by =250; 
				}
				if ((enemy[i].enemyhp <= 0) && (enemy[i].enemyalive == true))
				{
					enemy[i].enemyalive = false;
 					bits[enemyskilled] = enemy[i].enemybit;
					enemyskilled++;
					activeenemies--;
					
				}
			}
		}
		
		if (!(playerhp <= 0))
		{
			//Player outline
			triangle(backBuffer, * x+200, y-3, * x+196, y+11, * x+204, y+11, SCREENCOLOUR);
			triangle(backBuffer, * x+201, y+6, * x+193, y+11, * x+207, y+11, SCREENCOLOUR);
			rectfill(backBuffer, * x+193, y+10, * x+207, y+28, SCREENCOLOUR);
			triangle(backBuffer, * x+200, y+12, * x+180, y+24, * x+220, y+24, SCREENCOLOUR);
			triangle(backBuffer, * x+200, y+26, * x+180, y+25, * x+220, y+25, SCREENCOLOUR);
			triangle(backBuffer, * x+196, y+25, * x+192, y+29, * x+196, y+32, SCREENCOLOUR);
			triangle(backBuffer, * x+202, y+26, * x+208, y+29, * x+202, y+32, SCREENCOLOUR);
			triangle(backBuffer, * x+195, y+26, * x+198, y+30, * x+201, y+26, SCREENCOLOUR);
			triangle(backBuffer, * x+205, y+26, * x+202, y+30, * x+199, y+26, SCREENCOLOUR);

			//Player
			triangle(backBuffer, * x+200, y-2, * x+197, y+10, * x+203, y+10, TEXTCOLOUR);
			triangle(backBuffer, * x+200, y+7, * x+194, y+10, * x+206, y+10, TEXTCOLOUR);
			rectfill(backBuffer, * x+194, y+10, * x+206, y+27, TEXTCOLOUR);
			triangle(backBuffer, * x+200, y+13, * x+181, y+23, * x+219, y+23, TEXTCOLOUR);
			triangle(backBuffer, * x+200, y+27, * x+181, y+24, * x+219, y+24, TEXTCOLOUR);
			triangle(backBuffer, * x+197, y+27, * x+193, y+29, * x+197, y+31, TEXTCOLOUR);
			triangle(backBuffer, * x+203, y+27, * x+207, y+29, * x+203, y+31, TEXTCOLOUR);
			triangle(backBuffer, * x+196, y+27, * x+198, y+29, * x+200, y+27, TEXTCOLOUR);
			triangle(backBuffer, * x+204, y+27, * x+202, y+29, * x+200, y+27, TEXTCOLOUR);
		}
	}
	else if (Game1 == true)
	{
		// Whats that byte game display
		Split(bits, &p1, &p2, &p3, &p4, size);
		textprintf_ex(backBuffer, outputscreen, * x+10, 25, TEXTCOLOUR, SCREENCOLOUR, "What's that byte?");
		textprintf_ex(backBuffer, outputscreen, * x, 90, TEXTCOLOUR, SCREENCOLOUR, "Insert binary representation: %u", randomnum);
		textprintf_ex(backBuffer, outputscreen, * x, 120, TEXTCOLOUR, SCREENCOLOUR, "%s", choice);
		if (answer == randomnum)
		{
			textprintf_ex(backBuffer, outputscreen, * x, 150, TEXTCOLOUR, SCREENCOLOUR, "Do you want to play Again?");
		}
	}
	else if (Game2 == true)
	{
		// Higher or Lower game display
		Split(bits, &p1, &p2, &p3, &p4, size);
		decimal = toDecimal(bits, size);
		textprintf_ex(backBuffer, outputscreen, * x, 25, TEXTCOLOUR, SCREENCOLOUR, "Higher or Lower?");
		if (bit8 == true)
		{
			textprintf_ex(backBuffer, outputscreen, * x+10, 60, TEXTCOLOUR, SCREENCOLOUR, "Number is between 0 and 255");
		}
		if (bit16 == true)
		{
			textprintf_ex(backBuffer, outputscreen, * x+10, 60, TEXTCOLOUR, SCREENCOLOUR, "Number is between 0 and 65535");
		}
		if (bit24 == true)
		{
			textprintf_ex(backBuffer, outputscreen, * x+10, 60, TEXTCOLOUR, SCREENCOLOUR, "Number is between 0 and 1677215");
		}
		if (bit32 == true)
		{
			textprintf_ex(backBuffer, outputscreen, * x+10, 60, TEXTCOLOUR, SCREENCOLOUR, "Number is between 0 and 4294967295");
		}
		textprintf_ex(backBuffer, outputscreen, * x+10, 90, TEXTCOLOUR, SCREENCOLOUR, "Guess Count: %d", GuessCounter);
		textprintf_ex(backBuffer, outputscreen, * x+10, 120, TEXTCOLOUR, SCREENCOLOUR, "Your Guess: %u", decimal);
		textprintf_ex(backBuffer, outputscreen, * x+10, 150, TEXTCOLOUR, SCREENCOLOUR, "%s", choice);
		if (answer == randomnum)
		{
			textprintf_ex(backBuffer, outputscreen, * x+10, 180, TEXTCOLOUR, SCREENCOLOUR, "You guessed it in %d turns,", GuessCounter);
			textprintf_ex(backBuffer, outputscreen, * x+95, 210, TEXTCOLOUR, SCREENCOLOUR, "play again?");
		}
	}
	else if (Calculator == true)
	{
		// Calculator display
		Split(bits, &p1, &p2, &p3, &p4, size);
		if (First == true)
		{
			fnum1 = complement2(bits, size);	
		}
		if (Second == true)
		{
			fnum2 = complement2(bits, size);
		}
		if (Equals == true)
		{
			textprintf_ex(backBuffer, outputscreen, * x, 150, TEXTCOLOUR, SCREENCOLOUR, "%d %s %d", fnum1, equat, fnum2);
			textprintf_ex(backBuffer, outputscreen, * x+15, 180, TEXTCOLOUR, SCREENCOLOUR, "= %.2f", Result);
		}
		textprintf_ex(backBuffer, outputscreen, * x+10, 25, TEXTCOLOUR, SCREENCOLOUR, "Calculator");
		textprintf_ex(backBuffer, outputscreen, * x, 90, TEXTCOLOUR, SCREENCOLOUR, "First Number: %d", fnum1);
		textprintf_ex(backBuffer, outputscreen, * x, 120, TEXTCOLOUR, SCREENCOLOUR, "Second Number: %d", fnum2);
		if (First == true)
		{
			textprintf_ex(backBuffer, outputscreen, * x, 120, TEXTCOLOUR, SCREENCOLOUR, "                          ");
		}
	}
	else
	{
		//Default display
		Split(bits, &p1, &p2, &p3, &p4, size);
		decimal = toDecimal(bits, size);
		long long int complement = complement2(bits, size);
		textprintf_ex(backBuffer, outputscreen, * x, 25, TEXTCOLOUR, SCREENCOLOUR, "Andrew's Binary Converter");
		textprintf_ex(backBuffer, outputscreen, * x, 60, TEXTCOLOUR, SCREENCOLOUR, "Binary: ");
		int j = 0;
		for (int i = size-1; i >= 0; i--)
		{
			textprintf_ex(backBuffer, outputscreen, * x+78+j, 60, TEXTCOLOUR, SCREENCOLOUR, "%d", bits[i]);
			j+=11;
		}
		textprintf_ex(backBuffer, outputscreen, * x, 90, TEXTCOLOUR, SCREENCOLOUR, "Decimal: %u", decimal);
		textprintf_ex(backBuffer, outputscreen, * x, 120, TEXTCOLOUR, SCREENCOLOUR, "2's Complement: %d", complement);
		textprintf_ex(backBuffer, outputscreen, * x, 150, TEXTCOLOUR, SCREENCOLOUR, "Hexadecimal: %X", decimal);	
		textprintf_ex(backBuffer, outputscreen, * x, 180, TEXTCOLOUR, SCREENCOLOUR, "Octal: %o", decimal);	
		textprintf_ex(backBuffer, outputscreen, * x, 210, TEXTCOLOUR, SCREENCOLOUR, "IP: %d.%d.%d.%d", p4, p3, p2, p1);
		if (bit8 == true)
		{
			int character = decimal;
			if ((character >= 127) || (character < 32))
				character = 32;
			textprintf_ex(backBuffer, outputscreen, * x, 240, TEXTCOLOUR, SCREENCOLOUR, "Ascii: %c", character);
		}
	}

	// RGB display
	rectfill(backBuffer, 523, 19, 607, 58, makecol(5, 5, 5));
	rectfill(backBuffer, 524, 20, 606, 57, makecol(150, 150, 150));
	rectfill(backBuffer, 529, 24, 601, 52, makecol(5, 5, 5));
	rectfill(backBuffer, 530, 25, 600, 51, makecol(p1, p2, p3));
	textprintf_ex(backBuffer, outputscreen, 545, 26, makecol(0,0,0), makecol(p1, p2, p3), "RGB");

	if (stat == true)
	{
		for (int i = 0; i < 150; i ++)
		{
			int x1 = rand() % 443;
			int y1 = rand() % 273;
			putpixel(backBuffer, x1+31, y1+21, TEXTCOLOUR);
		}
	}
}

void StaticButton(float * move)
{
	
	circlefill(backBuffer, 14, 14, 4, makecol(10,10,10));
	circlefill(backBuffer, 14, 14, 3, makecol(255,0,0));
	
	if ((mouse_x > 10) && (mouse_x < 18) && (mouse_y > 10) && (mouse_y < 18))
	{
		if (mouse_b & 1)
		{
			* move = 0;
			stat = !stat;
		}
	}
}


void main()
{
	// First sets size as 8
	int size = 8;
	// Creates a bool array with 32 elements
	bool bits[32];
	// sets each element in as false
	for (int i = 0; i < 32; i++)
	{
		bits[i] = false;
	}
	// sets each enemy as false for Extreme Binary
	for (int i = 0; i < 32; i++)
	{
		enemy[i].enemyalive = false;
	}
	//sets buttons as false
	for (int i = 0; i < 100; i++)
	{
		bullet[i].be = false;
	}
	
	//bools for changing byte size
	bool bit8 = true;
	bool bit16 = false;
	bool bit24 = false;
	bool bit32 = false;

	float gate1 = -160;
	float gate2 = 320;

	if (allegro_init() != 0)
	{
	      return;
	}

	set_color_depth(desktop_color_depth());
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0) 
	{
		return;
	}
	backBuffer = create_bitmap(640, 480);
	BITMAP * backgroundimage = load_bitmap ("metal.bmp", NULL);
	BITMAP * gate = load_bitmap ("gate.bmp", NULL);
	
	install_keyboard();
	install_mouse();
	bool close = false;
	
	PALETTE palette;
	outputscreen = load_font("binaryfont1.pcx", palette, NULL); 
	button = load_font("mfont1.pcx", palette, NULL);
	signs = load_font("signsfont.pcx", palette, NULL);

	float x = 35;

	float screenstatic = 0.1f;
	
	set_close_button_callback (Handler);
	set_window_title("Binary Converter");
	while (!done)
	{
		if (key[KEY_ESC])
		{
			close = !close;	
			rest(150);
		}
		if (close == true)
		{
			gate1 += GATESPEED;
			gate2 -= GATESPEED; 
			if ((gate1 >= -160) && (gate2 <= 320))
			{
				done = true;
				break;
			}
		}

		clear_to_color(backBuffer, BACKGROUND);
		// Displays background image
		blit(backgroundimage, backBuffer, 0, 0, 0, 0, 640, 480);
		//
		Buttons(bits, &size, &bit8, &bit16, &bit24, &bit32, &x);
		if (BinaryGame == true)
		{
			// Game controls
			Arrows(bits, &size, &bit8, &bit16, &bit24, &bit32, &x);
		}
		if (BinaryGame == false)
		{
			// Binary bit buttons
			drawBits(bits, size, bit8, bit16, bit24, bit32);
		}
		// Turns off screen static
		StaticButton(&screenstatic);
		// Screen which displays output
		DisplayOutput(bits, size, &x, &screenstatic, bit8, bit16, bit24, bit32);
		
		if (close == false)
		{
			if (gate1 > -480)
				gate1 -= GATESPEED;
			if (gate2 < 640)
				gate2 += GATESPEED; 
		}
	
		// Alternative mouse
		//show_mouse(backBuffer);
		// Mouse
		triangle(backBuffer, mouse_x-1, mouse_y-2, mouse_x-1, mouse_y+14, mouse_x+11, mouse_y+14, makecol(0, 0, 0));
		line(backBuffer, mouse_x+4, mouse_y+5, mouse_x+7, mouse_y+21, makecol(0,0,0));
		line(backBuffer, mouse_x+3, mouse_y+4, mouse_x+6, mouse_y+21, makecol(0,0,0));
		line(backBuffer, mouse_x+4, mouse_y+6, mouse_x+8, mouse_y+21, makecol(0,0,0));
		line(backBuffer, mouse_x+2, mouse_y+3, mouse_x+5, mouse_y+21, makecol(0,0,0));
		line(backBuffer, mouse_x+4, mouse_y+5, mouse_x+7, mouse_y+20, makecol(250, 250, 250));
		line(backBuffer, mouse_x+3, mouse_y+4, mouse_x+6, mouse_y+20, makecol(250, 250, 250));
		triangle(backBuffer, mouse_x, mouse_y, mouse_x, mouse_y+13, mouse_x+9, mouse_y+13, makecol(250, 250, 250));

		blit(gate, backBuffer, 0, 0, gate1, 0, 480, 480);
		blit(gate, backBuffer, 0, 0, gate2, 0, 480, 480);

		blit(backBuffer, screen, 0, 0, 0, 0, 640, 480);	
	}
}

END_OF_MAIN()