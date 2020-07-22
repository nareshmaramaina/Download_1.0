#include<header.h>
extern int FTP_MODE;

int wget_download (char *url,char *filename) // Mode= Active or Passive, retry_count =retry count to wget
{

	int retry_count_wget = 12;

	char wget_cmd[1024]="";

	int ret=0;

	int i=0;


	memset(wget_cmd,0x00,sizeof(wget_cmd));

	if ( FTP_MODE == ACTIVE_MODE)
	{
		fprintf(stdout,"Active Mode Downloading...\n");
		sprintf(wget_cmd,"/usr/bin/wget -c -T %d %s &> /dev/null",retry_count_wget,url);

	}



	else if ( FTP_MODE == PASSIVE_MODE )
	{
		fprintf(stdout,"Passive Mode Downloading...\n");
		sprintf(wget_cmd,"/usr/bin/wget --passive-ftp -c -T %d %s &> /dev/null",retry_count_wget,url);

	}
	else 
	{
		fprintf(stderr,"FTP Mode ( Active/Passive)  selection Error\n");
		return -1;
	}

	puts(wget_cmd);

	for (  i= 0 ; i < 20; i++)
	{
		Wait_for_internet(); //Block until internet 

		ret = system(wget_cmd);

		if (ret == 0)
			break;
		else 
			fprintf(stdout,"Error in server response, retrying %d\n",i);

		sleep(40);
	}	

	if (ret == 0 )
	{
		fprintf(stdout,"\n%s File Download Success\n",filename);
		return 0;
	}
	else 
	{
		fprintf(stderr,"\n%s File Download Failed\n",filename);
		return -1;
	}
}



