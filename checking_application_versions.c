#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int App_updates();
struct RHMS
{
	int  Total_Apps; 
	char ApplicationType[50][128];
	char ApplicationName[50][128];
	char ApplicationURL[50][512];
	float ApplicationVersion[50];
}Download;


int main()
{
	App_updates();	
	return 0;
}

int App_updates()
{
	FILE *fp = NULL;

	char *line=NULL,*str=NULL;
	int Update_count,Update_flag;
	size_t len=20;
	int i,j,check=0,  Total_Device_Apps=0,  Total_Server_Apps=0;
	char ServerApplicationType[50][128];
	char ServerApplicationName[50][128];
	char ServerApplicationURL[50][512];
	float ServerApplicationVersion[50];

	char DeviceApplicationType[50][128];
	char DeviceApplicationName[50][128];
	float DeviceApplicationVersion[50];


	memset(ServerApplicationType,0,sizeof(ServerApplicationType));
	memset(ServerApplicationName,0,sizeof(ServerApplicationName));
	memset(ServerApplicationURL,0,sizeof(ServerApplicationURL));
	memset(ServerApplicationVersion,0,sizeof(ServerApplicationVersion));
	memset(DeviceApplicationType,0,sizeof(DeviceApplicationType));
	memset(DeviceApplicationName,0,sizeof(DeviceApplicationName));
	memset(DeviceApplicationVersion,0,sizeof(DeviceApplicationVersion));
	fp = fopen("/etc/Application_release","r");
	if(fp == NULL)
	{
		fprintf(stdout," /etc/Application_release  file not found \n");
		return -1;
	}

	for(i=0,j=0,check=0;getline(&line, &len, fp) > 0;)
	{
		if((str = (char *)strstr(line,"ApplicationType:")) != NULL)
		{
			if( check == 0 || check == 2 || check == 4 )
			{

				strcpy(ServerApplicationType[i],str+16);
				if(ServerApplicationType[i][ strlen(ServerApplicationType[i]) -1 ] == '\n')
					ServerApplicationType[i][ strlen(ServerApplicationType[i]) - 1 ]='\0';
				i++;
				check=1;
			}
			else 
			{
				fprintf(stdout,"ApplicationType Error: Wrong Application Response in /etc/Application_release\n");
				return -1;
			}
		}
		else if((str = (char *)strstr(line,"ApplicationName:")) != NULL)
		{
			if ( check == 1 )
			{
				strcpy(ServerApplicationName[j],str+16);
				if(ServerApplicationName[j][strlen(ServerApplicationName[j])-1] == '\n')
					ServerApplicationName[j][strlen(ServerApplicationName[j])-1]='\0';
				j++;
				check = 2;
			}
			else 
			{
				fprintf(stdout,"ApplicationName Error: Wrong Application Response in /etc/Application_release\n");
				return -1;
			}

		}
		else if((str = (char *)strstr(line,"Version:")) != NULL)
		{
			if( check == 2  && i == j )
			{
				ServerApplicationVersion[j-1]  = atof(str+8);
				check=3;
			}
			else 
			{	
				fprintf(stdout," %d  %d \n", i,j);
				fprintf(stdout,"Version Error: Wrong Application Response in /etc/Application_release\n");
				return 0;
			}
		}

		else if((str = (char *)strstr(line,"ApplicationURL:")) != NULL)
		{
			if( check == 3  && i == j )
			{
				strcpy(ServerApplicationURL[j-1],str+15);
				if(ServerApplicationURL[j-1][strlen(ServerApplicationURL[j-1])-1] == '\n')
					ServerApplicationURL[j-1][strlen(ServerApplicationURL[j-1])-1]='\0';
				check=4;
			}
			else
			{
				fprintf(stdout," %d  %d \n", i,j);
				fprintf(stdout,"ApplicationURL Error: Wrong Application Response in /etc/Application_release\n");
				return 0;
			}
		}

		else fprintf(stdout," Line = %s \n",line);

		if ( i == 49  )
		{
			fprintf(stdout,"Caution: Maximum 50 applications are servered, skipping remains"); 
			break;
		}


	}

	Total_Server_Apps = i;
	fprintf(stdout," Total_Server_Apps = %d\n", Total_Server_Apps);
	free(line);
	line=NULL;
	fclose(fp);

	if ( check == 3 )
	{
		fprintf(stdout,"ApplicationURL Error: Wrong Application Response in /etc/Application_release\n");
		return -1;
	}
	for(i=0;i<j;i++)
		fprintf(stdout,"ApplicationType =%s ,ApplicationName=%s,ApplicationVersion= %.1f ApplicationURL = %s\n",ServerApplicationType[i],ServerApplicationName[i],ServerApplicationVersion[i],ServerApplicationURL[i]);


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
			strcpy(DeviceApplicationType[i],str+16);
			if(DeviceApplicationType[i][ strlen(DeviceApplicationType[i]) -1 ] == '\n')
				DeviceApplicationType[i][ strlen(DeviceApplicationType[i]) - 1 ]='\0';
			i++;
		}
		else if((str = (char *)strstr(line,"ApplicationName:")) != NULL)
		{
			strcpy(DeviceApplicationName[j],str+16);
			if(DeviceApplicationName[j][strlen(DeviceApplicationName[j])-1] == '\n')
				DeviceApplicationName[j][strlen(DeviceApplicationName[j])-1]='\0';
			j++;
		}
		else if((str = (char *)strstr(line,"Version:")) != NULL)
		{
			if( i > 0 && i == j )
				DeviceApplicationVersion[j-1]  = atof(str+8);

			else 
			{	
				fprintf(stdout," %d  %d \n", i,j);
				fprintf(stdout,"Wrong Application Response in /etc/visiontek_Application_release\n");
				return 0;
			}
		}


		else fprintf(stdout,"Line = %s \n",line);

		if ( i == 49  )
		{
			fprintf(stdout,"Caution: Maximum 50 applications are servered, skipping remains");
			break;
		}


	}
	Total_Device_Apps = i;
	fprintf(stdout," Total_Device_Apps = %d\n", Total_Device_Apps);
	fprintf(stdout," %d  %d \n", i,j);
	free(line);
	line=NULL;
	fclose(fp);

	for(i=0,Update_count=0;i<Total_Server_Apps;i++)
	{

		for ( Update_flag=0,j=0; j < Total_Device_Apps ; j++ )
		{
			if (  strcmp(ServerApplicationType[i],DeviceApplicationType[j]) == 0 && strcmp(ServerApplicationName[i],DeviceApplicationName[j]) ==  0  )
			{

				if( ServerApplicationVersion[i] > DeviceApplicationVersion[j] )
				{
					fprintf(stdout,"Update Found,ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f,DeviceApplicationVersion = %f\n",DeviceApplicationType[j],DeviceApplicationName[j],ServerApplicationVersion[i] , DeviceApplicationVersion[i]);

					Update_flag=1;		
				}

				else 
					fprintf(stdout,"No Update Found, ApplicationType = %s, ApplicationName = %s, DeviceApplicationVersion = %f\n",DeviceApplicationType[j],DeviceApplicationName[j], DeviceApplicationVersion[i]);
				break;

			}
		}

		if ( j == Total_Device_Apps || Update_flag == 1)
		{
			if ( j == Total_Device_Apps )
				fprintf(stdout,"Update Found,Newly Adding  ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f \n",ServerApplicationType[i],ServerApplicationName[i],ServerApplicationVersion[i] );
			strcpy(Download.ApplicationType[Update_count],ServerApplicationType[i]);
			strcpy(Download.ApplicationName[Update_count],ServerApplicationName[i]);
			strcpy(Download.ApplicationURL[Update_count],ServerApplicationURL[i]);
			Download.ApplicationVersion[Update_count]=ServerApplicationVersion[i];
			Update_count++;
		}
	}
	Download.Total_Apps=Update_count;

	fprintf(stdout,"Application Update Count = %d \n",Download.Total_Apps );

	//		fprintf(stdout,"ApplicationType =%s ,ApplicationName=%s,ApplicationVersion= %.1f ApplicationURL = %s\n",ServerApplicationType[i],ServerApplicationName[i],ServerApplicationVersion[i],ServerApplicationURL[i]);
	//		fprintf(stdout,"ApplicationType =%s ,ApplicationName=%s,ApplicationVersion= %.1f \n",DeviceApplicationType[i],DeviceApplicationName[i],DeviceApplicationVersion[i]);


	return 0;
}
