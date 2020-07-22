include<header.h>

int Download_lock()
{
	int fd,ret=0;

	fd = open("/tmp/.Download_lock",O_CREAT | O_CLOEXEC);

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
	short int i,App_Apply = 0, Firmware_Apply = 0;
	short int RHMS_ret,ret;
	DIR *dp=NULL;

	memset(&RHMS_Server,0,sizeof(struct Download_rhms));
	memset(&Device,0,sizeof(struct POS));


	system("mkdir -p /mnt/sysuser/App-Upgrade/");
	system("mkdir -p /mnt/sysuser/Firmware-Upgrade/");

	fprintf(stdout,"\n*****************\nApp	: Download ( Firmware and Application )\nVersion	: 1.3\n*****************\n");

	ret = Download_lock();

	if(ret < 0)    /* Case is Not To run Twice*/
	{
		fprintf(stderr,"Download Application is already Running\n");
		return -1;
	}

	while(1)
	{

		if( ( access("/mnt/sysuser/Firmware-Upgrade/download_complete",F_OK) == 0 ) && ( ( dp = opendir("/mnt/sysuser/Firmware-Upgrade/Extract/")) != NULL)  )

		{
			fprintf(stdout,"\n\033[1m\033[32mFirmware Downloads Completed, Reboot to Apply Patches ( Battery is Mandatory )\33[0m\n");

			Firmware_Apply =1 ;
		}
		if( ( access("/mnt/sysuser/App-Upgrade/download_complete",F_OK) == 0 ) && ( (dp = opendir("/mnt/sysuser/App-Upgrade/Extract/")) != NULL)  )
		{
			fprintf(stdout,"\n\033[1m\033[32mApplication Downloads Completed, Reboot to Apply Patches ( Battery is Mandatory )\33[0m\n");

			Send_Update_singal_to_app();
			App_Apply =1;
		}       


		if ( Firmware_Apply == 1 && App_Apply == 1 )
		{
			fprintf(stdout,"Reboot the Device for Applying Firmware and Application Patches\n");
			sleep(60*60*1); // Wait for 1 hours
			continue;
		}

		else 
		{	
			Wait_for_internet();

			for ( i = 0; i < 6 ;i++)
			{
				RHMS_ret = get_RHMS_response();

				if ( RHMS_ret == -404 )
					continue;

				else break;
			}

		}


		if (  Firmware_Apply != 1 )
		{
			get_POS_Firmware_version();

			if ( Device.FirmWarePatchVersion < RHMS_Server.FirmWarePatchVersion )
			{
				fprintf(stdout,"Device Firmware Update Found\n");
				Download_Firmware_Updates();			
			}
			else fprintf(stdout,"\nNo Firmware Updates\n");
			fprintf(stdout,"\nDevice.FirmWarePatchVersion = %.1f RHMS_Server.FirmWarePatchVersion = %.1f\n", Device.FirmWarePatchVersion,RHMS_Server.FirmWarePatchVersion);
		}

		if ( App_Apply != 1 )
		{

			get_POS_Application_version();

			if ( Device.ApplicationVersion < RHMS_Server.ApplicationVersion )
			{
				fprintf(stdout,"\nApplication Update Found\n");
				Download_Application_Updates();
			}
			else fprintf(stdout,"\nNo Application Updates\n");
			fprintf(stdout,"\n,Device.ApplicationVersion = %.1f RHMS_Server.ApplicationVersion = %.1f\n",Device.ApplicationVersion,RHMS_Server.ApplicationVersion);

		}

		if ( RHMS_ret == -404 )
		{
			fprintf(stdout,"Waiting  2mins for Server Connection ");
			sleep(60*2);

		}
		else
		{	
			fprintf(stdout,"Waiting for New Updates (  Application and Firmware ), sleep 3 hours\n");
			sleep(120);
			//sleep(60*60*3); // Wait for 3 hours

		}

	}

	return 0;
}
