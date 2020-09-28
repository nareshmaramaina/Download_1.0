#include<header.h>
char SerialID[24];

int Get_SerialID(char *SerialID)
{
	char macid[24];
	char format_machineid[24];
	int ret;

	memset(format_machineid,0,sizeof(format_machineid));
	ret = Get_format_machineid(format_machineid);

	if( ret == 0 )
		strcpy(SerialID,format_machineid);

	else 
	{

		memset(macid, 0, sizeof(macid));
		get_macid(macid);

		if(strlen(macid) == 0 )
		{
			strcpy(macid,"ERROR");
			return -1;
		}

		strcpy(SerialID,macid);

	}
	
	return 0;
}





int Get_format_machineid(char *format_machineid)
{
	short int ret=0,i;

	FILE *fp=NULL;

	char machineid[24]="";
 	char Buffer[24]="";


	for ( i=0; i< 5;i++)
	{
		ret = system("fw_printenv  machineid  > /tmp/.machineid");

		if ( ret == 0)
			break;

		sleep(1);
	}
	if ( ret != 0 )
	{
		fprintf(stderr,"Machine ID not Found \n");
		return ret;
	}


	fp = fopen("/tmp/.machineid", "r");

	if(fp ==NULL)
	{

		fprintf(stderr,"LIB#Unable To Open The /tmp/.machineid\n");
		return -1;

	}
	fread(Buffer,sizeof(Buffer),1,fp);
	fclose(fp);

	remove("/tmp/.machineid");

	if( Buffer[strlen(Buffer)-1] == '\n')
		Buffer[strlen(Buffer)-1] = '\0';
	if (  strlen(Buffer+10) != 10 )
	{
		fprintf(stderr,"Error: machineid  not 10 digits\n");
		return -1;
	}
	
	strcpy(machineid,Buffer+10);

	if(machineid[0]=='1' && machineid[1]=='1')
		sprintf(format_machineid,"1%s",machineid);

	else if (machineid[0]=='1' && machineid[1]>'4')
		sprintf(format_machineid,"11%s",machineid);

	else if(machineid[1] == '0' || machineid[0]=='2' )
		sprintf(format_machineid,"111%s",machineid);
	else 
	{
		fprintf(stderr,"machineid not in RHMS serialID format %s\n", machineid);
		return -2;
	}

	return 0;
}

int get_macid(char *macid)
{
	typedef unsigned char UC;
	struct ifreq ifr;
	int fd=0;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name,"eth0", IFNAMSIZ-1);
	if(ioctl(fd,SIOCGIFHWADDR, &ifr) <0){
		close(fd);
		return -1;
	}
	close(fd);

	sprintf(macid,"%02X:%02X:%02X:%02X:%02X:%02X",(UC)ifr.ifr_hwaddr.sa_data[0], (UC)ifr.ifr_hwaddr.sa_data[1],
			(UC)ifr.ifr_hwaddr.sa_data[2], (UC)ifr.ifr_hwaddr.sa_data[3],
			(UC)ifr.ifr_hwaddr.sa_data[4], (UC)ifr.ifr_hwaddr.sa_data[5]);
	return 0;
}

