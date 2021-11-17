#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO 0xB8000
#define LINES 24
#define COLUMNS 80

const unsigned char COLOR_BLACK 	= 0;
const unsigned char COLOR_BLUE 	= 1;
const unsigned char COLOR_GREEN 	= 2;
const unsigned char COLOR_CYAN 	= 3;
const unsigned char COLOR_RED 		= 4;
const unsigned char COLOR_MAGENTA 	= 5;
const unsigned char COLOR_BROWN 	= 6;
const unsigned char COLOR_LIGHT_GREY 	= 7;
const unsigned char COLOR_DARK_GREY 	= 8;
const unsigned char COLOR_LIGHT_BLUE 	= 9;
const unsigned char COLOR_LIGHT_GREEN 	= 10;
const unsigned char COLOR_LIGHT_CYAN 	= 11;
const unsigned char COLOR_LIGHT_RED 	= 12;
const unsigned char COLOR_LIGHT_MAGENTA 	= 13;
const unsigned char COLOR_LIGHT_BROWN 		= 14;
const unsigned char COLOR_WHITE 	= 15;

class Screen
{
public:
	Screen();	
	void clean();	
	
	void printString(const char *str);
	void printChar(unsigned char c);
	
	void setTextColor(unsigned char forecolor, unsigned char backcolor);
	
	void printInt(int k);
	void printBinary(void * mem, unsigned int length);
	void printBinary(int num);
	void printHex(int k);
	void printEndl();
	
private:
	// cursor position
	int x;
	int y;
	
	unsigned char *video_;
	unsigned char attribute_; // forecolor | backcolor
};

#endif //SCREEN_H
