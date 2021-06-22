// ---------------------------------------------------------------------------
// - Predmth.hpp                                                             -
// - afnix:mth module - predicates declaration                               -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#ifndef  AFNIX_PREDMTH_HPP
#define  AFNIX_PREDMTH_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:mth
  /// standard module.
  /// @author amaury darsch

  /// the avi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_avip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the ivi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_ivip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the nvi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_nvip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rvi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rvip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the ami object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_amip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the nmi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_nmip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rmi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rmip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the aai object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_aaip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the nai object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_naip (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the api object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_apip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the npi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_npip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rpi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rpip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the cpi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_cpip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rfi object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rfip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the real samples array object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rsap (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the real matrix datum object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rmdp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the solver object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_slvp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the direct object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_dlsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the parallel object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_parp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the qr direct object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_qrdp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the mgs direct object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_mgsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the iterative object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_ilsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the cgs solver object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_cgsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the bcs solver object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_bcsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the qmr solver object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_tqmrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the linear object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_lnrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the solver factory object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_sfcp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the linear factory object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_lfcp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the newton object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_ntwp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the integer plane point datum object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_ippdp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the permute object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_prmtp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the ivector object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_ivectp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the nvector object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_nvectp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rvector object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rvectp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the numeral block object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_nblkp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the numeral array object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_naryp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the rblock object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rblkp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rmatrix object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rmtrxp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rfunction object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rfuncp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rpolynom object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rpolyp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the real givens matrix object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rgvnsp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the analytic object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_anap (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the npoint object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_nptnp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the rpoint object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rptnp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the mean object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_meanp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the covariance object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_covp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the fitter object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_fitp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the lufit object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_lufp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the prng object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_prngp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the transit object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_xsitp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the point 1 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rpnt1p (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the point 2 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rpnt2p (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the point 3 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rpnt3p (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the point 4 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rpnt4p (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the vector 1 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rvec1p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the vector 2 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rvec2p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the vector 3 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rvec3p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the vector 4 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rvec4p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the matrix 1 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rmtx1p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the matrix 2 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rmtx2p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the matrix 3 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rmtx3p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the matrix 4 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rmtx4p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the transform object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* mth_rtfrmp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
