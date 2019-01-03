#include "trace.h"

extern fs_user_mount_t fs_user_mount_flash; 
FIL trace_log_fp;


#ifdef MICROPY_PY_DEBUG
int trace_init() {
	FRESULT ret;
	fs_user_mount_t *vfs_fat = &fs_user_mount_flash;

	ret = f_open(&vfs_fat->fatfs, &trace_log_fp, "/trace.log", FA_WRITE | FA_OPEN_APPEND);
	if(ret == FR_OK)
		return 0;
	else if(ret == FR_NO_FILE){
		ret = f_open(&vfs_fat->fatfs, &trace_log_fp, "/trace.log", FA_WRITE | FA_CREATE_NEW);
		if(ret == FR_OK)
			return 0;
		else 
			return 1;
	}
	else 
		return 1;

}

int trace_write(char* fmt, ...){

	if(fmt == NULL)
		return 1;

	va_list arg_ptr;
	char buf[TRACE_BUFFER_SIZE_C];
	
	memset(buf, 0, sizeof(buf));

	va_start(arg_ptr, fmt);
	vsnprintf(&buf[strlen(buf)], sizeof(buf) - strlen(buf), fmt, arg_ptr);
	va_end(arg_ptr);

	UINT n;
	f_write(&trace_log_fp, buf, strlen(buf), &n);
	if(n != strlen(buf))
		return 1;
	f_sync(&trace_log_fp);

	return 0;
}

int trace_deinit() {
	f_close(&trace_log_fp);

	return 0;
}
#else
int trace_init() {
	return 0;
}

int trace_write(char* fmt, ...) {
	return 0;
}
int trace_deinit() {
	return 0;
}
#endif
