/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * USB debug support.
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include "py/runtime.h"
#include "py/compile.h"
#include "py/gc.h"
#include "usbdbg.h"
#include "pendsv.h"
#include "trace.h"

static int xfer_bytes;
static int xfer_length;
static enum usbdbg_cmd cmd;

static volatile bool script_ready;
static volatile bool script_running;
static vstr_t script_buf;
static mp_obj_t mp_const_ide_interrupt = MP_OBJ_NULL;

extern fs_user_mount_t fs_user_mount_flash;
FIL script_fp;

void usbdbg_init()
{
    script_ready=false;
    script_running=false;
    vstr_init(&script_buf, 32);
    mp_const_ide_interrupt = mp_obj_new_exception_msg(&mp_type_Exception, "IDE interrupt");
}

bool usbdbg_script_ready()
{
    return script_ready;
}

vstr_t *usbdbg_get_script()
{
    return &script_buf;
}

void usbdbg_set_script_running(bool running)
{
    script_running = running;
}

inline void usbdbg_set_irq_enabled(bool enabled)
{
    if (enabled) {
        HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
    } else {
        HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
    }
    __DSB(); __ISB();
}

void usbdbg_data_in(void *buffer, int *length)
{
    switch (cmd) {
        case USBDBG_FW_VERSION: {  /* length = 4 */
            uint8_t *ver_buf = buffer;
			*length = 4;
            ver_buf[0] = FIRMWARE_VERSION_MAJOR;
            ver_buf[1] = FIRMWARE_VERSION_MINOR;
            ver_buf[2] = FIRMWARE_VERSION_PATCH;
			ver_buf[3] = 0;
            cmd = USBDBG_NONE;
            break;
        }

		case USBDBG_SCRIPT_RUNNING: {  /* length = 6 */
			uint8_t *ack = buffer;
			*length = 4;
			ack[0] = (uint32_t) script_running;
            cmd = USBDBG_NONE;        
            break;
        }

		case USBDBG_SCRIPT_DOWNLOAD_ACK: {  /* length = 3 */
			uint8_t *ack = buffer;
			*length = 3;
			ack[0] = 0x30;
			ack[1] = USBDBG_SCRIPT_DOWNLOAD_ACK;
			ack[2] = script_ready;
			cmd = USBDBG_NONE;
			break;
		}

        default: /* error */
			*length = 0;
			cmd = USBDBG_NONE;
            break;
    }
}

void usbdbg_data_out(void *buffer, int length)
{
    switch (cmd) {
        case USBDBG_SCRIPT_DOWNLOAD: {
            if (!script_running && !gc_is_locked()) {
                trace_write("add script to buf: len: %d, data: %d, %d, %d, %d.\r\n", length, *(char *)buffer, *((char*)buffer+1), *((char*)buffer+2), *((char*)buffer+3));
                vstr_add_strn(&script_buf, buffer, length);
                xfer_bytes += length;     
                if (xfer_bytes == xfer_length) {
                    FRESULT ret;
					UINT n;
                    fs_user_mount_t *vfs_fat = &fs_user_mount_flash;
					
                    ret = f_open(&vfs_fat->fatfs, &script_fp, "/main_bak.py", FA_WRITE | FA_CREATE_ALWAYS);
                    if(ret == FR_OK){
						ret = f_write(&script_fp, script_buf.buf, script_buf.len, &n);
						if(ret == FR_OK && n == script_buf.len){
							trace_write("script in buffer ready.\r\n");
							f_close(&script_fp);
							f_unlink(&vfs_fat->fatfs, "/main.py");
							f_rename (&vfs_fat->fatfs, "/main_bak.py", "/main.py");
							script_ready = true;
						}
						else{
							trace_write("script file write failed.\r\n");
							f_close(&script_fp);
						}
					}
			        else
			            trace_write("script file open failed.\r\n");
					cmd = USBDBG_SCRIPT_DOWNLOAD_ACK;
			    }
            }
		}
            break;
        default: /* error */
            break;
    }
}

void usbdbg_control(void *buffer, uint8_t request, uint32_t length)
{
    cmd = (enum usbdbg_cmd) request;
	trace_write("cmd from openmv: 0x%x.\r\n", cmd);
    switch (cmd) {
        case USBDBG_FW_VERSION:   
            break;

		case USBDBG_SCRIPT_STOP:
            if (script_running) {
               // Set script running flag
               script_running = false;
#if 0
			   // Disable IDE IRQ (re-enabled by pyexec or main).
			   usbdbg_set_irq_enabled(false);
               // interrupt running code by raising an exception
               mp_obj_exception_clear_traceback(mp_const_ide_interrupt);
               pendsv_nlr_jump_hard(mp_const_ide_interrupt);
#else
			   pendsv_kbd_intr();
#endif
			}
            cmd = USBDBG_NONE;
            break;

        case USBDBG_SCRIPT_RUNNING:
            break;

        case USBDBG_SCRIPT_DOWNLOAD:
			xfer_bytes = 0;
			xfer_length = length;
			script_ready = false;
			vstr_reset(&script_buf);
            break;
        
        case USBDBG_SYS_RESET:
            NVIC_SystemReset();
            break;

        default: /* error */
            cmd = USBDBG_NONE;
            break;
    }
}


