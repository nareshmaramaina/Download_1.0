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
	int Downloader_Current_Version=3;
	int Apps_Downloads=0,Firmware_Downloads =0,run_time=0;
	fprintf(stdout,"\n*****************\nApp	: Downloader_FirmwareAndApps\nVersion	: %d\n*****************\n",Downloader_Current_Version);

	ret = Firm_Apps_Download_lock();

	if(ret < 0)    /* Case is Not To run Twice*/
	{
		fprintf(stderr,"Download Application is already Running\n");
		return -1;
	}
	fprintf(stdout,"Downloader: Sleep 8 secs\n");
	sleep(8); // For Startx Purpose 
	Write_Current_Version(Downloader_Current_Version);
	ret = Get_SerialID(SerialID);
	if ( ret != 0)
	{
		fprintf(stderr,"SerialID Error, %s, Please Set Unique MachineId\n",SerialID);
		return -1;
	}
	else 
		fprintf(stdout,"Device SerialID : %s\n\n",SerialID);

	Update_Configured_Server_Addr();

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

		run_time = is_RHMS_multiple_run();
		if (  ( run_time % 60 == 0 ) && run_time < 3600  )
		{
			fprintf(stdout,"Downloader: Sleep for %d secs\n",run_time);
			sleep(run_time);
		}
		else
		{
			fprintf(stdout,"Downloader: sleep for 1 hr\n");
			sleep(3600);
		}
		fprintf(stdout,"Searching For New Apps/Firmware Download Updates\n");
	}

	return 0;
}
