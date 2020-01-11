/*
 *   sawtooth.c  - differentiated piecewize polynomial sawtooth generator
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


typedef struct sawtoothctl
{
    t_float c_phase, c_increment, c_delay_1, c_delay_2;
} t_sawtoothctl;

typedef struct sawtooth
{
    t_object x_obj;
    t_float x_f;
    t_sawtoothctl x_ctl;
} t_sawtooth;


void sawtooth_phase(t_sawtooth *x,  t_floatarg f)
{
    x->x_ctl.c_phase = f;
}

void sawtooth_freq(t_sawtooth *x,  t_floatarg f)
{
    x->x_ctl.c_increment = f / sys_getsr();
    post ("increment %f",  x->x_ctl.c_increment);
}


void sawtooth_bang(t_sawtooth *x)
{
    sawtooth_phase(x, 0);
}



static t_int *sawtooth_perform(t_int *w)
{
    t_float *out    = (float *)(w[3]);
    t_sawtoothctl *ctl  = (t_sawtoothctl *)(w[1]);
    int i;
    int n = (int)(w[2]);
    t_float phase, x, _x;
    t_float x1 = ctl->c_delay_1;
    t_float x2 = ctl->c_delay_2;

    phase = ctl->c_phase;
    
    for (i = 0; i < n; i++)
	{
        phase += ctl->c_increment;
        
        /* Limit range to [0,1[ */
        x = phase - ((t_float)((int)phase));

        /* Bring range to [-1,1[ */
        x = 2 * x - 1;
        
        /* Cubic. */
        x = (x-1) * x * (x + 1);

        /* Differentiate twice, (backward diff) */
        _x = x;  x -= x1; x1 = _x;
        _x = x;  x -= x2; x2 = _x;

	    *out++ = x;
        //post("%f", x);
	}

    /* Oscillator and filter state. */
    ctl->c_phase   = phase; 
    ctl->c_delay_1 = x1;
    ctl->c_delay_2 = x2;
    
    return (w+4);
}

static void sawtooth_dsp(t_sawtooth *x, t_signal **sp)
{
    dsp_add(sawtooth_perform, 3, &x->x_ctl, sp[0]->s_n, sp[0]->s_vec);

}                                  
void sawtooth_free(void)
{

}

t_class *sawtooth_class;

void *sawtooth_new(void)
{
    t_sawtooth *x = (t_sawtooth *)pd_new(sawtooth_class);
    outlet_new(&x->x_obj, gensym("signal")); 
    sawtooth_bang(x);
    sawtooth_freq(x, 0);
    return (void *)x;
}



void sawtooth_tilde_setup(void)
{
    // post("sawtooth~ v0.1");
    sawtooth_class = class_new(gensym("sawtooth~"), (t_newmethod)sawtooth_new,
    	(t_method)sawtooth_free, sizeof(t_sawtooth), 0, 0);

    class_addmethod(sawtooth_class, (t_method)sawtooth_bang, gensym("bang"), 0);
    class_addmethod(sawtooth_class, (t_method)sawtooth_dsp, gensym("dsp"), 0); 
    class_addmethod(sawtooth_class, (t_method)sawtooth_phase, gensym("phase"), A_FLOAT, A_NULL); 
    class_addmethod(sawtooth_class, (t_method)sawtooth_freq, gensym("freq"),   A_FLOAT, A_NULL); 

    class_addfloat(sawtooth_class, (t_method)sawtooth_phase); 
}

