#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<netdb.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
using namespace std;
#define print(a) cout<<(#a)<<" = "<<a<<"\n";
#define maxn 1000

string tost( int in)
{
	char ans[10];
	sprintf(ans,"%d",in);
	return ans;
}

void *talktoclient( void *sockID)
{
	int new_sock = (long long)sockID;

	string prefix= tost( new_sock);
	char input[maxn];
	cout<<" ********* Acceted new request  from  "<<prefix<<"  *********** \n\n";
       
	while(1)
	{
		cout<<" *******\n";

	     int gs = recv ( new_sock , input , maxn , 0);  

	     string got = string( input);

	     cout<<"Got is "<<got<<"   and got size is "<<gs<<endl;

	     string ans = prefix +" "+" sent: "+got;

	     printf("About to send: %s  \n",ans.c_str() );

	     //if( got =="exit")
	     	//ans= prefix + "  

	     ans = "hi";

         int sent = send( new_sock ,ans.c_str(), ans.size(), 0 );
         printf("Sent size is %d \n",sent);

    }

     close( new_sock);
     
	cout<<" ********* Acceted new request  from  "<<prefix<<"  *********** \n\n";
}



int main(int argc, char *argv[] )
{

  //argc should be 2 for correct execution
  if ( argc != 2 )
  {
      printf( "usage: %s port_number \n", argv[0] );
	    return 0;
  }

  printf("Port Number is %s \n",argv[1] );

  struct  addrinfo host_info;
  struct  addrinfo  *host_list;//host_pointer is to store info 

  //poplulate pointer
  memset( &host_info , 0 ,sizeof(host_info));

  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags = AI_PASSIVE;

  //server k lia localohost hi lena hai 
  int status = getaddrinfo( "localhost", argv[1], &host_info ,&host_list);

  if( status!=0)
  {
    cout<<"Error while getting addinfo "<<gai_strerror(status)<<endl;
  }
 
 //cout<<"Now creating socket\n";

  int sockid = socket(host_list->ai_family , host_list->ai_socktype,
		      host_list->ai_protocol);
  
 if( sockid == -1 )
   cout<<"ERROR while creating socket \n";


  int yes = 1;
  // reuse if previous version was running due to xyz
  status = setsockopt( sockid , SOL_SOCKET , SO_REUSEADDR , &yes , sizeof(int) );

  //bind here 
  status = bind( sockid , host_list->ai_addr , host_list->ai_addrlen );

  if( status == -1 )
   cout<<" some error while BINDING"<<gai_strerror(status)<<endl;

   //without bind() , oS will choose any port 
   //similarly ,on client side , we can use bind() to use a specific port

   int backlog = 10;//number of request put in queue
   status = listen( sockid , backlog);

   if( status == -1 )
   cout<<" some error while Listening"<<gai_strerror(status)<<endl;


   struct sockaddr_storage their_addr;
   socklen_t addr_size = sizeof(their_addr);

   //listen
   cout<<" **********  Setup completed and now listening  ********\n\n\n\n";
   //since on clinet side ,we are always creating a socket  ( BAD design lekin chalega )
   //to send , always ecpect server to create a new socket to recive and process data

  while(1)
  {
      long long  new_sock = accept(sockid, (struct sockaddr *)&their_addr, &addr_size);

      if( new_sock == -1)
       cout<<" some error while  creating a "<<gai_strerror(status)<<endl;
      else
      {
			pthread_t tid;
			pthread_create(&tid, NULL, talktoclient, (void*)new_sock);
			pthread_detach(tid);
      }

  }

         
  return 0;
}
