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
    if(argc!=2){
        cerr<<"Missing port number!"<<endl;
        exit(0);
    }
    int port=atoi(argv[1]);

    char message[1500];
    pid_t childPID;


    sockaddr_in serverSocket;
    bzero((char *)&serverSocket,sizeof(serverSocket));
    serverSocket.sin_family=AF_INET;
    serverSocket.sin_port=htons(port);
    serverSocket.sin_addr.s_addr=htonl(INADDR_ANY);


    int serverSocketID=socket(AF_INET,SOCK_STREAM,0);
    if(serverSocketID<0){
        cerr<<"Not successful"<<endl;
        exit(0);
    }
    


    int bindStatus=bind(serverSocketID,(struct sockaddr *)&serverSocket,sizeof(serverSocket));
    if(bindStatus<0){
        cerr<<"Not successful binding"<<endl;
        exit(0);
    }
    
    cout<<"Waiting for client to connect..."<<endl;

    if(listen(serverSocketID,1)==0){
        printf("Listening successful\n");
    }
    else{
        printf("Listening error\n");
    }
    


    sockaddr_in newSocket;
    socklen_t newSocketLen=sizeof(newSocket);
    int newSocketID;
    

    while(true){
        newSocketID=accept(serverSocketID,(sockaddr *) &newSocket,&newSocketLen);
        if(newSocketID<0){
            cerr<<"Unsuccessful accept()"<<endl;
            exit(0);
        }
        cout<<"Client connection successful..."<<endl;


        cout<<"Waiting for a message from client"<<endl;
        childPID=fork();
        if(childPID==0){//child process        
            close(serverSocketID);

			while(1){
				recv(newSocketID, message, sizeof(message), 0);
				if(strcmp(message,"exit")==0){
                    cout<<"Session terminated"<<endl;
                    break;
                }
                else{
					cout<<"Client : "<<message<<endl;
                    string data;
                    cout<<"Server : ";
                    getline(cin, data);
                    strcpy(message,data.c_str());
                    send(newSocketID, message, sizeof(message), 0);
                    if(data=="exit"){
                        cout<<"Session terminated"<<endl;
                        break;
                    }
					bzero(message, sizeof(message));
				}
			}         
        }
    }
    close(newSocketID);
    close(serverSocketID);   
    return 0;
}   