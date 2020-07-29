#include<header.h>
char *Server_Application_release_file="/etc/Application_release";
char *Device_Application_release_file="/etc/visiontek_Application_release";
int App_updates(int Total_Current_Device_Apps, int Total_Current_Server_Apps);
int Get_Server_And_Device_Applications_Count(int *Total_Device_apps,int *Total_Server_apps);
int Download_Application_Updates(void)
{
	int Total_Current_Device_Apps=0, Total_Current_Server_Apps=0,ret;
	ret = Get_Server_And_Device_Applications_Count(&Total_Current_Device_Apps, &Total_Current_Server_Apps);
	fprintf(stdout,"Total_Current_Device_Apps = %d, Total_Current_Server_Apps = %d\n",Total_Current_Device_Apps, Total_Current_Server_Apps);
	if ( ret != 0)
		return -1;
	App_updates(Total_Current_Device_Apps,Total_Current_Server_Apps);	
	return 0;
}
int Download_applications(int Update_count,struct RHMSApplication DownloadApplication[Update_count])
{
	char FileName_with_path[320];
	char path[310];
	char cmd[320];
	fprintf(stdout,"Applications Update count %d \n",Update_count);
	int i,ret;
	for(i=0;i<Update_count;i++)
	{	
		memset(path,0,sizeof(path));
		memset(cmd,0,sizeof(cmd));
		memset(FileName_with_path,0,sizeof(FileName_with_path));
		sprintf(path,"/mnt/sysuser/Software-Upgrade/Applications_Downloads/%s/%s",DownloadApplication[i].Type,DownloadApplication[i].Name);
		sprintf(cmd,"mkdir -p %s",path);

		system(cmd);

		sprintf(FileName_with_path,"%s/app-%.1f.zip",path,DownloadApplication[i].Version);

		ret = Download_Update(DownloadApplication[i].URL,FileName_with_path);

		if ( ret == 0 )
		{
			fprintf(stdout,"%s File Download Success\n",FileName_with_path);
			Add_to_installation(FileName_with_path,2); // 2 for Applications 
		}
		else 
			fprintf(stdout,"%s File Download Failure\n",FileName_with_path);

		//		fprintf(stdout,"DownloadApplication[%d].Type = %s,DownloadApplication[%d].Name = %s,DownloadApplication[%d].URL = %s,DownloadApplication[%d].Version = %f\n",i,DownloadApplication[i].Type,i,DownloadApplication[i].Name,i,DownloadApplication[i].URL,i,DownloadApplication[i].Version);
	}

	return 0;
}


int App_updates(int Total_Current_Device_Apps, int Total_Current_Server_Apps)
{
	struct RHMSApplication ServerApplication[Total_Current_Server_Apps];
	struct RHMSApplication DownloadApplication[Total_Current_Server_Apps];
	struct POSApplication DeviceApplication[Total_Current_Device_Apps];;
	FILE *fp = NULL;
	char *line=NULL,*str=NULL;
	int Update_count,Update_flag;
	size_t len=20;
	int i,j,check=0,  Total_Device_Apps=0,  Total_Server_Apps=0;
	memset(ServerApplication,0,sizeof(ServerApplication));
	memset(DeviceApplication,0,sizeof(DeviceApplication));
	memset(DownloadApplication,0,sizeof(DownloadApplication));
	fp = fopen(Server_Application_release_file,"r");
	if(fp == NULL)
	{
		fprintf(stdout," %s  file not found \n",Server_Application_release_file);
		return -1;
	}

	for(i=0,j=0,check=0;getline(&line, &len, fp) > 0;)
	{
		if((str = (char *)strstr(line,"ApplicationType:")) != NULL)
		{
			if( check == 0 || check == 2 || check == 4 )
			{

				strcpy(ServerApplication[i].Type,str+16);
				if(ServerApplication[i].Type[ strlen(ServerApplication[i].Type) -1 ] == '\n')
					ServerApplication[i].Type[ strlen(ServerApplication[i].Type) - 1 ]='\0';
				i++;
				check=1;
			}
			else 
			{
				fprintf(stdout,"ApplicationType Error: Wrong Application Response in %s\n",Server_Application_release_file);
				return -1;
			}
		}
		else if((str = (char *)strstr(line,"ApplicationName:")) != NULL)
		{
			if ( check == 1 )
			{
				strcpy(ServerApplication[j].Name,str+16);
				if(ServerApplication[j].Name[strlen(ServerApplication[j].Name)-1] == '\n')
					ServerApplication[j].Name[strlen(ServerApplication[j].Name)-1]='\0';
				j++;
				check = 2;
			}
			else 
			{
				fprintf(stdout,"ApplicationName Error: Wrong Application Response in %s\n",Server_Application_release_file);
				return -1;
			}

		}
		else if((str = (char *)strstr(line,"Version:")) != NULL)
		{
			if( check == 2  && i == j )
			{
				ServerApplication[j-1].Version  = atof(str+8);
				check=3;
			}
			else 
			{	
				fprintf(stdout," %d  %d \n", i,j);
				fprintf(stdout,"Version Error: Wrong Application Response in %s\n",Server_Application_release_file);
				return 0;
			}
		}

		else if((str = (char *)strstr(line,"ApplicationURL:")) != NULL)
		{
			if( check == 3  && i == j )
			{
				strcpy(ServerApplication[j-1].URL,str+15);
				if(ServerApplication[j-1].URL[strlen(ServerApplication[j-1].URL)-1] == '\n')
					ServerApplication[j-1].URL[strlen(ServerApplication[j-1].URL)-1]='\0';
				check=4;
			}
			else
			{
				fprintf(stdout," %d  %d \n", i,j);
				fprintf(stdout,"ApplicationURL Error: Wrong Application Response in %s\n",Server_Application_release_file);
				return 0;
			}
		}

		else fprintf(stdout," Line = %s",line);


	}

	Total_Server_Apps = i;
	fprintf(stdout," Total_Server_Apps = %d\n", Total_Server_Apps);
	free(line);
	line=NULL;
	fclose(fp);

	if ( check == 3 )
	{
		fprintf(stdout,"ApplicationURL Error: Wrong Application Response in %s\n",Server_Application_release_file);
		return -1;
	}
	//	for(i=0;i<j;i++)
	//		fprintf(stdout,"ApplicationType =%s ,ApplicationName=%s,ApplicationVersion= %.1f ApplicationURL = %s\n",ServerApplication[i].Type,ServerApplication[i].Name,ServerApplication[i].Version,ServerApplication[i].URL);


	fp = fopen(Device_Application_release_file,"r");
	if(fp == NULL)
	{
		fprintf(stdout," %s  file not found \n",Device_Application_release_file);
	}
	else 
	{
		for(i=0,j=0;getline(&line, &len, fp) > 0;)
		{
			if((str = (char *)strstr(line,"ApplicationType:")) != NULL)
			{
				strcpy(DeviceApplication[i].Type,str+16);
				if(DeviceApplication[i].Type[ strlen(DeviceApplication[i].Type) -1 ] == '\n')
					DeviceApplication[i].Type[ strlen(DeviceApplication[i].Type) - 1 ]='\0';
				i++;
			}
			else if((str = (char *)strstr(line,"ApplicationName:")) != NULL)
			{
				strcpy(DeviceApplication[j].Name,str+16);
				if(DeviceApplication[j].Name[strlen(DeviceApplication[j].Name)-1] == '\n')
					DeviceApplication[j].Name[strlen(DeviceApplication[j].Name)-1]='\0';
				j++;
			}
			else if((str = (char *)strstr(line,"Version:")) != NULL)
			{
				if( i > 0 && i == j )
					DeviceApplication[j-1].Version  = atof(str+8);

				else 
				{	
					fprintf(stdout," %d  %d \n", i,j);
					fprintf(stdout,"Wrong Application Response in %s\n",Device_Application_release_file);
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
	}
	for(i=0,Update_count=0;i<Total_Server_Apps;i++)
	{

		for ( Update_flag=0,j=0; j < Total_Device_Apps ; j++ )
		{
			if (  strcmp(ServerApplication[i].Type,DeviceApplication[j].Type) == 0 && strcmp(ServerApplication[i].Name,DeviceApplication[j].Name) ==  0  )
			{

				if( ServerApplication[i].Version > DeviceApplication[j].Version )
				{
					fprintf(stdout,"Update Found,ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f,DeviceApplicationVersion = %f\n",DeviceApplication[j].Type,DeviceApplication[j].Name,ServerApplication[i].Version , DeviceApplication[j].Version);

					Update_flag=1;		
				}

				else 
					fprintf(stdout,"No Update Found, ApplicationType = %s, ApplicationName = %s, DeviceApplicationVersion = %f\n",DeviceApplication[j].Type,DeviceApplication[j].Name, DeviceApplication[j].Version);
				break;

			}
		}

		if ( j == Total_Device_Apps || Update_flag == 1)
		{
			if ( strlen(ServerApplication[i].URL) < 12 )
			{
				fprintf(stdout,"URL Not found/ Not Correct,  ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f \n",ServerApplication[i].Type,ServerApplication[i].Name,ServerApplication[i].Version );
				continue;
			}
			if ( strlen(ServerApplication[i].Type) <= 0 || strlen(ServerApplication[i].Name) <= 0 || ServerApplication[i].Version <=0.0 )
			{
				fprintf(stdout,"Type/Name/Version Not Found / Not Correct  ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f \n",ServerApplication[i].Type,ServerApplication[i].Name,ServerApplication[i].Version );
				continue;
			}
			if ( j == Total_Device_Apps )
				fprintf(stdout,"Update Found,Newly Adding  ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f \n",ServerApplication[i].Type,ServerApplication[i].Name,ServerApplication[i].Version );
			strcpy(DownloadApplication[Update_count].Type,ServerApplication[i].Type);
			strcpy(DownloadApplication[Update_count].Name,ServerApplication[i].Name);
			strcpy(DownloadApplication[Update_count].URL,ServerApplication[i].URL);
			DownloadApplication[Update_count].Version=ServerApplication[i].Version;
			Update_count++;
		}
	}

	Download_applications(Update_count,DownloadApplication);

	return Update_count;
}
int Get_Server_And_Device_Applications_Count(int *Total_Device_apps,int *Total_Server_apps)
{
	FILE *fp = NULL;

	char *line=NULL;
	int i,j;
	size_t len;
	fp = fopen(Server_Application_release_file,"r");
	if(fp == NULL)
	{
		fprintf(stdout," %s  file not found \n",Server_Application_release_file);
		return -1;
	}
	else 
	{
		for(i=0,j=0;getline(&line, &len, fp) > 0;)
		{
			if(	strstr(line,"ApplicationType:") != NULL)
				i++;	
			else if(	strstr(line,"ApplicationName:")  != NULL)
				j++;

		}
		if( i == j )
			*Total_Server_apps=i;
		else 
		{
			fprintf(stdout,"Wrong Response in %s, We Can't Continue \n",Server_Application_release_file); 
			return -1;
		}	
		fclose(fp);

	}
	fp = fopen(Device_Application_release_file,"r");
	if(fp == NULL)
	{
		fprintf(stdout," %s  file not found \n",Device_Application_release_file);
		*Total_Device_apps=0;
	}

	else 
	{
		for(i=0,j=0;getline(&line, &len, fp) > 0;)
		{
			if(	strstr(line,"ApplicationType:") != NULL)
				i++;
			else if(	strstr(line,"ApplicationName:") != NULL)
				j++;

		}
		if( i == j )
			*Total_Device_apps=i;
		else
		{
			fprintf(stdout,"Wrong Response in %s, moving  to bkp apply All application patches \n",Device_Application_release_file);
			//	system("mv %s /etc/.__Rejected_visiontek_Application_release_wrong_");
			*Total_Device_apps=0;
		}
		fclose(fp);

	}
	return 0;
}
