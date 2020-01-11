/*
 *   dt_ms.c - compute time it takes to propagate a message
 *   Copyright (c) by Tom Schouten <tom@ziwzwa.be>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */


#include "m_pd.h"
#include <stdlib.h>
#include <sys/time.h>

typedef struct dt_ms_struct {
    t_object x_obj;
    t_float x_f;
    t_outlet *x_outlet_dt;
    t_outlet *x_outlet_bang;
} t_dt_ms;


static void dt_ms_bang(t_dt_ms *x) {
    struct timeval tv_1; gettimeofday(&tv_1, NULL);

    /* Measure time it takes to execute this subtree. */
    outlet_bang(x->x_outlet_bang);

    struct timeval tv_2; gettimeofday(&tv_2, NULL);

    suseconds_t us = tv_2.tv_usec - tv_1.tv_usec;
    time_t      s  = tv_2.tv_sec  - tv_1.tv_sec;
    if (us < 0) {
        us += 1000000;
        s--;
    }
    t_float f_us = us;
    t_float f_s = s;
    f_s *= 10000000.0;

    f_us += f_s;
    t_float f_ms = f_us / 1000.0f;

    outlet_float(x->x_outlet_dt, f_ms);
}

static void dt_ms_free(t_dt_ms *x  __attribute__((__unused__))) {
}

t_class *dt_ms_class;

static void *dt_ms_new(void ) {
    t_dt_ms *x = (t_dt_ms *)pd_new(dt_ms_class);
    x->x_outlet_dt = outlet_new(&x->x_obj, &s_float);
    x->x_outlet_bang = outlet_new(&x->x_obj, &s_float);
    return (void *)x;
}


#ifdef __cplusplus
extern "C"
{
#endif


void dt_ms_setup(void) {
    dt_ms_class = class_new(gensym("dt_ms"), (t_newmethod)dt_ms_new,
    	(t_method)dt_ms_free, sizeof(t_dt_ms), 0, A_NULL);
    class_addbang(dt_ms_class, (t_method)dt_ms_bang);
}

#ifdef __cplusplus
}
#endif
