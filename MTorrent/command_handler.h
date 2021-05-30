#include <iostream>
#include "create_torrent.h"
#include <fstream>
#include <openssl/sha.h>
#include <string>
#include <sys/types.h>  //for socket        
#include <sys/socket.h> //for socket
using namespace std;
void share(int sock_fd,string tracker1_ip_and_port,string tracker2_ip_and_port,string client_ip_and_port,string cmd,string s);
void get(int sock_fd,string tracker1_ip_and_port,string tracker2_ip_and_port,string client_ip_and_port,string cmd,string s);
void command_handler(int sock_fd,string tracker1_ip_and_port,string tracker2_ip_and_port,string client_ip_and_port,string cmd,string s);