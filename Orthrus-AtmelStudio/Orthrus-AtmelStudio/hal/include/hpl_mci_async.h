/**
 * \file
 *
 * \brief SAM MCI HPL
 *
 * Copyright (C) 2017 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef _HPL_MCI_ASYNC_H_INCLUDED
#define _HPL_MCI_ASYNC_H_INCLUDED

#include "hpl_mci_sync.h"
#include "hpl_irq.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief MCI callback types
 */
enum _mci_async_callback_type {
	MCI_ASYNC_READ_DONE,  /**< Read data done. */
	MCI_ASYNC_WRITE_DONE, /**< Write data done. */
	MCI_ASYNC_ERROR       /**< Error when read or write. */
};

/**
 * \brief MCI device structure
 *
 * The MCI device structure forward declaration.
 */
struct _mci_async_device;

/**
 * \brief MCI interrupt callbacks
 */
struct _mci_async_callbacks {
	void (*read_done_cb)(struct _mci_async_device *device);
	void (*write_done_cb)(struct _mci_async_device *device);
	void (*error_cb)(struct _mci_async_device *device);
};

/**
 * \brief mci async device structure
 */
struct _mci_async_device {
	void *                      hw;
	struct _mci_async_callbacks cb;
	struct _irq_descriptor      irq;
	uint8_t *                   xfer_buf;   /**< Transfer buffer pointer. */
	uint64_t                    xfer_count; /**< Transfer count in number of bytes. */
	uint16_t                    xfer_size;  /**< Transfer size in number of blocks. */
	uint16_t                    block_size; /**< Block size in number of bytes. */
};

/**
 *  \brief Initialize MCI low level driver.
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *
 *  \return Operation status.
 *  \retval ERR_NONE Success.
 *  \retval <0 Error code.
 */
int32_t _mci_async_init(struct _mci_async_device *const dev, void *const hw);

/**
 *  \brief Deinitialize MCI low level driver.
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *  \param[in] hw         The pointer to hardware instance
 *
 *  \return Operation status.
 *  \retval ERR_NONE Success.
 *  \retval <0 Error code.
 */
int32_t _mci_async_deinit(struct _mci_async_device *const dev);

/**
 *  \brief Select a device and initialize it
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *  \param[in] slot    Selected slot
 *  \param[in] clock   Maximum clock to use (Hz)
 *  \param[in] bus_width  Bus width to use (1, 4 or 8)
 *  \param[in] high_speed true, to enable high speed mode
 *
 *  \return Operation status.
 *  \retval ERR_NONE Success.
 *  \retval <0 Error code.
 */
int32_t _mci_async_select_device(struct _mci_async_device *const dev, uint8_t slot, uint32_t clock, uint8_t bus_width,
                                 bool high_speed);

/**
 *  \brief Deselect a device by an assigned slot
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *  \param[in] slot    Selected slot
 *
 *  \return Operation status.
 *  \retval ERR_NONE Success.
 *  \retval <0 Error code.
 */
int32_t _mci_async_deselect_device(struct _mci_async_device *const dev, uint8_t slot);

/**
 *  \brief Get the maximum bus width of a device
 *         by a selected slot
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *  \param[in] slot       Selected slot
 *
 *  \return bus width.
 */
uint8_t _mci_async_get_bus_width(struct _mci_async_device *const dev, uint8_t slot);

/**
 *  \brief Get the high speed capability of the device.
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *
 *  \return true, if the high speed is supported.
 */
bool _mci_async_is_high_speed_capable(struct _mci_async_device *const dev);

/**
 *  \brief Send 74 clock cycles on the line.
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *
 *   Note: It is required after card plug and before card install.
 */
void _mci_async_send_init_sequence(struct _mci_async_device *const dev);

/**
 *  \brief Send a command on the selected slot
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *  \param[in] cmd        Command definition
 *  \param[in] arg        Argument of the command
 *
 *  \return true if success, otherwise false
 */
bool _mci_async_send_cmd(struct _mci_async_device *const dev, uint32_t cmd, uint32_t arg);

/**
 *  \brief Get 32 bits response of the last command.
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *
 *  \return 32 bits response.
 */
uint32_t _mci_async_get_response(struct _mci_async_device *const dev);

/**
 *  \brief Get 128 bits response of the last command.
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *  \param[in] response   Pointer on the array to fill
 *                        with the 128 bits response.
 */
void _mci_async_get_response_128(struct _mci_async_device *const dev, uint8_t *response);

/**
 *  \brief Send an ADTC command on the selected slot
 *         An ADTC (Addressed Data Transfer Commands)
 *         command is used for read/write access..
 *
 *  \param[in, out] dev     Pointer to the MCI device instance
 *  \param[in] cmd          Command definition.
 *  \param[in] arg          Argument of the command.
 *  \param[in] block_size   Block size used for the transfer.
 *  \param[in] nb_block     Total number of block for this transfer
 *  \param[in] access_block if true, the x_read_blocks() and x_write_blocks()
 *                          functions must be used after this function.
 *                          If false, the mci_read_word() and mci_write_word()
 *                          functions must be used after this function.
 *
 * \return true if success, otherwise false
 */
bool _mci_async_adtc_start(struct _mci_async_device *const dev, uint32_t cmd, uint32_t arg, uint16_t block_size,
                           uint16_t nb_block, bool access_block);

/**
 *  \brief Send a command to stop an ADTC command on the selected slot.
 *
 *  \param[in, out] dev   Pointer to the MCI device instance
 *  \param[in] cmd        Command definition
 *  \param[in] arg        Argument of the command
 *
 * \return true if success, otherwise false
 */
bool _mci_async_adtc_stop(struct _mci_async_device *const dev, uint32_t cmd, uint32_t arg);

/**
 *  \brief Read bytes on the line
 *
 *  \param[in, out] dev  Pointer to the MCI device instance
 *  \param[in] value     Pointer on a word to fill
 *
 *  \return true if success, otherwise false
 */
bool _mci_async_read_bytes(struct _mci_async_device *const dev, uint32_t *value);

/**
 *  \brief Write bytes on the line
 *
 *  \param[in, out] dev  Pointer to the MCI device instance
 *  \param[in] value     Word to send
 *
 *  \return true if success, otherwise false
 */
bool _mci_async_write_bytes(struct _mci_async_device *const dev, uint32_t value);

/**
 *  \brief Register the MCI device callback
 *
 *  \param[in, out] dev Pointer to the MCI device instance
 *  \param[in] cb_type  The callback type
 *  \param[in] func     The callback function to register. NULL to disable callback
 *
 *  \return Always ERR_NONE
 */
int32_t _mci_async_register_callback(struct _mci_async_device *dev, const enum _mci_async_callback_type cb_type,
                                     const FUNC_PTR func);

/**
 * \brief Enable/disable mci interrupt
 *
 *  \param[in, out] dev  Pointer to the MCI device instance
 *  \param[in] type      The type of interrupt to disable/enable if applicable
 *  \param[in] state     Enable or disable
 */
void _mci_async_set_irq_state(struct _mci_async_device *const dev, const enum _mci_async_callback_type type,
                              const bool state);

#ifdef __cplusplus
}
#endif

#endif /* _HPL_MCI_ASYNC_H_INCLUDED */
