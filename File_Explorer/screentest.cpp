//Name: Prabha Pandey
//Roll No. : 2018201053
#include <stack>
#include <dirent.h>
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
#include "gotoxy.h"
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>
#include "command.h"
using namespace std;

stack<string> back;
stack<string> forward_stack;
static struct termios term, oterm;
static int distancefromup=0;
//static int ptrup = 0; //use in scrolling
//static int ptrbtm =rows;//use in scrolling
extern vector<std::vector<string>> navigate;
extern vector<string> pathstore;
extern int present;
static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);
int normal_mode;
int rows;
int file_count;
// int x_cordinate;
// int y_cordinate;

int is_regular_file(string path)//to check if the file clicked is actually a file or a directory
{
    struct stat ex1;
    stat(path.c_str(), &ex1);
    return S_ISREG(ex1.st_mode);
}
std::vector<string> file_name;//storing the names of files 
std::vector<string> store;//storing the complete status that appears in the terminal after list_file is executed
static int getch(void)//taking character as input
{
    int c = 0;
    c = getchar();
    return c;
}

void change_attributes()//setting the application to be open in Noncanonical mode and off Echo
{
    tcgetattr(0, &oterm);
    term = oterm;
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
}

static int kbhit(void) // setting vmin and vtime then restoring it back to normal
{
    int c = 0;
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

static int kbesc(void)
{
    int c;

    if (!kbhit()) return 0x001b; //esc
    c = getch();
    if (c == '[') {//checking if the key pressed is esc,right,left,up, down,backspace or home
        switch (getch()) {
            case 'A':
                c =  0x0105;//up
                break;
            case 'B':
                c = 0x0106;//down
                break;
            case 'C':
                c = 0x0108;//right
                break;
            case 'D':
                c = 0x0107; //left
                break;
            default:
                c = 0;
                break;
        }
    } else {
        if(c=='?')
            c = 0x7f;//backspace
        else if(c=='h'|| c=='H') //home
            c = c;
        else if(c==':')//command mode
            c=c;
        else
            c = 0;
    }
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
void clear(stack<string> forward_stack) //empty forward stack
{
    while(!forward_stack.empty())
    {
        forward_stack.pop();
    }
}
int find_index(string x,vector<string> pathstore) //find index of an element in a vector
{
    for(int i=0;i<pathstore.size();i++)
    {
        if(pathstore[i]==x)
            return i;
    }
    return -1;
}

void scroll()
{  
    string path;
    int c;
    while (1) {
        if(normal_mode==1)
        {c = kbget();

        if (c == 0x000a)  {//enter
            path=pathstore[present];
            path.append("/");    
            path.append(file_name[distancefromup]);//appending the name of file to be accessed by pressing enter to the path that we are at currently
            string path1= string(get_current_dir_name())+"/"+"..";
            if(path.compare(path1)==0)
            {
                continue;
            }
            else
            {
                int x =  is_regular_file(path);//is it regular file or directory
                if(x==0) //if it is a directory access elements inside it
                {
                    printf("\033[?1049h\033[H");
                    fflush(stdout);
                    clear(forward_stack);
                    back.push(pathstore[present]);
                    list_file(path);
                    gotoxy(0,0);
                    distancefromup=0;
                }  
                else//if it is a file then open it using xdg-open
                {
                    pid_t pid;
                    pid = fork();
                    if (pid == 0) {
                        execl("/usr/bin/xdg-open", "xdg-open",path.c_str(), (char *)0);
                        exit(1);
                    }
                }
            }
            
        } 
        else if(c == 0x001b )//esc 
        {
            exit(1);
        }
        else if( c == 0x0107 )//left 
        {
            if(back.empty())
            {
                continue;
            }
            else{
                if(!back.empty())
                {
                    forward_stack.push(pathstore[present]);
                    string x=back.top();
                    back.pop();
                    int i=find_index(x,pathstore);
                    present=i;
                    printf("\033[?1049h\033[H");
                    fflush(stdout);
                    list_file(pathstore[present]);
                }
                else
                    continue;                
            }
           
        }
         else if( c ==  0x0108 )//right
        {
            if((present+1)==pathstore.size())
            {
                continue;
            }
            else
            {           
                if(!forward_stack.empty())
                {
                    back.push(pathstore[present]);
                    string x = forward_stack.top();
                    forward_stack.pop();
                    int i=find_index(x,pathstore);
                    present=i;
                    printf("\033[?1049h\033[H");
                    fflush(stdout);
                    list_file(pathstore[present]);    
                }
                else{
                    continue;
                }
               
            }
            
        }
        else
        if (c == 0x0106) {//down
            int i=distancefromup;//work here for scrolling functionality
            if(distancefromup == file_count)
                continue;
             else{
                distancefromup++;
                i++;
                printf("\033[%dB", (1));
                if(distancefromup==rows){
                    fflush(stdout);//call funtion to paint the screen again to get the files below to come up
                    while(i!=file_count || distancefromup!= rows )
                    {
                       printf("%s\n", store[i].c_str() );
                       i++;
                    }
            }
        } }else
        if (c ==  0x0105) {//up
             if(distancefromup==0)
                 continue;
             else{
                distancefromup--;
                printf("\033[%dA", (1));
                
            }
                
        } 
        
        else if(c == 0x7f){ //backspace 
            string path1=pathstore[present];
            if(path1==string(get_current_dir_name()))
            {
                continue;
            }
            else
            {
                if(back.top()!=path1)
                    back.push(path1);
                size_t found = path1.find_last_of("/");//find last occurance of "/" and trim from that point till the last of string to go back to previously visited directory
                path=path1.substr(0,found);
                list_file(path);
                present--;
                distancefromup=0;

            }
        }
        
        else if(c == 'h' || c=='H'){//home 
            char * path1 = get_current_dir_name();//get the path of home by using getcwd (i.e. path of folder where application was launched)
            string path = string(path1);
            back.push(pathstore[present]);
            list_file(path);
            present=0;
        }
        else if(c == ':') //if ":" is entered then command mode is to be entered
        {
             normal_mode=0;
             printf("\33[%d;%dH%s",rows,0,":");
             string path_present=pathstore[present];
             //cout<<path_present;
             //cout << "This is command mode"; 
             commands(path_present);
            // cout << "exited";
        }
       }
        
    }
    printf("\n");
}
