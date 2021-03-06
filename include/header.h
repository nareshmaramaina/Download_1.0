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
#define FIRMWARE 1
#define APPLICATION 2
struct RHMSApplication
{
	char Type[128];
	char Name[128];
	char URL[512];
	float Version;
};

struct POSApplication 
{
	char Type[128];
	char Name[128];
	float Version;
};
struct RHMSFirmware
{
	char URL[512];
	float Version;
};
int Add_to_installation(float ,char *path,char *patch,int type); // Type 1 for Firmware 2 for Application
int App_updates();
int Firmware_updates();
int Get_format_machineid(char *);
int get_macid(char *);
int Get_SerialID(char *SerialID);
int firmware_request();
int applications_request();
int Parse_Application_response_xml();
int Parse_Firmware_response_xml();
int  Update_Configured_Server_Addr();
int  Firm_Apps_Download_lock();
int Update_Configured_Server_Addr();
void Wait_for_internet();
int  Firmware_Request_and_Response();
int  Applications_Request_and_Response();
int Download_Firmware_Updates();
int Download_Application_Updates();
int Download_Application_Updates(void);
int Download_Firmware_Updates(void);
int Download_Update(char *URL, char *Filename_with_Download_Location);
void Update_Current_Date_with_Time(char *Date_time);
int check_Download_complete(char *patch,int type);
int Get_Total_Downloaded_Updates(int type);
int Device_App_info_Details(char *Device_Application_release_file,char *DeviceApplicationType,char *DeviceApplicationName,float *DeviceApplicationVersion );
void Create_Firmware_DownloadCompleted_file(char *path,float Version);
void Create_Application_DownloadCompleted_file(char *path,float Version);
int Check_Address_Error_and_Update_Server_Addr_If_Error_Present();
int Firmware_Request();
int Applications_Request();
int is_RHMS_multiple_run();
void Write_Current_Version(int Version);
int Get_Tokens_of_FirmwarePatchfile(char *file,char *FirmwareName,float *Version,char *ProjectName);
int Check_date_set_if_wrong();

int Get_Tokens_of_ApplicationPatchfile(char *file,char *ApplicationType,char *ApplicationName,float *Version,char *ProjectName);
int Check_7days_Validity(char *filename);
int filesize_at_server(char *URL,int Env_flag);
int size_of_file(char *file);
int Check_md5sum_of_Patch(char *PatchFile );
