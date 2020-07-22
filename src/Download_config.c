#include<header.h>
int FTP_MODE;
int Download_config(int Type)//  1 - Firmware type config Download , 2 - Application type config Download
{
	char wget_cmd[2121];

	int i,mode,ret=0, retry_count_wget = 5;

	char url[2070];

	memset(wget_cmd,0x00,sizeof(wget_cmd));
	memset(url,0x00,sizeof(url));

	if ( Type == 1 )
	{
		chdir("/mnt/sysuser/Firmware-Upgrade/");
		sprintf(url,"%s%s",RHMS_Server.FTP_PATH_FIRMWARE,"config-patchversions");
		fprintf(stdout,"Type = 1, Firmware patch config Downloading..\n");

	}
	else if ( Type == 2 )
	{

		chdir("/mnt/sysuser/App-Upgrade/");
		sprintf(url,"%s%s",RHMS_Server.FTP_PATH_APP,"config-appversions");
		fprintf(stdout,"Type = 2, Application patch config Downloading..\n");
	}
	else
	{
		fprintf(stdout," Type Error \n"	);
		return -1;
	}

	for ( mode = 1, i=1; i <= 20; i++,mode++)
	{
		Wait_for_internet(); //Block until internet 

		if(mode == 1 || mode == 2 )
		{
			FTP_MODE = ACTIVE_MODE;
			sprintf(wget_cmd,"/usr/bin/wget -c -T %d %s &> /dev/null",retry_count_wget,url);
		}

		else if(mode == 3 || mode == 4 ) 
		{
			FTP_MODE = PASSIVE_MODE;
			sprintf(wget_cmd,"/usr/bin/wget --passive-ftp -c -T %d %s &> /dev/null",retry_count_wget,url);
		}

		fprintf(stdout,"PATH =  %s\n",wget_cmd);

		ret = system(wget_cmd);

		if (ret == 0)
			break;
		else 
			fprintf(stdout,"Error in server response, retrying %d\n",i);

		if(mode == 4)
			mode = 0;

		sleep(40);

	}

	return ret;

}
