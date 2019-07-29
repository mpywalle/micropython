/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * USB debug support.
 *
 */
#ifndef __USBDBG_H__
#define __USBDBG_H__
/**
  * Firmware major, minor and patch versions.
  * Increment the major version if the ABI has changed.
  * Increment the minor version when a new command is added.
  * Increment the patch version for fixes that don't affect the ABI.
  *
  * Note: incrementing the major version will require a fw upgrade,
  * the IDE will Not connect if the major version number is different.
  */
#define FIRMWARE_VERSION_MAJOR      0x55
#define FIRMWARE_VERSION_MINOR      0xaa
#define FIRMWARE_VERSION_PATCH      0x5a

/**
  * To add a new debugging command, increment the last command value used.
  * Set the MSB of the value if the request has a device-to-host data phase.
  * Add the command to usr/openmv.py using the same value.
  * Handle the command control and data in/out (if any) phases in usbdbg.c.
  *
  * See usbdbg.c for examples.
  */
enum usbdbg_cmd {
    USBDBG_NONE             =0x00,
	USBDBG_SCRIPT_STOP		=0x06,
    USBDBG_FW_VERSION       =0x80,
	USBDBG_SCRIPT_RUNNING   =0x87,
    USBDBG_SYS_RESET        =0x0C,
    USBDBG_SCRIPT_DOWNLOAD  =0x0e,
	USBDBG_SCRIPT_DOWNLOAD_ACK =0x8e,
};
void usbdbg_init();
bool usbdbg_script_ready();
vstr_t *usbdbg_get_script();
bool usbdbg_get_irq_enabled();
void usbdbg_set_irq_enabled(bool enabled);
void usbdbg_set_script_running(bool running);
void usbdbg_data_in(void *buffer, int *length);
void usbdbg_data_out(void *buffer, int length);
void usbdbg_control(void *buffer, uint8_t brequest, uint32_t wlength);
#endif /* __USBDBG_H__ */
