#include<header.h>
/*void Write_Current_Version(int Version);
  int main()
  {
  Write_Current_Version(10);
  return 0;
  }*/
void Write_Current_Version(int Version)
{
	FILE *fp=NULL;
	char Buff[24];
	int RHMS_Ver=0,Download_ver=0,Install_Ver=0;
	char filename[]="/etc/vision/RHMS/RHMSClientversion.info";
	system("mkdir -p /etc/vision/RHMS/");
	fp = fopen(filename,"r");
	if ( fp == NULL )
		fprintf(stderr,"%s file not found\n",filename);
	else 
	{
		fread(Buff,20,1,fp);
		sscanf(Buff+8,"%d.%d.%d",&RHMS_Ver,&Download_ver,&Install_Ver);
		fclose(fp);
	}	

	fp = fopen(filename,"w");
	if ( fp == NULL )
		return;

	fprintf(fp,"Version:%d.%d.%d",RHMS_Ver,Version,Install_Ver);

	fclose(fp);

	return;
}
