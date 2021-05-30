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
//extern int file_count;
int list_file(string path);
