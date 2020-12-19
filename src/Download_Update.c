#include<header.h>

int Download_Update(char *URL, char *Filename_with_Download_Location)
{

	char Download_cmd[1024];
	
	char *ptr=NULL;

	int ret=0,DownloadedFileSize=0,ServerFileSize=0,md5sumret;

	short int  i=0,Env_flag=0;

	char curl_name[64]="/vision/DeviceManagement/system/usr/bin/DM_curl";
	if ( access(curl_name,F_OK) != 0 )
	{
		fprintf(stdout,"%s not found \n",curl_name);
		memset(curl_name,0,sizeof(curl_name));
		strcpy(curl_name,"curl");
	}
	else
	{
		setenv("LD_LIBRARY_PATH","/vision/DeviceManagement/system/usr/lib/",1);
		Env_flag=1;

	}


	memset(Download_cmd,0x00,sizeof(Download_cmd));

	sprintf(Download_cmd,"%s --cacert  /vision/DeviceManagement/certs/curl-ca-bundle.crt -C -  %s -o %s",curl_name,URL,Filename_with_Download_Location);

	puts(Download_cmd);

	for (  i= 0 ; i < 20; i++)
	{
		Wait_for_internet(); //Block until internet 

		Check_date_set_if_wrong(0);

		ServerFileSize = filesize_at_server(URL,Env_flag);
		if ( ServerFileSize > 0 )
			fprintf(stdout,"ServerFileSize = %d\n",ServerFileSize);
		else 
		{
			fprintf(stdout,"URL: %s, File information Getting Failure \nRetrying .., sleep 30secs",URL);
			sleep(30);
			if ( i < 10 )
				continue;
		}

		ret = system(Download_cmd);
		DownloadedFileSize = size_of_file(Filename_with_Download_Location);	
		if (ret == 0 || ret == 8448 || ( ( DownloadedFileSize > 0 && ServerFileSize >0 )  && DownloadedFileSize == ServerFileSize ) )
		{	
			md5sumret = Check_md5sum_of_Patch(Filename_with_Download_Location); 
			if ( md5sumret == 0 )
			{
				ret = 0;
				break;
			}
			else 
			{
				remove(Filename_with_Download_Location);

				fprintf(stdout,"Deleted %s, Due to md5sum Verification Failure\n",Filename_with_Download_Location);

			}
			if ( ret == 8448 )
			{
				fprintf(stdout,"byte range position Error, Retrying Count %d, Cmd ret = %d, shell curl understand ret value  = %d\n",i,ret,ret/255);
				ptr = strstr(Download_cmd,"-C -");
				if ( ptr != NULL )
				{
					ptr[0]=ptr[1]=ptr[2]=ptr[3]=' ';
					i--;
				}

				fprintf(stdout,"No -C - Download mode, Download CMD = %s\n",Download_cmd);
			}
			continue;
		}
		else 
		{	

			fprintf(stdout,"Error in server response, retrying %d, Cmd ret = %d, shell curl understand ret value  = %d \nDownload: Sleep 30secs\n",i,ret,ret/255);

			if ( i > 10  && Check_7days_Validity(Filename_with_Download_Location) == 0 )
			{
				fprintf(stdout,"Deleting %s Patch File, The Previous Download, Due to 7days completed\n", Filename_with_Download_Location);	
				remove(Filename_with_Download_Location);
			}			
			if ( ServerFileSize > 0 &&  DownloadedFileSize > ServerFileSize )
			{	
				remove(Filename_with_Download_Location);
				fprintf(stdout," %s Deleted, Due to DownloadedFileSize ( %d ) is greater than ServerFileSize ( %d)\n", Filename_with_Download_Location,DownloadedFileSize,ServerFileSize);
			}	
		}

		sleep(30);
	}	
	if ( Env_flag == 1 )
		unsetenv("LD_LIBRARY_PATH");
	return ret;
}



