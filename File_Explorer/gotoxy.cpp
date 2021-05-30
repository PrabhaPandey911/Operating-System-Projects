//Name: Prabha Pandey
//Roll No. : 2018201053
#include <stdio.h>
#include <stdlib.h>
void gotoxy(int x,int y)    
{
    printf("%c[%d;%df",0x1B,y,x);//taking cursor to a particular coordinate on screen
}
