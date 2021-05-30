#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/sha.h>
using namespace std;
string sha(vector<char>& buffer,size_t bytes_read);
void create_mtorrent(string tracker1_ip_and_port,string tracker2_ip_and_port,string file_path,string torrent_file);