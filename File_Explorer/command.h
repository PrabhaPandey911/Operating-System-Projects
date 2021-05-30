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

int is_regular_file(string path);
void delete_dir(string path1);

vector<string> split_string(string s);
void options(char * arr, string path);
int commands(string path);

