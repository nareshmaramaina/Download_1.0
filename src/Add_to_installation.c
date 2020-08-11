#include<header.h>
char *Install_Applications_file="/mnt/sysuser/Software-Upgrade/Applications_Downloads/Install_Applications.info";
char *Install_Firmwares_file="/mnt/sysuser/Software-Upgrade/Firmware_Downloads/Install_Firmwares.info";
int Add_to_installation(char *path,char *patch,int type)
{
	FILE *fp=NULL;
	char filename[128];
	char Date_time[64];
	char Download_complete_file[330];
	int ret;
	memset(filename,0,sizeof(memset));
	memset(Date_time,0,sizeof(Date_time));
	memset(Download_complete_file,0,sizeof(Download_complete_file));
	if ( type == FIRMWARE )
		strcpy(filename,Install_Firmwares_file);	
	else if ( type == APPLICATION )
		strcpy(filename,Install_Applications_file);
	else 
	{
		fprintf(stdout,"Unknown type\n");
		return -1;
	}

	if ( access(patch,F_OK) != 0 )
	{
		fprintf(stdout,"\033[1;31m Download Error: %s Download Status getting Success but file not found, Type = %d \033[0m\n",patch,type); 
		return -1;
	}
	ret = check_Download_complete(patch,type);	
	if ( ret == 0 || ret == 1 )
	{
		fprintf(stdout,"\033[1;31m Caution: Already Added For installation %s Type = %d \033[0m\n",patch,type); 
		return -1;
	}
	Update_Current_Date_with_Time(Date_time);

	sprintf(Download_complete_file,"%s/.DownloadCompletedHistory",path);

	fp = fopen(Download_complete_file,"a");
	if ( fp == NULL )
	{
		fprintf(stderr," file not found %s\n",Download_complete_file);
		return -1;
	}
	else
	{	
		fprintf(fp,"%s\n%s\n",patch,Date_time);
		fclose(fp);
	}

	fp = fopen(filename,"a");
	if ( fp == NULL )
	{
		fprintf(stderr," file not found %s\n",filename);
		return -1;
	}

	else
	{	
		fprintf(fp,"%s\n",patch);
		fclose(fp);
		fprintf(stdout,"Added to patch installation list\tReady for installation = %s\n",patch);
	}

	memset(Download_complete_file,0,sizeof(Download_complete_file));
	
	sprintf(Download_complete_file,"%s/.DownloadCompletedHistory",path);

        fp = fopen(Download_complete_file,"a");
        if ( fp == NULL )
        {
                fprintf(stderr," file not found %s\n",Download_complete_file);
                return -1;
        }
        else
        {
                fprintf(fp,"%s\n%s\n",patch,Date_time);
                fclose(fp);
        }
		
	return 0;
}
int check_Download_complete(char *patch,int type)
{
	FILE *fp=NULL;
	char filename[128];
	char *line=NULL;
	size_t len=0;
	int Found=0;
	memset(filename,0,sizeof(memset));

	if ( type == FIRMWARE )
		strcpy(filename,Install_Firmwares_file);	
	else if ( type == APPLICATION )
		strcpy(filename,Install_Applications_file);
	else 
	{
		fprintf(stdout,"Unknown type\n");
		return -1;
	}
	fp = fopen(filename,"r");
	if ( fp == NULL )
	{
		fprintf(stdout," file not found %s\n",filename);
		return -1;
	}

	while( (getline(&line, &len, fp) > 0) )
	{
		if ( line[strlen(line)-1] == '\n' )
			line[strlen(line)-1] = '\0';

		if((strcmp(line,patch)) == 0)
		{
			Found=1;
			break;
		}
	}
	free(line);
	line=NULL;
	fclose(fp);

	if ( Found == 1 )
	{
		if (  access(patch,F_OK) == 0  ) 
			return 0;
		else 
		{
			fprintf(stdout,"%s Download file not found \n",patch);
			return 1;
		}
	}
	else fprintf(stdout,"No previous Download completion, patch = %s\n",patch);

	return -1;
}
void Update_Current_Date_with_Time(char *Date_time)
{
	struct tm *Today=NULL;
	struct timeval tv;

	gettimeofday (&tv,NULL);

	Today = localtime (&tv.tv_sec) ;

	sprintf(Date_time,"%04d-%02d-%02dT%02d:%02d:%02d",Today->tm_year+1900,Today->tm_mon+1,Today->tm_mday,Today->tm_hour,Today->tm_min,Today->tm_sec);
	fprintf(stdout,"Today Date and Time, %s \n",Date_time);

	return;
}

