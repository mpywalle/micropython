/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * Trace buffer.
 *
 */

#ifndef __TRACE_H__
#define __TRACE_H__
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "stdarg.h"     /* va_start() */
#include "lib/oofatfs/ff.h"
#include "extmod/vfs.h"
#include "extmod/vfs_fat.h"

#define TRACE_BUFFER_SIZE_C 1024

int trace_init();

int trace_write(char* log, ...);

int trace_deinit();
#endif /* __TRACE_H__ */
