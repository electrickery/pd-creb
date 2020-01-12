/*
 *   blockramp.c  - retriggerable counter for dsp signals 
 *   Copyright (c) 2000-2003 by Tom Schouten
 * 
 *   Restructuring for independent objects and library blob mode by
 *   Fred Jan Kraan 2020-01-12
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

#include "ramp-common.h"

t_class *blockramp_class;

void *blockramp_new(void)
{
    t_ramp *x = (t_ramp *)pd_new(blockramp_class);
    outlet_new(&x->x_obj, gensym("signal")); 
    x->x_ctl.c_blockscale = 0;
    ramp_bang(x);
    
    x->x_ctl.c_blockscale = 1;
    return (void *)x;
}

void blockramp_tilde_setup(void)
{
  //post("blockramp~ v0.1");
    blockramp_class = class_new(gensym("blockramp~"), (t_newmethod)blockramp_new,
    	(t_method)ramp_free, sizeof(t_ramp), 0, 0);

    class_addmethod(blockramp_class, (t_method)ramp_bang, gensym("bang"), 0);
    class_addmethod(blockramp_class, (t_method)ramp_dsp, gensym("dsp"), 0); 
    class_addfloat(blockramp_class,  (t_method)ramp_offset); 
}
