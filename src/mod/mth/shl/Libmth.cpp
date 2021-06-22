// ---------------------------------------------------------------------------
// - Libmth.cpp                                                              -
// - afnix:mth module - declaration & implementation                         -
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
#include "Math.hpp"
#include "Meta.hpp"
#include "Mpia.hpp"
#include "Ippd.hpp"
#include "Prng.hpp"
#include "Lufit.hpp"
#include "Random.hpp"
#include "Linear.hpp"
#include "Newton.hpp"
#include "Nblock.hpp"
#include "Rblock.hpp"
#include "Narray.hpp"
#include "Npoint.hpp"
#include "Rpoint.hpp"
#include "Mthsrl.hxx"
#include "Libmth.hpp"
#include "Predmth.hpp"
#include "Permute.hpp"
#include "Ivector.hpp"
#include "Nvector.hpp"
#include "Rvector.hpp"
#include "Rgivens.hpp"
#include "Rmatrix.hpp"
#include "Rmatrix1.hpp"
#include "MthCalls.hpp"
#include "Function.hpp"
#include "Analytic.hpp"
#include "Rsamples.hpp"
#include "Rpolynom.hpp"
#include "Rfunction.hpp"
#include "Primality.hpp"
#include "MthTransit.hpp"
#include "Rtransform.hpp"
#include "LinearFactory.hpp"

namespace afnix {

  // initialize the afnix:mth module

  Object* init_afnix_mth (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    // bind the serial dispatcher
    if (Serial::isvdid (SRL_DEOD_DID) == false) {
      throw Exception ("interp-error", "invalid mth serial dispatcher");
    }
    // create the afnix:mth nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* mset = aset->mknset   ("mth");

    // bind standard math constants
    mset->symcst ("+INFINITY",           new Real (Math::CV_PINF));
    mset->symcst ("-INFINITY",           new Real (Math::CV_NINF));
    mset->symcst ("NAN",                 new Real (Math::CV_NAN));
    mset->symcst ("PI",                  new Real (Math::CV_PI));
    mset->symcst ("TWO-PI",              new Real (Math::CV_TP));
    mset->symcst ("HALF-PI",             new Real (Math::CV_HP));
    mset->symcst ("E",                   new Real (Math::CV_E));

    // bind all symbols in the afnix:mth nameset
    mset->symcst ("Qr",                  new Meta (Qr::mknew));
    mset->symcst ("Mgs",                 new Meta (Mgs::mknew));
    mset->symcst ("Rmd",                 new Meta (Rmd::mknew));
    mset->symcst ("Cgs",                 new Meta (Cgs::mknew));
    mset->symcst ("Bcs",                 new Meta (Bcs::mknew));
    mset->symcst ("Cov",                 new Meta (Cov::mknew));
    mset->symcst ("Mean",                new Meta (Mean::mknew));
    mset->symcst ("Tqmr",                new Meta (Tqmr::mknew));
    mset->symcst ("Rmit",                new Meta (Rmit::meval));
    mset->symcst ("Ippd",                new Meta (Ippd::mknew));
    mset->symcst ("Prng",                new Meta (Prng::mknew));
    mset->symcst ("Lufit",               new Meta (Lufit::mknew));
    mset->symcst ("Linear",              new Meta (Linear::mknew));
    mset->symcst ("Newton",              new Meta (Newton::mknew));
    mset->symcst ("Nblock",              new Meta (Nblock::mknew));
    mset->symcst ("Rblock",              new Meta (Rblock::mknew));
    mset->symcst ("Narray",              new Meta (Narray::mknew));
    mset->symcst ("Npoint",              new Meta (Npoint::mknew));
    mset->symcst ("Rpoint",              new Meta (Rpoint::mknew));
    mset->symcst ("Permute",             new Meta (Permute::mknew));
    mset->symcst ("Ivector",             new Meta (Ivector::mknew));
    mset->symcst ("Nvector",             new Meta (Nvector::mknew));
    mset->symcst ("Rvector",             new Meta (Rvector::mknew));
    mset->symcst ("Rgivens",             new Meta (Rgivens::mknew));
    mset->symcst ("Rmatrix",             new Meta (Rmatrix::mknew));
    mset->symcst ("Rpoint1",             new Meta (Rpoint1::mknew));
    mset->symcst ("Rpoint2",             new Meta (Rpoint2::mknew));
    mset->symcst ("Rpoint3",             new Meta (Rpoint3::mknew));
    mset->symcst ("Rpoint4",             new Meta (Rpoint4::mknew));
    mset->symcst ("Rvector1",            new Meta (Rvector1::mknew));
    mset->symcst ("Rvector2",            new Meta (Rvector2::mknew));
    mset->symcst ("Rvector3",            new Meta (Rvector3::mknew));
    mset->symcst ("Rvector4",            new Meta (Rvector4::mknew));
    mset->symcst ("Rmatrix1",            new Meta (Rmatrix1::mknew));
    mset->symcst ("Rmatrix2",            new Meta (Rmatrix2::mknew));
    mset->symcst ("Rmatrix3",            new Meta (Rmatrix3::mknew));
    mset->symcst ("Rmatrix4",            new Meta (Rmatrix4::mknew));
    mset->symcst ("Rsamples",            new Meta (Rsamples::mknew));
    mset->symcst ("Rpolynom",            new Meta (Rpolynom::mknew));
    mset->symcst ("Analytic",            new Meta (Analytic::mknew));
    mset->symcst ("Rfunction",           new Meta (Rfunction::mknew));
    mset->symcst ("MthTransit",          new Meta (MthTransit::mknew));
    mset->symcst ("Rtransform",          new Meta (Rtransform::mknew));
    mset->symcst ("LinearFactory",       new Meta (LinearFactory::mknew));
    
    // bind the predicates
    mset->symcst ("qr-p",                new Function (mth_qrdp));
    mset->symcst ("avi-p",               new Function (mth_avip));
    mset->symcst ("ivi-p",               new Function (mth_ivip));
    mset->symcst ("nvi-p",               new Function (mth_nvip));
    mset->symcst ("rvi-p",               new Function (mth_rvip));
    mset->symcst ("ami-p",               new Function (mth_amip));
    mset->symcst ("nmi-p",               new Function (mth_nmip));
    mset->symcst ("rmi-p",               new Function (mth_rmip));
    mset->symcst ("aai-p",               new Function (mth_aaip));
    mset->symcst ("nai-p",               new Function (mth_naip));
    mset->symcst ("api-p",               new Function (mth_apip));
    mset->symcst ("npi-p",               new Function (mth_npip));
    mset->symcst ("rpi-p",               new Function (mth_rpip));
    mset->symcst ("cpi-p",               new Function (mth_cpip));
    mset->symcst ("rfi-p",               new Function (mth_rfip));
    mset->symcst ("mgs-p",               new Function (mth_mgsp));
    mset->symcst ("rmd-p",               new Function (mth_rmdp));
    mset->symcst ("cgs-p",               new Function (mth_cgsp));
    mset->symcst ("bcs-p",               new Function (mth_bcsp));
    mset->symcst ("cov-p",               new Function (mth_covp));
    mset->symcst ("fit-p",               new Function (mth_fitp));
    mset->symcst ("mean-p",              new Function (mth_meanp));
    mset->symcst ("tqmr-p",              new Function (mth_tqmrp));
    mset->symcst ("ippd-p",              new Function (mth_ippdp));
    mset->symcst ("prng-p",              new Function (mth_prngp));
    mset->symcst ("lufit-p",             new Function (mth_lufp));
    mset->symcst ("solver-p",            new Function (mth_slvp));
    mset->symcst ("direct-p",            new Function (mth_dlsp));
    mset->symcst ("linear-p",            new Function (mth_lnrp));
    mset->symcst ("newton-p",            new Function (mth_ntwp));
    mset->symcst ("transit-p",           new Function (mth_xsitp));
    mset->symcst ("permute-p",           new Function (mth_prmtp));
    mset->symcst ("analytic-p",          new Function (mth_anap));
    mset->symcst ("parallel-p",          new Function (mth_parp));
    mset->symcst ("iterative-p",         new Function (mth_ilsp));
    mset->symcst ("n-block-p",           new Function (mth_nblkp));
    mset->symcst ("r-block-p",           new Function (mth_rblkp));
    mset->symcst ("n-array-p",           new Function (mth_naryp));
    mset->symcst ("n-point-p",           new Function (mth_nptnp));
    mset->symcst ("r-point-p",           new Function (mth_rptnp));
    mset->symcst ("i-vector-p",          new Function (mth_ivectp));
    mset->symcst ("n-vector-p",          new Function (mth_nvectp));
    mset->symcst ("r-vector-p",          new Function (mth_rvectp));
    mset->symcst ("r-givens-p",          new Function (mth_rgvnsp));
    mset->symcst ("r-block-p",           new Function (mth_rblkp));
    mset->symcst ("r-matrix-p",          new Function (mth_rmtrxp));
    mset->symcst ("r-samples-p",         new Function (mth_rsap));
    mset->symcst ("r-polynom-p",         new Function (mth_rpolyp));
    mset->symcst ("r-function-p",        new Function (mth_rfuncp));
    mset->symcst ("r-point-1-p",         new Function (mth_rpnt1p));
    mset->symcst ("r-point-2-p",         new Function (mth_rpnt2p));
    mset->symcst ("r-point-3-p",         new Function (mth_rpnt3p));
    mset->symcst ("r-point-4-p",         new Function (mth_rpnt4p));
    mset->symcst ("r-vector-1-p",        new Function (mth_rvec1p));
    mset->symcst ("r-vector-2-p",        new Function (mth_rvec2p));
    mset->symcst ("r-vector-3-p",        new Function (mth_rvec3p));
    mset->symcst ("r-vector-4-p",        new Function (mth_rvec4p));
    mset->symcst ("r-matrix-1-p",        new Function (mth_rmtx1p));
    mset->symcst ("r-matrix-2-p",        new Function (mth_rmtx2p));
    mset->symcst ("r-matrix-3-p",        new Function (mth_rmtx3p));
    mset->symcst ("r-matrix-4-p",        new Function (mth_rmtx4p));
    mset->symcst ("r-transform-p",       new Function (mth_rtfrmp));
    mset->symcst ("solver-factory-p",    new Function (mth_sfcp));
    mset->symcst ("linear-factory-p",    new Function (mth_lfcp));

    // bind all mth calls in the afnix:mth nameset
    mset->symcst ("gcd",                 new Function (mth_gcd));
    mset->symcst ("lcm",                 new Function (mth_lcm));
    mset->symcst ("mmi",                 new Function (mth_mmi));

    mset->symcst ("to-rvi",              new Function (mth_torvi));
    mset->symcst ("to-infix",            new Function (mth_toinfix));
    
    mset->symcst ("get-random-byte",     new Function (mth_byternd));
    mset->symcst ("get-random-integer",  new Function (mth_longrnd));
    mset->symcst ("get-random-real",     new Function (mth_realrnd));
    mset->symcst ("get-random-relatif",  new Function (mth_relnrnd));
    mset->symcst ("get-random-prime",    new Function (mth_primrnd));
    mset->symcst ("get-random-safe",     new Function (mth_safernd));
    mset->symcst ("get-random-bitset",   new Function (mth_bitsrnd));
    mset->symcst ("get-random-i-vector", new Function (mth_ivecrnd));
    mset->symcst ("get-random-n-vector", new Function (mth_nvecrnd));
    mset->symcst ("get-random-r-vector", new Function (mth_rvecrnd));
    mset->symcst ("get-random-n-block",  new Function (mth_nblkrnd));
    mset->symcst ("get-random-r-block",  new Function (mth_rblkrnd));
    mset->symcst ("get-random-r-matrix", new Function (mth_rmtxrnd));
    mset->symcst ("get-sparse-r-block",  new Function (mth_rblksps));
    mset->symcst ("get-sparse-r-matrix", new Function (mth_rmtxsps));
    mset->symcst ("get-uniform-deviate", new Function (mth_unidev));
    mset->symcst ("get-normal-deviate",  new Function (mth_nrmdev));

    mset->symcst ("fermat-p",            new Function (mth_frmtp));
    mset->symcst ("miller-rabin-p",      new Function (mth_mrptp)); 
    mset->symcst ("prime-safe-p",        new Function (mth_safep));
    mset->symcst ("prime-probable-p",    new Function (mth_ppntp));
   
    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_mth (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_mth (interp, argv);
  }
}
