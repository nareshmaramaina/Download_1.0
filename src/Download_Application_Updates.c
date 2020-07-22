#include<header.h>
int Download_Application_Updates(void)
{
	short int ret;
	

	ret = Download_config(2); // 2 - App type config Download

	if ( ret != 0 )
	{
		fprintf(stderr," App config-patchversions file Download failed\n");
		return ret;	
	}


	return  Download_patch(2); // 2 - App type patch Download
	
}
