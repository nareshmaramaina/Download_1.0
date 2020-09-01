#include<header.h>
extern char *Server_Application_release_file;
int App_updates( int Total_Current_Server_Apps);
int Get_Total_Server_Apps();
int Get_Total_Device_Apps();
int Download_Application_Updates(void)
{
	int Total_Current_Server_Apps=0;
	Total_Current_Server_Apps = Get_Total_Server_Apps();
	fprintf(stdout,"\nApplications Request and Response, Total Applications count = %d\n",Total_Current_Server_Apps);
	if( Total_Current_Server_Apps > 0 )
		App_updates(Total_Current_Server_Apps);
	else 
		fprintf(stdout,"\nError: Application Request and Response not happened \n");	
	return 0;
}
int Download_applications(int Update_count,struct RHMSApplication DownloadApplication[Update_count])
{
	char FileName_with_path[320];
	char path[310];
	char cmd[320];
	int i,ret;
	fprintf(stdout,"\nNeed to Download %d Applications \n",Update_count);
	for(i=0;i<Update_count;i++)
	{	
		memset(path,0,sizeof(path));
		memset(cmd,0,sizeof(cmd));
		memset(FileName_with_path,0,sizeof(FileName_with_path));
		sprintf(path,"/mnt/sysuser/Software-Upgrade/Applications_Downloads/%s/%s",DownloadApplication[i].Type,DownloadApplication[i].Name);
		sprintf(cmd,"mkdir -p %s",path);

		system(cmd);

		sprintf(FileName_with_path,"%s/app-%.1f.zip",path,DownloadApplication[i].Version);
		ret = check_Download_complete(FileName_with_path,APPLICATION);	
		if ( ret == 0 )
		{
			fprintf(stdout,"ApplicationType = %s, ApplicationName = %s ApplicationVersion = %f, Already Download Completed, Ready to installation\n",DownloadApplication[i].Type,DownloadApplication[i].Name,DownloadApplication[i].Version);
			continue;	
		}
		else 
			ret = Download_Update(DownloadApplication[i].URL,FileName_with_path);

		if ( ret == 0 )
		{
			fprintf(stdout,"%s File Download Success\n",FileName_with_path);
			ret = Add_to_installation(DownloadApplication[i].Version,path,FileName_with_path,APPLICATION); // 2 arg Application Type
			if ( ret != 0 )
				fprintf(stderr,"Error, Failed to  Add to Installation in Application %s ",FileName_with_path);
		}
		else 
			fprintf(stdout,"%s File Download Failure\n",FileName_with_path);

		//		fprintf(stdout,"DownloadApplication[%d].Type = %s,DownloadApplication[%d].Name = %s,DownloadApplication[%d].URL = %s,DownloadApplication[%d].Version = %f\n",i,DownloadApplication[i].Type,i,DownloadApplication[i].Name,i,DownloadApplication[i].URL,i,DownloadApplication[i].Version);
	}

	return 0;
}


int App_updates( int Total_Current_Server_Apps)
{
	char Device_Application_release_file[300];
	struct RHMSApplication ServerApplication[Total_Current_Server_Apps];
	struct RHMSApplication DownloadApplication[Total_Current_Server_Apps];
	struct POSApplication DeviceApplication;
	FILE *fp = NULL;
	char *line=NULL,*str=NULL;
	int Update_count,Update_flag,Device_App_Wrong;
	size_t len=20,sizeofBuffer=0;
	int ret, i,j,check=0,  Total_Server_Apps=0;
	memset(ServerApplication,0,sizeof(ServerApplication));
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
				sizeofBuffer = sizeof(ServerApplication[i].Type);
                                if( strlen(str+16) > sizeofBuffer )
                                {
                                        fprintf(stderr,"Invalid: ServerApplication[%d].Type Length More than %d bytes \n",i,sizeofBuffer);
                                        continue;
                                }


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
				sizeofBuffer = sizeof(ServerApplication[j].Name);
                                if( strlen(str+16) > sizeofBuffer )
                                {
                                        fprintf(stderr,"Invalid: ServerApplication[%d].Name Length More than %d bytes \n",j,sizeofBuffer);
                                        continue;
                                }

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
				sizeofBuffer = sizeof(ServerApplication[j-1].URL);
                                if( strlen(str+15) > sizeofBuffer )
                                {
                                        fprintf(stderr,"Invalid: ServerApplication[%d].URL Length More than %d bytes \n",j-1,sizeofBuffer);
                                        continue;
                                }

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

	}

	Total_Server_Apps = i;
	//fprintf(stdout," Total_Server_Apps = %d\n", Total_Server_Apps);
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


	for(i=0,Update_count=0;i<Total_Server_Apps;i++)
	{
		memset(Device_Application_release_file,0,sizeof(Device_Application_release_file));
		sprintf(Device_Application_release_file,"/etc/vision/RHMS/Apps/%s/%s/AppUpdated.info",ServerApplication[i].Type,ServerApplication[i].Name);
		Device_App_Wrong=0;
		ret = access(Device_Application_release_file,F_OK);
		if ( ret == 0 )
		{
			memset(&DeviceApplication,0,sizeof(struct POSApplication));
			Device_App_info_Details(Device_Application_release_file,DeviceApplication.Type,DeviceApplication.Name,&DeviceApplication.Version );
			if (  strcmp(ServerApplication[i].Type,DeviceApplication.Type) == 0 && strcmp(ServerApplication[i].Name,DeviceApplication.Name) ==  0  )
			{

				if( ServerApplication[i].Version > DeviceApplication.Version )
				{
					fprintf(stdout,"Update Found, ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f,DeviceApplicationVersion = %f\n",DeviceApplication.Type,DeviceApplication.Name,ServerApplication[i].Version , DeviceApplication.Version);

					Update_flag=1;		
				}

				else
				{	
					fprintf(stdout,"No Update Found, ApplicationType = %s, ApplicationName = %s, DeviceApplicationVersion = %f\n",DeviceApplication.Type,DeviceApplication.Name, DeviceApplication.Version);
					continue;
				}

			}
			else 
			{
				fprintf(stdout,"Device Application Type or Application Name incorrectly defined in AppUpdated.info\n");
				Device_App_Wrong=1;
			}
		}
		else
		{
			Update_flag =1;
			fprintf(stdout,"Update Found,Newly Adding  ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f \n",ServerApplication[i].Type,ServerApplication[i].Name,ServerApplication[i].Version );
		}
		if ( Device_App_Wrong == 1 || Update_flag == 1)
		{
			if ( strlen(ServerApplication[i].URL) < 12 )
			{
				fprintf(stdout,"		URL Not found, May be intial Update at server side / Not Correct,  ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f \n",ServerApplication[i].Type,ServerApplication[i].Name,ServerApplication[i].Version );
				continue;
			}
			if ( strlen(ServerApplication[i].Type) <= 0 || strlen(ServerApplication[i].Name) <= 0 || ServerApplication[i].Version <=0.0 )
			{

				fprintf(stdout,"Error: Type/Name/Version Not Found, May be intial Update at server side / Not Correct  ApplicationType = %s, ApplicationName = %s ServerApplicationVersion = %f \n",ServerApplication[i].Type,ServerApplication[i].Name,ServerApplication[i].Version );
				continue;
			}
			strcpy(DownloadApplication[Update_count].Type,ServerApplication[i].Type);
			strcpy(DownloadApplication[Update_count].Name,ServerApplication[i].Name);
			strcpy(DownloadApplication[Update_count].URL,ServerApplication[i].URL);
			DownloadApplication[Update_count].Version=ServerApplication[i].Version;
			Update_count++;
		}
	}
	if ( Update_count > 0 )
		Download_applications(Update_count,DownloadApplication);
	else fprintf(stdout," No Applications Update Found \n");

	return Update_count;
}
int Get_Total_Server_Apps()
{
	int Total_Server_apps= 0;
	FILE *fp = NULL;
	char *line=NULL;
	int i,j;
	size_t len;
	fp = fopen(Server_Application_release_file,"r");
	if(fp == NULL)
		fprintf(stdout," %s  file not found \n",Server_Application_release_file);
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
			Total_Server_apps=i;
		else 
			fprintf(stdout,"Wrong Response in %s, We Can't Continue \n",Server_Application_release_file); 
		fclose(fp);

	}
	return Total_Server_apps;
}

