#include <unistd.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<time.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
int Get_format_machineid(char *);
int get_macid(char *);
int Get_SerialID(char *SerialID);
int firmware_request();
int applications_request();
int Request(int request_no); //1 Firmware_Request, 2 Application_Request, remain numbres unknown requests
int Parse_Application_response_xml();
int Parse_Firmware_response_xml(); 
char SerialID[24];
int main()
{
	short int ret;

	ret = Get_SerialID(SerialID);
	if ( ret != 0)
	{
		fprintf(stderr,"SerialID Error, %s, Please Set Unique MachineId\n",SerialID);
		return -1;
	}

	ret = firmware_request();
	if ( ret == -2 )
	{
	fprintf(stdout,"Please Do Register SerialID number in Suitable project, SerialID = %s\n", SerialID ); 
	return ret;
	}

	applications_request();

	return 0;
}

int firmware_request()
{
	int ret;

	ret  = Request(1); 

	if ( ret == 0 )
		fprintf(stdout," Firmware response Updated Successfully\n");
	else 
		fprintf(stderr,"Firmware request Failure\n");

	return ret;

}
int applications_request()
{
	int ret;

	ret = Request(2); 

	if ( ret == 0 )
		fprintf(stdout,"Application response Updated Successfully\n");
	else 
		fprintf(stderr,"Applications request Failure\n");

	return ret;
}


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

	char machineid[15]="";


	for ( i=0; i< 5;i++)
	{
		ret = system("fw_printenv  machineid | cut -d '=' -f2 > /tmp/.machineid");

		if ( ret == 0)
			break;

		sleep(1);
	}

	fp = fopen("/tmp/.machineid", "r");

	if(fp ==NULL)
	{

		fprintf(stderr,"LIB#Unable To Open The /tmp/.machineid\n");
		return -1;

	}

	fscanf(fp,"%s",machineid);

	fclose(fp);

	remove("/tmp/.machineid");

	if (  strlen(machineid) != 10 )
	{
		fprintf(stderr,"Error: machineid not 10 digits, %s\n",machineid);
		return -1;
	}

	if(machineid[0]=='1' && machineid[1]=='1')
		sprintf(format_machineid,"1%s",machineid);

	else if (machineid[0]=='1' && machineid[1]>'4')
		sprintf(format_machineid,"11%s",machineid);

	else if(machineid[1] == '0')
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

	sprintf(macid,"%02X%02X%02X%02X%02X%02X",(UC)ifr.ifr_hwaddr.sa_data[0], (UC)ifr.ifr_hwaddr.sa_data[1],
			(UC)ifr.ifr_hwaddr.sa_data[2], (UC)ifr.ifr_hwaddr.sa_data[3],
			(UC)ifr.ifr_hwaddr.sa_data[4], (UC)ifr.ifr_hwaddr.sa_data[5]);
	return 0;
}

int  Get_Server_Addr(char *Server_Addr)
{
	FILE *fp;
	size_t len=0;
	char *str=NULL;


	fp = fopen("/etc/.RHMS_Server.config","r");

	if (fp == NULL)
	{
		fprintf(stderr," /etc/.RHMS_Server.config open error\n");
		return -1;
	}

	while((getline(&str,&len,fp)) != -1)
	{
		if  (strstr(str,"ServerAddress:") != NULL )
		{
			strcpy(Server_Addr,str+14);
			break;
		}
	}
	free(str);
	str=NULL;
	fclose(fp);

	if(Server_Addr[strlen(Server_Addr)-1] == '\n')
		Server_Addr[strlen(Server_Addr)-1]='\0';

	return 0;
}
int Request(int request_no) //1 Firmware_Request, 2 Application_Request, remain numbres unknown requests
{

	int ret;
	char Server_Addr[512];
	FILE    *fp;
	char    cmd[1024];
	size_t len=0;
	char *str=NULL;
	char Response_xml_file[34]="";

	memset(Server_Addr,0,sizeof(Server_Addr));
	memset(cmd,0,sizeof(cmd));

	ret = Get_Server_Addr(Server_Addr);
	if( ret != 0 )
		strcpy(Server_Addr,"https://rhms2.callippus.co.uk");


	if ( request_no == 1 ) //  Firmware_Request 
	{
		strcpy(Response_xml_file,"/opt/Firmware_response.xml");
		sprintf(cmd,"curl -k %s/api/FirmwareStatus?serialNo=%s  > %s",Server_Addr,SerialID,Response_xml_file);
	}
	else if ( request_no == 2) // Application_Request 
	{
		strcpy(Response_xml_file,"/opt/Application_response.xml");
		sprintf(cmd,"curl -k %s/api/ApplicationStatus?serialNo=%s  > %s",Server_Addr,SerialID,Response_xml_file);
	}

	else
	{
		fprintf(stderr,"Unknown request\n");
		return -1;
	}


	system(cmd);

	puts(cmd);

	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"dos2unix %s",Response_xml_file);

	system(cmd);


	fp = fopen(Response_xml_file,"r");

	if (fp == NULL)
	{
		fprintf(stderr,"%s open error\n",Response_xml_file);
		return -1;
	}


	while((getline(&str,&len,fp)) != -1)
	{
		if  (strstr(str,"Device is not registered") != NULL )
		{
			ret = -2;
			break;
		}

		puts(str);
	}

	free(str);
	str=NULL;
	fclose(fp);

	if( ret == -2 )
		return ret;

	if ( request_no == 1 )
		ret = 	Parse_Firmware_response_xml(); 

	else if ( request_no == 2 )
		ret =	Parse_Application_response_xml();

	return ret;
}

