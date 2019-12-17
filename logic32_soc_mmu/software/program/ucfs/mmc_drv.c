
#include <stdio.h>

#include "fs_port.h"
#include "fs_dev.h"
#include "fs_api.h"
#include "fs_fsl.h"
#include "fs_int.h"
#include "fs_conf.h"

static int mmc_status(FS_u32 id);
static int mmc_read(FS_u32 id, FS_u32 block, void *buffer);
static int mmc_write(FS_u32 id, FS_u32 block, void *buffer);
static int mmc_ioctl(FS_u32 id, FS_i32 cmd, FS_i32 aux, void *buffer);

const FS__device_type FS__mmcdevice_driver = {
	"mmc1",
	mmc_status,
	mmc_read,
	mmc_write,
	mmc_ioctl,
};

static int mmc_status(FS_u32 id)
{
	printf("mmc_status(id:%d)\n", id);
	return 0;
}

#include "disk.h"

static int mmc_read(FS_u32 id, FS_u32 block, void *buffer)
{
	int i = block;
	char *pbuffer = (char *)buffer;
	printf("mmc_read(id:%d, block:%d, buffer:%p)\n", id, block, buffer);
	for(i=0;i<block;i++) {
		if( disk_readp0(pbuffer, block) != DISK_OK ) {
			return -1;
		}
		pbuffer += 512;
	}
	return 0;
}

static int mmc_write(FS_u32 id, FS_u32 block, void *buffer)
{
	printf("mmc_write(id:%d, block:%d, buffer:%p)\n", id, block, buffer);
	return -1;
}

static int mmc_ioctl(FS_u32 id, FS_i32 cmd, FS_i32 aux, void *buffer)
{
	printf("mmc_ioctl(id:%d, cmd:%d, aux:%d, buffer:%p)\n", id, cmd, aux, buffer);
	return -1;
}

