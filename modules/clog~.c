/*
 *   clog.c  - some complex math dsp objects
 *   Copyright (c) 2000-2003 by Tom Schouten
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
 */

#include "m_pd.h"
#include <math.h>

#define MINNORM 0.0000000001

t_class *clog_class;

typedef struct clog
{
    t_object x_obj;
    t_float x_f;
} t_clog;

void clog_free(void)
{
}

static t_int *clog_perform(t_int *w)
{
    t_float *inx    = (t_float *)(w[2]);
    t_float *iny    = (t_float *)(w[3]);
    t_float *outx   = (t_float *)(w[5]); // clockwize addressing
    t_float *outy   = (t_float *)(w[4]);
    t_int n         = (t_int)(w[1]);

    while (n--) {
        t_float x = *inx++;
        t_float y = *iny++;
        t_float norm = sqrt(x*x + y*y);
        t_float arg = atan2(y, x);
        if (norm < MINNORM) {
            norm = MINNORM;
        }
        *outx++ = log(norm);
        *outy++ = arg;
    }
    return (w+6);
}

static void clog_dsp(t_clog *x, t_signal **sp)
{
    dsp_add(clog_perform, 5, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, 
        sp[2]->s_vec, sp[3]->s_vec);
}

t_clog *clog_new(void)
{
    t_clog *x = (t_clog *)pd_new(clog_class);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));  
    outlet_new(&x->x_obj, gensym("signal")); 
    outlet_new(&x->x_obj, gensym("signal")); 
    return x;
}

void clog_tilde_setup(void)
{
    clog_class = class_new(gensym("clog~"), (t_newmethod)clog_new,
        (t_method)clog_free, sizeof(t_clog), 0, 0);

    CLASS_MAINSIGNALIN(clog_class, t_clog, x_f);

    class_addmethod(clog_class, (t_method)clog_dsp, gensym("dsp"), 0); 
}
