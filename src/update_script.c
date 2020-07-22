#include<header.h>
char *Update_buff="#!/bin/bash\nExtracted_path=/mnt/sysuser/Firmware-Upgrade/Extract/\nupdate_path=/mnt/sysuser/Firmware-Upgrade/Extract/patch\nBootfile_Images_Path=$update_path/boot_files\nclear_uboot_files()\n{\nrm -rf $Bootfile_Images_Path/u-boot.bin\nrm -rf $Bootfile_Images_Path/uboot.md5\n}\nclear_kernel_files()\n{\nrm -rf $Bootfile_Images_Path/kernel.IMG\nrm -rf $Bootfile_Images_Path/kernel.md5\n}\nkernel_loading()\n{\ncd $Bootfile_Images_Path/\nmd5sum -c kernel.md5\nretval=$?\nif [ $retval -ne 0 ] \nthen\necho \"Error:1 Kernel File Corrupted\"\necho \"\"\ncd\nclear_kernel_files\nreturn 1\nfi\nif [ -f $Bootfile_Images_Path/kernel.IMG ]\nthen\n\necho \"Loading  GL11 iMX6  Kenel Image\" \ndd if=$Bootfile_Images_Path/kernel.IMG of=/dev/mmcblk0 bs=1M seek=1 conv=fsync\necho \"Kernel Loaded Successfully\"\nreturn 0\nelse\necho \"Error 2:Kernel File Not Found\"\necho \"\"\nreturn 2\nfi\n}\nuboot_loading()\n{\ncd $Bootfile_Images_Path\nmd5sum -c uboot.md5\nretval=$?\nif [ $retval -ne 0 ]\nthen\necho \"Error:1 Uboot File Corrupted\"\necho \"\"\ncd\nclear_uboot_files\nreturn  1\nfi\nif [ -f $Bootfile_Images_Path/u-boot.bin  ]\nthen\necho \"Loading GL11 iMX6 U-Boot Image\"\necho 8 > /sys/devices/platform/sdhci-esdhc-imx.1/mmc_host/mmc1/mmc1:0001/boot_config\ndd if=/dev/zero of=/dev/mmcblk0 bs=512 seek=1536 count=8\necho 0 > /sys/block/mmcblk0boot0/force_ro\ndd if=$Bootfile_Images_Path/u-boot.bin of=/dev/mmcblk0boot0 bs=512 seek=2 skip=2\necho 1 > /sys/block/mmcblk0boot0/force_ro\necho 8 > /sys/devices/platform/sdhci-esdhc-imx.1/mmc_host/mmc1/mmc1:0001/boot_config\necho \"U-Boot Loaded Successfully\"\nreturn 0\nelse\necho \"Error :2UBoot File Not Found\"\necho \"\"\nreturn  2\nfi\n}\n\nupdate_etc_visiontek_release()\n{\nUpdated_Version=$(cat /root/.Firmware_version)\nDate=$(date +%d%m%Y-%H:%M)\nmd5sum=$(cut /mnt/sysuser/Firmware-Upgrade/Extract/patch.md5 -d ' ' -f1)\necho \"PATCH_VERSION=$Updated_Version\nPATCH_DATE=$Date\nPATCH_MD5SUM=$md5sum\" > /etc/visiontek_release\necho \"$Updated_Version Firmware Patch updated successfully...\"\n}\nif [ ! -d $update_path ]\nthen\necho \"$update_path Directory not found\" \nexit 1\nfi\nchmod 777 -R  $update_path/ \nif [ -e $Bootfile_Images_Path/u-boot.bin ]  && [ -e $Bootfile_Images_Path/uboot.md5 ]\nthen\necho \"---------------UBoot Files Found ------------------\"\nuboot_loading\nret=$?\nif [ $ret -eq 0 ]\nthen\nclear_uboot_files\nfi\nsleep 3\nsync\nelse\necho \"---------------UBoot Files Not found-----------\"\nfi\nif [ -e $Bootfile_Images_Path/kernel.IMG ]  &&  [  -e $Bootfile_Images_Path/kernel.md5 ]\nthen\necho \"--------------Kernel Files Found------------------\"       \nsleep 3\nkernel_loading\nret=$?\nif [ $ret -eq 0 ]\nthen\nclear_kernel_files\nfi\nsleep 3\nsync\nelse\necho \"------------------Kernel Files Not found----------\"       \nfi\nif [ -e $update_path/tmp/start_fw.sh ]\nthen\necho \"Running Pre Management Work For patch \"\nsh $update_path/tmp/start_fw.sh \nsleep 1\nfi\n\n`yes | cp -r $update_path/*   /`\n\nupdate_etc_visiontek_release \n\nif [ -e $update_path/tmp/end_fw.sh ]\nthen\necho \"Post Managemnt Work For Patch \"\nsh $update_path/tmp/end_fw.sh \nfi\nrm -rf $Extracted_path\nexit 0\n#VISIONTEK#\n";
int Update_script()
{

	FILE *fp= fopen("/vision/update.sh","w");

	if ( fp == NULL )
	{
		fprintf(stderr,"/vision/update.sh write Failed\n");
		return -1;
	}

	fprintf(fp,"%s",Update_buff);

	fclose(fp);

	sleep(5);

	return 0;
}
