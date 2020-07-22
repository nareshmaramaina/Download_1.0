#include<header.h>

void get_POS_Firmware_version()
{
	FILE *fp = NULL;

	char *line=NULL,*str=NULL;

	size_t len=20;
	char buff[20]="";
	fp = fopen("/etc/visiontek_release","r");
	if(fp == NULL)
		fprintf(stdout," /etc/visiontek_release  file not found \n");


	else 
	{
		while((getline(&line, &len, fp)) > 0)
		{
			if((str = (char *)strstr(line,"PATCH_VERSION=")) != NULL)
			{

				strcpy(buff,str+14);
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
		fprintf(stdout,"Writing Default Device.FirmWarePatchVersion \n" );
		fp = fopen("/etc/visiontek_release","w");

		if (fp == NULL)
		{
			Device.FirmWarePatchVersion=1.0;
			return;
		}

		fprintf(fp,"PATCH_VERSION=1.0");

		Device.FirmWarePatchVersion = 1.0; 
		fclose(fp);
	}

	else 
		Device.FirmWarePatchVersion   = atof(buff);
//fprintf(stdout," Device.FirmWarePatchVersion = %f\n",Device.FirmWarePatchVersion);
	return ;
}

