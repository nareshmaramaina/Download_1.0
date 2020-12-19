#include<header.h>
extern char SerialID[24];
extern char *Install_Applications_file;
extern char *Install_Firmwares_file;

int Firm_Apps_Download_lock()
{
	int fd,ret=0;

	fd = open("/tmp/.Firm_Apps_Download_lock",O_CREAT | O_CLOEXEC);

	ret = flock(fd,LOCK_EX|LOCK_NB);

	if (ret != 0)
	{
		close(fd);
		return -1;
	}
	return 0;

}


int main()
{
	short int ret;
	int Downloader_Current_Version=5;
	int Apps_Downloads=0,Firmware_Downloads =0,imx_ret;
	fprintf(stdout,"\n*****************\nApp	: Downloader_FirmwareAndApps\nVersion	: %d\n*****************\n",Downloader_Current_Version);

	ret = Firm_Apps_Download_lock();

	if(ret < 0)    /* Case is Not To run Twice*/
	{
		fprintf(stderr,"Download Application is already Running\n");
		return -1;
	}
	sleep(1);
	Write_Current_Version(Downloader_Current_Version);
	fprintf(stdout,"Downloader: Sleep 6 secs\n");
	sleep(6); // For Startx Purpose 
	ret = Get_SerialID(SerialID);
	if ( ret != 0)
	{
		fprintf(stderr,"SerialID Error, %s, Please Set Unique MachineId\n",SerialID);
		return -1;
	}
	else 
		fprintf(stdout,"Device SerialID : %s\n\n",SerialID);

	Update_Configured_Server_Addr();


	imx_ret  = system("grep Hardware /proc/cpuinfo  |grep MX25 -q");
	if ( imx_ret == 0 )
		fprintf(stdout,"This is Imx25 device\n");


	ret = system("test -L /mnt/sysuser/Software-Upgrade"); // testing for symbolic link present or not

	if( imx_ret == 0 && ret != 0 )
	{

		system("mkdir -p /mnt/jffs2/Software-Upgrade/");
		if ( access("/mnt/sysuser/Software-Upgrade/",F_OK) == 0 )
		{
			system("mv /mnt/sysuser/Software-Upgrade/* /mnt/jffs2/Software-Upgrade/");
			remove("/mnt/sysuser/Software-Upgrade/");
		}
		ret = symlink("/mnt/jffs2/Software-Upgrade/","/mnt/sysuser/Software-Upgrade");
		if ( ret == 0 )
			fprintf(stdout,"Symbolic Link created Successfully\n");	
		else 
			fprintf(stdout,"Symbolic Link creation Failed \n");	
	}

	while(1)
	{
		Firmware_Downloads = Get_Total_Downloaded_Updates(FIRMWARE);
		if ( Firmware_Downloads > 0 )
			fprintf(stdout,"\n\033[1m\033[32m %d Firmware Downloads Completed, Reboot to Apply Patches ( Battery is Mandatory )\33[0m\n",Firmware_Downloads);
		Apps_Downloads = Get_Total_Downloaded_Updates(APPLICATION);
		if ( Apps_Downloads > 0 )
			fprintf(stdout,"\n\033[1m\033[32m %d Application Downloads Completed, Reboot to Apply Patches ( Battery is Mandatory )\33[0m\n",Apps_Downloads);


		ret = Firmware_Request_and_Response(); 
		if ( ret == -2 )
		{
			fprintf(stdout,"Please Do Register SerialID number in Suitable project, SerialID = %s\n", SerialID ); 
			return ret;
		}

		ret = Applications_Request_and_Response();
		if ( ret == -2 )
		{
			fprintf(stdout,"Please Do Register SerialID number in Suitable project, SerialID = %s\n", SerialID ); 
			return ret;
		}
		Download_Firmware_Updates();			

		Download_Application_Updates();

		/*run_time = is_RHMS_multiple_run();
		  if (  ( run_time % 60 == 0 ) && run_time < 3600  )
		  {
		  fprintf(stdout,"Downloader: Sleep for %d secs\n",run_time);
		  sleep(run_time);
		  }
		  else
		  {*/
		fprintf(stdout,"Downloader: sleep for 3 hr\n");
		sleep(3600*3);
		//}
		fprintf(stdout,"Looping For New Apps/Firmware Download Updates\n");
	}

	return 0;
}
