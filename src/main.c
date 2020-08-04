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
	int Apps_Downloads=0,Firmware_Downloads =0;

	fprintf(stdout,"\n*****************\nApp	: Firmware_and_Apps_Downloader\nVersion	: 1.0\n*****************\n");

	ret = Firm_Apps_Download_lock();

	if(ret < 0)    /* Case is Not To run Twice*/
	{
		fprintf(stderr,"Download Application is already Running\n");
		return -1;
	}
	ret = Get_SerialID(SerialID);
	if ( ret != 0)
	{
		fprintf(stderr,"SerialID Error, %s, Please Set Unique MachineId\n",SerialID);
		return -1;
	}


	Update_Configured_Server_Addr();

	while(1)
	{
			Firmware_Downloads = Get_Total_Downloaded_Updates(FIRMWARE);
			if ( Firmware_Downloads > 0 )
			fprintf(stdout,"\n\033[1m\033[32m %d Firmware Downloads Completed, Reboot to Apply Patches ( Battery is Mandatory )\33[0m\n",Firmware_Downloads);
			Apps_Downloads = Get_Total_Downloaded_Updates(APPLICATION);
			if ( Apps_Downloads > 0 )
		       	fprintf(stdout,"\n\033[1m\033[32m %d Application Downloads Completed, Reboot to Apply Patches ( Battery is Mandatory )\33[0m\n",Apps_Downloads);

		Wait_for_internet();

		ret = Firmware_Request_and_Response(); 
		if ( ret == -2 )
		{
			fprintf(stdout,"Please Do Register SerialID number in Suitable project, SerialID = %s\n", SerialID ); 
			return ret;
		}
		else if ( ret == 0 )
			fprintf(stdout,"Firmware Request  and Response done Successfully\n");
		else 
			fprintf(stderr,"Firmware Request Failure\n");

		ret = Applications_Request_and_Response();
		if ( ret == -2 )
		{
			fprintf(stdout,"Please Do Register SerialID number in Suitable project, SerialID = %s\n", SerialID ); 
			return ret;
		}
		else if ( ret == 0 )
			fprintf(stdout,"Applications Request and Response done Successfully\n");
		else 
			fprintf(stderr,"Applications Request Failure\n");

		Download_Firmware_Updates();			

		Download_Application_Updates();
		
		fprintf(stdout,"Download sleep for 1 hr\n");
		sleep(60*60*1); // Wait for 1 hours
	}

	return 0;
}
