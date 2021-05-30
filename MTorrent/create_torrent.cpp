#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/sha.h>
using namespace std;
string sha(vector<char>& buffer,size_t bytes_read)
{
	unsigned char result[SHA_DIGEST_LENGTH];
	char buf[SHA_DIGEST_LENGTH*2];
	char* buffert;
	buffert=reinterpret_cast<char *> (buffer.data());
	SHA1((unsigned char *)buffert,bytes_read,result);

	for(int i=0;i<SHA_DIGEST_LENGTH;i++)
	{
		sprintf((char*)&(buf[i*2]),"%02x", result[i]);
	}
	string str(buf);
	return str;
}
void create_mtorrent(string tracker1_ip_and_port,string tracker2_ip_and_port,string file_path,string torrent_file)
{
	ifstream actual_file(file_path);
	if(!actual_file)
	{
		cout<<"Error in opening file to be hashed for mtorrent!"<<endl;
		return;
	}
	// torrent_file="./"+torrent_file;

	// cout << "torrent_file=> "<<torrent_file <<endl;
	ofstream new_torrent_file(torrent_file);
	if(!new_torrent_file)
	{
		cout<<"opening Torrent file failed! "<<endl;
		return;
	}
	new_torrent_file<<tracker1_ip_and_port<<"\n";
	new_torrent_file<<tracker2_ip_and_port<<"\n";
	new_torrent_file<<file_path<<"\n";

	 //tell size of file
	streampos fsize=0;
	fsize=actual_file.tellg();
	actual_file.seekg(0,ios::end);
	fsize=actual_file.tellg() - fsize;
	new_torrent_file<<fsize<<"\n";
	actual_file.seekg(0);	
	//read file in chunks of 512KB and make hash of it, append all the hashes of all the chunks
	vector<char> buffer(512*1024,0);
	// char buffer[512*1024];
	string hash_string="";
	// cout << "hashing"<<endl;
	while(actual_file.read(buffer.data(),sizeof(buffer)))
	{
		size_t bytes_read=actual_file.gcount();
		string temp=sha(buffer,bytes_read);
		// cout << temp <<endl;
		hash_string+=temp;
	}
	// cout << "Appending hash string to torrent_file= "<<hash_string<<endl;
	new_torrent_file<<hash_string<<"\n";
	actual_file.close();
	new_torrent_file.close();
	return;
}
// int main()
// {
// 	// string tracker1_ip_and_port,string tracker2_ip_and_port,string file_path,string torrent_file
// 	create_mtorrent("123.123.123.123:8080","121.121.121.121:8080","/home/prabha/IIITH/Sem_1_IIITH/Operating_Systems/Bit-Torrent/test_new.pdf","test.mtorrent");
// 	return 0;
// }