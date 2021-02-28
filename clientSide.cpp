#include <sys/types.h>                                                                                                 
#include <sys/socket.h> 
#include <netinet/in.h>             
#include <arpa/inet.h> 
#include <unistd.h>
#include <netdb.h>      
#include <sys/time.h>   
#include <sys/wait.h>                                  
#include<bits/stdc++.h> 
using namespace std;

int main(int argc,char *argv[]){

    if(argc!=3){
        cout<<"Missing hostname and port number";
        exit(0);
    }
    char* serverName=argv[1];
    int port=atoi(argv[2]);
    char message[1500];

    struct hostent* host=gethostbyname(serverName);
    sockaddr_in clientSocket;
    clientSocket.sin_family=AF_INET;//ipv4
    clientSocket.sin_port=htons(port);
    clientSocket.sin_addr=**(struct in_addr **)host->h_addr_list;


    int clientSocketID=socket(AF_INET,SOCK_STREAM,0);


    int status=connect(clientSocketID,(struct sockaddr *)&clientSocket,sizeof(clientSocket));
    if(status<0){
        cout<<"Connecting error"<<endl;
        exit(0);
    }
    cout<<"Connected successfully!"<<endl;


    while(true){
        string data;
        cout<<"Client :";
        getline(cin, data);
        strcpy(message,data.c_str());
        send(clientSocketID,(char *) &message,sizeof(message),0);
        if(data=="exit"){
            cout<<"Session terminated"<<endl;
            break;
        }
        

        cout<<"Server: ";
        recv(clientSocketID,(char *)&message,sizeof(message),0);
        if(!strcmp(message,"exit")){
            cout<<"Session terminated"<<endl;
            break;
        }
        cout<<message<<endl;        
    }
    close(clientSocketID);
    return 0;
}