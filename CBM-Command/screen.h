#ifndef _SCREEN_H
#define _SCREEN_H

unsigned char SCREEN_BUFFER[1000];
unsigned char COLOR_BUFFER[1000];

void setupScreen(void);
void writeMenuBar(void);
void writeStatusBar(unsigned char[], 
					unsigned char, 
					unsigned char);

//void writePanel(
//	unsigned drawBorder,
//	unsigned reverse,
//	unsigned char color,
//	unsigned char x, unsigned char y
//	unsigned char h, unsigned char w,
//	unsigned char *title,
//	unsigned char *cancel,
//	unsigned char *ok);


void drawBox(
	unsigned char, unsigned char,
	unsigned char, unsigned char,
	unsigned char, unsigned);

void saveScreen(void);
void retrieveScreen(void);
#endif