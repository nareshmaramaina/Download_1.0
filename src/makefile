CC= arm-none-linux-gnueabi-gcc -Wall 
bin_strip=arm-none-linux-gnueabi-strip

CFLAGS=-Wall  -I ../include
libs=-L ../lib -lxml2 -lz
Firmware_And_Apps_Downloader:main.c Application_request.c  firm_apps_response_parser.c  Firmware_request.c Get_SerialID.c Download_Application_Updates.c  Download_Firmware_Updates.c Update_Configured_Server_Addr.c  Wait_for_internet.c Download_Update.c Add_to_installation.c Device_App_info_Details.c 


	$(CC)  $?   $(CFLAGS)  $(libs) -o $@
	$(bin_strip) $@
clean:
	rm  Firmware_And_Apps_Downloader

