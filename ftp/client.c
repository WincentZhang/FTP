#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<malloc.h>
#include<fcntl.h>
#include<assert.h>
#include "tmax.h" 
#include "packet.h"
char command[50];
//struct sockaddr_in cliaddr;
int main(int argc,char* argv[]){
	unsigned int port=21;
int result=connectServer(&s,argv[1],port);
strcpy(local,argv[1]);
	printf("the connection result is %d",result);
    recvControl(1,&s);
sendUser(senddata,&s);
  recvControl(1,&s);
sendPass(senddata,&s);
recvControl(1,&s);
 while(1){

//printf("\n input the control command");
//while(getchar()!='\n');
 printf("\n input the control command   ");
scanf("%[^\n]",command);
while(getchar()!='\n');

  controlCom(command);
//memset(command,0,sizeof(command));


 }
	getchar();
	return 0;
		
}

