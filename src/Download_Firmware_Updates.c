#include<header.h>
int Download_Firmware_Updates(void)
{
	short int ret;
	

	ret = Download_config(1); // 1 - Firmware type config Download

	if ( ret != 0 )
	{
		fprintf(stderr," Firmware config-patchversions file Download failed\n");
		return ret;	
	}


	return  Download_patch(1); // 1 - Firmware type patch Download
	
}
