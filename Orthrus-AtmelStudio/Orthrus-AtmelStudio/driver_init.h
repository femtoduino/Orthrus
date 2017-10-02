/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_H_INCLUDED
#define DRIVER_INIT_H_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_aes_sync.h>

#include <hal_mci_sync.h>

#include <hal_timer.h>

#include <hal_rand_sync.h>

#include "hal_usb_device.h"

extern struct aes_sync_descriptor CRYPTOGRAPHY_0;

extern struct mci_sync_desc    MCI_0;
extern struct timer_descriptor TIMER_0;

extern struct rand_sync_desc RAND_0;

void MCI_0_PORT_init(void);
void MCI_0_CLOCK_init(void);
void MCI_0_init(void);

void RAND_0_CLOCK_init(void);
void RAND_0_init(void);

void USB_DEVICE_INSTANCE_CLOCK_init(void);
void USB_DEVICE_INSTANCE_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_H_INCLUDED
