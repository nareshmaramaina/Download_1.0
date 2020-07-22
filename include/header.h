#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <time.h>
#include<sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <termios.h>
#include <net/if.h>
#include<unistd.h>
#include<termios.h>
#include<errno.h>
#include <netdb.h> 
#include <resolv.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <dirent.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <signal.h>

struct Download_rhms
{
	char RTMServerIP[128];
	char ApplicationImageName[64];
	char ProjectName[64];
	char PortNo[32];
	char FTP_PATH_APP[2048];
	char FTP_PATH_FIRMWARE[2048];
	float ApplicationVersion;
	float FirmWarePatchVersion;
};
struct Download_rhms RHMS_Server;

struct POS 
{
	float ApplicationVersion;
	float FirmWarePatchVersion;
}Device;

struct Download
{
	int Total_Updates_count;
	float Update_Version[100];
}patch;
#define RHMS_Response_file "/tmp/.response_file.xml"
#define ACTIVE_MODE 1
#define PASSIVE_MODE 2



























int Download_Firmware_Updates(void);
int Download_Application_Updates(void);
int get_RHMS_response();
void get_POS_Firmware_version();
void get_POS_Application_version();
int  Download_config(int); // 1 - Firmware type config Download
int Download_patch(int); // 1 - Firmware type patch Download
int Download_lock();
int get_Machineid(char *machineid);
int get_macid(char *macid);
int wait_for_data(int sockfd,int timeout);
int get_device_serialnumber(char *serialnum);  // Updating Device serial number in RHMS health structures
int get_RHMS_Server_details(void);
void Wait_for_internet(void);
int Check_internet_by_hostname(char *host);
int Parsing_ACK_from_Server(); 
int wget_download (char *url,char *filename); // Mode= Active or Passive, retry_count =retry count to wget
int get_updates_info(int Type);
int Apply_Firmware_Patch(float patch_toUpdate );
int Apply_Application_Patch(float patch_toUpdate );
int Update_script();
void Send_Update_singal_to_app();
