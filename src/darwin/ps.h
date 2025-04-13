/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)ps.h	8.1 (Berkeley) 5/31/93
 *	$FreeBSD: ps.h,v 1.6 1998/09/14 08:32:20 dfr Exp $
 */

#include <mach/mach.h>

#include "config.h"

struct usave {
  struct timeval u_start;
  struct rusage u_ru;
  struct rusage u_cru;
  char u_acflag;
  char u_valid;
};

#define KI_PROC(ki) (&(ki)->ki_p->kp_proc)
#define KI_EPROC(ki) (&(ki)->ki_p->kp_eproc)

typedef struct thread_values {
  struct thread_basic_info tb;
  /* struct policy_infos schedinfo; */
  union {
    struct policy_timeshare_info tshare;
    struct policy_rr_info rr;
    struct policy_fifo_info fifo;
  } schedinfo;
} thread_values_t;

typedef struct kinfo {
  struct kinfo_proc *ki_p; /* proc structure */
  struct usave ki_u;       /* interesting parts of user */
  char *ki_args;           /* exec args */
  char *ki_env;            /* environment */
  task_port_t task;
  int state;
  int cpu_usage;
  int curpri;
  int basepri;
  int swapped;
  struct task_basic_info tasks_info;
  struct task_thread_times_info times;
  /* struct policy_infos schedinfo; */
  union {
    struct policy_timeshare_info tshare;
    struct policy_rr_info rr;
    struct policy_fifo_info fifo;
  } schedinfo;
  int invalid_tinfo;
  int thread_count;
  thread_port_array_t thread_list;
  thread_values_t *thval;
  int invalid_thinfo;
} KINFO;

#ifdef __cplusplus
extern "C" {
#endif
process_list *getProcessList(void);
#ifdef __cplusplus
}
#endif
