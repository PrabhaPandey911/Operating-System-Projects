#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;
map<string,vector<string>> seeders;


//Loading Seeders from file to map whenever tracker wakes up
void loadSeeders(string file_path)
{
	ifstream file(file_path);
	if(!file)
	{
		cout << "Error in opening the seeder file\n" ;
		return;
	}
	string line;
	// cout << "loading seeders:"<<endl;
	while(getline(file,line))
	{
		// cout << "line from file: "<<line<<endl; 
		int index=line.find("|");
		string hash=line.substr(0,index);
		string peers=line.substr(index+1);
		index=0;
		vector<string> vector_peers;
		while(peers[index]!='\0')
		{
			if(peers[index]==',')
			{	
				vector_peers.push_back(peers.substr(0,index));
				peers=peers.substr(index+1);
				index=0;
			}
			else
				index++;
		}

		seeders.insert({hash,vector_peers});
		vector_peers.clear();
	}
	file.close();
	return;
}

void share(int clientSocket,string seederlist_file)
{
	char buffer[1024];
	bzero(buffer,1024);
	int read_size=read(clientSocket,buffer,1024);
	string hash(buffer);
	cout << "double hash=> "<<hash<<endl;
	bzero(buffer,1024);
	char bufip[1024];
	bzero(bufip,1024);
	// cout << "hello there"<<endl;
	read_size=read(clientSocket,bufip,1024);
	string client_ip_and_port(bufip);
	cout <<"client ip and port=> "<<client_ip_and_port<<endl;
	if(seeders.find(hash)==seeders.end())
	{
		vector<string> ip_and_port;
		ip_and_port.push_back(client_ip_and_port);
		seeders.insert({hash,ip_and_port});
		ofstream seederFile(seederlist_file,ios::app | ios::out);
		if(!seederFile)
		{
			cout << "Can't Open seederlist_file" <<endl;
			return;
		}
		string input=hash+"|"+client_ip_and_port+",";
		seederFile<<input<<endl;
		seederFile.close();
	}
	else
	{
		int flag=0;
		auto f=seeders.find(hash);

		for(int i=0;i<f->second.size();i++)
		{
			if(f->second[i]==client_ip_and_port)
			{
				flag=1;//this Client IP and port already exists in the file
				break;
			}
		}
		if(flag==0)
		{
			seeders[hash].push_back(client_ip_and_port);
			ofstream seederFile(seederlist_file,std::ios::out | std::ios::trunc);
			if(!seederFile)
			{
				cout << "Can't open seederlist_file"<<endl;
				return;
			}
			for(auto i=seeders.begin(); i!=seeders.end(); i++)
			{
				seederFile<<i->first<<"|";
				for(int j=0;j<i->second.size();j++)
				{
					seederFile<<i->second[j]<<",";
				}
				seederFile<<"\n";
			}
			seederFile.close();

		}
		
	}
	bzero(buffer,1024);
	// cout << "successfully updated seeder list file!"<<endl;
	return;
}
void get(int clientSocket,string seederlist_file)
{
	char bufget[2000];
	bzero(bufget,2000);
	int read_size=read(clientSocket,bufget,2000);
	string doubleHash(bufget);
	cout<<"double hash: "<<doubleHash<<endl;
	auto fd=seeders.find(doubleHash);
	if(fd!=seeders.end())
	{
		char* bufpeer={0};
		for(auto i=fd->second.begin();i!=fd->second.end();i++)
		{
			cout<<"peer "<<*i<<endl;
			string peerList=*i;
			bufpeer=const_cast<char*> (peerList.c_str());
			send(clientSocket,bufpeer,strlen(bufpeer),0);
			bzero(bufpeer,strlen(bufpeer));
		}
		bzero(bufpeer,strlen(bufpeer));
		string end="Peer List Sent";
		bufpeer=const_cast<char*> (end.c_str());
		send(clientSocket,bufpeer,strlen(bufpeer),0);
		cout<< "Peer List sent"<<endl;
	}
	else
	{
		cout << "File Not in database"<<endl;
	}
	return;
}
void handleClientCommands(int clientSocket,string seederlist_file)
{
	char readBuf[1024]={0};
	bzero(readBuf,1024);
	while(int readSize=read(clientSocket,readBuf,1024))
	{
		if(readSize<0){
			cout << "Error in reading "<<endl;
			continue;
		}
		string command(readBuf);
		bzero(readBuf,1024);

		// cout<<"command received: "<<command<<endl;
		if(command=="share")
			share(clientSocket,seederlist_file);
		if(command=="get")
			get(clientSocket,seederlist_file);
	}
	return ;
}
int main(int argc,char* argv[])
{
	string tracker1_ip_and_port=argv[1];
	string tracker2_ip_and_port=argv[2];
	string seederlist_file = argv[3];
	string logfile_path = argv[4];

	//loading the data from seeder file to seeders map.
	loadSeeders(seederlist_file);

	//getting ip and port of tracker1 for making it a server.(socket programming)
	string tracker1_ip = tracker1_ip_and_port.substr(0,tracker1_ip_and_port.find(":"));
	string tracker1_port = tracker1_ip_and_port.substr(tracker1_ip_and_port.find(":")+1);

	//socket programming...
	int server_fd;
	struct sockaddr_in address;
	int opt=1;
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	if(server_fd<0)
	{
		cout<<"Error in creating server socket!"<<endl;
		exit(1);
	}
	cout << "Server socket created successfully!" <<endl;

	int port=stoi(tracker1_port.c_str());
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(tracker1_ip.c_str());

	int ret=bind(server_fd,(struct sockaddr*)&address,sizeof(address));
	if(ret<0)
	{
		cout << "Error in binding!"<<endl;
		exit(1);
	}
	cout << "Bound to port "<< tracker1_port <<endl;
	if(listen(server_fd,10)==0)
		cout << "Listening..."<<endl;
	else
		cout << "Not able to listen."<<endl;
	int clientSocket;
	struct sockaddr_in newAddress;
	socklen_t address_size;
	while(1)
	{
		clientSocket=accept(server_fd,(struct sockaddr*)&newAddress, (socklen_t*)&address_size);
		if(clientSocket<0)
			cout << "Couldn't accept the request!"<<endl;
		else
		{
			thread clientTread(handleClientCommands,clientSocket,seederlist_file);
			clientTread.detach();
			cout << "Thread handler assigned to client!"<<endl;
			cout << "Listening again.."<<endl;
		}
	}
	return 0;
	
}