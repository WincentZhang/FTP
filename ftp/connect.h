int connectServer(int* s,char *ip,unsigned int port){//socket
struct sockaddr_in sock;
memset(&sock,0,sizeof(sock));
sock.sin_family=AF_INET;
sock.sin_port=htons(port);
sock.sin_addr.s_addr=inet_addr(ip);
*s=socket(PF_INET,SOCK_STREAM,0);
if(*s<0)
printf("control sock create error\n");
int con=connect(*s,(struct sockaddr*)&sock,sizeof(sock));
if(con<0)
printf("control connection error\n") ;	
	return con;
} 
//客户端充当服务器listen
int activeBind(int *sock,unsigned int port){//自己选定的端口
  struct sockaddr_in server;
	int qlength;
memset(&server,0,sizeof(server));
server.sin_family=AF_INET;
server.sin_port=htons(port);
server.sin_addr.s_addr=htonl(INADDR_ANY);
	*sock=socket(PF_INET,SOCK_STREAM,0);
	if(*sock==-1)
	printf("active socket failed\n");
 	if(bind(*sock,(struct sockaddr*)&server,sizeof(server))<0)              
 	printf("active socket bind failed\n");
 int con=listen (*sock,qlength);
 	if(con<0)
 	printf("connect failed\n");
	return con;	
}
