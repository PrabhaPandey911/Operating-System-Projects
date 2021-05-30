//Name: Prabha Pandey
//Roll No. : 2018201053
#include "static_variables.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <vector>
#include <unistd.h>
#include "list_files.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>
static struct termios term, oterm;
//int file_count;
int is_regular_file(string path);
static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);
static int getch(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

static int kbhit(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

static int kbesc(void)
{
    int c;

    if (!kbhit()) return 0x001b;
    c = getch();
    if (c == '[') {
        switch (getch()) {
            case 'A':
                c =  0x0105;
                break;
            case 'B':
                c = 0x0106;
                break;
            case 'C':
                c = 0x0107;
                break;
            case 'D':
                c = 0x0108;
                break;
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }
    if (c == 0) while (kbhit()) getch();
    return c;
}

static int kbget(void)
{
    int c;

    c = getch();
    if(c == 0x001b)
        c=kbesc();
    else
        c=c;
    return c;
}

void scroll();
