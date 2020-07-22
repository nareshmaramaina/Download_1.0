#include<header.h>
int main(int argc, char *argv[])
{
	FILE *fp;

	char cmd[128];


	char macid[24]="";

	int cfd,count=0,ret=0,sent=0;

	char server_ip[50];

	char port_num[12];

	char Project_name[24];

	struct timeval tv;

	char read_buff[1024];

	char write_buff[2048];

	struct sockaddr_in addr;

	char start_buff[256]="";

	char start_buff_rev[256]="";

	int startbuff_len=0;

	char serialnum[25];

	memset( start_buff,0,sizeof(start_buff));
	memset(server_ip,0,sizeof(server_ip));
	memset(port_num,0,sizeof(port_num));
	memset(Project_name,0,sizeof(Project_name));

	memset(serialnum,0,sizeof(serialnum));

	get_device_serialnumber(serialnum);

	ret = get_project_details(server_ip, port_num, Project_name);

	if( ret < 0 )
	{
		strcpy(server_ip,"202.53.78.186");
		strcpy(port_num,"5051");
	}


	if ( (strcmp(serialnum,"ERROR") ) == 0 || strlen(serialnum) == 0 )
	{
		memset(macid,0,sizeof(macid));
		get_macid(macid);
		if (strlen(macid) < 10 )
		{
			fprintf(stderr,"Macid error : Value miss match / or eth0 interface not found \n");
			return -1;
		}

		if(ret != 0)

			sprintf(start_buff,"#Health#%s#",macid);

		else
			sprintf(start_buff,"#Health#%s#%s",macid,Project_name);
	}

	else
	{
		if(ret != 0)

			sprintf(start_buff,"#Health#%s#",serialnum);

		else
			sprintf(start_buff,"#Health#%s#%s",serialnum,Project_name);

	}





	startbuff_len = strlen(start_buff);



	tv.tv_sec=1;		// set to 15 sec for recv to w
	tv.tv_usec=0;		

	cfd=socket(PF_INET, SOCK_STREAM,0);


	if(setsockopt(cfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,sizeof(tv))<0)
		perror("setsockopt");


	memset(&addr, '0', sizeof(addr));

	addr.sin_family=PF_INET;

	addr.sin_addr.s_addr=inet_addr(server_ip); 

	addr.sin_port=htons(atoi(port_num));

	fprintf(stdout,"Connecting ... Server Ip Address = %s\t Port Number = %s\n",server_ip,port_num);
	if(connect(cfd, (struct sockaddr *)&addr,sizeof(addr))<0) 
	{
		perror("Server connection error...\n");
		return -1;
	}

	fprintf(stdout,"\nConnected to server\n");


	fprintf(stderr,"Sending Handshake Frame is %s\n",start_buff);	
	if(send(cfd, start_buff, strlen(start_buff),0) != startbuff_len)
	{
		close(cfd);
		perror("SEND : ");	
		return -1;
	}


	memset( start_buff_rev,0,sizeof(start_buff_rev));
	if ( wait_for_data(cfd,30) < 0 )
	{	
		close(cfd);
		puts("Acknowledgment failed, Time out  ");
		return -1;
	}

	recv(cfd,start_buff_rev,sizeof(start_buff_rev),0);


	if((strcmp(start_buff_rev,start_buff))!=0)
	{
		close(cfd);
		fprintf(stderr,"Response doesn't matched with the Handshaking frame\n");
		return -1;
	}

	send(cfd,"#END#",6,0); // We are Stopping for Response


	fprintf(stdout,"Number of bytes Sent are %d\n",sent);

	memset(read_buff ,0x00, sizeof(read_buff));	

	memset(write_buff,0x00,sizeof(write_buff));

	count=0;
	fprintf(stdout,"Receving... Server Response\n");

	for (count=0;count< 2;count++)
	{	

		if ( wait_for_data(cfd,80) < 0 )
		{	
			close(cfd);
			fprintf(stderr,"Timeout, Unable to receive data from Server\n");
			return -1;
		}

		recv(cfd,read_buff,1024,0);

		if(strstr(read_buff,"</GeneralSettings>") != NULL)
		{
			sprintf(write_buff,"%s%s",write_buff,read_buff);
			printf("Succesfully Received %d bytes from server\n",(int)strlen(write_buff));
			break;
		}
		else if(strstr(read_buff,"@END@")!=NULL)
		{
			printf("Server Response not found, xml file is Empty...\n");
			close(cfd);
			return -1;
		}
		else
		{
			sprintf(write_buff,"%s%s",write_buff,read_buff);
			memset(read_buff,0x00,sizeof(read_buff));
		}

	}


	close(cfd);  // socket 

	fp = fopen("/tmp/.response_file.xml","w");
	if(fp  == NULL)
	{
		fprintf(stderr,"Ack_form_server File Not created \n");
		return -1;
	}

	fprintf(fp,"%s",write_buff);

	fclose(fp);


	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"dos2unix %s","/tmp/.response_file.xml");

	system(cmd);


	fprintf(stdout,"\n\n\33[31m************		Response		************\33[0m\n\n%s\n\n\33[31m************         Response                ************\33[0m\n\n", write_buff);

	return 0;
}

int wait_for_data(int sockfd,int timeout)
{
	int bytes,i;
	printf("Timeout secs = %d\n",timeout);
	for(bytes=0,i=0 ; i < timeout * 2 ;i++)
	{
		ioctl(sockfd,FIONREAD,&bytes);

		if(bytes>0)
			return bytes;

		usleep(500000); // Wait for 0.5 sec
		if(i%10 == 0 )
			printf("No Data Found for read, Retrying..., Secs completed %d\n",i/2);
	}
	return -1;
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

int get_Machineid(char *machineid)
{

	int ret=0;

	FILE *fp=NULL;

	ret = system("fw_printenv machineid | grep machineid= | cut -d '=' -f2 > /tmp/.machineid.txt");

	if(ret != 0 )
		return -1;

	fp = fopen("/tmp/.machineid.txt", "r");

	if(fp ==NULL)
	{

		fprintf(stderr,"LIB#Unable To Open The /tmp/.machineid.txt\n");
		return -1;

	}

	fscanf(fp,"%s",machineid);

	fclose(fp);

	remove("/tmp/.machineid.txt");

	if ( strlen(machineid) == 0 )
		return -1;
	else
		return 0;

}

int get_device_serialnumber(char *serialnum)  // Updating Device serial number in RHMS health structures
{

	char machineid[25];

	memset(machineid,0,sizeof(machineid));	
	get_Machineid(machineid);

	if(strlen(machineid) != 10)
	{
		fprintf(stdout,"Fail to get machine ID Or Wrong Machineid set\n");
		strcpy(serialnum,"ERROR");
		return -1;
	}


	if(machineid[0]=='1' && machineid[1]=='1')
		sprintf(serialnum,"1%s",machineid);

	else if (machineid[0]=='1' && machineid[1]>'4')
		sprintf(serialnum,"11%s",machineid);

	else if(machineid[1] == '0')
		sprintf(serialnum,"111%s",machineid);

	else
	{
		sprintf(serialnum,"ERROR");
		fprintf(stderr,"Serial Number is  not in RHMS order %s \n",serialnum);
		return -1;
	}

	return 0;
} 
