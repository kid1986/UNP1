#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define LISTENQ 1024
#define SERV_PORT 9877
#define bzero(ptr,n) memset(ptr, 0, n)
typedef struct sockaddr SA;

size_t readline(int fd, void *buf, int size)
{
	char *strtmp;
	ssize_t reval, realcount=0;
	strtmp = (char *)buf;

	while(size>1)
	{
		reval = read(fd, strtmp, 1);
		if (reval<0)
			if (errno == EINTR)
				continue;
			else return -1;
		else if (reval == 0)
			break;
		else
		{
			realcount++;
			size--;
			if (*strtmp++ =='\n')
				break;
		}
	}
	*strtmp='\0';
	return realcount;
}

void str_cli(FILE *fp, int sockfd)
{
	char sendline[MAXLINE],recvline[MAXLINE];

	while(fgets(sendline,MAXLINE,fp) != NULL)
	{
		write(sockfd, sendline, strlen(sendline));
		if(readline(sockfd, recvline, MAXLINE) == 0)
			fprintf(stderr,"str_cli: server terminated prematurely");
		fputs(recvline, stdout);
	}
}

int main(int argc, char ** argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if(argc != 2)
	{
		fprintf(stderr,"usage: client <IPaddress>");
		exit(-1);
	}

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);

	connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

	str_cli(stdin,sockfd);

	exit(0);
}
