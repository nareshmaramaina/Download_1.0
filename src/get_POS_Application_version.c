#include<header.h>

void get_POS_Application_version()
{
	FILE *fp = NULL;

	char *line=NULL,*str=NULL;

	size_t len=20;
	char buff[20]="";
	fp = fopen("/etc/app_release","r");
	if(fp == NULL)
		fprintf(stdout," /etc/app_release  file not found \n");


	else 
	{
		while((getline(&line, &len, fp)) > 0)
		{
			if((str = (char *)strstr(line,"APP_VERSION=")) != NULL)
			{

				strcpy(buff,str+12);
				if(buff[strlen(buff)-1] == '\n')
					buff[strlen(buff)-1]='\0';

				break;
			}

		}
		free(line);
		line=NULL;
		fclose(fp);
	}

	if( strlen(buff) == 0 || fp == NULL )
	{
		fprintf(stdout,"Writing Default Device.ApplicationVersion \n" );
		fp = fopen("/etc/app_release","w");

		if (fp == NULL)
		{
			Device.ApplicationVersion=1.0;
			return;
		}

		fprintf(fp,"APP_VERSION=1.0");

		Device.ApplicationVersion = 1.0; 
		fclose(fp);
	}

	else 
		Device.ApplicationVersion   = atof(buff);
//	fprintf(stdout," Device.ApplicationVersion = %f\n",Device.ApplicationVersion);
	return ;
}

