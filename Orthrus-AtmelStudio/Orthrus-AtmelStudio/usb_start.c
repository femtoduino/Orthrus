/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include "usb_start.h"

/* use a mock buffer for clarify transfer process */
#define MOCK_BUF_AMOUNT 6
#define MOCK_BUF_SIZE 1
#define MOCK_DATA_TYPE uint8_t *

volatile bool         no_disk_load, disk_busy, usb_busy;
static uint8_t        buf_rdy, xfer_dir, disk_id, usb_id;
static uint32_t       disk_addr, usb_addr, disk_blks, usb_blks;
static MOCK_DATA_TYPE mock_buf[MOCK_BUF_AMOUNT][MOCK_BUF_SIZE];

/* use internal ram as a virtual disk for example */
static uint8_t virtual_disk[CONF_USB_MSC_LUN0_CAPACITY * 1024];

static uint8_t single_desc_bytes[] = {
    /* Device descriptors and Configuration descriptors list. */
    MSC_DESCES_LS_FS,
};

static struct usbd_descriptors single_desc = {single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)};

/* Ctrl endpoint buffer */
static uint8_t ctrl_buffer[64];

#define DISK_INFORMATION(n)                                                                                            \
	{                                                                                                                  \
		CONF_USB_MSC_LUN##n##_TYPE, (CONF_USB_MSC_LUN##n##_RMB << 7),                                                  \
		    ((CONF_USB_MSC_LUN##n##_ISO << 6) + (CONF_USB_MSC_LUN##n##_ECMA << 3) + CONF_USB_MSC_LUN##n##_ANSI),       \
		    CONF_USB_MSC_LUN##n##_REPO, 31, 0x00, 0x00, 0x00, CONF_USB_MSC_LUN##n##_FACTORY,                           \
		    CONF_USB_MSC_LUN##n##_PRODUCT, CONF_USB_MSC_LUN##n##_PRODUCT_VERSION                                       \
	}

#define DISK_CAPACITY(n)                                                                                               \
	{                                                                                                                  \
		(uint8_t)(CONF_USB_MSC_LUN##n##_LAST_BLOCK_ADDR >> 24),                                                        \
		    (uint8_t)(CONF_USB_MSC_LUN##n##_LAST_BLOCK_ADDR >> 16),                                                    \
		    (uint8_t)(CONF_USB_MSC_LUN##n##_LAST_BLOCK_ADDR >> 8),                                                     \
		    (uint8_t)(CONF_USB_MSC_LUN##n##_LAST_BLOCK_ADDR >> 0),                                                     \
		    (uint8_t)((uint32_t)(CONF_USB_MSC_LUN##n##_BLOCK_SIZE) >> 24),                                             \
		    (uint8_t)((uint32_t)(CONF_USB_MSC_LUN##n##_BLOCK_SIZE) >> 16),                                             \
		    (uint8_t)((uint32_t)(CONF_USB_MSC_LUN##n##_BLOCK_SIZE) >> 8),                                              \
		    (uint8_t)((uint32_t)(CONF_USB_MSC_LUN##n##_BLOCK_SIZE) >> 0)                                               \
	}

/* Inquiry Information */
static uint8_t inquiry_info[CONF_USB_MSC_MAX_LUN + 1][36] = {DISK_INFORMATION(0),
#if CONF_USB_MSC_LUN1_ENABLE == 1
                                                             DISK_INFORMATION(1),
#endif
#if CONF_USB_MSC_LUN2_ENABLE == 1
                                                             DISK_INFORMATION(2),
#endif
#if CONF_USB_MSC_LUN3_ENABLE == 1
                                                             DISK_INFORMATION(3),
#endif
#if CONF_USB_MSC_LUN4_ENABLE == 1
                                                             DISK_INFORMATION(4),
#endif
#if CONF_USB_MSC_LUN5_ENABLE == 1
                                                             DISK_INFORMATION(5),
#endif
#if CONF_USB_MSC_LUN6_ENABLE == 1
                                                             DISK_INFORMATION(6),
#endif
#if CONF_USB_MSC_LUN7_ENABLE == 1
                                                             DISK_INFORMATION(7),
#endif
#if CONF_USB_MSC_LUN8_ENABLE == 1
                                                             DISK_INFORMATION(8),
#endif
#if CONF_USB_MSC_LUN9_ENABLE == 1
                                                             DISK_INFORMATION(9),
#endif
#if CONF_USB_MSC_LUN10_ENABLE == 1
                                                             DISK_INFORMATION(10),
#endif
#if CONF_USB_MSC_LUN11_ENABLE == 1
                                                             DISK_INFORMATION(11),
#endif
#if CONF_USB_MSC_LUN12_ENABLE == 1
                                                             DISK_INFORMATION(12),
#endif
#if CONF_USB_MSC_LUN13_ENABLE == 1
                                                             DISK_INFORMATION(13),
#endif
#if CONF_USB_MSC_LUN14_ENABLE == 1
                                                             DISK_INFORMATION(14),
#endif
#if CONF_USB_MSC_LUN15_ENABLE == 1
                                                             DISK_INFORMATION(15)
#endif
};

/* Capacities of Disk */
static uint8_t format_capa[CONF_USB_MSC_MAX_LUN + 1][8] = {DISK_CAPACITY(0),
#if CONF_USB_MSC_LUN1_ENABLE == 1
                                                           DISK_CAPACITY(1),
#endif
#if CONF_USB_MSC_LUN2_ENABLE == 1
                                                           DISK_CAPACITY(2),
#endif
#if CONF_USB_MSC_LUN3_ENABLE == 1
                                                           DISK_CAPACITY(3),
#endif
#if CONF_USB_MSC_LUN4_ENABLE == 1
                                                           DISK_CAPACITY(4),
#endif
#if CONF_USB_MSC_LUN5_ENABLE == 1
                                                           DISK_CAPACITY(5),
#endif
#if CONF_USB_MSC_LUN6_ENABLE == 1
                                                           DISK_CAPACITY(6),
#endif
#if CONF_USB_MSC_LUN7_ENABLE == 1
                                                           DISK_CAPACITY(7),
#endif
#if CONF_USB_MSC_LUN8_ENABLE == 1
                                                           DISK_CAPACITY(8),
#endif
#if CONF_USB_MSC_LUN9_ENABLE == 1
                                                           DISK_CAPACITY(9),
#endif
#if CONF_USB_MSC_LUN10_ENABLE == 1
                                                           DISK_CAPACITY(10),
#endif
#if CONF_USB_MSC_LUN11_ENABLE == 1
                                                           DISK_CAPACITY(11),
#endif
#if CONF_USB_MSC_LUN12_ENABLE == 1
                                                           DISK_CAPACITY(12),
#endif
#if CONF_USB_MSC_LUN13_ENABLE == 1
                                                           DISK_CAPACITY(13),
#endif
#if CONF_USB_MSC_LUN14_ENABLE == 1
                                                           DISK_CAPACITY(14),
#endif
#if CONF_USB_MSC_LUN15_ENABLE == 1
                                                           DISK_CAPACITY(15)
#endif
};

/**
 * \brief Eject Disk
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static int32_t disk_eject(uint8_t lun)
{
	if (lun > CONF_USB_MSC_MAX_LUN) {
		return ERR_NOT_FOUND;
	}
	no_disk_load = true;
	return ERR_NONE;
}

/**
 * \brief Inquiry whether Disk is ready
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static int32_t disk_is_ready(uint8_t lun)
{
	if (lun > CONF_USB_MSC_MAX_LUN) {
		return ERR_NOT_FOUND;
	}

	return (no_disk_load == true ? ERR_NOT_READY : ERR_NONE);
}

/**
 * \brief Callback invoked when a new read blocks command received
 * \param[in] lun logic unit number
 * \param[in] addr start address of disk to be read
 * \param[in] nblocks block amount to be read
 * \return Operation status.
 */
static int32_t msc_new_read(uint8_t lun, uint32_t addr, uint32_t nblocks)
{
	if (lun > CONF_USB_MSC_MAX_LUN) {
		return ERR_DENIED;
	}

	xfer_dir  = 1;
	disk_addr = addr;
	usb_addr  = addr;
	disk_blks = nblocks;
	usb_blks  = nblocks;
	disk_id   = 0;
	usb_id    = 0;

	return ERR_NONE;
}

/**
 * \brief Callback invoked when a new write blocks command received
 * \param[in] lun logic unit number
 * \param[in] addr start address of disk to be written
 * \param[in] nblocks block amount to be written
 * \return Operation status.
 */
static int32_t msc_new_write(uint8_t lun, uint32_t addr, uint32_t nblocks)
{
	if (lun > CONF_USB_MSC_MAX_LUN) {
		return ERR_DENIED;
	}

	xfer_dir  = 0;
	disk_addr = addr;
	usb_addr  = addr;
	disk_blks = nblocks;
	usb_blks  = nblocks;
	disk_id   = 0;
	usb_id    = 0;

	return ERR_NONE;
}

/**
 * \brief Callback invoked when a blocks transfer is done
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static int32_t msc_xfer_done(uint8_t lun)
{
	if (lun > CONF_USB_MSC_MAX_LUN) {
		return ERR_DENIED;
	}

	usb_busy = false;

	if (0 != --usb_blks) {
		usb_addr++;
		if (MOCK_BUF_AMOUNT == ++usb_id) {
			usb_id = 0;
		}
	}

	if (0 == xfer_dir) {
		buf_rdy++;
	} else {
		buf_rdy--;
	}

	return ERR_NONE;
}

/**
 * \brief Get the data from disk to ram
 * \param[in] start start address of disk to be read
 * \param[in] dst_addr address of ram to be written
 * \param[in] block_size the size of block in bytes
 * \param[in] nblocks block amount to be read
 * \return Operation status.
 */
static int32_t disk_2_ram(uint32_t start, void *dst_addr, uint32_t block_size, uint32_t nblocks)
{
	/*  There is no need to do memory copy for virtual disk,
	 *  since the data is already located in RAM.
	 *  But if disk is a stuff likes memory card, here user may call
	 *  related functions as:
	 *  - sd_mmc_init_read_blocks(0, start, nblocks);
	 *  - sd_mmc_start_read_blocks(dst_addr, nblocks);
	 */
	(void)nblocks;
	MOCK_DATA_TYPE *pmock = (MOCK_DATA_TYPE *)dst_addr;

	/** Make mock buffer point to the location in virtual disk. */
	*pmock = &virtual_disk[start];

	/** No data need transfer, fake disk read done here. */
	disk_busy = false;
	buf_rdy++;

	if (0 != --disk_blks) {
		disk_addr++;
		if (MOCK_BUF_AMOUNT == ++disk_id) {
			disk_id = 0;
		}
	}

	return ERR_NONE;
}

/**
 * \brief Put the data from ram to disk
 * \param[in] start start address of disk to be written
 * \param[in] src_addr address of ram to be read
 * \param[in] block_size the size of block in bytes
 * \param[in] nblocks block amount to be written
 * \return Operation status.
 */
static int32_t ram_2_disk(uint32_t start, void *src_addr, uint32_t block_size, uint32_t nblocks)
{
	/*  There is no need to do memory copy for virtual disk,
	 *  since the data is already located in RAM.
	 *  But if disk is a stuff likes memory card, here user may call
	 *  related functions as:
	 *  - sd_mmc_init_write_blocks(0, start, nblocks);
	 *  - sd_mmc_start_write_blocks(src_addr, nblocks);
	 */
	(void)start;
	(void)src_addr;
	(void)block_size;
	(void)nblocks;

	/** No data need transfer, fake disk write done here. */
	disk_busy = false;
	buf_rdy--;

	if (0 == --disk_blks) {
		/* All data have been written into disk. */
		return mscdf_xfer_blocks(false, NULL, 0);
	} else {
		disk_addr++;
		if (MOCK_BUF_AMOUNT == ++disk_id) {
			disk_id = 0;
		}
	}

	return ERR_NONE;
}

/**
 * \brief Disk loop
 */
static void disk_task(void)
{
	if (false == disk_busy && 0 != disk_blks) {
		if (1 == xfer_dir) {
			/* Read Disk Process */
			if (buf_rdy < MOCK_BUF_AMOUNT) {
				disk_busy = true;
				disk_2_ram(
				    disk_addr * CONF_USB_MSC_LUN0_BLOCK_SIZE, &mock_buf[disk_id][0], CONF_USB_MSC_LUN0_BLOCK_SIZE, 1);
			}
		} else {
			/* Write Disk Process */
			if (buf_rdy > 0) {
				disk_busy = true;
				ram_2_disk(
				    disk_addr * CONF_USB_MSC_LUN0_BLOCK_SIZE, &mock_buf[disk_id][0], CONF_USB_MSC_LUN0_BLOCK_SIZE, 1);
			}
		}
	}
}

/**
 * \brief USB loop
 */
static void usb_task(void)
{
	if (false == usb_busy && 0 != usb_blks) {
		if (0 == xfer_dir) {
			/* Write Disk Request */
			if (buf_rdy < MOCK_BUF_AMOUNT) {
				usb_busy            = true;
				mock_buf[usb_id][0] = &virtual_disk[usb_addr * CONF_USB_MSC_LUN0_BLOCK_SIZE];
				mscdf_xfer_blocks(false, mock_buf[usb_id][0], 1);
			}
		} else {
			/* Read Disk Request */
			if (buf_rdy > 0) {
				usb_busy = true;
				mscdf_xfer_blocks(true, mock_buf[usb_id][0], 1);
			}
		}
	}
}

/**
 * \brief Callback invoked when inquiry data command received
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static uint8_t *msc_inquiry_info(uint8_t lun)
{
	if (lun > CONF_USB_MSC_MAX_LUN) {
		return NULL;
	} else {
		no_disk_load = false;
		disk_busy    = false;
		usb_busy     = false;
		buf_rdy      = 0x00;
		return &inquiry_info[lun][0];
	}
}

/**
 * \brief Callback invoked when read format capacities command received
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static uint8_t *msc_get_capacity(uint8_t lun)
{
	if (lun > CONF_USB_MSC_MAX_LUN) {
		return NULL;
	} else {
		return &format_capa[lun][0];
	}
}

/**
 * \brief USB MSC Init
 */
void usbd_msc_init(void)
{
	/* usb stack init */
	usbdc_init(ctrl_buffer);

	/* usbdc_register_funcion inside */
	mscdf_init(CONF_USB_MSC_MAX_LUN);
}

/**
 * Example of using MSC Function.
 * \note
 * In this example, we will use a PC as a USB host:
 * - Connect the DEBUG USB on XPLAINED board to PC for program download.
 * - Connect the TARGET USB on XPLAINED board to PC for running program.
 * The application will behave as a massive storage device which can be
 * recognized by PC. Only one logic unit is supported in this example.
 */
void usbd_msc_example(void)
{
	usbd_msc_init();
	mscdf_register_callback(MSCDF_CB_INQUIRY_DISK, (FUNC_PTR)msc_inquiry_info);
	mscdf_register_callback(MSCDF_CB_GET_DISK_CAPACITY, (FUNC_PTR)msc_get_capacity);
	mscdf_register_callback(MSCDF_CB_START_READ_DISK, (FUNC_PTR)msc_new_read);
	mscdf_register_callback(MSCDF_CB_START_WRITE_DISK, (FUNC_PTR)msc_new_write);
	mscdf_register_callback(MSCDF_CB_EJECT_DISK, (FUNC_PTR)disk_eject);
	mscdf_register_callback(MSCDF_CB_TEST_DISK_READY, (FUNC_PTR)disk_is_ready);
	mscdf_register_callback(MSCDF_CB_XFER_BLOCKS_DONE, (FUNC_PTR)msc_xfer_done);
	usbdc_start(&single_desc);
	usbdc_attach();

	while (!mscdf_is_enabled()) {
		/* wait massive storage to be installed */
	};

	while (1) {
		disk_task();
		usb_task();
	}
}

void usb_init(void)
{
}
