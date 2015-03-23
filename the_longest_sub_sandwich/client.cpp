#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<netdb.h>
#include"common.h"
using namespace std;
#define print(a) cout<<(#a)<<" = "<<a<<"\n";

#define server_count 3
string server_ip[] = {"localhost","localhost","localhost"};
string server_port[] = { "5556", "5557" , "5558"};

#define maxn  10000
ll input[maxn];
ll rt_ans[maxn];
ll lf_ans[maxn];
int n;

//TODO: do it using pthreads
void solve(int id )//assign task to server {id}, and find result 
{
  // TODO : for simplicity assume that n%server_count == 0

  int st = id*(n/server_count) , ed= (id+1)*(n/server_count) - 1;

  vector<ll> curr;

  for(int i=st;i<=ed;i++)
 	curr.push_back( input[i] );

  string to_send = incode_input( curr);

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
  else
  {
      cout<<"Got addinfo successfully "<<endl;
  }

 cout<<"Now creating socket\n";

 int sockid = socket(host_list->ai_family , host_list->ai_socktype,
		      host_list->ai_protocol);
  
                      
  if( sockid == -1 )
   cout<<"ERROR while creating socket \n";
 else
   print(sockid);


  cout<<"Now trying to connect\n";

  status = connect(sockid , host_list->ai_addr , host_list->ai_addrlen );

  if( status == -1 )
   cout<<" some error while connecting "<<gai_strerror(status)<<endl;
  else
   cout<<"connected succesfully \n";

  while(1)
     {
 	//send 
         int sent = send( sockid , to_send.c_str(), to_send.size() , 0 );

         print(sent);
         
         char incoming[5000];
         int rec = recv ( sockid , incoming , 5000 , 0);
 
         string ret = incoming;

         pair<ll,ll> ans  = decode_result( ret );

         lf_ans[id]=ans.first;
         rt_ans[id]=ans.second;

         return;//NO pthread here , so have to return 

      }
     
}

int main()
{
 
 while(1)
  {
  
      cin>>n;
 
      for(int i=0;i<n;i++)
 	cin>>input[i];

      for(int i=0;i<server_count; i++)
	solve(i);

     ll ans = 0 ;

      ll tp = 0;

      for(int i=0;i<server_count;i++)
	{
          tp+=lf_ans[i];
          ans = max( ans , tp );
        }

	print(tp);

         tp = 0 ;

       for(int i= server_count -1 ; i >=0 ; i--)
         {

            tp+=rt_ans[i];

             ans = max( ans , tp );
         }

	print(tp);

        cout<<" final asnwer is "<<ans<<endl;

    }
 
  return 0;
}

