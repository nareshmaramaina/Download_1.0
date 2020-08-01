#include<header.h>
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
	if ( type == 1 )
		strcpy(filename,"/mnt/sysuser/Software-Upgrade/Firmware_Downloads/Install_Firmwares");	
	else if ( type == 2 )
		strcpy(filename,"/mnt/sysuser/Software-Upgrade/Applications_Downloads/Install_Applications");
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
	ret = check_Download_complete(path,patch,type);	
	if ( ret == 0 || ret == 1 )
	{
		fprintf(stdout,"\033[1;31m Caution: Already Added For installation %s Type = %d \033[0m\n",patch,type); 
		return -1;
	}
	Update_Current_Date_with_Time(Date_time);

	sprintf(Download_complete_file,"%s/DownloadCompleted",path);

	fp = fopen(Download_complete_file,"a");
	if ( fp == NULL )
	{
		fprintf(stderr," Open error = %s\n",Download_complete_file);
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
		fprintf(stderr," Open error = %s\n",filename);
		return -1;
	}

	else
	{	
		fprintf(fp,"%s\n",patch);
		fclose(fp);
		fprintf(stdout,"Ready for installation = %s\n",patch);
	}

	return 0;
}
int check_Download_complete(char *path,char *patch,int type)
{
	char Download_complete_file[330];
	FILE *fp=NULL;
	char filename[128];
	char *line=NULL;
	size_t len=0;
	int Found=0;
	memset(filename,0,sizeof(memset));

	if ( type == 1 )
		strcpy(filename,"/mnt/sysuser/Software-Upgrade/Firmware_Downloads/Install_Firmwares");	
	else if ( type == 2 )
		strcpy(filename,"/mnt/sysuser/Software-Upgrade/Applications_Downloads/Install_Applications");
	else 
	{
		fprintf(stdout,"Unknown type\n");
		return -1;
	}
	fp = fopen(filename,"r");
	if ( fp == NULL )
	{
		fprintf(stdout," Open error = %s\n",filename);
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
	if ( Found == 1 )
	{
		memset(Download_complete_file,0,sizeof(Download_complete_file));
		sprintf(Download_complete_file,"%s/DownloadCompleted",path);

		if ( ( access(Download_complete_file,F_OK) == 0 ) && ( access(patch,F_OK) == 0 ) ) 
			return 0;
		else 
		{
			fprintf(stdout," %s file or %s file not found \n",Download_complete_file,patch);
			return 1;
		}
	}
	else fprintf(stdout,"Notttttt Found %s",patch);

	return -1;
}
void Update_Current_Date_with_Time(char *Date_time)
{
	struct tm *Today=NULL;
	struct timeval tv;

	gettimeofday (&tv,NULL);

	Today = localtime (&tv.tv_sec) ;

	sprintf(Date_time,"Date:%02d/%02d/%04d Time:%02d-%02d-%02d",Today->tm_mday,Today->tm_mon+1,Today->tm_year+1900,Today->tm_hour,Today->tm_min,Today->tm_sec);

	return;
}

