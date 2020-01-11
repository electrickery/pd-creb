/*
 *   cexp.c  - some complex math dsp objects
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

t_class *cexp_class;

typedef struct cexp
{
    t_object x_obj;
    t_float x_f;
} t_cexp;

void cexp_free(void)
{
}

static t_int *cexp_perform(t_int *w)
{
    t_float *inx    = (t_float *)(w[2]);
    t_float *iny    = (t_float *)(w[3]);
    t_float *outx   = (t_float *)(w[5]); // clockwize addressing
    t_float *outy   = (t_float *)(w[4]);
    t_int n         = (t_int)(w[1]);

    while (n--){
        t_float x = *inx++;
        t_float y = *iny++;
        t_float norm = exp(x);
        *outx++ = norm * cos(y);
        *outy++ = norm * sin(y);
    }
    return (w+6);
}

static void cexp_dsp(t_cexp *x, t_signal **sp)
{
    dsp_add(cexp_perform, 5, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, 
        sp[2]->s_vec, sp[3]->s_vec);
}                                  

t_cexp *cexp_new(void)
{
    t_cexp *x = (t_cexp *)pd_new(cexp_class);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));  
    outlet_new(&x->x_obj, gensym("signal")); 
    outlet_new(&x->x_obj, gensym("signal")); 
    return x;
}

void cexp_tilde_setup(void)
{
    cexp_class = class_new(gensym("cexp~"), (t_newmethod)cexp_new,
        (t_method)cexp_free, sizeof(t_cexp), 0, 0);

    CLASS_MAINSIGNALIN(cexp_class, t_cexp, x_f);

    class_addmethod(cexp_class, (t_method)cexp_dsp, gensym("dsp"), 0);
}
