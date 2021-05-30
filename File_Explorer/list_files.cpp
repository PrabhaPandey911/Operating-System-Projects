//Name: Prabha Pandey
//Roll No. : 2018201053
#include <algorithm>
#include <sys/types.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <vector>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "static_variables.h"
#include "gotoxy.h"
#include <string>
#include <chrono>
#include <thread>
#include <syslog.h>
#include <cstring>
using namespace std;
extern int file_count;
vector<string> pathstore; //to store the path
vector<std::vector<string>> navigate;
int present;//pointing at index of pathstore
int list_file(string path)
{   gotoxy(0,0);//go to top-left corner of screen
	//cout << path;
	DIR *dir;
	struct dirent *myfile;
	struct stat mystat;
	struct group *grp;
	struct passwd *pwd;
	
	extern std::vector<string> file_name;
	extern std::vector<string> store;
	
	file_count=0;
	
	dir = opendir(path.c_str());
	if(dir)
	{	file_name.clear();//clear the file_name vector
		store.clear();//clear the store vector everytime ls is called so as to start afresh for that folder
		int i=0;	
		
		while((myfile = readdir(dir)) != NULL) //reading the files in the directory
		{
			file_count++;//counting #files
			//getting file name
			string fname = string(myfile->d_name);
			string pathnew = string(path) + "/" + fname;
			string s;//making string to store each line that would be printed on the terminal to be pushed into the store vector 
			stat(pathnew.c_str(),&mystat);
			//getting group to which it belongs
			grp = getgrgid(mystat.st_gid);
			printf("%s ", grp->gr_name);
			s.append(grp->gr_name);
			s.append("\t");
			printf("\t");
			//getting user details 
			pwd = getpwuid(mystat.st_uid);
			printf("%s ", pwd->pw_name);
			s.append(pwd->pw_name);
			s.append("\t");
			printf("\t");
		//getting size
			printf("%zu ",mystat.st_size);
			s += std::to_string(mystat.st_size);
			s.append("\t");
			printf("\t");
			
			//getting ownership details
			const char * ownership[10];

			ownership[0]=(S_ISDIR(mystat.st_mode)) ? "d" : "-";
			ownership[1]= (mystat.st_mode & S_IRUSR) ? "r" : "-";
			ownership[2]= (mystat.st_mode & S_IWUSR) ? "w" : "-";
			ownership[3]= (mystat.st_mode & S_IXUSR) ? "x" : "-";
			ownership[4]= (mystat.st_mode & S_IRGRP) ? "r" : "-";
			ownership[5]= (mystat.st_mode & S_IWGRP) ? "w" : "-";
			ownership[6]= (mystat.st_mode & S_IXGRP) ? "x" : "-";
			ownership[7]= (mystat.st_mode & S_IROTH) ? "r" : "-";
			ownership[8]= (mystat.st_mode & S_IWOTH) ? "w" : "-";
			ownership[9]= (mystat.st_mode & S_IXOTH) ? "x" : "-";
			for(int i=0;i<10;i++)
			{
				printf("%s", ownership[i]);
				s.append(ownership[i]);
			}
			//printing file name
			printf("\t");
			s.append("\t");
			printf("%s ",myfile->d_name);
			string fn(myfile->d_name);
			s.append(fn);
			s.append("\t");
			printf("\t");			
			file_name.push_back(fn);
			printf("\t");
			//last modified time
			printf("%s" , ctime(&mystat.st_mtime));
			s.append(ctime(&mystat.st_mtime));
			store.push_back(s);	
			i++;
		}
		gotoxy(0,0);//going to the starting of screen again
		if(find(pathstore.begin(),pathstore.end(),path)==pathstore.end())//if path is not in pathstore vector then only push it in it(will help in left right home and backspace traversal)
		{
			pathstore.push_back(path);
        	present++;
		}
		
		navigate.push_back(store);
		
		closedir(dir);
	}
	return 0;
}