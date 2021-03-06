#include "m_pd.h"

void ead_tilde_setup(void);
void ear_tilde_setup(void);
void eadsr_tilde_setup(void);
void dist_tilde_setup(void);
void tabreadmix_tilde_setup(void);
void xfm_tilde_setup(void);
void biquadseries_tilde_setup(void);
void filterortho_tilde_setup(void);
void qmult_tilde_setup(void);
void qnorm_tilde_setup(void);
void cheby_tilde_setup(void);
void abs_tilde_setup(void);
void ramp_tilde_setup(void);
void dwt_tilde_setup(void);
void bfft_tilde_setup(void);
void dynwav_tilde_setup(void);
void statwav_tilde_setup(void);
void bdiag_tilde_setup(void);
void diag_tilde_setup(void);
void bmatrix_tilde_setup(void);
void permut_tilde_setup(void);
void lattice_tilde_setup(void);
void ratio_setup(void);
void ffpoly_setup(void);
void dt_ms_setup(void);
void fwarp_setup(void);
void junction_tilde_setup(void);
void fdn_tilde_setup(void);
void bwin_tilde_setup(void);
void blosc_tilde_setup(void);
void cmath_tilde_setup(void);
void bitsplit_tilde_setup(void);
void sbosc_tilde_setup(void);
void blocknorm_tilde_setup(void);
void resofilt_tilde_setup(void);
void scrollgrid1D_tilde_setup(void);
void sawtooth_tilde_setup(void);


#ifndef CREB_VERSION
#define CREB_VERSION "externals cvs"
#endif


void creb_setup(void)
{
  post("CREB: version " CREB_VERSION);

  /* setup tilde objects */
  ead_tilde_setup();
  ear_tilde_setup();
  eadsr_tilde_setup();
  dist_tilde_setup();
  tabreadmix_tilde_setup();
  xfm_tilde_setup();
  qmult_tilde_setup();
  qnorm_tilde_setup();
  cheby_tilde_setup();
  ramp_tilde_setup();
  dwt_tilde_setup();
  bfft_tilde_setup();
  dynwav_tilde_setup();
  statwav_tilde_setup();
  bdiag_tilde_setup();
  diag_tilde_setup();
  bmatrix_tilde_setup();
  permut_tilde_setup();
  lattice_tilde_setup();
  junction_tilde_setup();
  fdn_tilde_setup();
  bwin_tilde_setup();
  blosc_tilde_setup();
  cmath_tilde_setup();
  bitsplit_tilde_setup();
  sbosc_tilde_setup();
  blocknorm_tilde_setup();
  resofilt_tilde_setup();
  scrollgrid1D_tilde_setup();
  sawtooth_tilde_setup();

  /* setup other objects */
  ratio_setup();
  ffpoly_setup();
  dt_ms_setup();
  fwarp_setup();

  /* setup c++ modules */
  biquadseries_tilde_setup();
  filterortho_tilde_setup();

  /* optional modules */
#ifdef HAVE_ABS_TILDE
  abs_tilde_setup();
#endif

}
