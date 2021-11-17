#include "screen.h"

//Constructor
Screen:: Screen()
{
	video_ = reinterpret_cast<unsigned char *>(VIDEO);
	
	unsigned char backcolor = COLOR_BLACK;
	unsigned char forecolor = COLOR_WHITE;
	attribute_ = (backcolor << 4) | (forecolor & 0x0F);
	
	x = 0; // number of column 0, 1, 2, ...
	y = 0; // number of lines 0, 1, 2 ...
}

//Clean Screen
void Screen:: clean()
{
	for(int i = 0; i < COLUMNS * LINES * 2; i++)
		*(video_ + i) = 0;
	
	x = 0;
	y = 0;
}

//Print char
void Screen:: printChar(unsigned char c)
{
	if (c != '\n') //new line
    {
		*(video_ + (x + y * COLUMNS) * 2) = c & 0xFF;
		*(video_ + (x + y * COLUMNS) * 2 + 1) = attribute_;
		x++;
		if (x >= COLUMNS)
			goto NewLine;			
	}
	else
	{
NewLine:
		x = 0;
		y++;
		if(y >= LINES)
			y = 0;
	}
}

//
void Screen:: printString(const char * str)
{	
	unsigned char * sstr = (unsigned char*)str;
	int i=0;
	while(sstr[i])
	{
		printChar(sstr[i]);
		i++;
	}

}

//
void Screen::setTextColor(unsigned char backcolor, unsigned char forecolor)
{
	attribute_ = (backcolor << 4) | (forecolor & 0x0F);
}

//
void Screen:: printInt(int k)
{
	int num = 0;
	int k1 = k;
	do
	{
		k1 /= 10;
		num++;
	}
	while(k1 != 0);
	//printEndl();
	//printHex(num);
	//printString("->");
	
	while(num != 0)
	{
		k1 = k;
		for(int i = 0; i < num - 1; i++)
		{
			k1 /= 10;
			
		}
		
		k1 %= 10;
		*(video_ + (x + y * COLUMNS) * 2) = k1 + '0';
		*(video_ + (x + y * COLUMNS) * 2 + 1) = attribute_;
		x++;
		
		num --;
	}
}

void Screen:: printBinary(void * mem, unsigned int length)
{
	for ( unsigned int i = 0; i < length; ++i )
	{
		unsigned char a = 1 << 7;
		for (unsigned int j = 0; j < 8; ++j)
		{
			if (a & *((char*)mem + i))
				printString ("1");
			else
				printString("0");
			a >>= 1;
		}
		printString(" ");
	}
	printString("\n");
}

void Screen::printBinary(int num)
{
	for(int i = sizeof(num); i > 0; i--)
	{
		for(int j = 1; j <= 8; j++)
		{
			if ((num >> i*8 - j) & 1)
				printChar ('1');
			else
				printChar('0');
		}
		printString(" ");
	}
	printEndl();
}

void Screen:: printHex(int k)
{
	printString("0x");
	for(int i = 2 * sizeof(k) - 1; i >= 0; i--)
		printChar("0123456789ABCDEF"[(k >> 4*i) & 0x0000000F]);
}

void Screen:: printEndl()
{
	printString("\n");
}
