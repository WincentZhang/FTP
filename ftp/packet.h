#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<malloc.h>
#include<fcntl.h>
#include "file.h"
#include "connect.h"
#include "password.h"
int s;
int lenlen;
int sp=0;
int activesock=0;
int newsock;
int mode=0;
char packbuf[8000];
char packbuf2[2000];
char senddata[2000];
char passport[1000];
char local[50];
struct sockaddr_in cliaddr;
struct Packet{
           char *user;
           char* pass;
           char* ls;
           char* pwd;
           char* a;
           char* quit;
           char* get;
           char* put;
           char* cd;
           char* md;
           char* rm;
           char* del;
           char* rn;
           char* pasv;
           char* active;
           char* r1;
           char* r2;
           }pack={"USER ","PASS ","LIST","PWD","TYPE ","QUIT","RETR ","STOR ","CWD","MKD ","RMD ","DELE ","","PASV","PORT ","RNFR ","RNTO "
           };
 int convertPort(char* p1,char* p2){     
        
    	int result=atoi(p1)*256+atoi(p2);//atoi asc2代码转换为整数型
    	return result;	
    }
int showPort(char *buf){
     char *s1=strtok(buf,",");//分解字符串为一组字符
     char *s2=strtok(NULL,",");
     int result=convertPort(s1,s2);
  return result;
}

int recvControl(int type,int *s ){
int len,len2;
	memset(packbuf,0,sizeof(packbuf));//总的作用：将已开辟内存空间 packbuf 的首 n 个字节的值设为值 0
	memset(packbuf2,0,sizeof(packbuf2));
	if(type==1){
	len=recv(*s,packbuf,8000,0);//接收数据 int recv( 指定接收端套接字描述符, 指明一个缓冲区，该缓冲区用来存放recv函数接收到的数据, 指明buf的长度, 置为0)

	printf("\n %s",packbuf);
	}
	else if(type==2){
        len=recv(*s,packbuf,2000,0);
        len2=recv(*s,packbuf2,2000,0);
		printf("\n %s",packbuf);
		printf("\n %s",packbuf2);	
	}
    return len;
}
//发包，正确格式封装，这样服务器才会响应
int sendUser(char*input,int *s){
	char* data=input;
	char name[20];
	strcpy(data,pack.user);//复制
	data+=5;
	printf("\n please input your username");
	scanf("%s",name);
	strcpy(data,name);
	data+=strlen(name);
		strcpy(data,"\r\n");
        data+=2;
      int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n user packet send error");
         return len;
	
}
int sendPass(char *input,int *s){
	memset(input,0,sizeof(input));//���
	char *data=input;
	char name[20];
        memset(name,0,sizeof(name));
	strcpy(data,pack.pass);
	data+=5;//指针平移
   printf("\n please input your password\n");
//	scanf("%s",name);
        while(strcmp(name,"")==0){
        getpasswd(name,20);
}
	strcpy(data,name);
	data+=strlen(name);
         strcpy(data,"\r\n");
         data+=2;//指针平移
 int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n password packet send error");
         return len;
}
int sendPWD(char *input,int *s){
	memset(input,0,sizeof(input));
	char* data=input;
	strcpy(data,pack.pwd);
	data+=3;//指针平移三字节，因为pwd 是三个字节
	 strcpy(data,"\r\n");
         data+=2;
         int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n pwd packet send error");
         return len;
}
int sendAs(char *input,int *s){
	memset(input,0,sizeof(input));
	char* data=input;
	strcpy(data,pack.a);
	data+=5;
	strcpy(data,"A");
	data+=1;
	 strcpy(data,"\r\n");
         data+=2;
         int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n ascii packet send error");
         return len;
}
int sendBi(char *input,int *s){//发送二进制文件
        printf("invoke");
	memset(input,0,sizeof(input));
	char* data=input;
	strcpy(data,pack.a);
	data+=5;
	strcpy(data,"I");
	data+=1;
	 strcpy(data,"\r\n");
         data+=2;
         int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n ascii packet send error");
         return len;
}
int sendQuit(char *input,int *s){
	memset(input,0,sizeof(input));
	char* data=input;
	strcpy(data,pack.quit);
	data+=4;
	 strcpy(data,"\r\n");
         data+=2;
         int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n quit packet send error");
         return len;
}
int sendCD(char*input,int *s,char *path){
	char* data=input;
	strcpy(data,pack.cd);
	data+=4;
	strcpy(data,path);
	data+=strlen(path);
		strcpy(data,"\r\n");
        data+=2;
      int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n cd packet send error");
         return len;
}
int sendRename(char* input,int *s,char* n1,char *n2){
char* data=input;
strcpy(data,pack.r1);
data+=5;
strcpy(data,n1);
data+=strlen(n1);
strcpy(data,"\r\n");
data+=2;
int len=  send(*s,input,data-input,0);
 if(len<=0)
 printf("\n cd packet send error");
recvControl(1,s);
memset(input,0,sizeof(input));
data=input;
strcpy(data,pack.r2);
data+=5;
strcpy(data,n2);
data+=strlen(n2); 
strcpy(data,"\r\n");
data+=2;
int len2=  send(*s,input,data-input,0);
 if(len2<=0)
 printf("\n cd packet send error");
recvControl(1,s);
return len2;
}
int sendMK(char*input,int *s,char *path){
	char* data=input;
	strcpy(data,pack.md);
	data+=4;
	strcpy(data,path);
	data+=strlen(path);
		strcpy(data,"\r\n");
        data+=2;
      int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n mkdir packet send error");
         return len;
}
int sendRM(char*input,int *s,char *path){
	char* data=input;
	strcpy(data,pack.rm);
	data+=4;
	strcpy(data,path);
	data+=strlen(path);
		strcpy(data,"\r\n");
        data+=2;
      int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n rmdir packet send error");
         return len;
}
int sendDel(char*input,int *s,char *path){
	char* data=input;
	strcpy(data,pack.del);
	data+=5;
	strcpy(data,path);
	data+=strlen(path);
		strcpy(data,"\r\n");
        data+=2;
      int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n delete packet send error");
         return len;
}
int sendPasv(char *input,int *s){
	memset(input,0,sizeof(input));
	char* data=input;
	strcpy(data,pack.pasv);
	data+=4;
	 strcpy(data,"\r\n");
         data+=2;
         int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n pasv packet send error");
         return len;
}
int sendActive(char *input,int *s){
   memset(input,0,sizeof(input));
   char *data=input;
   strcpy(data,pack.active);
   data+=5;
   strcpy(data,"127,0,0,1,136,246");
   data+=strlen("127,0,0,1,136,246");	
   strcpy(data,"\r\n");
   data+=2;
    int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n pasv packet send error");
         return len;
}
int sendGET(char*input,int *s,char *path){
	

	char* data=input;
	strcpy(data,pack.get);
	data+=5;
	strcpy(data,path);
	data+=strlen(path);
		strcpy(data,"\r\n");
        data+=2;
      int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n get packet send error");
         return len;
}
int sendPUT(char*input,int *s,char *path){
	char* data=input;
	strcpy(data,pack.put);
	data+=5;
	strcpy(data,path);
	data+=strlen(path);
		strcpy(data,"\r\n");
        data+=2;
      int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n put packet send error");
         return len;
}
int sendLs(char *input,int *s){
	memset(input,0,sizeof(input));
	char* data=input;
	strcpy(data,pack.ls);
	data+=4;
	 strcpy(data,"\r\n");
         data+=2;
         int len=  send(*s,input,data-input,0);
        if(len<=0)
         printf("\n ls packet send error");
         return len;
}
void recvCode(char *input){
	     char code[20];
          memset(code,0,sizeof(code));
          char * data=input;
          memcpy(code,data,3);
         if(strcmp(code,"530")==0){
            printf("\n wrong username or password");
            exit(0);
         }
         else if(strcmp(code,"550")==0){
           printf("\n such file does not exists");
            exit(0);
}     
		 else if (strcmp(code, "500") == 0) {
			 printf("\n such command not exist");
			 exit(0);
		 }
}
//跳过无用的只获得port号，拷贝到passport
void getPassivePort(int length,char *input){
	 char * data = input;
  int change=4+strlen(local)+strlen(" Entering Passive Mode  ");
  data+=change;
memcpy(passport,data,length-change-3);
}




int controlCom(char *command){
      
	char *s1=strtok(command," ");
	char *s2=strtok(NULL," ");
    char *s3=strtok(NULL," ");
	//改过
	if (strcmp(s1, "get") == 0 && 2 = 0) {
		printf("\nplease input the filename you want to download")
	}
	else if (strcmp(s1,"put")==0&&s2==0){
		prinf("\nplease input the filename you want to update")

	}
	{

	}
       // printf("\ncommand is %s %s %s",s1,s2,s3);
	 else if(strcmp(command,"pwd")==0){
		sendPWD(senddata,&s);
		recvControl(1,&s);
		
	}
	else if(strcmp(command,"ascii")==0){
		sendAs(senddata,&s);
		recvControl(1,&s);
	}
		else if(strcmp(command,"binary")==0){
		sendBi(senddata,&s);
		recvControl(1,&s);
	}
		else if(strcmp(command,"quit")==0){
		sendQuit(senddata,&s);
		recvControl(1,&s);
	}
		else if(strcmp(s1,"cd")==0){
		sendCD(senddata,&s,s2);
		recvControl(1,&s);
	}
		else if(strcmp(s1,"mkdir")==0){
		sendMK(senddata,&s,s2);
		recvControl(1,&s);
	}
		else if(strcmp(s1,"rmdir")==0){
		sendRM(senddata,&s,s2);
		recvControl(1,&s);
	}
		else if(strcmp(s1,"delete")==0)
 {
		sendDel(senddata,&s,s2);
		recvControl(1,&s);
	}
                else if(strcmp(s1,"rename")==0){
               sendRename(senddata,&s,s2,s3);
              }
		else if(strcmp(command,"passive")==0){
		sendPasv(senddata,&s);
		int length=recvControl(1,&s);
		recvCode(packbuf);
		getPassivePort(length,packbuf);
		int port=showPort(passport);
		connectServer(&sp,local,port);
		mode=0;
	}
	    else if(strcmp(command,"active")==0){
    	sendActive(senddata,&s);
		activeBind(&activesock,35062);	
    	recvControl(1,&s);	
    		mode=1;
    		
    		
    	}
	        else if(strcmp(s1,"get")==0){
    		sendGET(senddata,&s,s2);
    		recvControl(1,&s);
                if(mode==0){
    	    int len=recvControl(1,&sp);
    		storeFile(packbuf,s2,len);
			recvControl(1,&s);
}
                if(mode==1){
                newsock=accept (activesock, (struct sockaddr *)&cliaddr, &lenlen);
                if(newsock<0)
                printf("accept failed\n"); 
                int len=recvControl(1,&newsock);
                storeFile(packbuf,s2,len);
                recvControl(1,&s);
}
    	}
               else if(strcmp(s1,"put")==0){
               sendPUT(senddata,&s,s2);
               recvControl(1,&s);
               char* fileContext=preFile(s2);
               if(mode==0){
               send(sp,fileContext,strlen(fileContext),0);	
							 close(sp);
	           	 recvControl(1,&s);
}                 
               if(mode==1){
               newsock=accept(activesock,(struct sockaddr*)&cliaddr,&lenlen);
               if(newsock<0)
               printf("\naccept failed");
               send(newsock,fileContext,strlen(fileContext),0);
							 close(netsock);
               recvControl(1,&s);
	}
}
	
	    else if(strcmp(command,"ls")==0){
                
	    	sendLs(senddata,&s);
                if(mode==0){
	    	recvControl(1,&sp);
	    	recvControl(2,&s);
}
                if(mode==1){
                 newsock=accept(activesock,(struct sockaddr*)&cliaddr,&lenlen);
                 if(newsock<0)
                printf("accept failed\n");
                recvControl(1,&newsock);
                recvControl(2,&s);
    	}}
           else{
               printf("\n such command is invidity");
}	
	
	
	
		
}
