#include <iostream>
#include "create_torrent.cpp"
// #include "sha.cpp"
#include <fstream>
#include <openssl/sha.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>  //for socket        
#include <sys/socket.h> //for socket
using namespace std;

void share(int sock_fd,string tracker1_ip_and_port,string tracker2_ip_and_port,string client_ip_and_port,string cmd,string s)
{
	send(sock_fd,cmd.c_str(),strlen(cmd.c_str()),0);
	string remcmd=s.substr(s.find(" ")+1);
	string file_path=remcmd.substr(0,remcmd.find(" "));
	string torrent_file=remcmd.substr(remcmd.find(" ")+1);
	create_mtorrent(tracker1_ip_and_port,tracker2_ip_and_port,file_path,torrent_file);

	ifstream mtorrent_file(torrent_file);
	if(!mtorrent_file)
	{
		cout << "couldn't open mtorrent file!"<<endl;
		exit(1);
	}
	// double hash of the hash in torrent
	string hash;
	string temp;
	int i=4;
	while(i!=0)
	{
		getline(mtorrent_file,temp);
		i--;
	}
	while(getline(mtorrent_file,temp))
	{
		hash+=temp;
	}
	

	unsigned char result[SHA_DIGEST_LENGTH];
	char buf[SHA_DIGEST_LENGTH*2];

	SHA1((unsigned char *)hash.c_str(), strlen((char *)hash.c_str()),result);
	bzero(buf,SHA_DIGEST_LENGTH*2);

	for(int i=0;i<SHA_DIGEST_LENGTH;i++)
	{
		sprintf((char*)&(buf[i*2]), "%02x", result[i]);
	}
	
	//sending Double hash to tracker
	if(send(sock_fd,buf,strlen(buf),0))
		cout << "Double hash sent to tracker successfully! "<<endl;
	else
		cout << "Double hash could not be sent!"<<endl;
	
	
	bzero(buf,SHA_DIGEST_LENGTH*2);
	
	//sending IP and port of client
	// cout <<"client_ip_and_port: "<<client_ip_and_port<<endl;
	char* hello=const_cast<char*>(client_ip_and_port.c_str());
	// cout << "hello: "<<hello<<endl;
	if(send(sock_fd,hello,strlen(hello),0))
		cout << "sent client IP and port "<<endl;
	else
		cout << "could not send IP and port"<<endl;
}
 
void get(int sock_fd,string tracker1_ip_and_port,string tracker2_ip_and_port,string client_ip_and_port,string cmd,string s)
{
	send(sock_fd,cmd.c_str(),strlen(cmd.c_str()),0);
	string remcmd=s.substr(s.find(" ")+1);
	string torrent_path=remcmd.substr(0,remcmd.find(" "));
	string download_file=remcmd.substr(remcmd.find(" ")+1);
	ifstream mtorrent_file(torrent_path);
	if(!mtorrent_file)
	{
		cout << "couldn't open torrent file"<<endl;
		exit(1);
	}
	//double hash of the hash in torrent
	string hash;
	string temp;
	int i=4;
	while(i!=0)
	{
		getline(mtorrent_file,temp);
		i--;
	}
	while(getline(mtorrent_file,temp))
	{
		hash+=temp;
	}
	// cout<<"hash: "<<hash<<endl;
	unsigned char result[SHA_DIGEST_LENGTH];
	char buf[SHA_DIGEST_LENGTH*2];
	SHA1((unsigned char *)hash.c_str(), strlen((char *)hash.c_str()),result);
	bzero(buf,SHA_DIGEST_LENGTH*2);

	for(int i=0;i<SHA_DIGEST_LENGTH;i++)
	{
		sprintf((char*)&(buf[i*2]), "%02x", result[i]);
	}
	// cout << "double hash: "<<buf<<endl;
	//sending Double hash to tracker
	if(send(sock_fd,buf,strlen(buf),0))
		cout << "Double hash sent to tracker successfully! "<<endl;
	else
		cout << "Double hash could not be sent!"<<endl;
	
	//receive the list of IP addresses...
	char ips[2000];
	bzero(ips,2000);
	vector<string> IPs;
	while(1)
	{
		read(sock_fd,ips,2000);
		string ips_having_file(ips);
		// cout << "IP received: "<<ips_having_file<<endl;
		if(ips_having_file=="Peer List Sent")
			break;
		IPs.push_back(ips_having_file);
		bzero(ips,2000);
	}
	
	cout<<"IP addresses having file: "<<endl;
	for(int i=0;i<IPs.size();i++)
	{
		cout << IPs[i] << endl;
	}
}

void command_handler(int sock_fd,string tracker1_ip_and_port,string tracker2_ip_and_port,string client_ip_and_port,string cmd,string s)
{
	// cout << "In command_handler"<<client_ip_and_port<<endl;
	if(cmd=="share")
		share(sock_fd,tracker1_ip_and_port,tracker2_ip_and_port,client_ip_and_port,cmd,s);
	if(cmd=="get")
	{
		get(sock_fd,tracker1_ip_and_port,tracker2_ip_and_port,client_ip_and_port,cmd,s);
	}
	if(cmd=="remove")
	{

	}
}