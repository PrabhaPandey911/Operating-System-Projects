//Name: Prabha Pandey
//Roll No. : 2018201053
#include <dirent.h>
#include "list_files.h"
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include "gotoxy.h"
using namespace std;
extern vector<string> pathstore;
extern int present;
extern int rows;
extern int normal_mode;

int regular_file(string path)//to check if the file clicked is actually a file or a directory
{
    struct stat ex1;
    stat(path.c_str(), &ex1);
    return S_ISREG(ex1.st_mode);
}

// void delete_dir(string path1)
// {
// 		DIR *dir;
// 		struct dirent *myfile;
// 		dir = opendir(path1.c_str());
// 		if(dir)
// 		{
// 			while((myfile = readdir(dir)) != NULL) //reading the files in the directory
// 			{
// 				string fname = string(myfile->d_name);
// 				string path2=path1;
// 				path2.append("/");
// 				path2.append(fname);
// 				int x =  regular_file(path2);
// 				if(x==0)
// 				{
// 					delete_dir(path2);
// 					//directory
// 				}
// 				else
// 				{
// 					remove(path1.c_str());
// 					//file
// 				}
// 			}
// 		}

// }
vector<string> split_string(string s)
{
	vector<string> v;
	istringstream ss(s);
	string word;
	while(ss >> word)
	{
		v.push_back(word);	
	}
	return v;
}
void options(char * arr, string path)
{
	
	string s=string(arr);
	vector<string> v=split_string(s);
	if(v[0]=="create_dir")
	{
		string dir_name=v[1];
		string destination=v[2];
		string path1=path;
		if(destination==".")
		{
			string opath=path1;
			path1.append("/");
			path1.append(dir_name);
			int res=mkdir(path1.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		else
		{
			destination=destination.substr(1);
			string opath=path1;
			path1.append("/");
			path1.append(destination);
			path1.append("/");
			path1.append(dir_name);
			int res=mkdir(path1.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		return;
	}
	if(v[0]=="create_file")
	{
		string dir_name=v[1];
		string destination=v[2];
		string path1=path;
		if(destination==".")
		{
			string opath=path1;
			path1.append("/");
		    path1.append(dir_name);
		    int res=open(path1.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		else
		{
			destination=destination.substr(1);
			string opath=path1;
			path1.append("/");
			path1.append(destination);
		    path1.append("/");
		    path1.append(dir_name);
		    int res=open(path1.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		return;
	}
	if(v[0]=="rename")  
	{
		string old_name=v[1];
		string new_name=v[2];
		// string opath=path.append(old_name);
		// string npath=path.append(new_name);
		//int result=rename(opath.c_str(),npath.c_str());
		//gotoxy(1,rows);
		int result=rename(old_name.c_str(),new_name.c_str());
		return;
	}
	if(v[0]=="delete_file")
	{
		string file_name=v[1];
		string path1=path;
		path1.append("/");
		path1.append(file_name);
		int result=remove(path1.c_str());
		gotoxy(1,rows);
		return;
	}
	// if(v[0]=="delete_dir")
	// {
	// 	string dir_name = v[1];
	// 	string path1=path;
	// 	path1.append("/");
	// 	path1.append(dir_name);
	// 	delete_dir(path1);
	// }

}
int commands(string path)
{
	char arr[10000];
	char c=getchar();
	int i=0;
	while(1)
	{
		if(c==0x001b)
		{
			normal_mode=1;
			return 0;
		}
		if(c!='\n')
		{		
			if(c==0x7f)
			{
				printf("\r                                                                                 ");
				gotoxy(0,rows);
				
				printf(":");
				commands(path);
			}
			arr[i]=c;
			cout << c;
		}
		else
		{	
			arr[i]='\0';
			printf("\r                                                                                 ");
			//gotoxy(0,rows);
			options(arr,path);
			printf("\r                                                                                 ");
			gotoxy(0,rows);
			printf(":");
			commands(path);
		}
		c=getchar();
		i++;		
	}
	return 0;

}
