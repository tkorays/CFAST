/**********************************************
 * Copyright (c) 2018, tkorays
 * All rights reserved.
 * 
 * @file    cfast.h
 * @brief   Include all CFAST header files.
 * @version 1.0
 * @author  tkorays<tkorays@hotmail.com>
 * @date    2018-1-19
 *********************************************/
#ifndef __CFAST_H__
#define __CFAST_H__


/* 
 * CFAST is a C library that provides a collection of useful functions and data structures.
 * It includes functionality for types, assertions, command-line interfaces, debugging, dynamic shared objects, 
 * error handling, file I/O, linked lists, logging, memory management, memory pools, messaging, mutexes, 
 * options parsing, plugins, queues, select loops, sockets, strings, threads, time, polling, paths, and system calls.
 */


#include <cf/types.h>
#include <cf/assert.h>
#include <cf/cli.h>
#include <cf/dbg.h>
#include <cf/dso.h>
#include <cf/err.h>
#include <cf/file.h>
#include <cf/list.h>
#include <cf/logging.h>
#include <cf/memory.h>
#include <cf/mpool.h>
#include <cf/msg.h>
#include <cf/mutex.h>
#include <cf/opt.h>
#include <cf/plugin.h>
#include <cf/queue.h>
#include <cf/select.h>
#include <cf/socket.h>
#include <cf/str.h>
#include <cf/thread.h>
#include <cf/time.h>
#ifndef CF_OS_WIN
#include <cf/poll.h>
#endif
#include <cf/path.h>
#include <cf/system.h>


#endif /* __CFAST_H__ */
