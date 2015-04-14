#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<netdb.h>
using namespace std;
#define print(a) cout<<(#a)<<" = "<<a<<"\n";

#define server_count 1

string server_ip[] = {"localhost","localhost","localhost"};
string server_port[] = { "5556", "5557" , "5558"};


void solve(int id )//assign task to server {id}, and find result 
{
  //Now socket creation .. etc 

  struct  addrinfo host_info;
  struct  addrinfo  *host_list;//host_pointer is to store info 

   //poplulate pointer
  memset( &host_info , 0 ,sizeof(host_info));

  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  int status = getaddrinfo( server_ip[id].c_str(), server_port[id].c_str() ,&host_info , &host_list);

  if( status!=0)
  {
    cout<<"Error while getting addinfo "<<gai_strerror(status)<<endl;
  }


  int sockid = socket(host_list->ai_family , host_list->ai_socktype,
		      host_list->ai_protocol);
  
                      
  if( sockid == -1 )
   cout<<"ERROR while creating socket \n";

  status = connect(sockid , host_list->ai_addr , host_list->ai_addrlen );

  if( status == -1 )
   cout<<" some error while connecting "<<gai_strerror(status)<<endl;

  while(1)
  {
    string to_send="hello";

    cin>>to_send;
  
	  //send 
    send( sockid , to_send.c_str(), to_send.size() , 0 );

   
    char incoming[5000];
    recv ( sockid , incoming , 5000 , 0);

    string ret = incoming;

    print(to_send);
    print(ret);
 }

  close(sockid);
  return;       
}

int main()
{
 
  solve(0);

  return 0;
}

