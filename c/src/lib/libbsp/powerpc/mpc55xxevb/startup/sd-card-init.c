#include  <stdio.h>

#include <mpc55xx/mpc55xx.h>
#include <mpc55xx/regs.h>
#include <mpc55xx/dspi.h>

#include <libchip/spi-sd-card.h>

#define DEBUG

#include <rtems/status-checks.h>

#include <bsp.h>

static rtems_status_code mpc55xx_dspi_init()
{
	int rv = 0;
	int i = 0;
	char device_name [] = "/dev/spi0";
	union SIU_PCR_tag pcr = MPC55XX_ZERO_FLAGS;

	rv = rtems_libi2c_initialize();
	CHECK_RVSC( rv, "rtems_libi2c_initialize");

	/* DSPI D inputs are taken from DSPI C */
	SIU.DISR.R = 0x000000FC;

	/* DSPI A signals */
	pcr.B.PA = 1;
	pcr.B.ODE = 0;
	pcr.B.HYS = 0;
	pcr.B.SRC = 3;
	pcr.B.WPE = 1;
	pcr.B.WPS = 1;

	/* SCK */
	pcr.B.OBE = 1;
	pcr.B.IBE = 0;
	SIU.PCR [93].R = pcr.R;

	/* SIN */
	pcr.B.OBE = 0;
	pcr.B.IBE = 1;
	SIU.PCR [94].R = pcr.R;

	/* SOUT */
	pcr.B.OBE = 1;
	pcr.B.IBE = 0;
	SIU.PCR [95].R = pcr.R;

	/* PCSx */
	pcr.B.OBE = 1;
	pcr.B.IBE = 0;
	SIU.PCR [96].R = pcr.R;
	SIU.PCR [97].R = pcr.R;
	SIU.PCR [98].R = pcr.R;
	SIU.PCR [99].R = pcr.R;
	SIU.PCR [100].R = pcr.R;
	SIU.PCR [101].R = pcr.R;

	mpc55xx_dspi_bus_table [3].master = 0;
	for (i = 0; i < MPC55XX_DSPI_NUMBER; ++i) {
		device_name [8] = '0' + i;
		rv = rtems_libi2c_register_bus( device_name, (rtems_libi2c_bus_t *) &mpc55xx_dspi_bus_table [i]);
		CHECK_RVSC( rv, device_name);
	}

	return RTEMS_SUCCESSFUL;
}

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <rtems/fsmount.h>
#include <rtems/dosfs.h>
#include <rtems/ide_part_table.h>
#include <rtems/console.h>

#define MPC55XX_DEVICE "sd-card-a"
#define MPC55XX_DEVICE_FILE "/dev/" MPC55XX_DEVICE
#define MPC55XX_PARTITION "/dev/sd-card-a1"
#define MPC55XX_MOUNT_POINT "/mnt"

static fstab_t mpc55xx_fs_table [] = { {
		MPC55XX_PARTITION, MPC55XX_MOUNT_POINT,
		&msdos_ops, RTEMS_FILESYSTEM_READ_WRITE,
		FSMOUNT_MNT_OK | FSMOUNT_MNTPNT_CRTERR | FSMOUNT_MNT_FAILED,
		FSMOUNT_MNT_OK
	}, {
		MPC55XX_DEVICE_FILE, MPC55XX_MOUNT_POINT,
		&msdos_ops, RTEMS_FILESYSTEM_READ_WRITE,
		FSMOUNT_MNT_OK | FSMOUNT_MNTPNT_CRTERR | FSMOUNT_MNT_FAILED,
		0
	}
};

#define SD_CARD_NUMBER 1

sd_card_driver_entry sd_card_driver_table [SD_CARD_NUMBER] = { {
		.driver = {
			.ops = &sd_card_driver_ops,
			.size = sizeof( sd_card_driver_entry)
		},
		.table_index = 0,
		.minor = 0,
		.device_name = "sd-card-a",
		.disk_device_name = "/dev/sd-card-a",
		.transfer_mode = SD_CARD_TRANSFER_MODE_DEFAULT,
		.command = SD_CARD_COMMAND_DEFAULT,
		/* response : whatever, */
		.response_index = SD_CARD_COMMAND_SIZE,
		.n_ac_max = SD_CARD_N_AC_MAX_DEFAULT,
		.block_number = 0,
		.block_size = 0,
		.block_size_shift = 0,
		.busy = 1,
		.verbose = 1,
		.schedule_if_busy = 0,
	}
};

rtems_status_code mpc55xx_sd_card_init()
{
	rtems_status_code sc = RTEMS_SUCCESSFUL;
	int rv = 0;
	sd_card_driver_entry *e = &sd_card_driver_table [0];

	DEBUG_PRINT( "Task started\n");

	sc = mpc55xx_dspi_init();
	CHECK_SC( rv, "Intitalize DSPI bus");

	rv = rtems_libi2c_register_drv( e->device_name, (rtems_libi2c_drv_t *) e, mpc55xx_dspi_bus_table [0].bus_number, 0);
	CHECK_RVSC( rv, "Register SD Card driver");

	sc = rtems_ide_part_table_initialize( MPC55XX_DEVICE_FILE);
	CHECK_SC( sc, "Initialize IDE partition table");

	rv = mkdir( MPC55XX_MOUNT_POINT, S_IRWXU);
	CHECK_RVSC( rv, "Create mount point");

	rv = rtems_fsmount( mpc55xx_fs_table, sizeof( mpc55xx_fs_table) / sizeof( mpc55xx_fs_table [0]), NULL);
	CHECK_RVSC( rv, "Mount file systems");

	return RTEMS_SUCCESSFUL;
}
