/*
 *   nfft.c  - some complex math dsp objects
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

t_class *nfft_class;

typedef struct nfft
{
    t_object x_obj;
    t_float x_f;
} t_nfft;

void nfft_free(void)
{
}

static t_int *nfft_perform(t_int *w)
{
    t_float *inx    = (t_float *)(w[2]);
    t_float *iny    = (t_float *)(w[3]);
    t_float *outx    = (t_float *)(w[5]); // clockwize addressing
    t_float *outy    = (t_float *)(w[4]);
    t_int i;
    t_int n = (t_int)(w[1]);
    t_float x;
    t_float scale = 1.0 / (sqrt((t_float)n));

    mayer_fft(n, inx, outx);

    while (n--){
        t_float x = *inx++;
        t_float y = *iny++;
        *outx++ = scale * x;
        *outy++ = scale * y;
    }
    return (w+6);
}

static void nfft_dsp(t_nfft *x, t_signal **sp)
{
    dsp_add(nfft_perform, 5, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec, 
        sp[2]->s_vec, sp[3]->s_vec);
}

t_nfft *nfft_new(void)
{
    t_nfft *x = (t_nfft *)pd_new(nfft_class);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("signal"), gensym("signal"));  
    outlet_new(&x->x_obj, gensym("signal")); 
    outlet_new(&x->x_obj, gensym("signal")); 
    return x;
}

void nfft_tilde_setup(void)
{
    nfft_class = class_new(gensym("nfft~"), (t_newmethod)nfft_new,
        (t_method)nfft_free, sizeof(t_nfft), 0, 0);

    CLASS_MAINSIGNALIN(nfft_class, t_nfft, x_f);

    class_addmethod(nfft_class, (t_method)nfft_dsp, gensym("dsp"), 0); 
}
