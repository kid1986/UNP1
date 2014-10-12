#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define LISTENQ 1024
#define SERV_PORT 9877
#define bzero(ptr,n) memset(ptr, 0, n)
typedef struct sockaddr SA;

void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];

	again:
	while( (n = read(sockfd,buf,MAXLINE)) > 0)
		write(sockfd,buf,n);

	if(n < 0 && errno == EINTR)
		goto again;
	else if( n < 0)
		fprintf(stderr,"str_echo: read error");
}

int main(int argc,char **argv)
{
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (SA*)&servaddr,sizeof(servaddr));

	listen(listenfd,LISTENQ);

	for(;;)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (SA *) &cliaddr, &clilen);

		if( (childpid = fork()) == 0) //child progress
		{
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}

		//parent progress
		close(connfd);
	}

	return 0;	
}
