#include<header.h>
int Check_apps(const char *file);
int Get_Total_Device_Apps()
{
	int ret,count=0;
	DIR *dp;
	struct dirent *dirp;
	char *path="/etc/vision/RHMS/Apps/";
	dp = opendir(path);
	if( dp == NULL )
		fprintf(stdout,"%s directory not found\n",path);
	else 
	{
	//	chdir(path);
		while ( (dirp = readdir(dp)) )
		{
			if (	strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0 )
				continue;


			ret = Check_apps(dirp->d_name);
			if ( ret > 0 )
			{
				count=count+ret;
				fprintf(stdout,"Application Count %d, ApplicatinType = %s \n",ret,dirp->d_name );
			}
		}
		closedir(dp);
	}
	return count;
}
int main()
{

	///etc/vision/RHMS/APPS/PDS/APPDS/app.info  //
	//	/etc/vision/RHMS/FIRMWARE/4G/firmware.info  //

	//	/etc/vision/RHMS/APPS/PDS/APPDS/visiontek_app_release //  Downloaded file version maintenance

	int Total_Device_Apps =Get_Total_Device_Apps();
	fprintf(stdout,	" Total Device Applications = %d\n", Total_Device_Apps);

	return 0;
}
int Check_apps(const char *file)
{
	char *path="/etc/vision/RHMS/Apps";
	int count=0;
	char Extend_path[300];
	DIR *dp;
	struct dirent *dirp;
	memset(Extend_path,0,sizeof(Extend_path));
	sprintf(Extend_path,"%s/%s",path,file);
	dp = opendir(Extend_path);
	if( dp == NULL)
		return -1;
	else
	{
		while ( ( dirp = readdir(dp) ) )
		{
			if (    strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0 )
				continue;
			memset(Extend_path,0,sizeof(Extend_path));
			sprintf(Extend_path,"%s/%s/%s/app.info",path,file,dirp->d_name);

			if ( access(Extend_path,F_OK) == 0 )
			{
				count++;
				fprintf(stdout," %s \n",Extend_path );
			}
			else 
				fprintf(stdout," file %s not found \n",Extend_path );


		}
		closedir(dp);
	}
	return count;
}
