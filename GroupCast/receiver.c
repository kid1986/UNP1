#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MY_PORT 12345
#define MY_GROUP "225.0.0.37"
#define MSGBUFSIZE 256

int main(int argc, char ** argv)
{
	struct sockaddr_in addr;
	int fd,nbytes,addrlen;
	struct ip_mreq mreq;//data structure
	char msgbuf[MSGBUFSIZE];

	u_int yes = 1;

	if((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket failed\n");
		exit(1);
	}

	//set the socket reused
	if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)
	{
		perror("Reusing ADDR failed\n");
		exit(1);
	}

	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port=htons(MY_PORT);

	if(bind(fd,(struct sockaddr *)&addr,sizeof(addr)) < 0)
	{
		perror("bind error\n");
		exit(1);
	}

	//use setsockopt() to request that the kernel join a multicast group
	mreq.imr_multiaddr.s_addr=inet_addr(MY_GROUP);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	if(setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) 
	//if(setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mrep,sizeof(mrep)) < 0)
	{
		perror("setsockopt failed\n");
		exit(1);
	}

	while(1)
	{
		addrlen=sizeof(addr);
		if((nbytes=recvfrom(fd,msgbuf,MSGBUFSIZE,0,(struct sockaddr *)&addr,(socklen_t *)&addrlen)) < 0)
		{
			perror("recvfrom failed\n");
			exit(1);
		}
		puts(msgbuf);
	}

	return 0;
}
