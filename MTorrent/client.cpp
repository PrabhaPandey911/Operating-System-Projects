#include <iostream>
#include <string>
#include <sys/types.h>  //for socket        
#include <sys/socket.h> //for socket
#include <netinet/in.h> //for inet_addr
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h> //for memset
#include "command_handler.cpp"
#include "peer_handler.cpp"
#include <fstream>
#include <openssl/sha.h>
#include <thread>
using namespace std;


int main(int argc,char* argv[])
{
	string client_ip_and_port=argv[1];
	string tracker1_ip_and_port=argv[2];
	string tracker2_ip_and_port=argv[3];
	string log_file_path=argv[4];

	// struct sockaddr_in address;
	int sock_fd;
	struct sockaddr_in serv_addr;

	if((sock_fd = socket(AF_INET, SOCK_STREAM,0)) < 0)
	{
		cout << "Error in creating socket"<<endl;
		exit(1); 
	}
	int opt=1;
	//to be able to reuse address and port
	if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(1);
	}
	cout<<"Client socket created!"<<endl;
	string tracker1_ip=tracker1_ip_and_port.substr(0,tracker1_ip_and_port.find(":"));
	string tracker1_port = tracker1_ip_and_port.substr(tracker1_ip_and_port.find(":")+1);
	int port=stoi(tracker1_port);
	memset(&serv_addr,'0',sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; //specifying IPv4
	serv_addr.sin_port = htons(port); //connection socket to port
	serv_addr.sin_addr.s_addr = inet_addr(tracker1_ip.c_str()); //specifying the ip address to which this socket must get connected to(in this case IP address of server).
	if(connect(sock_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
	{
		cout<<"connection failed!!"<<endl;
		exit(1);
	}
	cout<<"connected to server!"<<endl;

	//thread that handles requests sent by other clients for download.
	thread pr_handler(peer_handler);
	pr_handler.detach();

	cout<<endl;
	cout << "Type any one of the following Commands: "<<endl;
	cout << "1. \"share <local file path> <filename>.mtorrent\""<<endl;
	cout << "2. \"get <path to .mtorrent file> <destination path>\""<<endl;
	cout << "3. \"remove <filename.mtorrent>\""<<endl;
	while(1)
	{
		cout<<"Command: ";
		string s;
		getline(cin,s);
		string cmd=s.substr(0,s.find(" "));
		// cout << "In client.c "<<client_ip_and_port<<endl;
		//thread that handles commands //DOUBT: why to create thread for each command executed by the client.
		// thread cmd_handler(command_handler,sock_fd,tracker1_ip_and_port,tracker2_ip_and_port,client_ip_and_port,cmd,s);
		// cmd_handler.detach();
		command_handler(sock_fd,tracker1_ip_and_port,tracker2_ip_and_port,client_ip_and_port,cmd,s);

	}
}