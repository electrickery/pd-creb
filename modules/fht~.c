/*
 *   fht.c - Code for some fourrier transform variants and utility
 *   functions. Data organization is in (real, imag) pairs the first 2
 *   components are (DC, NY)
 *
 *   Copyright (c) 2000-2003 by Tom Schouten
 * 
 *   Restructured code to separate objects and library blob mode
 *   Fred Jan Kraan, 2020-01-12
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
#include <stdio.h>
#include <stdlib.h>

#define MAXORDER 64

typedef struct fhtctl
{
    t_int c_levels;
    char c_name[16];
    t_int *c_clutter;
    t_int *c_unclutter;
    t_int c_kill_DC;
    t_int c_kill_NY;
} t_fhtctl;

typedef struct fht
{
    t_object x_obj;
    t_float x_f;
    t_fhtctl x_ctl;
} t_fht;

t_class *fht_class;

static t_int *fht_perform(t_int *w)
{
//  t_float *in   = (t_float *)(w[3]);
  t_float *out  = (t_float *)(w[4]);
//  t_fhtctl *ctl = (t_fhtctl *)(w[1]);
  t_int n       = (t_int)(w[2]);

  mayer_fht(out, n);

  return (w+5);
}

static void fht_dsp(t_fht *x, t_signal **sp)
{
  int n = sp[0]->s_n;
  t_float *in = sp[0]->s_vec;
  t_float *out = sp[1]->s_vec;

  if (in != out)
    {
      dsp_add_copy(in, out, n);
      in = out;
    }
  dsp_add(fht_perform, 4, &x->x_ctl, n, in, out);
}

static void fht_free(t_fht *x)
{
  if (x->x_ctl.c_clutter)   free(x->x_ctl.c_clutter);
  if (x->x_ctl.c_unclutter) free(x->x_ctl.c_unclutter);
}

static void *fht_new(void)
{
    t_fht *x = (t_fht *)pd_new(fht_class);

    outlet_new(&x->x_obj, gensym("signal")); 

    x->x_ctl.c_clutter = NULL;
    x->x_ctl.c_unclutter = NULL;

    sprintf(x->x_ctl.c_name,"fht");

    return (void *)x;
}

void fht_tilde_setup(void)
{
  //post("fht~ v0.1");
   fht_class = class_new(gensym("fht~"), (t_newmethod)fht_new,
    	(t_method)fht_free, sizeof(t_fht), 0, 0);

    CLASS_MAINSIGNALIN(fht_class, t_fht, x_f);
    class_addmethod(fht_class, (t_method)fht_dsp, gensym("dsp"), 0); 
}
