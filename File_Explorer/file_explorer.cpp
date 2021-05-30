//Name: Prabha Pandey
//Roll No. : 2018201053
#include "static_variables.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include "list_files.h"
#include "gotoxy.h"
#include "screentest.h"
#include <stack>
#include "command.h"
using namespace std;
extern vector<std::vector<string>> navigate;
extern int present;
extern vector<string> pathstore;
extern stack<string> back;
extern stack<string> forward_stack;
void change_attributes(); //setting Noncanonical mode and echo off
extern int normal_mode;


int main()
{
	change_attributes();

	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);//for getting #rows
	printf("\033[?1049h\033[H");
	//printf("Alternate screen buffer\n");
	fflush(stdout);
	char * path=(char *) malloc(1000*sizeof(char));
	path=get_current_dir_name();
	extern int rows;
	
	present=-1;
	//printf("%s\n",path);
	list_file(path); //listing files

	rows=w.ws_row;
	printf("\33[%d;%dH%s",rows,0,"Normal Mode");
	normal_mode=1;
	gotoxy(0,0);

	scroll();//adding functoinality of keys
	
 	int temp;
	scanf("%d",&temp);
	printf("\033[?1049l");
	return 0;
}
