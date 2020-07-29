#include<header.h>
int Check_internet_by_hostname(char *host);
static int retrieve_nw_details(void);
static int retrieve_nw_details(void)
{        
	FILE *fp;
	char buff[30]="";

	memset(buff,0x00,30);

	fp = fopen("/var/gprs","r");

	if(fp==NULL)
	{
		fprintf(stderr,"/var/gprs/ file not found\n");
		return -1;
	}

	fread(buff,1,sizeof(buff),fp);

	fclose(fp);

	if( ( buff[2]  == 'E' ) || ( buff[2]  == 'H' ) || ( buff[2]  == 'L' ) ||  buff[2]  == 'W' ||  buff[2]  == 'X')
		return 0;
	else
		return -1;
}

void Wait_for_internet(void)
{
	short int ret,count=0,sleep_secs=0;

	while(1)
	{
		ret = retrieve_nw_details();

		if(ret == 0)
		{
			printf("Internet Found\n");
			return;
		}

		else if ( Check_internet_by_hostname("google.co.in") == 0 )
		{
			fprintf(stdout,"Internet Found by hostname api\n");
			return;
		}

		if ( count < 8 )
			sleep_secs = 30;
		else if ( count < 16 )
			sleep_secs = 60;
		else if ( count < 32 )
			sleep_secs = 120;
		else
			sleep_secs = 300;

		sleep(sleep_secs);
		count++;
		fprintf(stdout,"Unable to Connect to Internet Retrying..\n");
		printf("checking for network details,\nCount = %d sleep secs = %d\n",count+1,sleep_secs);

	}

	return;
}
int Check_internet_by_hostname(char *host)
{
	int sockfd;  
	struct hostent *he;
	struct sockaddr_in their_addr;

	res_init();

	if ((he=gethostbyname(host)) == NULL) {  
		herror("gethostbyname");
		return -1;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(80);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, \
				sizeof(struct sockaddr)) == -1) {
		perror("connect");
		close(sockfd);
		return -1;
	}
	close(sockfd);
	return 0;

}
