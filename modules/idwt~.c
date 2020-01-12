/*
 *   idwt.c  - code for discrete wavelet transform 
 *   (symmetric interpolating biorthogonal wavelets using the lifting transform) 
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dwt-common.h"

#define MAXORDER 64

t_class *idwt_class;

static t_int *idwt_perform(t_int *w)
{
  t_float *in     = (t_float *)(w[3]);
  t_float *out    = (t_float *)(w[4]);
  t_dwtctl *ctl  = (t_dwtctl *)(w[1]);

  t_int n = (t_int)(w[2]);

  int i;

  int numcoef = 1;
  int source_u = ((2 - ctl->c_nupdate) - 1) * (n/2);
  int source_p = ((2 - ctl->c_npredict)) * (n/2);
  int increment = n;
  int dest = n/2;
  int backup_u = (ctl->c_nupdate-1)*n;
  int backup_p = (ctl->c_npredict-1)*n;
  int fake_in = ctl->c_fakein;
  t_float fake_val = ctl->c_fakeval;

  /* copy input to output */
  if (in != out)
    for (i=0; i<n; i++) 
        out[i]=in[i];

  /* fake input */
  
  if ((fake_in >= 0) && (fake_in<n)){
    for (i=0; i<n; i++) 
        out[i]=0; 
    out[fake_in]=fake_val;
  }

  if (ctl->c_permute) 
    dwt_perform_permutation(out, n, ctl->c_clutter);

  /* backward transform */

  /* iterate over all levels */
  for (i=0; i < ctl->c_levels; i++){

    /* backward update */
    dwtloop(out, (source_u & (n-1)), 0,    increment, backup_u, numcoef, n-1, ctl->c_update,  ctl->c_nupdate,  -1);


    /* backward predict */
    dwtloop(out, (source_p & (n-1)), dest, increment, backup_p, numcoef, n-1, ctl->c_predict, ctl->c_npredict, +1);

    /* update control parameters */
    numcoef *= 2;
    source_p /= 2;
    source_u /= 2;
    backup_p /= 2;
    backup_u /= 2;
    increment /= 2;
    dest /= 2;
  }
  return (w+5);
}

static void idwt_dsp(t_dwt *x, t_signal **sp)
{
    int n = sp[0]->s_n;
    int ln = 0;

    dwt_permutation(x, n);

    x->x_ctl.c_mask = n-1;
    while (n >>= 1) 
        ln++;     
    x->x_ctl.c_levels = ln;

    dsp_add(idwt_perform, 4, &x->x_ctl, sp[0]->s_n, sp[0]->s_vec, sp[1]->s_vec);
}

static void *idwt_new(t_floatarg permute)
{
    t_dwt *x = (t_dwt *)pd_new(idwt_class);

    outlet_new(&x->x_obj, gensym("signal")); 

    /* init data */
    dwt_reset(x);

    x->x_ctl.c_clutter = NULL;
    x->x_ctl.c_unclutter = NULL;
    x->x_ctl.c_permute = (t_int) permute;

    sprintf(x->x_ctl.c_name,"idwt");
    x->x_ctl.c_type = IDWT;
    return (void *)x;
}

void idwt_tilde_setup(void)
{
  //post("idwt~ v0.1");

    idwt_class = class_new(gensym("idwt~"), (t_newmethod)idwt_new,
        (t_method)dwt_free, sizeof(t_dwt), 0, A_DEFFLOAT, 0);

    CLASS_MAINSIGNALIN(idwt_class, t_dwt, x_f);
    class_addmethod(idwt_class, (t_method)dwt_print, gensym("print"), 0);
    class_addmethod(idwt_class, (t_method)idwt_dsp, gensym("dsp"), 0); 

    class_addmethod(idwt_class, (t_method)dwt_filter, gensym("predict"), A_GIMME, 0); 
    class_addmethod(idwt_class, (t_method)dwt_filter, gensym("update"), A_GIMME, 0); 
    class_addmethod(idwt_class, (t_method)dwt_filter, gensym("mask"), A_GIMME, 0); 

    class_addmethod(idwt_class, (t_method)idwt_coef, gensym("coef"), A_DEFFLOAT, A_DEFFLOAT, 0); 

    class_addmethod(idwt_class, (t_method)dwt_even, gensym("even"), A_DEFFLOAT, 0);
}
