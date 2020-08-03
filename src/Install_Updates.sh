#!/bin/bash
kernel_loading()
{
	cd $1
	md5sum -c kernel.md5
	retval=$?
	if [ $retval -ne 0 ] 
	then
		echo "Error:1 Kernel File Corrupted"
		echo ""
		return 1
	fi
	if [ -f $1/kernel.IMG ]
	then

		echo "Loading  GL11 iMX6  Kenel Image" 
		dd if=$1/kernel.IMG of=/dev/mmcblk0 bs=1M seek=1 conv=fsync
		echo "Kernel Loaded Successfully"
		return 0
	else
		echo "Error 2:Kernel File Not Found"
		echo ""
		return 2
		fi
	}
uboot_loading()
{
	cd $1
	md5sum -c uboot.md5
	retval=$?
	if [ $retval -ne 0 ]
	then
		echo "Error:1 Uboot File Corrupted"
		echo ""
		return  1
	fi
	if [ -f $1/u-boot.bin  ]
	then
		echo "Loading GL11 iMX6 U-Boot Image"
		echo 8 > /sys/devices/platform/sdhci-esdhc-imx.1/mmc_host/mmc1/mmc1:0001/boot_config
		dd if=/dev/zero of=/dev/mmcblk0 bs=512 seek=1536 count=8
		echo 0 > /sys/block/mmcblk0boot0/force_ro
		dd if=$1/u-boot.bin of=/dev/mmcblk0boot0 bs=512 seek=2 skip=2
		echo 1 > /sys/block/mmcblk0boot0/force_ro
		echo 8 > /sys/devices/platform/sdhci-esdhc-imx.1/mmc_host/mmc1/mmc1:0001/boot_config
		echo "U-Boot Loaded Successfully"
		return 0
	else
		echo "Error :2UBoot File Not Found"
		echo ""
		return 2
		fi
	}

update_etc_visiontek_Firmware_release()
{
	release_file=/etc/visiontek_Firmware_release
	Updated_Version=$( echo $1 | cut -d '-'  -f3 | cut  -c1-3 )
	Date=$(date +%d%m%Y-%H:%M)
	md5sum=$(cut $2/patch.md5  -d ' ' -f1)
	echo "FirmwareName:$(echo $1| cut -d '/' -f6)" > $release_file
	echo "Version:$Updated_Version" >> $release_file
	echo "Firmware_md5sum=$md5sum" >> $release_file
	echo "Installed_Date=$Date" >> $release_file
	echo "$Updated_Version Firmware Patch updated successfully..."

}
Firmware_Update()
{
	Firmware_install_file=/mnt/sysuser/Software-Upgrade/Firmware_Downloads/Install_Firmwares
	#	/mnt/sysuser/Software-Upgrade/Applications_Downloads/Install_Applications
	if test -f $Firmware_install_file
	then
		echo " Pending firmware installations $(wc -l $Firmware_install_file)"
	else 
		echo "$Firmware_install_file file not found"
		exit 1
	fi

	for patch in $(cat $Firmware_install_file)
	do
		echo "Installing $patch" 

		if test ! -f $patch 
		then
			echo "$patch file not found"
			exit 1
		fi

		Extracted_path=$(dirname $patch)/Extract	
		update_path=$Extracted_path/patch
		Bootfile_Images_Path=$update_path/boot_files
		echo $patch $Extracted_path
		rm -rf $Extracted_path 
		mkdir  -p $Extracted_path
		unzip $patch -d $Extracted_path
		ret=$?
		if [ $ret -ne 0 ]
		then
			echo "$patch zip failed"
			return 1
		fi
		cd $Extracted_path

		if test ! -f patch.tar.bz2  || test ! -f patch.md5 
		then
			echo " After Extration firmware patch files not found"
			exit 1
		fi
		md5sum -c patch.md5
		ret=$?
		if [ $ret -ne 0 ]
		then
			echo "md5sum  failed"
			return 1
		fi
		tar -xvjf patch.tar.bz2 >> /etc/.Firmware_patch_history  
		ret=$?
		if [ $ret -ne 0 ]
		then
			tar -xvf patch.tar.bz2 >> /etc/.Firmware_patch_history 
			ret=$?
			if [ $ret -ne 0 ]
			then
				echo "tar  failed"
				return 1
			fi
		fi

		if [ ! -d $update_path ]
		then
			echo "$update_path Directory not found" 
			exit 1
		fi
		chmod 777 -R  $update_path/ 
		if [ -e $Bootfile_Images_Path/u-boot.bin ]  && [ -e $Bootfile_Images_Path/uboot.md5 ]
		then
			echo "---------------UBoot Files Found ------------------"
			uboot_loading $Bootfile_Images_Path
			ret=$?
			if [ $ret -eq 0 ]
			then

				rm  $Bootfile_Images_Path/u-boot.bin
				rm  $Bootfile_Images_Path/uboot.md5
			fi
			sleep 3
			sync
		fi
		if [ -e $Bootfile_Images_Path/kernel.IMG ]  &&  [  -e $Bootfile_Images_Path/kernel.md5 ]
		then
			echo "--------------Kernel Files Found------------------"       
			sleep 3
			kernel_loading $Bootfile_Images_Path
			ret=$?
			if [ $ret -eq 0 ]
			then
				rm  $Bootfile_Images_Path/kernel.IMG
				rm  $Bootfile_Images_Path/kernel.md5
			fi
			sleep 3
			sync
		fi
		if [ -e $update_path/tmp/start_fw.sh ]
		then
			echo "Running Pre Management Work For patch "
			sh $update_path/tmp/start_fw.sh 
			sync
		fi

		`yes | cp -r $update_path/*   /`


		if [ -e $update_path/tmp/end_fw.sh ]
		then
			echo "Post Managemnt Work For Patch "
			sh $update_path/tmp/end_fw.sh 
			sync
		fi
		update_etc_visiontek_Firmware_release $patch $Extracted_path 

		#sed '/$patch/d/' Firmware_install_file
		#rm -rf $Extracted_path
		#rm $patch
		#reboot
	done
	return 0
}
if test $1 
then 
	echo "Argument Found"
else 
	echo "Argument  not Found"
	exit 0
fi
if [ $1 = 1 ] 
then
	echo "Firmware Installing ..."
	Firmware_Update
elif [ $1 = 2 ]
then
	echo "Application Installing ..."
else 
	echo "Input Error"
	exit 1
fi

exit 0
