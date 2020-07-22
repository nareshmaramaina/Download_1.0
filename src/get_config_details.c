#include<header.h>
int get_RHMS_Server_details(void)
{
	FILE *fp;
	size_t len=0;
	char *str=NULL;
	unsigned int check_port;


	fp = fopen("/etc/rtm_response","r");

	if (fp == NULL)
	{
		fprintf(stderr," /etc/rtm_response open error\n");
		return -1;
	}

	while((getline(&str,&len,fp)) != -1)
	{
		if  (strstr(str,"RTMServerIP:") != NULL )
			strcpy(RHMS_Server.RTMServerIP,str+12);
		else if  (strstr(str,"PortNo:") != NULL )
			strcpy(RHMS_Server.PortNo,str+7);
		else if  (strstr(str,"FTP_PATH_APP:") != NULL )
			strcpy(RHMS_Server.FTP_PATH_APP,str+13);
		else if  (strstr(str,"ApplicationImageName:") != NULL )
			strcpy(RHMS_Server.ApplicationImageName,str+21);
		else if  (strstr(str,"ApplicationVersion:") != NULL )
			RHMS_Server.ApplicationVersion = atof(str+19);
		else if  (strstr(str,"FTP_PATH_FIRMWARE:") != NULL )
			strcpy(RHMS_Server.FTP_PATH_FIRMWARE,str+18);
		else if  (strstr(str,"FirmWarePatchVersion:") != NULL )
			RHMS_Server.FirmWarePatchVersion = atof(str+21);
		else if  (strstr(str,"ProjectName:") != NULL )
			strcpy(RHMS_Server.ProjectName,str+12);

	}
	free(str);
	str=NULL;
	fclose(fp);

	if( RHMS_Server.RTMServerIP[strlen(RHMS_Server.RTMServerIP)-1] == '\n' )
		RHMS_Server.RTMServerIP[strlen(RHMS_Server.RTMServerIP)-1]='\0';
	if( RHMS_Server.PortNo[strlen(RHMS_Server.PortNo)-1] == '\n' )
		RHMS_Server.PortNo[strlen(RHMS_Server.PortNo)-1]='\0';
	if( RHMS_Server.FTP_PATH_APP[strlen(RHMS_Server.FTP_PATH_APP)-1] == '\n' )
		RHMS_Server.FTP_PATH_APP[strlen(RHMS_Server.FTP_PATH_APP)-1]='\0';
	if( RHMS_Server.ApplicationImageName[strlen(RHMS_Server.ApplicationImageName)-1] == '\n' )
		RHMS_Server.ApplicationImageName[strlen(RHMS_Server.ApplicationImageName)-1]='\0';
	if( RHMS_Server.FTP_PATH_FIRMWARE[strlen(RHMS_Server.FTP_PATH_FIRMWARE)-1] == '\n' )
		RHMS_Server.FTP_PATH_FIRMWARE[strlen(RHMS_Server.FTP_PATH_FIRMWARE)-1]='\0';
	if( RHMS_Server.ProjectName[strlen(RHMS_Server.ProjectName)-1] == '\n' )
		RHMS_Server.ProjectName[strlen(RHMS_Server.ProjectName)-1]='\0';


	check_port = atoi(RHMS_Server.PortNo);

	if( (strlen(RHMS_Server.RTMServerIP) <  7 ) ||  check_port <= 0 || check_port > 65535 )
	{
		fprintf(stdout,"Error, Wrong server or port num Details are found, Taking Default ip address and port num %u\n",check_port);
		return -2;
	}

	fprintf(stdout,",RHMS_Server.RTMServerIP = %s\n,RHMS_Server.PortNo= %s\n,RHMS_Server.FTP_PATH_APP= %s\n,RHMS_Server.ApplicationImageName= %s\n,RHMS_Server.ApplicationVersion= %f\n,RHMS_Server.FTP_PATH_FIRMWARE= %s\n,RHMS_Server.FirmWarePatchVersion= %f\n,RHMS_Server.ProjectName= %s\n",RHMS_Server.RTMServerIP,RHMS_Server.PortNo,RHMS_Server.FTP_PATH_APP,RHMS_Server.ApplicationImageName,RHMS_Server.ApplicationVersion,RHMS_Server.FTP_PATH_FIRMWARE,RHMS_Server.FirmWarePatchVersion,RHMS_Server.ProjectName);
	return 0;
}
