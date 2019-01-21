/*
 * Copyright (C) 2016 Stefan Roese <sr@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _CONFIG_MVEBU_ARMADA_H
#define _CONFIG_MVEBU_ARMADA_H

#include <asm/arch/soc.h>

/*
 * High Level Configuration Options (easy to change)
 */

#define	CONFIG_SYS_TEXT_BASE	0x00000000

/* additions for new ARM relocation support */
#define CONFIG_SYS_SDRAM_BASE	0x00000000

/* auto boot */
#define CONFIG_PREBOOT

/* Memory size, should define by defconfig file */
#ifndef CONFIG_MEMORY_SIZE
#define CONFIG_MEMORY_SIZE "512m"
#endif


#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, \
					  115200, 230400, 460800, 921600 }

/* Boot from SPI NOR flash, no eMMC on board, find rootfs from SD */
#if defined(CONFIG_MVEBU_SPI_BOOT)
#define CH_MMC_DEV_NUM "0"
#define CH_FDT_SUFFIX "-non-emmc"
/* Boot from eMMC, find rootfs from eMMC */
#elif defined(CONFIG_MVEBU_MMC_BOOT)
#define CH_MMC_DEV_NUM "1"
#define CH_FDT_SUFFIX "-emmc"
/* Default have eMMC */
#else
#define CH_MMC_DEV_NUM "1"
#define CH_FDT_SUFFIX "-emmc"
#endif

/* Default Env vars */
#define CONFIG_IPADDR			0.0.0.0	/* In order to cause an error */
#define CONFIG_SERVERIP			0.0.0.0	/* In order to cause an error */
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_GATEWAYIP		10.4.50.254
#define CONFIG_HAS_ETH1
#define CONFIG_HAS_ETH2
#define CONFIG_ETHPRIME			"eth0"
#define CONFIG_ROOTPATH                 "/srv/nfs/" /* Default Dir for NFS */
#define CONFIG_EXTRA_ENV_SETTINGS	"kernel_addr=0x5000000\0"	\
					"initrd_addr=0xa00000\0"	\
					"initrd_size=0x2000000\0"	\
					"fdt_addr=0x4f00000\0"		\
					"loadaddr=0x5000000\0"		\
					"fdt_high=0xffffffffffffffff\0"	\
					"hostname=marvell\0"		\
					"ramfs_addr=0x8000000\0"	\
					"ramfs_name=-\0"		\
					"fdt_name=fdt.dtb\0"		\
					"netdev=eth0\0"			\
					"ethaddr=00:51:82:11:22:00\0"	\
					"eth1addr=00:51:82:11:22:01\0"	\
					"eth2addr=00:51:82:11:22:02\0"	\
					"eth3addr=00:51:82:11:22:03\0"	\
					"image_name=Image\0"		\
					"get_ramfs=if test \"${ramfs_name}\"" \
						" != \"-\"; then setenv " \
						"ramfs_addr 0x8000000; " \
						"tftpboot $ramfs_addr " \
						"$ramfs_name; else setenv " \
						"ramfs_addr -;fi\0"	\
					"get_images=tftpboot $kernel_addr " \
						"$image_name; tftpboot " \
						"$fdt_addr $fdt_name; " \
						"run get_ramfs\0"	\
					"console=" CONFIG_DEFAULT_CONSOLE "\0"\
					"root=root=/dev/nfs rw\0"	\
					"set_bootargs=setenv bootargs $console"\
						" $root ip=$ipaddr:$serverip:" \
						"$gatewayip:$netmask:$hostname"\
						":$netdev:none nfsroot="\
						"$serverip:$rootpath " \
						"$extra_params\0"	\
"ch_memory_size=" CONFIG_MEMORY_SIZE "\0"				\
"ch_image_name=boot/Image\0"						\
"ch_fdt_name=boot/armada-3720-espressobin-" CONFIG_MEMORY_SIZE		\
	CH_FDT_SUFFIX ".dtb\0"					\
"ch_serial_number=ffffffff\0"						\
"ch_reset_button_pressed=0\0"						\
"ch_reboot_button_pressed=0\0"						\
"ch_need_reset=0\0"							\
"ch_check_serial_number=mmc dev " CH_MMC_DEV_NUM "; mmc info;\0"	\
"ch_check_button=gpio input GPIO25; gpio input GPIO15; "		\
	"if test $ch_reset_button_pressed=1; "				\
	"then run ch_bootcmd_usb_fat; fi; "				\
	"if test $ch_reboot_button_pressed=1; "				\
	"then run ch_bootcmd_rescue; fi; \0"				\
"ch_bootcmd_normal=mmc dev " CH_MMC_DEV_NUM "; ext4load mmc "		\
	CH_MMC_DEV_NUM ":2 $kernel_addr $ch_image_name; ext4load mmc "	\
	CH_MMC_DEV_NUM ":2 $fdt_addr $ch_fdt_name; setenv bootargs "	\
	"$console ch_need_reset=$ch_need_reset root=/dev/mmcblk0p2 "	\
	"rw rootwait; booti $kernel_addr - $fdt_addr\0"			\
"ch_bootcmd_rescue=mmc dev " CH_MMC_DEV_NUM "; ext4load mmc "		\
	CH_MMC_DEV_NUM ":1 $kernel_addr $ch_image_name; ext4load mmc "	\
	CH_MMC_DEV_NUM ":1 $fdt_addr $ch_fdt_name; setenv bootargs "	\
	"$console root=/dev/mmcblk0p1 rw rootwait; booti $kernel_addr "	\
	" - $fdt_addr\0"						\
"ch_bootcmd_usb_fat=usb start;fatload usb 0:1 $kernel_addr "		\
	"$ch_image_name;fatload usb 0:1 $fdt_addr $ch_fdt_name; "	\
	"setenv bootargs $console root=/dev/sda1 rw rootwait; booti "	\
	"$kernel_addr - $fdt_add\0"					\
"ch_bootcmd_net_tftp=gpio input GPIO25\0"				\
"ch_boot_flows=normal rescue usb_fat net_tftp\0"			\
"ch_distro_bootcmd=for flow in ${ch_boot_flows}; do run "		\
	"ch_bootcmd_${flow}; done; reset"
#if 0
#define CONFIG_BOOTCOMMAND	"run get_images; run set_bootargs; " \
				"booti $kernel_addr $ramfs_addr $fdt_addr"
#endif

#define CONFIG_BOOTCOMMAND	"run ch_check_serial_number; "		\
				"run ch_check_button; "			\
				"run ch_distro_bootcmd"

#define CONFIG_ENV_OVERWRITE	/* ethaddr can be reprogrammed */
/*
 * For booting Linux, the board info and command line data
 * have to be in the first 8 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define CONFIG_CMDLINE_TAG		/* enable passing of ATAGs  */
#define CONFIG_INITRD_TAG		/* enable INITRD tag */
#define CONFIG_SETUP_MEMORY_TAGS	/* enable memory tag */

#define	CONFIG_SYS_CBSIZE	1024	/* Console I/O Buff Size */
#define	CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE \
		+sizeof(CONFIG_SYS_PROMPT) + 16)	/* Print Buff */

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN	(8 << 20) /* 8MiB for malloc() */

/*
 * Other required minimal configurations
 */
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_ARCH_CPU_INIT		/* call arch_cpu_init() */
#define CONFIG_SYS_LOAD_ADDR	0x00800000	/* default load adr- 8M */
#define CONFIG_SYS_MEMTEST_START 0x00800000	/* 8M */
#define CONFIG_SYS_MEMTEST_END	0x00ffffff	/*(_16M -1) */
#define CONFIG_SYS_RESET_ADDRESS 0xffff0000	/* Rst Vector Adr */
#define CONFIG_SYS_MAXARGS	32	/* max number of command args */

#define CONFIG_SYS_ALT_MEMTEST

/* End of 16M scrubbed by training in bootrom */
#define CONFIG_SYS_INIT_SP_ADDR         (CONFIG_SYS_TEXT_BASE + 0xFF0000)

/*
 * SPI Flash configuration
 */
#define CONFIG_ENV_SPI_BUS		0
#define CONFIG_ENV_SPI_CS		0

/* SPI NOR flash default params, used by sf commands */
#define CONFIG_SF_DEFAULT_SPEED		1000000
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#define CONFIG_ENV_SPI_MODE		CONFIG_SF_DEFAULT_MODE

/* eMMC Flash configuration */
#define CONFIG_SUPPORT_EMMC_BOOT	/* Enable boot from eMMC */
#define CONFIG_SUPPORT_EMMC_RPMB	/* Enable RPMB to save SN number */

/* Environment in SPI NOR flash */
#ifdef CONFIG_MVEBU_SPI_BOOT
#define CONFIG_ENV_IS_IN_SPI_FLASH
/* Environment in NAND flash */
#elif defined(CONFIG_MVEBU_NAND_BOOT)
#define CONFIG_ENV_IS_IN_NAND
/* Environment in MMC */
#elif defined(CONFIG_MVEBU_MMC_BOOT)
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_PART		1 /* 0 - DATA, 1 - BOOT0, 2 - BOOT1 */
/* Environment in SPI NAND flash */
#elif defined(CONFIG_MVEBU_SPINAND_BOOT)
#define CONFIG_ENV_IS_IN_SPI_NAND
#endif

/* Assume minimum flash/eMMC boot partition size of 4MB
 * and save the environment at the end of the boot device
*/
#define CONFIG_ENV_SIZE			(64 << 10) /* 64KiB */
#define CONFIG_ENV_SECT_SIZE		(64 << 10) /* 64KiB sectors */
#if defined(CONFIG_ENV_IS_IN_NAND) || defined(CONFIG_ENV_IS_IN_SPI_NAND)
#define CONFIG_ENV_OFFSET		0x400000
#elif defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET		(0x200000 - CONFIG_ENV_SIZE)
#else
#define CONFIG_ENV_OFFSET		(0x400000 - CONFIG_ENV_SIZE)
#endif

#define CONFIG_USB_MAX_CONTROLLER_COUNT (CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS + \
					 CONFIG_SYS_USB_XHCI_MAX_ROOT_PORTS)

/* USB ethernet */
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_ETHER_MCS7830
#define CONFIG_USB_ETHER_RTL8152
#define CONFIG_USB_ETHER_SMSC95XX

/*
 * SATA/SCSI/AHCI configuration
 */
#define CONFIG_SCSI
#define CONFIG_SCSI_AHCI
#define CONFIG_SCSI_AHCI_PLAT
#define CONFIG_LIBATA
#define CONFIG_LBA48
#define CONFIG_SYS_64BIT_LBA

#define CONFIG_SYS_SCSI_MAX_SCSI_ID	2
#define CONFIG_SYS_SCSI_MAX_LUN		1
#define CONFIG_SYS_SCSI_MAX_DEVICE	(CONFIG_SYS_SCSI_MAX_SCSI_ID * \
					 CONFIG_SYS_SCSI_MAX_LUN)

/* MMC/SD IP block */
#define CONFIG_SUPPORT_VFAT

/*
 * The EEPROM ST M24C64 has 32 byte page write mode and takes up to 10 msec.
 */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS 10

#endif /* _CONFIG_MVEBU_ARMADA_H */
