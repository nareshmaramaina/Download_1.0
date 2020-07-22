CC= arm-none-linux-gnueabi-gcc -Wall 
bin_strip=arm-none-linux-gnueabi-strip

CFLAGS=-Wall  -I ../include
libs=-L ../lib -lxml2 -lz
Common_Download:main.c get_config_details.c get_POS_Application_version.c get_POS_Firmware_version.c get_RHMS_response.c  xmlparser.c Wait_for_internet.c Download_patch.c get_updates_info.c  Download_Firmware_Updates.c Download_config.c wget.c  update_script.c Apply_Application_Patch.c  Apply_Firmware_patch.c  Download_Application_Updates.c Send_Update_singal_to_app.c 
	  
	$(CC)  $?   $(CFLAGS)  $(libs) -o $@
	$(bin_strip) $@
#	cp Common_Download  /u01/Common_apps	
clean:
	rm Common_Download 

