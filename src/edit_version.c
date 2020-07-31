#include<header.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
int Applications_Details(int Total_Device_Apps)
{
	xmlNodePtr childofchildnode = NULL;/* node pointers */
	struct Device
	{
		char Version[12];
		char Type[128];
		char Name[128];
	}Application[Total_Device_Apps];
	
	memset(Application,0,sizeof(Application));
	FILE *fp = NULL;

	char *line=NULL,*str=NULL,*Type="PDS",*Name="APPDS";
	float Version = 2.0;
	size_t len=20;
	int i,j;
	fp = fopen("/etc/visiontek_Application_release","r");
	if(fp == NULL)
	{
		fprintf(stdout," /etc/visiontek_Application_release  file not found \n");
		return -1;
	}

	for(i=0,j=0;getline(&line, &len, fp) > 0;)
	{
		if((str = (char *)strstr(line,"ApplicationType:")) != NULL)
		{
			strcpy(Application[i].Type,str+16);
			if(Application[i].Type[ strlen(Application[i].Type) -1 ] == '\n')
				Application[i].Type[ strlen(Application[i].Type) - 1 ]='\0';
			i++;
		}
		else if((str = (char *)strstr(line,"ApplicationName:")) != NULL)
		{
			strcpy(Application[j].Name,str+16);
			if(Application[j].Name[strlen(Application[j].Name)-1] == '\n')
				Application[j].Name[strlen(Application[j].Name)-1]='\0';
			j++;
		}
		else if((str = (char *)strstr(line,"Version:")) != NULL)
		{
			if( i > 0 && i == j )
			{
				strcpy(Application[j-1].Version,str+8);
				if(Application[j-1].Version[strlen(Application[j-1].Version)-1] == '\n')
					Application[j-1].Version[strlen(Application[j-1].Version)-1]='\0';
			}
			else 
			{	
				fprintf(stdout," %d  %d \n", i,j);
				fprintf(stdout,"Wrong Application Response in /etc/visiontek_Application_release\n");
				return 0;
			}
		}


		else fprintf(stdout,"Line = %s \n",line);


	}
	Total_Device_Apps = i;
	fprintf(stdout," Total_Device_Apps = %d\n", Total_Device_Apps);
	free(line);
	line=NULL;
	fclose(fp);
	fp = fopen("/tmp/visiontek_Application_release","w");
	if(fp == NULL)
	{
		fprintf(stdout," /tmp/visiontek_Application_release  file not found \n");
		return -1;
	}

	for(i=0;i<Total_Device_Apps;i++)
	{
		fprintf(stdout,"ApplicationType:%s\nApplicationName:%s\nVersion:%s\n",Application[i].Type,Application[i].Name,Application[i].Version);
		if ( strcmp(Application[i].Type,Type) == 0 && strcmp(Application[i].Name,Name) == 0 )
			fprintf(fp,"ApplicationType:%s\nApplicationName:%s\nVersion:%.1f\n",Application[i].Type,Application[i].Name,Version);
		else fprintf(fp,"ApplicationType:%s\nApplicationName:%s\nVersion:%s\n",Application[i].Type,Application[i].Name,Application[i].Version);
	}


	return 0;
}
int Get_Device_Applications_Count()
{
	FILE *fp = NULL;

	char *line=NULL;
	int i,j;
	size_t len;
	fp = fopen("/etc/visiontek_Application_release","r");
	if(fp == NULL)
	{
		fprintf(stdout," /etc/visiontek_Application_release  file not found \n");
		return -1;
	}

	else
	{
		for(i=0,j=0;getline(&line, &len, fp) > 0;)
		{
			if( strstr(line,"ApplicationType:") != NULL)
				i++;
			else if( strstr(line,"ApplicationName:") != NULL)
				j++;

		}
		if( i != j )
		{
			fprintf(stdout,"Wrong Format in /etc/visiontek_Application_release, moving /etc/visiontek_Application_release to bkp apply All application patches \n");

			//                        system("mv /etc/visiontek_Application_release /etc/.__Rejected_visiontek_Application_release_wrong_");
			return -1;
		}
		fclose(fp);

	}
	return i;
}
int main()
{
	int Total_Device_Apps = Get_Device_Applications_Count();
	fprintf(stdout," Getting Device Count = %d\n",Total_Device_Apps);
	if ( Total_Device_Apps > 0 )
		Applications_Details(Total_Device_Apps);



	return 0;
}
