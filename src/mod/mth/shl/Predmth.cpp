// ---------------------------------------------------------------------------
// - Predmth.cpp                                                             -
// - afnix:mth module - predicates implementation                            -
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

#include "Qr.hpp"
#include "Mgs.hpp"
#include "Cgs.hpp"
#include "Bcs.hpp"
#include "Cov.hpp"
#include "Mean.hpp"
#include "Tqmr.hpp"
#include "Cons.hpp"
#include "Ippd.hpp"
#include "Prng.hpp"
#include "Lufit.hpp"
#include "Linear.hpp"
#include "Newton.hpp"
#include "Nblock.hpp"
#include "Rblock.hpp"
#include "Narray.hpp"
#include "Npoint.hpp"
#include "Rpoint.hpp"
#include "Predmth.hpp"
#include "Boolean.hpp"
#include "Permute.hpp"
#include "Ivector.hpp"
#include "Nvector.hpp"
#include "Rvector.hpp"
#include "Rgivens.hpp"
#include "Rmatrix.hpp"
#include "Rmatrix1.hpp"
#include "Analytic.hpp"
#include "Rsamples.hpp"
#include "Rpolynom.hpp"
#include "Parallel.hpp"
#include "Rfunction.hpp"
#include "Exception.hpp"
#include "MthTransit.hpp"
#include "Rtransform.hpp"
#include "LinearFactory.hpp"

namespace afnix {

  // this procedure checks that we have one argument only and returns
  // the evaluated object
  static inline Object* get_obj (Evaluable* zobj, Nameset* nset, Cons* args,
                                 const String& pname) {
    Object* car = nullptr;
    if ((args == nullptr) || (args->length () != 1))
      throw Exception ("argument-error", "illegal arguments with predicate",
                       pname);
    car = args->getcar ();
    return (car == nullptr) ? nullptr : car->eval (zobj,nset);
  }

  // avip: avi object predicate

  Object* mth_avip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "avi-p");
    bool result = (dynamic_cast <Avi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ivip: ivi object predicate

  Object* mth_ivip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "ivi-p");
    bool result = (dynamic_cast <Ivi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // nvip: nvi object predicate

  Object* mth_nvip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "nvi-p");
    bool result = (dynamic_cast <Nvi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rvip: rvi object predicate

  Object* mth_rvip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "rvi-p");
    bool result = (dynamic_cast <Rvi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // amip: ami object predicate

  Object* mth_amip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "ami-p");
    bool result = (dynamic_cast <Ami*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // nmip: nmi object predicate

  Object* mth_nmip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "nmi-p");
    bool result = (dynamic_cast <Nmi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rmip: rmi object predicate

  Object* mth_rmip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "rmi-p");
    bool result = (dynamic_cast <Rmi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // aaip: aai object predicate

  Object* mth_aaip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "aai-p");
    bool result = (dynamic_cast <Aai*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // naip: nai object predicate

  Object* mth_naip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "nai-p");
    bool result = (dynamic_cast <Nai*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // apip: api object predicate

  Object* mth_apip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "api-p");
    bool result = (dynamic_cast <Api*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // npip: npi object predicate

  Object* mth_npip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "npi-p");
    bool result = (dynamic_cast <Npi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rpip: rpi object predicate

  Object* mth_rpip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "rpi-p");
    bool result = (dynamic_cast <Rpi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // cpip: cpi object predicate

  Object* mth_cpip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "cpi-p");
    bool result = (dynamic_cast <Cpi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rfip: rfi object predicate
  
  Object* mth_rfip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "rfi-p");
    bool result = (dynamic_cast <Rfi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rsap: real samples array object predicate

  Object* mth_rsap  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-samples-p");
    bool result = (dynamic_cast <Rsamples*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rmdp: rmd object predicate

  Object* mth_rmdp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "rmd-p");
    bool result = (dynamic_cast <Rmd*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // slvp: solver object predicate

  Object* mth_slvp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "solver-p");
    bool result = (dynamic_cast <Solver*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // dlsp: direct object predicate

  Object* mth_dlsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "direct-p");
    bool result = (dynamic_cast <Direct*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // parp: parallel object predicate

  Object* mth_parp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "parallel-p");
    bool result = (dynamic_cast <Parallel*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // qrdp: qr object predicate

  Object* mth_qrdp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "qr-p");
    bool result = (dynamic_cast <Qr*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // mgsp: mgs direct object predicate

  Object* mth_mgsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "mgs-p");
    bool result = (dynamic_cast <Mgs*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ilsp: iterative object predicate

  Object* mth_ilsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "iterative-p");
    bool result = (dynamic_cast <Iterative*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // cgsp: cgs solver object predicate

  Object* mth_cgsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "cgs-p");
    bool result = (dynamic_cast <Cgs*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // bcsp: bcs solver object predicate

  Object* mth_bcsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "bcs-p");
    bool result = (dynamic_cast <Bcs*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
 
  // tqmrp: tqmr solver object predicate

  Object* mth_tqmrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "tqmr-p");
    bool result = (dynamic_cast <Tqmr*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lnrp: linear object predicate

  Object* mth_lnrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "linear-p");
    bool result = (dynamic_cast <Linear*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sfcp: solver factory object predicate

  Object* mth_sfcp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "solver-factory-p");
    bool result =
      (dynamic_cast <SolverFactory*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
 
  // lfcp: linear factory object predicate

  Object* mth_lfcp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "linear-factory-p");
    bool result =
      (dynamic_cast <LinearFactory*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ntwp: newton object predicate

  Object* mth_ntwp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "newton-p");
    bool result = (dynamic_cast <Newton*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ippdp: integer plane point object predicate

  Object* mth_ippdp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "ippd-p");
    bool result = (dynamic_cast <Ippd*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // prmtp: permute object predicate

  Object* mth_prmtp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "permute-p");
    bool result = (dynamic_cast <Permute*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ivectp: ivector object predicate
  
  Object* mth_ivectp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "i-vector-p");
    bool result = (dynamic_cast <Ivector*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // nvectp: nvector object predicate
  
  Object* mth_nvectp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "n-vector-p");
    bool result = (dynamic_cast <Nvector*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rvectp: rvector object predicate

  Object* mth_rvectp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-vector-p");
    bool result = (dynamic_cast <Rvector*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // nblkp: numeral block object predicate

  Object* mth_nblkp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "n-block-p");
    bool result = (dynamic_cast <Nblock*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rblkp: real block object predicate

  Object* mth_rblkp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-block-p");
    bool result = (dynamic_cast <Rblock*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // naryp: numeral array object predicate

  Object* mth_naryp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "n-array-p");
    bool result = (dynamic_cast <Narray*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // rmtrxp: rmatrix object predicate

  Object* mth_rmtrxp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-matrix-p");
    bool result = (dynamic_cast <Rmatrix*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rfuncp: rfunction object predicate

  Object* mth_rfuncp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-function-p");
    bool result = (dynamic_cast <Rfunction*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rpolyp: rpolynom object predicate

  Object* mth_rpolyp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-polynom-p");
    bool result = (dynamic_cast <Rpolynom*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // rgvnsp: rgivens object predicate

  Object* mth_rgvnsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-givens-p");
    bool result = (dynamic_cast <Rgivens*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // anap: analytic object predicate

  Object* mth_anap (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "analytic-p");
    bool result = (dynamic_cast <Analytic*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // nptp: npoint object predicate

  Object* mth_nptnp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "n-point-p");
    bool result = (dynamic_cast <Npoint*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rptp: rpoint object predicate

  Object* mth_rptnp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-point-p");
    bool result = (dynamic_cast <Rpoint*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // meanp: mean object predicate

  Object* mth_meanp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "mean-p");
    bool result = (dynamic_cast <Mean*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // covp: covariance object predicate

  Object* mth_covp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "cov-p");
    bool result = (dynamic_cast <Cov*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // fitp: fitter object predicate

  Object* mth_fitp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "fit-p");
    bool result = (dynamic_cast <Fit*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // luf: lufit object predicate
  
  Object* mth_lufp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "lufit-p");
    bool result = (dynamic_cast <Lufit*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // prngp: prng object predicate
  
  Object* mth_prngp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "prng-p");
    bool result = (dynamic_cast <Prng*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xsitp: transit object predicate

  Object* mth_xsitp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "transit-p");
    bool result = (dynamic_cast<MthTransit*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // rpnt1p: point 1 object predicate

  Object* mth_rpnt1p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-point-1-p");
    bool result = (dynamic_cast<Rpoint1*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rpnt2p: point 2 object predicate

  Object* mth_rpnt2p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-point-2-p");
    bool result = (dynamic_cast<Rpoint2*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rpnt1p: point 3 object predicate

  Object* mth_rpnt3p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-point-3-p");
    bool result = (dynamic_cast<Rpoint3*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rpnt4p: point 4 object predicate

  Object* mth_rpnt4p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-point-4-p");
    bool result = (dynamic_cast<Rpoint4*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // rvec1p: vector 1 object predicate

  Object* mth_rvec1p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-vector-1-p");
    bool result = (dynamic_cast<Rvector1*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rvec2p: vector 2 object predicate

  Object* mth_rvec2p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-vector-2-p");
    bool result = (dynamic_cast<Rvector2*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rvec3p: vector 3 object predicate

  Object* mth_rvec3p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-vector-3-p");
    bool result = (dynamic_cast<Rvector3*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rvec4p: vector 4 object predicate

  Object* mth_rvec4p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-vector-4-p");
    bool result = (dynamic_cast<Rvector4*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rmtx1p: matrix 1 object predicate

  Object* mth_rmtx1p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-matrix-1-p");
    bool result = (dynamic_cast<Rmatrix1*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rmtx2p: matrix 2 object predicate

  Object* mth_rmtx2p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-matrix-2-p");
    bool result = (dynamic_cast<Rmatrix2*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // rmtx3p: matrix 3 object predicate

  Object* mth_rmtx3p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-matrix-3-p");
    bool result = (dynamic_cast<Rmatrix3*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // rmtx4p: matrix 4 object predicate

  Object* mth_rmtx4p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-matrix-4-p");
    bool result = (dynamic_cast<Rmatrix4*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rtfrmp: transform object predicate

  Object* mth_rtfrmp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "r-transform-p");
    bool result = (dynamic_cast<Rtransform*>(obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
