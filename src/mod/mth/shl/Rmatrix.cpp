// ---------------------------------------------------------------------------
// - Rmatrix.cpp                                                             -
// - afnix:mth module - real matrix implementation                           -
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

#include "Rmd.hpp"
#include "Real.hpp"
#include "Math.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Rmatrix.hpp"
#include "Algebra.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the row/col shift size
  static const long ROW_RMLB_SHLS = 5;
  static const long COL_RMLB_SHLS = 5;
  static const long ROW_RMCB_SHLS = 7;
  static const long COL_RMCB_SHLS = 7;
  static const long ROW_RMCC_SHLS = 8;
  static const long COL_RMCC_SHLS = 8;
  static const long ROW_RMCT_SHLS = 8;
  static const long COL_RMCT_SHLS = 8;
  static const long ROW_RMLT_SHLS = 4;
  static const long COL_RMLT_SHLS = 4;

  // the row/col index size
  static const long ROW_RMLB_SIZE = (1L << ROW_RMLB_SHLS);
  static const long COL_RMLB_SIZE = (1L << COL_RMLB_SHLS);
  static const long ROW_RMCB_SIZE = (1L << ROW_RMCB_SHLS);
  static const long COL_RMCB_SIZE = (1L << COL_RMCB_SHLS);
  static const long ROW_RMCC_SIZE = (1L << ROW_RMCC_SHLS);
  static const long COL_RMCC_SIZE = (1L << COL_RMCC_SHLS);
  static const long ROW_RMCT_SIZE = (1L << ROW_RMCT_SHLS);
  static const long COL_RMCT_SIZE = (1L << COL_RMCT_SHLS);
  static const long ROW_RMLT_SIZE = (1L << ROW_RMLT_SHLS);
  static const long COL_RMLT_SIZE = (1L << COL_RMLT_SHLS);

  // the row/col index mask
  static const long ROW_RMLB_MASK = 0x1FUL;
  static const long COL_RMLB_MASK = 0x1FUL;
  static const long ROW_RMCB_MASK = 0x7FUL;
  static const long COL_RMCB_MASK = 0x7FUL;
  static const long ROW_RMCC_MASK = 0xFFUL;
  static const long COL_RMCC_MASK = 0xFFUL;
  static const long ROW_RMCT_MASK = 0xFFUL;
  static const long COL_RMCT_MASK = 0xFFUL;
  static const long ROW_RMLT_MASK = 0x0FUL;
  static const long COL_RMLT_MASK = 0x0FUL;
  static const long ROW_RMHB_MASK = 0xFFFFFFFFUL;
  static const long COL_RMHB_MASK = 0xFFFFFFFFUL;

  // the row/col shift mask
  static const long ROW_RMCB_SHFT = ROW_RMLB_SHLS;
  static const long COL_RMCB_SHFT = COL_RMLB_SHLS ;
  static const long ROW_RMCC_SHFT = ROW_RMCB_SHFT + ROW_RMCB_SHLS;
  static const long COL_RMCC_SHFT = COL_RMCB_SHFT + COL_RMCB_SHLS;
  static const long ROW_RMCT_SHFT = ROW_RMCC_SHFT + ROW_RMCC_SHLS;
  static const long COL_RMCT_SHFT = COL_RMCC_SHFT + COL_RMCC_SHLS;
  static const long ROW_RMLT_SHFT = ROW_RMCT_SHFT + ROW_RMCT_SHLS;
  static const long COL_RMLT_SHFT = COL_RMCT_SHFT + COL_RMCT_SHLS;
  static const long ROW_RMHB_SHFT = ROW_RMLT_SHFT + ROW_RMLT_SHLS;
  static const long COL_RMHB_SHFT = COL_RMLT_SHFT + COL_RMLT_SHLS;

  // the column shifted mask
  static const long COL_RMLB_SMSK = COL_RMLB_MASK;
  static const long COL_RMCB_SMSK = COL_RMCB_MASK << COL_RMCB_SHFT;
  static const long COL_RMCC_SMSK = COL_RMCC_MASK << COL_RMCC_SHFT;
  static const long COL_RMCT_SMSK = COL_RMCT_MASK << COL_RMCT_SHFT;
  static const long COL_RMLT_SMSK = COL_RMLT_MASK << COL_RMLT_SHFT;

  // the block array size
  static const long BLK_RMLB_SIZE = ROW_RMLB_SIZE * COL_RMLB_SIZE;
  static const long BLK_RMCB_SIZE = ROW_RMCB_SIZE * COL_RMCB_SIZE;
  static const long BLK_RMCC_SIZE = ROW_RMCC_SIZE * COL_RMCC_SIZE;
  static const long BLK_RMCT_SIZE = ROW_RMCT_SIZE * COL_RMCT_SIZE;
  static const long BLK_RMLT_SIZE = ROW_RMLT_SIZE * COL_RMLT_SIZE;
  
  // this procedure create a new lb block
  static inline Rmatrix::t_rmlb rm_new_lb (void) {
    // allocate and clear
    Rmatrix::t_rmlb result = new t_real[BLK_RMLB_SIZE];
    for (long k = 0; k < BLK_RMLB_SIZE; k++) result[k] = 0.0;
    return result;
  }

  // this procedure copy a lb block
  static inline Rmatrix::t_rmlb rm_copy_lb (const Rmatrix::t_rmlb lb) {
    // check for nil
    if (lb == nullptr) return nullptr;
    // allocate and copy
    Rmatrix::t_rmlb result = new t_real[BLK_RMLB_SIZE];
    for (long k = 0; k < BLK_RMLB_SIZE; k++) result[k] = lb[k];
    return result;
  }

  // this procedure free a lb block
  static inline void rm_free_lb (const Rmatrix::t_rmlb lb) {
    if (lb != nullptr) delete [] lb;
  }

  // this procedure maps the lb index by row and column
  static inline long rm_indx_lb (const t_long row, const t_long col) {
    long rix = row & ROW_RMLB_MASK;
    long cix = col & COL_RMLB_MASK;
    return (rix << COL_RMLB_SHLS) + cix;
  }

  // this procedure checks if a lb block is null
  static inline bool rm_null_lb (const Rmatrix::t_rmlb lb) {
    // check for null
    if (lb == nullptr) return true;
    // loop in the block
    for (long k = 0; k < BLK_RMLB_SIZE; k++) if (lb[k] != 0.0) return false;
    return true;
  }

  // this procedure creates a new cb block
  static inline Rmatrix::t_rmcb rm_new_cb (void) {
    // allocate and clear
    Rmatrix::t_rmcb result = new Rmatrix::t_rmlb[BLK_RMCB_SIZE];
    for (long k = 0; k < BLK_RMCB_SIZE; k++) result[k] = nullptr;
    return result;
  }

  // this procedure copy a cb block
  static inline Rmatrix::t_rmcb rm_copy_cb (const Rmatrix::t_rmcb cb) {
    // check for nil
    if (cb == nullptr) return nullptr;
    // allocate and copy
    Rmatrix::t_rmcb result = new Rmatrix::t_rmlb[BLK_RMCB_SIZE];
    for (long k = 0; k < BLK_RMCB_SIZE; k++) result[k] = rm_copy_lb (cb[k]);
    return result;
  }

  // this procedure free a cb block
  static inline void rm_free_cb (const Rmatrix::t_rmcb cb) {
    if (cb == nullptr) return;
    for (long k = 0; k < BLK_RMCB_SIZE; k++) rm_free_lb (cb[k]);
    delete [] cb;
  }

  // this procedure maps the cb index by row and column
  static inline long rm_indx_cb (const t_long row, const t_long col) {
    long rix = (row >> ROW_RMCB_SHFT) & ROW_RMCB_MASK;
    long cix = (col >> COL_RMCB_SHFT) & COL_RMCB_MASK;
    return (rix << COL_RMCB_SHLS) + cix;
  }

  // this procedure checks if a cb block is null
  static inline bool rm_null_cb (const Rmatrix::t_rmcb cb) {
    // check for null
    if (cb == nullptr) return true;
    // loop in the block
    for (long k = 0; k < BLK_RMCB_SIZE; k++) {
      if (rm_null_lb (cb[k]) == false) return false;
    }
    return true;
  }

  // this procedure compress a cb block
  static inline bool rm_pres_cb (const Rmatrix::t_rmcb cb) {
    bool result = true;
    if (cb == nullptr) return result;
    for (long k = 0; k < BLK_RMCB_SIZE; k++) {
      if (rm_null_lb (cb[k]) == false) {
	result = false;
	continue;
      }
      rm_free_lb (cb[k]);
      cb[k] = nullptr;
    }
    return result;
  }

  // this procedure creates a new cc block
  static inline Rmatrix::t_rmcc rm_new_cc (void) {
    // allocate and clear
    Rmatrix::t_rmcc result = new Rmatrix::t_rmcb[BLK_RMCC_SIZE];
    for (long k = 0; k < BLK_RMCC_SIZE; k++) result[k] = nullptr;
    return result;
  }

  // this procedure copy a cc block
  static inline Rmatrix::t_rmcc rm_copy_cc (const Rmatrix::t_rmcc cc) {
    // check for nil
    if (cc == nullptr) return nullptr;
    // allocate and copy
    Rmatrix::t_rmcc result = new Rmatrix::t_rmcb[BLK_RMCC_SIZE];
    for (long k = 0; k < BLK_RMCC_SIZE; k++) result[k] = rm_copy_cb (cc[k]);
    return result;
  }

  // this procedure free a cc block
  static inline void rm_free_cc (const Rmatrix::t_rmcc cc) {
    if (cc == nullptr) return;
    for (long k = 0; k < BLK_RMCC_SIZE; k++) rm_free_cb (cc[k]);
    delete [] cc;
  }

  // this procedure maps the cc index by row and column
  static inline long rm_indx_cc (const t_long row, const t_long col) {
    long rix = (row >> ROW_RMCC_SHFT) & ROW_RMCC_MASK;
    long cix = (col >> COL_RMCC_SHFT) & COL_RMCC_MASK;
    return (rix << COL_RMCC_SHLS) + cix;
  }

  // this procedure checks if a cc block is null
  static inline bool rm_null_cc (const Rmatrix::t_rmcc cc) {
    // check for null
    if (cc == nullptr) return true;
    // loop in the block
    for (long k = 0; k < BLK_RMCC_SIZE; k++) {
      if (rm_null_cb (cc[k]) == false) return false;
    }
    return true;
  }

  // this procedure compress a cc block
  static inline bool rm_pres_cc (const Rmatrix::t_rmcc cc) {
    bool result = true;
    if (cc == nullptr) return result;
    for (long k = 0; k < BLK_RMCC_SIZE; k++) {
      if (rm_pres_cb (cc[k]) == false) {
	result = false;
	continue;
      }
      rm_free_cb (cc[k]);
      cc[k] = nullptr;
    }
    return result;
  }

  // this procedure creates a new ct block
  static inline Rmatrix::t_rmct rm_new_ct (void) {
    // allocate and clear
    Rmatrix::t_rmct result = new Rmatrix::t_rmcc[BLK_RMCT_SIZE];
    for (long k = 0; k < BLK_RMCT_SIZE; k++) result[k] = nullptr;
    return result;
  }

  // this procedure copy a ct block
  static inline Rmatrix::t_rmct rm_copy_ct (const Rmatrix::t_rmct ct) {
    // check for nil
    if (ct == nullptr) return nullptr;
    // allocate and copy
    Rmatrix::t_rmct result = new Rmatrix::t_rmcc[BLK_RMCT_SIZE];
    for (long k = 0; k < BLK_RMCT_SIZE; k++) result[k] = rm_copy_cc (ct[k]);
    return result;
  }

  // this procedure free a ct block
  static inline void rm_free_ct (const Rmatrix::t_rmct ct) {
    if (ct == nullptr) return;
    for (long k = 0; k < BLK_RMCT_SIZE; k++) rm_free_cc (ct[k]);
    delete [] ct;
  }

  // this procedure maps the ct index by row and column
  static inline long rm_indx_ct (const t_long row, const t_long col) {
    long rix = (row >> ROW_RMCT_SHFT) & ROW_RMCT_MASK;
    long cix = (col >> COL_RMCT_SHFT) & COL_RMCT_MASK;
    return (rix << COL_RMCT_SHLS) + cix;
  }

  // this procedure checks if a ct block is null
  static inline bool rm_null_ct (const Rmatrix::t_rmct ct) {
    // check for null
    if (ct == nullptr) return true;
    // loop in the block
    for (long k = 0; k < BLK_RMCT_SIZE; k++) {
      if (rm_null_cc (ct[k]) == false) return false;
    }
    return true;
  }

  // this procedure compress a ct block
  static inline bool rm_pres_ct (const Rmatrix::t_rmct ct) {
    bool result = true;
    if (ct == nullptr) return result;
    for (long k = 0; k < BLK_RMCT_SIZE; k++) {
      if (rm_pres_cc (ct[k]) == false) {
	result = false;
	continue;
      }
      rm_free_cc (ct[k]);
      ct[k] = nullptr;
    }
    return result;
  }

  // this procedure creates a new lt block
  static inline Rmatrix::t_rmlt rm_new_lt (void) {
    // allocate and clear
    Rmatrix::t_rmlt result = new Rmatrix::t_rmct[BLK_RMLT_SIZE];
    for (long k = 0; k < BLK_RMLT_SIZE; k++) result[k] = nullptr;
    return result;
  }

  // this procedure copy a lt block
  static inline Rmatrix::t_rmlt rm_copy_lt (const Rmatrix::t_rmlt lt) {
    // check for nil
    if (lt == nullptr) return nullptr;
    // allocate and copy
    Rmatrix::t_rmlt result = new Rmatrix::t_rmct[BLK_RMLT_SIZE];
    for (long k = 0; k < BLK_RMLT_SIZE; k++) result[k] = rm_copy_ct (lt[k]);
    return result;
  }

  // this procedure free a lt block
  static inline void rm_free_lt (const Rmatrix::t_rmlt lt) {
    if (lt == nullptr) return;
    for (long k = 0; k < BLK_RMLT_SIZE; k++) rm_free_ct (lt[k]);
    delete [] lt;
  }

  // this procedure maps the lt index by row and column
  static inline long rm_indx_lt (const t_long row, const t_long col) {
    long rix = (row >> ROW_RMLT_SHFT) & ROW_RMLT_MASK;
    long cix = (col >> COL_RMLT_SHFT) & COL_RMLT_MASK;
    return (rix << COL_RMLT_SHLS) + cix;
  }

  // this procedure checks if a lt block is null
  static inline bool rm_null_lt (const Rmatrix::t_rmlt lt) {
    // check for null
    if (lt == nullptr) return true;
    // loop in the block
    for (long k = 0; k < BLK_RMLT_SIZE; k++) {
      if (rm_null_ct (lt[k]) == false) return false;
    }
    return true;
  }

  // this procedure compress a lt block
  static inline bool rm_pres_lt (const Rmatrix::t_rmlt lt) {
    bool result = true;
    if (lt == nullptr) return result;
    for (long k = 0; k < BLK_RMLT_SIZE; k++) {
      if (rm_pres_ct (lt[k]) == false) {
	result = false;
	continue;
      }
      rm_free_ct (lt[k]);
      lt[k] = nullptr;
    }
    return result;
  }

  // this procedure computes the hb array row size
  static inline long rm_rsiz_hb (const t_long rsiz) {
    return ((rsiz >> ROW_RMHB_SHFT) & ROW_RMHB_MASK) + 1;
  }

  // this procedure computes the hb array column size
  static inline long rm_csiz_hb (const t_long csiz) {
    return ((csiz >> COL_RMHB_SHFT) & COL_RMHB_MASK) + 1;
  }

  // this procedure computes the hb array size
  static inline long rm_size_hb (const t_long rsiz, const t_long csiz) {
    long rsz = rm_rsiz_hb (rsiz);
    long csz = rm_csiz_hb (csiz);
    return rsz * csz;
  }

  // this procedure creates a new hb array by size
  static inline Rmatrix::t_rmhb rm_new_hb (const t_long rsiz, 
					   const t_long csiz) {
    // get the hb array size
    long hbsz = rm_size_hb (rsiz, csiz);
    // allocate and clear
    Rmatrix::t_rmhb result = new Rmatrix::t_rmlt[hbsz];
    for (long k = 0; k < hbsz; k++) result[k] = nullptr;
    return result;
  }

  // this procedure copy a hb block
  static inline Rmatrix::t_rmhb rm_copy_hb (const t_long rsiz, 
					    const t_long csiz,
					    const Rmatrix::t_rmhb hb) {
    // get hb size and check for nil
    long hbsz = rm_size_hb (rsiz, csiz);
    if ((hbsz <= 0) || (hb == nullptr)) return nullptr;
    // allocate and copy
    Rmatrix::t_rmhb result = new Rmatrix::t_rmlt[hbsz];
    for (long k = 0; k < hbsz; k++) result[k] = rm_copy_lt (hb[k]);
    return result;
  }
  
  // this procedure free a hb block
  static inline void rm_free_hb (const t_long rsiz, const t_long csiz,
				 const Rmatrix::t_rmhb hb) {
    // get hb size and check for nil
    long hbsz = rm_size_hb (rsiz, csiz);
    if ((hbsz <= 0) || (hb == nullptr)) return;
    // clean the array
    for (long k = 0; k < hbsz; k++) rm_free_lt (hb[k]);
    delete [] hb;
  }
    
  // this procedure maps the hb index by row and column
  static inline long rm_indx_hb (const t_long row,  const t_long col,
				 const t_long rsiz, const t_long csiz) {
    // get size and index
    long rix = (row   >> ROW_RMHB_SHFT) & ROW_RMHB_MASK;
    long cix = (col   >> COL_RMHB_SHFT) & COL_RMHB_MASK;
    long rsz = ((rsiz >> ROW_RMHB_SHFT) & ROW_RMHB_MASK) + 1;
    long csz = ((csiz >> COL_RMHB_SHFT) & COL_RMHB_MASK) + 1;
    // check valid index
    if ((rix < 0) || (rix >= rsz) || (cix < 0) || (cix >= csz)) {
      throw Exception ("internal-error", "out of range rmatrix hb indexes");
    }
    // map the hb index in row mode
    return (rix * csz) + cix;
  }

  // this procedure check if a hb block is null
  static inline bool rm_null_hb (const t_long rsiz, const t_long csiz,
				 const Rmatrix::t_rmhb hb) {
    // get hb size and check for nil
    long hbsz = rm_size_hb (rsiz, csiz);
    if ((hbsz <= 0) || (hb == nullptr)) return true;
    // loop in the array
    for (long k = 0; k < hbsz; k++) {
      if (rm_null_lt (hb[k]) == false) return false;
    }
    return true;
  }

  // this procedure compress the hb block
  static inline bool rm_pres_hb (const t_long rsiz, const t_long csiz,
				 const Rmatrix::t_rmhb hb) {
    // get hb size and check for nil
    bool result = true;
    long hbsz = rm_size_hb (rsiz, csiz);
    if ((hbsz <= 0) || (hb == nullptr)) return result;
    // loop in the array
    for (long k = 0; k < hbsz; k++) {
      if (rm_pres_lt (hb[k]) == false) {
	result = false;
	continue;
      }
      rm_free_lt (hb[k]);
      hb[k] = nullptr;
    }
    return result;
  }

  // this procedure find the cb block by coordinates
  static Rmatrix::t_rmcb rm_find_cb (Rmatrix::t_rmhb hb, Mutex* rmbm,
				     const t_long   row, const t_long  col,
				     const t_long  rsiz, const t_long csiz,
				     const bool    cflg) {
    // check for nil first
    if (hb == nullptr) {
      throw Exception ("internal-error", "invalid nil hb in lb find");
    }
    // map the lt block if needed
    long hbi = rm_indx_hb (row, col, rsiz, csiz);
    Rmatrix::t_rmlt& lt = hb[hbi];
    if (lt == nullptr) {
      if (cflg == false) return nullptr;
      if (rmbm != nullptr) rmbm->lock ();
      if (lt == nullptr) lt = hb[hbi] = rm_new_lt ();
      if (rmbm != nullptr) rmbm->unlock ();
    }
    // map the ct block if needed
    long lti = rm_indx_lt (row, col);
    Rmatrix::t_rmct& ct = lt[lti];
    if (ct == nullptr) {
      if (cflg == false) return nullptr;
      if (rmbm != nullptr) rmbm->lock ();
      if (ct == nullptr) ct = lt[lti] = rm_new_ct ();
      if (rmbm != nullptr) rmbm->unlock ();
    }
    // map the cc block if needed
    long cti = rm_indx_ct (row, col);
    Rmatrix::t_rmcc& cc = ct[cti];
    if (cc == nullptr) {
      if (cflg == false) return nullptr;
      if (rmbm != nullptr) rmbm->lock ();
      if (cc == nullptr) cc = ct[cti] = rm_new_cc ();
      if (rmbm != nullptr) rmbm->unlock ();
    }
    // map the cb block if needed
    long cci = rm_indx_cc (row, col);
    Rmatrix::t_rmcb& cb = cc[cci];
    if (cb == nullptr) {
      if (cflg == false) return nullptr;
      if (rmbm != nullptr) rmbm->lock ();
      if (cb == nullptr) cb = cc[cci] = rm_new_cb ();
      if (rmbm != nullptr) rmbm->unlock ();
    }
    return cb;
  }

  // this procedure find the lb block by coordinates
  static Rmatrix::t_rmlb rm_find_lb (Rmatrix::t_rmhb hb, Mutex* rmbm,
				     const t_long   row, const t_long  col,
				     const t_long  rsiz, const t_long csiz,
				     const bool    cflg) {
    // get the cb block
    Rmatrix::t_rmcb cb = rm_find_cb (hb, rmbm, row, col, rsiz, csiz, cflg);
    if (cb == nullptr) return nullptr;
    // map the lb block if needed
    long cbi = rm_indx_cb (row, col);
    Rmatrix::t_rmlb& lb = cb[cbi];
    if (lb == nullptr) {
      if (cflg == false) return nullptr;
      if (rmbm != nullptr) rmbm->lock ();
      if (lb == nullptr) lb = cb[cbi] = rm_new_lb ();
      if (rmbm != nullptr) rmbm->unlock ();
    }
    // here it is
    return lb;
  }

  // this procedure free a lb block by coordinate
  static void rm_free_lb (Rmatrix::t_rmhb hb, Mutex* rmbm,
			  const t_long   row, const t_long col, 
			  const t_long  rsiz, const t_long csiz) {
    // get the cb block
    Rmatrix::t_rmcb cb = rm_find_cb (hb, rmbm, row, col, rsiz, csiz, false);
    if (cb == nullptr) return;
    // map the lb block if needed
    long cbi = rm_indx_cb (row, col);
    if (rmbm != nullptr) rmbm->lock ();
    Rmatrix::t_rmlb lb = cb[cbi];
    rm_free_lb (lb); cb[cbi] = nullptr;
    if (rmbm != nullptr) rmbm->unlock ();
  }

  // this procedure clears a lower triangular lb block
  static void rm_cllt_lb (const Rmatrix::t_rmlb lb) {
    // check for nil
    if (lb == nullptr) return;
    // loop in blocks
    for (long lbr = 0L; lbr < ROW_RMLB_SIZE; lbr++) {
      for (long lbc = 0L; lbc < COL_RMLB_SIZE; lbc++) {
	// exit after diagonal
	if (lbc > lbr) break;
	// get the lb index
	long lbi = lbr * COL_RMLB_SIZE + lbc;
	// check for diagonal block
	if (lbc < lbr) lb[lbi] = 0.0;
      }
    }
  }

  // this procedure clears a lower triangular cb block
  static void rm_cllt_cb (const Rmatrix::t_rmcb cb) {
    // check for nil
    if (cb == nullptr) return;
    // loop in blocks
    for (long cbr = 0L; cbr < ROW_RMCB_SIZE; cbr++) {
      for (long cbc = 0L; cbc < COL_RMCB_SIZE; cbc++) {
	// exit after diagonal
	if (cbc > cbr) break;
	// get the cb index
	long cbi = cbr * COL_RMCB_SIZE + cbc;
	// check for diagonal block
	if (cbr == cbc) rm_cllt_lb (cb[cbi]);
	else {
	  rm_free_lb (cb[cbi]);
	  cb[cbi] = nullptr;
	}
      }
    }
  }

  // this procedure clears a lower triangular cc block
  static void rm_cllt_cc (const Rmatrix::t_rmcc cc) {
    // check for nil
    if (cc == nullptr) return;
    // loop in blocks
    for (long ccr = 0L; ccr < ROW_RMCC_SIZE; ccr++) {
      for (long ccc = 0L; ccc < COL_RMCC_SIZE; ccc++) {
	// exit after diagonal
	if (ccc > ccr) break;
	// get the cc index
	long cci = ccr * COL_RMCC_SIZE + ccc;
	// check for diagonal block
	if (ccr == ccc) rm_cllt_cb (cc[cci]);
	else {
	  rm_free_cb (cc[cci]);
	  cc[cci] = nullptr;
	}
      }
    }
  }

  // this procedure clears a lower triangular lt block
  static void rm_cllt_ct (const Rmatrix::t_rmct ct) {
    // check for nil
    if (ct == nullptr) return;
    // loop in blocks
    for (long ctr = 0L; ctr < ROW_RMCT_SIZE; ctr++) {
      for (long ctc = 0L; ctc < COL_RMCT_SIZE; ctc++) {
	// exit after diagonal
	if (ctc > ctr) break;
	// get the ct index
	long cti = ctr * COL_RMCT_SIZE + ctc;
	// check for diagonal block
	if (ctr == ctc) rm_cllt_cc (ct[cti]);
	else {
	  rm_free_cc (ct[cti]);
	  ct[cti] = nullptr;
	}
      }
    }
  }

  // this procedure clears a lower triangular lt block
  static void rm_cllt_lt (const Rmatrix::t_rmlt lt) {
    // check for nil
    if (lt == nullptr) return;
    // loop in blocks
    for (long ltr = 0L; ltr < ROW_RMLT_SIZE; ltr++) {
      for (long ltc = 0L; ltc < COL_RMLT_SIZE; ltc++) {
	// exit after diagonal
	if (ltc > ltr) break;
	// get the lt index
	long lti = ltr * COL_RMLT_SIZE + ltc;
	// check for diagonal block
	if (ltr == ltc) rm_cllt_ct (lt[lti]);
	else {
	  rm_free_ct (lt[lti]);
	  lt[lti] = nullptr;
	}
      }
    }
  }

  // this procedure clear a lower-triangular matrix
  static void rm_cllt_hb (const t_long  rsiz, const t_long csiz,
			  Rmatrix::t_rmhb hb) {
    // get the hb size
    long rsz = rm_rsiz_hb (rsiz);
    long csz = rm_csiz_hb (csiz);
    // loop in the hb rows
    for (long hbr = 0L; hbr < rsz;hbr++) {
      for (long hbc = 0L; hbc < csz; hbc++) {
	// exit after diagonal
	if (hbc > hbr) break;
	// get hb index
	long hbi = hbr * csz + hbc;
	// check for diagonal block
	if (hbr == hbc) rm_cllt_lt (hb[hbi]);
	else {
	  rm_free_lt (hb[hbi]);
	  hb[hbi] = nullptr;
	}
      }
    }
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // generate a square random matrix by size

  Rmatrix* Rmatrix::random (const t_long size, const t_real rmin,
			    const t_real rmax) {
    // create a matrix by size
    Rmatrix* result = new Rmatrix (size);
    // fill the matrix
    Algebra::random (*result, rmin, rmax);
    // here it is
    return result;
  }

  // generate a square random matrix by size

  Rmatrix* Rmatrix::random (const t_long size, const t_real rmin,
			    const t_real rmax, const bool   ddom) {
    // create a matrix by size
    Rmatrix* result = new Rmatrix (size);
    // fill the matrix
    Algebra::random (*result, rmin, rmax);
    // force it dominant if requested
    if (ddom == true) Algebra::toddom (*result, (rmax + rmin) / 2.0);
    // here it is
    return result;
  }

  // generate a square sparse matrix by size

  Rmatrix* Rmatrix::sparse (const t_long size, const t_real rmin,
			    const t_real rmax, const t_long nzsz) {
    // create a matrix by size
    Rmatrix* result = new Rmatrix (size);
    // fill the matrix
    Algebra::sparse (*result, rmin, rmax, nzsz);
    // here it is
    return result;
  }

  // add a matrix with another one

  Rmatrix operator + (const Rmatrix& mx, const Rmatrix& my) {
    mx.rdlock ();
    my.rdlock ();
    try {
      // create a result matrix
      Rmatrix mr (mx.getrsiz (), my.getcsiz ());
      // add the matrices
      Algebra::add (mr, mx, my);
      // unlock and return
      mx.unlock ();
      my.unlock ();
      return mr;
    } catch (...) {
      mx.unlock ();
      my.unlock ();
      throw;
    }
  }

  // substract a matrix with another one

  Rmatrix operator - (const Rmatrix& mx, const Rmatrix& my) {
    mx.rdlock ();
    my.rdlock ();
    try {
      // create a result matrix
      Rmatrix mr (mx.getrsiz (), my.getcsiz ());
      // substract the matrices
      Algebra::sub (mr, mx, my);
      // unlock and return
      mx.unlock ();
      my.unlock ();
      return mr;
    } catch (...) {
      mx.unlock ();
      my.unlock ();
      throw;
    }
  }

  // multiply a matrix with a vector

  Rvector operator * (const Rmatrix& m, const Rvector& x) {
    m.rdlock ();
    x.rdlock ();
    try {
      // extract operating size
      t_long rows = m.getrsiz ();
      // create result vector
      Rvector r (rows);
      // perform the operation
      m.mul (r, x, 1.0);
      // unlock and return
      m.unlock ();
      x.unlock ();
      return r;
    } catch (...) {
      m.unlock ();
      x.unlock ();
      throw;
    }
  }

  // multiply two matrices

  Rmatrix operator * (const Rmatrix& mx, const Rmatrix& my) {
    mx.rdlock ();
    my.rdlock ();
    try {
      // create a result matrix
      Rmatrix mr (mx.getrsiz (), my.getcsiz ());
      // muliply the matrices
      Algebra::mul (mr, mx, my);
      // unlock and return
      mx.unlock ();
      my.unlock ();
      return mr;
    } catch (...) {
      mx.unlock ();
      my.unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default matrix

  Rmatrix::Rmatrix (void) {
    p_rmhb = nullptr;
    p_rmbm = nullptr;
#ifdef _OPENMP
    p_rmbm = new Mutex;
#endif
  }

  // create a square matrix by size

  Rmatrix::Rmatrix (const t_long size) : Rmi (size) {
    p_rmhb = rm_new_hb (d_rsiz, d_csiz);
    p_rmbm = nullptr;
#ifdef _OPENMP
    p_rmbm = new Mutex;
#endif
  }

  // create a matrix by size

  Rmatrix::Rmatrix (const t_long rsiz, const t_long csiz) : Rmi (rsiz, csiz) {
    p_rmhb = rm_new_hb (d_rsiz, d_csiz);
    p_rmbm = nullptr;
#ifdef _OPENMP
    p_rmbm = new Mutex;
#endif
  }

  // generate a matrix by vector multiply

  Rmatrix::Rmatrix (const Rvi& u, const Rvi& v) {
    // reset matrix
    p_rmhb = nullptr;
    p_rmbm = nullptr;
#ifdef _OPENMP
    p_rmbm = new Mutex;
#endif
    // get the target size
    t_long rsiz = u.getsize ();
    t_long csiz = v.getsize ();
    // resize the matrix
    resize (rsiz, csiz);
    // update the matrix
    Algebra::mul (*this, u, v);
  }

  // copy construct this matrix

  Rmatrix::Rmatrix (const Rmatrix& that) {
    // reset matrix
    p_rmhb = nullptr;
    p_rmbm = nullptr;
#ifdef _OPENMP
    p_rmbm = new Mutex;
#endif
    that.rdlock ();
    try {
      // copy base
      Rmi::operator = (that);
      // copy locally
      p_rmhb = rm_copy_hb (d_rsiz, d_csiz, that.p_rmhb);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
	
  // destroy this matrix

  Rmatrix::~Rmatrix (void) {
    rm_free_hb (d_rsiz, d_csiz, p_rmhb);
    delete p_rmbm;
  }

  // assign a matrix to this one

  Rmatrix& Rmatrix::operator = (const Rmatrix& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // delete the old matrix
      rm_free_hb (d_rsiz, d_csiz, p_rmhb);
      p_rmhb = nullptr;
      // assign base matrix
      Rmi::operator = (that);
      // assign locally
      p_rmhb = rm_copy_hb (d_rsiz, d_csiz, that.p_rmhb);
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  

  // return the class name

  String Rmatrix::repr (void) const {
    return "Rmatrix";
  }

  // return a clone of this object

  Object* Rmatrix::clone (void) const {
    return new Rmatrix (*this);
  }

  // return the serial did

  t_word Rmatrix::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Rmatrix::getsid (void) const {
    return SRL_RMTX_SID;
  }

  // get the matrix row barrier

  t_long Rmatrix::getrowb (void) const {
    rdlock ();
    try {
      t_long result = ROW_RMLB_SIZE;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the matrix column barrier

  t_long Rmatrix::getcolb (void) const {
    rdlock ();
    try {
      t_long result = COL_RMLB_SIZE;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the matrix is null

  bool Rmatrix::isnil (void) const {
    wrlock ();
    try {
      bool result = rm_null_hb (d_rsiz, d_csiz, p_rmhb);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this matrix

  void Rmatrix::reset (void) {
    wrlock ();
    try {
      // clean the matrix
      rm_free_hb (d_rsiz, d_csiz, p_rmhb);
      // reset members
      d_rsiz = 0LL;
      d_csiz = 0LL;
      p_rmhb = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this matrix

  void Rmatrix::clear (void) {
    wrlock ();
    try {
      // destroy all hb array
      rm_free_hb (d_rsiz, d_csiz, p_rmhb);
      // rebuild fresh array
      p_rmhb = rm_new_hb (d_rsiz, d_csiz);;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear the matrix lower triangular

  void Rmatrix::clt (void) {
    wrlock ();
    try {
      rm_cllt_hb (d_rsiz, d_csiz, p_rmhb);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this matrix

  void Rmatrix::resize (const t_long rsiz, const t_long csiz) {
    wrlock ();
    try {
      // check for valid size
      if ((rsiz < 0) || (csiz < 0)) {
	throw Exception ("rmatrix-error", "invalid resize parameters");
      }
      // get the current hb block size
      long cbsz = rm_size_hb (d_rsiz, d_csiz);
      // get the new hb block size
      long nbsz = rm_size_hb (rsiz, csiz);
      // check for lower
      if (nbsz < cbsz) {
	t_rmhb rmhb = rm_new_hb (rsiz, csiz);
	for (long k = 0; k < nbsz; k++) rmhb[k] = p_rmhb[k];
	for (long k = nbsz; k < cbsz; k++) rm_free_lt (p_rmhb[k]);
	delete [] p_rmhb;
	p_rmhb = rmhb;
      }
      // check for higher
      if (nbsz > cbsz) {
	t_rmhb rmhb = rm_new_hb (rsiz, csiz);
	for (long k = 0; k < cbsz; k++) rmhb[k] = p_rmhb[k];
	delete [] p_rmhb;
	p_rmhb = rmhb;
      }
      // check for nil with non null size
      if ((p_rmhb == nullptr) && (rsiz > 0) && (csiz > 0)) {
	p_rmhb = rm_new_hb (rsiz, csiz);
      }
      // update size and return
      d_rsiz = rsiz;
      d_csiz = csiz;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // copy a matrix row into a vector

  Rvi* Rmatrix::cpr (const t_long row) const {
    rdlock ();
    Rvi* result = nullptr;
    try {
      // prepare result vector
      result = (d_csiz == 0LL) ? nullptr : new Rvector (d_csiz);
      // copy if not nil
      if (result != nullptr) Algebra::cpr (*result, *this, row);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // copy a matrix column into a vector

  Rvi* Rmatrix::cpc (const t_long col) const {
    rdlock ();
    Rvi* result = nullptr;
    try {
      // prepare result vector
      result = (d_rsiz == 0LL) ? nullptr : new Rvector (d_rsiz);
      // copy if not nil
      if (result != nullptr) Algebra::cpc (*result, *this, col);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // compress the matrix if possible

  bool Rmatrix::compress (void) {
    wrlock ();
    try {
      bool result = rm_pres_hb (d_rsiz, d_csiz, p_rmhb);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a zero identical matrix

  Rmi* Rmatrix::zeros (void) const {
    rdlock ();
    try {
      Rmi* result = new Rmatrix (d_rsiz, d_csiz);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a new matrix iterator

  Iterator* Rmatrix::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new Rmatrixit (this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // permutate this matrix

  Rmi* Rmatrix::permutate (const Cpi& p) const {
    rdlock ();
    Rmi* result = nullptr;
    try {
      // create a result matrix
      result = new Rmatrix (d_rsiz, d_csiz);
      // permutate this matrix
      Algebra::permutate (*result, *this, p);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // reverse permutate this matrix

  Rmi* Rmatrix::reverse (const Cpi& p) const {
    rdlock ();
    Rmi* result = nullptr;
    try {
      // create a result matrix
      result = new Rmatrix (d_rsiz, d_csiz);
      // permutate this vector
      Algebra::reverse (*result, *this, p);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - no lock section                                                       -
  // -------------------------------------------------------------------------

  // no lock - clear a matrix zone

  void Rmatrix::nlclear (const t_long row, const t_long col,
			 const t_long rsz, const t_long csz) {
    // check for lb block aligned
    if (((row & ROW_RMLB_MASK) == 0LL) && ((col & COL_RMLB_MASK) == 0LL) &&
	((rsz & ROW_RMLB_MASK) == 0LL) && ((csz & COL_RMLB_MASK) == 0LL)) {
      t_long rmax = row + rsz;
      t_long cmax = col + csz;
      for (t_long i = row; i < rmax; i+= ROW_RMLB_SIZE) {
	for (t_long j = col; j < cmax; j+= COL_RMLB_SIZE) {
	  rm_free_lb (p_rmhb, p_rmbm, i, j, d_rsiz, d_csiz);
	}
      }
      return;
    }
    // default matrix clear
    Rmi::nlclear (row, col, rsz, csz);
  }

  // no lock - set a matrix by position

  void Rmatrix::nlset (const t_long row, const t_long col, const t_real val) {
    // set the block creation flag
    bool cflg = (val == 0.0) ? false : true;
    // get the lb block by position
    Rmatrix::t_rmlb lb = rm_find_lb (p_rmhb, p_rmbm, 
				     row, col, d_rsiz, d_csiz, cflg);
    if ((lb == nullptr) && (cflg == false)) return;
    if ((lb == nullptr) && (cflg == true)) {
      throw Exception ("internal-error", "invalid nil lb block in set");
    }
    // map the lb index and set
    long lbi = rm_indx_lb (row, col);
    lb[lbi] = val;
  }

  // set a matrix by position

  t_real Rmatrix::nlget (const t_long row, const t_long col) const {
    // get the lb block by position
    Rmatrix::t_rmlb lb = rm_find_lb (p_rmhb, nullptr, 
				     row, col, d_rsiz, d_csiz, false);
    if (lb == nullptr) return 0.0;
    // map the lb index and get value
    long lbi = rm_indx_lb (row, col);
    t_real result = lb[lbi];
    // done
    return result;
  }
  
  // no lock - perform a givens matrix update
  
  void Rmatrix::nlgivens (const t_long i, const t_long j, 
			  const t_real c, const t_real s, const bool pflg) {
    // select partial or full update
    t_long l = pflg ? j : 0LL;
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (t_long k = l; k < d_csiz; k++) {
      // get the lb block by position
      Rmatrix::t_rmlb bjk = rm_find_lb (p_rmhb, p_rmbm,
					j, k, d_rsiz, d_csiz, false);
      Rmatrix::t_rmlb bik = rm_find_lb (p_rmhb, p_rmbm,
					i, k, d_rsiz, d_csiz, false);
      // get the lb indexes
      long ijk = rm_indx_lb (j, k);
      long iik = rm_indx_lb (i, k);
      // get the matrix elements
      t_real mjk = (bjk == nullptr) ? 0.0 : bjk[ijk];
      t_real mik = (bik == nullptr) ? 0.0 : bik[iik];
      // get updated value
      t_real ujk = ( c * mjk) + (s * mik);
      t_real uik = (-s * mjk) + (c * mik);
      // update at row j
      if (bjk == nullptr) {
	if (ujk != 0.0) {
	  bjk = rm_find_lb (p_rmhb, p_rmbm, j, k, d_rsiz, d_csiz, true);
	  bjk[ijk] = ujk;
	}
      } else {
	bjk[ijk] = ujk;
      }
      // update at row i
      if (bik == nullptr) {
	if (uik != 0.0) {
	  bik = rm_find_lb (p_rmhb, p_rmbm, i, k, d_rsiz, d_csiz, true);
	  bik[iik] = uik;
	}
      } else {
	bik[iik] = uik;
      }
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Rmatrix::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Rmatrix;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Rmatrix (size);
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* zobj = argv->get (0);
      Object* cobj = argv->get (1);
      // check for integers
      Integer* ri = dynamic_cast <Integer*> (zobj);
      Integer* ci = dynamic_cast <Integer*> (cobj);
      if ((ri != nullptr) && (ci != nullptr)) {
	t_long rsiz = ri->tolong ();
	t_long csiz = ci->tolong ();
	return new Rmatrix (rsiz, csiz);
      }
      // check for vectors
      Rvi* u = dynamic_cast <Rvi*> (zobj);
      Rvi* v = dynamic_cast <Rvi*> (cobj);
      if ((u != nullptr) && (v != nullptr)) return new Rmatrix (*u, *v);
      // invalid object
      throw Exception ("type-error", "invalid objects as matrix arguments");
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with rmatrix object");
  }

  // return true if the given quark is defined

  bool Rmatrix::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      bool result = hflg ? Iterable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Rmi::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this matrix with another object

  Object* Rmatrix::oper (t_oper type, Object* object) {
    Rvector* vobj = dynamic_cast <Rvector*> (object);
    Rmatrix* mobj = dynamic_cast <Rmatrix*> (object);
    switch (type) {
    case Object::OPER_ADD:
      if (mobj != nullptr) return new Rmatrix (*this + *mobj);
      break;
    case Object::OPER_SUB:
      if (mobj != nullptr) return new Rmatrix (*this - *mobj);
      break;
    case Object::OPER_MUL:
      if (vobj != nullptr) return new Rvector (*this * *vobj);
      if (mobj != nullptr) return new Rmatrix (*this * *mobj);
      break;
    default:
      throw Exception ("matrix-error", "invalid operator with rmatrix",
		       Object::repr (object));
      break;
    }
    throw Exception ("type-error", "invalid operand with rmatrix",
		     Object::repr (object));
  }

  // apply this object with a set of arguments and a quark
  
  Object* Rmatrix::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // check the iterable method
    if (Iterable::isquark (quark, true) == true) {
      return Iterable::apply (zobj, nset, quark, argv);
    }
    // call the rmi method
    return Rmi::apply (zobj, nset, quark, argv);
  }

  // -------------------------------------------------------------------------
  // - iterator section                                                      -
  // -------------------------------------------------------------------------

  // create a new matrix iterator
  
  Rmatrixit::Rmatrixit (Rmatrix* mobj) {
    d_rmit = RMIT_SEQ;
    Object::iref (p_mobj = mobj);
    d_hbsz = (p_mobj == nullptr)
      ? 0
      : rm_size_hb (p_mobj->d_rsiz, p_mobj->d_csiz);
    begin ();
  }
  
  // create a new matrix iterator by type
  
  Rmatrixit::Rmatrixit (Rmatrix* mobj, const t_rmit rmit) {
    d_rmit = rmit;
    Object::iref (p_mobj = mobj);
    d_hbsz = (p_mobj == nullptr)
      ? 0
      : rm_size_hb (p_mobj->d_rsiz, p_mobj->d_csiz);
    begin ();
  }

  // destroy this matrix iterator
  
  Rmatrixit::~Rmatrixit (void) {
    Object::dref (p_mobj);
  }

  // return the class name

  String Rmatrixit::repr (void) const {
    return "Rmatrixit";
  }

  // reset the iterator to the begining

  void Rmatrixit::begin (void) {
    wrlock ();
    try {
      // reset indexes
      nlmove (0LL, 0LL);
      // move to the next position if needed
      if (nlgval () == 0.0) next ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
 
  // reset the iterator to the end

  void Rmatrixit::end (void) {
    wrlock ();
    try {
      // reset indexes
      if (p_mobj == nullptr) {
	unlock ();
	return;
      }
      nlmove (p_mobj->d_rsiz-1LL, p_mobj->d_csiz-1LL);
      // move the last end if needed
      if (nlgval () == 0.0) prev ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the object at the current position

  Object* Rmatrixit::getobj (void) const {
    rdlock ();
    try {
      if (p_mobj != nullptr) p_mobj->rdlock ();
      Object* result = new Rmd (nlgrow (), nlgcol (), nlgval ());
      if (p_mobj != nullptr) p_mobj->unlock ();
      unlock ();
      return result;
    } catch (...) {
      if (p_mobj != nullptr) p_mobj->unlock ();
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - iterator no lock section                                              -
  // -------------------------------------------------------------------------

  // no lock - move the iterator to the next position
  
  void Rmatrixit::nlnext (void) {
    switch (d_rmit) {
    case RMIT_SEQ:
      mvnext ();
      break;
    case RMIT_ROW:
      mrnext ();
      break;
    case RMIT_COL:
      mcnext ();
      break;
    }
  }

  // no lock - move the iterator to the previous position

  void Rmatrixit::nlprev (void) {
    // move to the previous position
    switch (d_rmit) {
    case RMIT_SEQ:
      mvprev ();
      break;
    case RMIT_ROW:
      mrprev ();
      break;
    case RMIT_COL:
      mcprev ();
      break;
    }
  }

  // no lock - return true if the iterator is at the end

  bool Rmatrixit::nlend (void) const {
    return (p_mobj == nullptr) || (d_bihb >= d_hbsz);
  }
  
  // no lock - move the iterator to a matrix point

  void Rmatrixit::nlmove (const t_long row, const t_long col) {
    // check for object valid coordinates
    if ((p_mobj == nullptr) || (row < 0LL) || (col < 0LL)) return;
    // update the coordinates
    d_mrow = row;
    d_mcol = col;
    // update the iterator coordinates
    d_bihb = rm_indx_hb (row, col, p_mobj->d_rsiz, p_mobj->d_csiz);
    d_bilt = rm_indx_lt (row, col);
    d_bict = rm_indx_ct (row, col);
    d_bicc = rm_indx_cc (row, col);
    d_bicb = rm_indx_cb (row, col);
    d_bilb = rm_indx_lb (row, col);
    // reset iterators barriers
    d_hbib = 0L;
    d_ltib = 0L; d_ltcm = 0L; d_ltrb = 0L;
    d_ctib = 0L; d_ctcm = 0L; d_ctrb = 0L;
    d_ccib = 0L; d_cccm = 0L; d_ccrb = 0L;
    d_cbib = 0L; d_cbcm = 0L; d_cbrb = 0L;
    d_lbib = 0L; d_lbcm = 0L; d_lbrb = 0L;
    // preset iterator barriers in row mode
    if (d_rmit == RMIT_ROW) {
      // set the reset barrier
      d_ltrb = rm_indx_lt (0LL, d_mcol);
      d_ctrb = rm_indx_ct (0LL, d_mcol);
      d_ccrb = rm_indx_cc (0LL, d_mcol);
      d_cbrb = rm_indx_cb (0LL, d_mcol);
      d_lbrb = rm_indx_lb (0LL, d_mcol);
    }
    // preset iterator barriers in column mode
    if (d_rmit == RMIT_COL) {
      // get hb column size
      long csiz = rm_csiz_hb (p_mobj->d_csiz);
      // update the iterator barriers
      d_hbib = (csiz == 0L) ? 0L : ((d_bihb / csiz) + 1L) * csiz;
      d_ltib = rm_indx_lt (d_mrow, p_mobj->d_csiz & COL_RMLT_SMSK);
      d_ctib = rm_indx_ct (d_mrow, p_mobj->d_csiz & COL_RMCT_SMSK);
      d_ccib = rm_indx_cc (d_mrow, p_mobj->d_csiz & COL_RMCC_SMSK);
      d_cbib = rm_indx_cb (d_mrow, p_mobj->d_csiz & COL_RMCB_SMSK);
      d_lbib = rm_indx_lb (d_mrow, p_mobj->d_csiz & COL_RMLB_SMSK);
      d_ltcm = ((d_ltib / COL_RMLT_SIZE) + 1L) * COL_RMLT_SIZE;
      d_ctcm = ((d_ctib / COL_RMCT_SIZE) + 1L) * COL_RMCT_SIZE;
      d_cccm = ((d_ccib / COL_RMCC_SIZE) + 1L) * COL_RMCC_SIZE;
      d_cbcm = ((d_cbib / COL_RMCB_SIZE) + 1L) * COL_RMCB_SIZE;
      d_lbcm = ((d_lbib / COL_RMLB_SIZE) + 1L) * COL_RMLB_SIZE;
      // adjust barriers at end
      d_ltib = ((d_ltib%COL_RMLT_SIZE)==0L) ? d_ltib+COL_RMLT_SIZE : d_ltib+1L;
      d_ctib = ((d_ctib%COL_RMCT_SIZE)==0L) ? d_ctib+COL_RMCT_SIZE : d_ctib+1L;
      d_ccib = ((d_ccib%COL_RMCC_SIZE)==0L) ? d_ccib+COL_RMCC_SIZE : d_ccib+1L;
      d_cbib = ((d_cbib%COL_RMCB_SIZE)==0L) ? d_cbib+COL_RMCB_SIZE : d_cbib+1L;
      d_lbib = ((d_lbib%COL_RMLB_SIZE)==0L) ? d_lbib+COL_RMLB_SIZE : d_lbib+1L;
      // set the reset barrier
      d_ltrb = rm_indx_lt (d_mrow, 0LL);
      d_ctrb = rm_indx_ct (d_mrow, 0LL);
      d_ccrb = rm_indx_cc (d_mrow, 0LL);
      d_cbrb = rm_indx_cb (d_mrow, 0LL);
      d_lbrb = rm_indx_lb (d_mrow, 0LL);
    }
  }

  // no lock - move the iterator to the next position
  
  void Rmatrixit::mvnext (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // move to the next position
    d_bilb++;
    // loop in the hb block
    for (long bihb = d_bihb; bihb < d_hbsz; bihb++) {
      // get the rmlt array
      Rmatrix::t_rmlt rmlt = p_mobj->p_rmhb[bihb];
      // check for a valid block and reset index
      if (rmlt == nullptr) {
	d_bilt = 0;
	d_bict = 0;
	d_bicc = 0;
	d_bicb = 0;
	d_bilb = 0;
	continue;
      }
      // loop in the lt block
      for (long bilt = d_bilt; bilt < BLK_RMLT_SIZE; bilt++) {
	// get the rmct array
	Rmatrix::t_rmct rmct = rmlt[bilt];
	// check for a valid block and reset index
	if (rmct == nullptr) {
	  d_bict = 0;
	  d_bicc = 0;
	  d_bicb = 0;
	  d_bilb = 0;
	  continue;
	}
	// loop in the ct block
	for (long bict = d_bict; bict < BLK_RMCT_SIZE; bict++) {
	  // get the rmcc array
	  Rmatrix::t_rmcc rmcc = rmct[bict];
	  // check for a valid block and reset index
	  if (rmcc == nullptr) {
	    d_bicc = 0;
	    d_bicb = 0;
	    d_bilb = 0;
	    continue;
	  }
	  // loop in the cc block
	  for (long bicc = d_bicc; bicc < BLK_RMCC_SIZE; bicc++) {
	    // get the rmcb array
	    Rmatrix::t_rmcb rmcb = rmcc[bicc];
	    // check for a valid block and reset index
	    if (rmcb == nullptr) {
	      d_bicb = 0;
	      d_bilb = 0;
	      continue;
	    }
	    // loop in the cb block
	    for (long bicb = d_bicb; bicb < BLK_RMCB_SIZE; bicb++) {
	      // get the rmlb array
	      Rmatrix::t_rmlb rmlb = rmcb[bicb];
	      // check for a valid block and reset index
	      if (rmlb == nullptr) {
		d_bilb = 0;
		continue;
	      }
	      // loop in the lb block at next position
	      for (long bilb = d_bilb; bilb < BLK_RMLB_SIZE; bilb++) {
		// get the value and check
		t_real val = rmlb[bilb];
		if (val != 0.0) {
		  d_bihb = bihb;
		  d_bilt = bilt;
		  d_bict = bict;
		  d_bicc = bicc;
		  d_bicb = bicb;
		  d_bilb = bilb;
		  return;
		}
	      }
	      d_bilb = 0;
	    }
	    d_bilb = 0;
	    d_bicb = 0;
	  }
	  d_bilb = 0;
	  d_bicb = 0;
	  d_bicc = 0;
	}
	d_bilb = 0;
	d_bicb = 0;
	d_bicc = 0;
	d_bict = 0;
      }
      d_bilb = 0;
      d_bicb = 0;
      d_bicc = 0;
      d_bict = 0;
      d_bilt = 0;
    }
    // here we are at the end
    d_bihb = d_hbsz;
    d_bilt = BLK_RMLT_SIZE;
    d_bict = BLK_RMCT_SIZE;
    d_bicc = BLK_RMCC_SIZE;
    d_bicb = BLK_RMCB_SIZE;
    d_bilb = BLK_RMLB_SIZE;
  }

  // no lock - move the iterator to the next row position

  void Rmatrixit::mrnext (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // move to the next row position
    d_bilb += COL_RMLB_SIZE;
    // get the array column size
    long csiz = rm_csiz_hb (p_mobj->d_csiz);
    if (csiz == 0) return;
    // loop in the hb block by columns
    for (long bihb = d_bihb; bihb < d_hbsz; bihb+=csiz) {
      // get the rmlt array
      Rmatrix::t_rmlt rmlt = p_mobj->p_rmhb[bihb];
      // check for a valid block and reset index
      if (rmlt == nullptr) {
	d_bilt = rm_indx_lt (0LL, d_mcol);
	d_bict = rm_indx_ct (0LL, d_mcol);
	d_bicc = rm_indx_cc (0LL, d_mcol);
	d_bicb = rm_indx_cb (0LL, d_mcol);
	d_bilb = rm_indx_lb (0LL, d_mcol);
	continue;
      }
      // loop in the lt block
      for (long bilt = d_bilt; bilt < BLK_RMLT_SIZE;
	   bilt+=COL_RMLT_SIZE) {
	// get the rmct array
	Rmatrix::t_rmct rmct = rmlt[bilt];
	// check for a valid block and reset index
	if (rmct == nullptr) {
	  d_bict = rm_indx_ct (0LL, d_mcol);
	  d_bicc = rm_indx_cc (0LL, d_mcol);
	  d_bicb = rm_indx_cb (0LL, d_mcol);
	  d_bilb = rm_indx_lb (0LL, d_mcol);
	  continue;
	}
	// loop in the ct block
	for (long bict = d_bict; bict < BLK_RMCT_SIZE; 
	     bict+=COL_RMCT_SIZE) {
	  // get the rmcc array
	  Rmatrix::t_rmcc rmcc = rmct[bict];
	  // check for a valid block and reset index
	  if (rmcc == nullptr) {
	    d_bicc = rm_indx_cc (0LL, d_mcol);
	    d_bicb = rm_indx_cb (0LL, d_mcol);
	    d_bilb = rm_indx_lb (0LL, d_mcol);
	    continue;
	  }
	  // loop in the cc block
	  for (long bicc = d_bicc; bicc < BLK_RMCC_SIZE; 
	       bicc+=COL_RMCC_SIZE) {
	    // get the rmcb array
	    Rmatrix::t_rmcb rmcb = rmcc[bicc];
	    // check for a valid block and reset index
	    if (rmcb == nullptr) {
	      d_bicb = rm_indx_cb (0LL, d_mcol);
	      d_bilb = rm_indx_lb (0LL, d_mcol);
	      continue;
	    }
	    // loop in the cb block
	    for (long bicb = d_bicb; bicb < BLK_RMCB_SIZE;
		 bicb+=COL_RMCB_SIZE) {
	      // get the rmlb array
	      Rmatrix::t_rmlb rmlb = rmcb[bicb];
	      // check for a valid block and reset index
	      if (rmlb == nullptr) {
		d_bilb = d_lbrb;
		continue;
	      }
	      // loop in the lb block at next position
	      for (long bilb = d_bilb; bilb < BLK_RMLB_SIZE; 
		   bilb+=COL_RMLB_SIZE) {
		// get the value and check
		t_real val = rmlb[bilb];
		if (val != 0.0) {
		  d_bihb = bihb;
		  d_bilt = bilt;
		  d_bict = bict;
		  d_bicc = bicc;
		  d_bicb = bicb;
		  d_bilb = bilb;
		  return;
		}
	      }
	      d_bilb = d_lbrb;
	    }
	    d_bilb = d_lbrb;
	    d_bicb = d_cbrb;
	  }
	  d_bilb = d_lbrb;
	  d_bicb = d_cbrb;
	  d_bicc = d_ccrb;
	}
	d_bilb = d_lbrb;
	d_bicb = d_cbrb;
	d_bicc = d_ccrb;
	d_bict = d_ctrb;
      }
      d_bilb = d_lbrb;
      d_bicb = d_cbrb;
      d_bicc = d_ccrb;
      d_bilt = d_ltrb;
    }
    // here we are at the end
    d_bihb = d_hbsz;
    d_bilt = BLK_RMLT_SIZE;
    d_bict = BLK_RMCT_SIZE;
    d_bicc = BLK_RMCC_SIZE;
    d_bicb = BLK_RMCB_SIZE;
    d_bilb = BLK_RMLB_SIZE;
  }

  // no lock - move the iterator to the next column position

  void Rmatrixit::mcnext (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // move to the next position
    d_bilb++;
    // loop in the hb block by columns
    for (long bihb = d_bihb; bihb < d_hbib; bihb++) {
      // get the rmlt array
      Rmatrix::t_rmlt rmlt = p_mobj->p_rmhb[bihb];
      // check for a valid block and reset index
      if (rmlt == nullptr) {
	d_bilt = d_ltrb;
	d_bict = d_ctrb;
	d_bicc = d_ccrb;
	d_bicb = d_cbrb;
	d_bilb = d_lbrb;
	continue;
      }
      // loop in the lt block
      bool hbmf = (bihb == (d_hbib - 1L));
      long ltmx = hbmf ? d_ltib : d_ltcm;
      for (long bilt = d_bilt; bilt < ltmx; bilt++) {
	// get the rmct array
	Rmatrix::t_rmct rmct = rmlt[bilt];
	// check for a valid block and reset index
	if (rmct == nullptr) {
	  d_bict = d_ctrb;
	  d_bicc = d_ccrb;
	  d_bicb = d_cbrb;
	  d_bilb = d_lbrb;
	  continue;
	}
	// loop in the ct block
	bool ltmf = hbmf && (bilt == (d_ltib - 1L));
	long ctmx = ltmf ? d_ctib : d_ctcm;
	for (long bict = d_bict; bict < ctmx; bict++) {
	  // get the rmcc array
	  Rmatrix::t_rmcc rmcc = rmct[bict];
	  // check for a valid block and reset index
	  if (rmcc == nullptr) {
	    d_bicc = d_ccrb;
	    d_bicb = d_cbrb;
	    d_bilb = d_lbrb;
	    continue;
	  }
	  // loop in the cc block
	  bool ctmf = ltmf && (bict == (d_ctib - 1L));
	  long ccmx = ctmf ? d_ccib : d_cccm;
	  for (long bicc = d_bicc; bicc < ccmx; bicc++) {
	    // get the rmcb array
	    Rmatrix::t_rmcb rmcb = rmcc[bicc];
	    // check for a valid block and reset index
	    if (rmcb == nullptr) {
	      d_bicb = d_cbrb;
	      d_bilb = d_lbrb;
	      continue;
	    }
	    // loop in the cb block
	    bool ccmf = ctmf && (bicc == (d_ccib - 1L));
	    long cbmx = ccmf ? d_cbib : d_cbcm;
	    for (long bicb = d_bicb; bicb < cbmx; bicb++) {
	      // get the rmlb array
	      Rmatrix::t_rmlb rmlb = rmcb[bicb];
	      // check for a valid block and reset index
	      if (rmlb == nullptr) {
		d_bilb = d_lbrb;
		continue;
	      }
	      // loop in the lb block at next position
	      bool cbmf = ccmf && (bicb == (d_cbib - 1L));
	      long lbmx = cbmf ? d_lbib : d_lbcm;
	      for (long bilb = d_bilb; bilb < lbmx; bilb++) {
		// get the value and check
		t_real val = rmlb[bilb];
		if (val != 0.0) {
		  d_bihb = bihb;
		  d_bilt = bilt;
		  d_bict = bict;
		  d_bicc = bicc;
		  d_bicb = bicb;
		  d_bilb = bilb;
		  return;
		}
	      }
	      // update at end of block for restart
	      if (cbmf == false) {
		d_bilb = d_lbrb;
	      }
	    }
	    // update at end of block for restart
	    if (ccmf == false) {
	      d_bicb = d_cbrb;
	      d_bilb = d_lbrb;
	    } 
	  }
	  // update at end of block for restart
	  if (ctmf == false) {
	    d_bicc = d_ccrb;
	    d_bicb = d_cbrb;
	    d_bilb = d_lbrb;
	  }
	}
	// update at end of block for restart
	if (ltmf == false) {
	  d_bict = d_ctrb;
	  d_bicc = d_ccrb;
	  d_bicb = d_cbrb;
	  d_bilb = d_lbrb;
	}
      }
      // update at end of block for restart
      if (hbmf == false) {
	d_bilt = d_ltrb;
	d_bict = d_ctrb;
	d_bicc = d_ccrb;
	d_bicb = d_cbrb;
	d_bilb = d_lbrb;
      }
    }
    // here we are at the end
    d_bihb = d_hbsz;
    d_bilt = BLK_RMLT_SIZE;
    d_bict = BLK_RMCT_SIZE;
    d_bicc = BLK_RMCC_SIZE;
    d_bicb = BLK_RMCB_SIZE;
    d_bilb = BLK_RMLB_SIZE;
  }

  // no lock - move the iterator to the previous position
  
  void Rmatrixit::mvprev (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // loop in the hb block
    for (long bihb = d_bihb; bihb >= 0; bihb--) {
      // get the rmlt array
      Rmatrix::t_rmlt rmlt = p_mobj->p_rmhb[bihb];
      // check for a valid block and reset index
      if (rmlt == nullptr) {
	d_bilt = BLK_RMLT_SIZE - 1;
	d_bict = BLK_RMCT_SIZE - 1;
	d_bicc = BLK_RMCC_SIZE - 1;
	d_bicb = BLK_RMCB_SIZE - 1;
	d_bilb = BLK_RMLB_SIZE - 1;
	continue;
      }
      // loop in the lt block
      for (long bilt = d_bilt; bilt >= 0; bilt--) {
	// get the rmct array
	Rmatrix::t_rmct rmct = rmlt[bilt];
	// check for a valid block and reset index
	if (rmct == nullptr) {
	  d_bict = BLK_RMCT_SIZE - 1;
	  d_bicc = BLK_RMCC_SIZE - 1;
	  d_bicb = BLK_RMCB_SIZE - 1;
	  d_bilb = BLK_RMLB_SIZE - 1;
	  continue;
	}
	// loop in the ct block
	for (long bict = d_bict; bict >= 0;  bict--) {
	  // get the rmcc array
	  Rmatrix::t_rmcc rmcc = rmct[bict];
	  // check for a valid block and reset index
	  if (rmcc == nullptr) {
	    d_bicc = BLK_RMCC_SIZE - 1;
	    d_bicb = BLK_RMCB_SIZE - 1;
	    d_bilb = BLK_RMLB_SIZE - 1;
	    continue;
	  }
	  // loop in the cc block
	  for (long bicc = d_bicc; bicc >= 0; bicc--) {
	    // get the rmcb array
	    Rmatrix::t_rmcb rmcb = rmcc[bicc];
	    // check for a valid block and reset index
	    if (rmcb == nullptr) {
	      d_bicb = BLK_RMCB_SIZE - 1;
	      d_bilb = BLK_RMLB_SIZE - 1;
	      continue;
	    }
	    // loop in the cb block
	    for (long bicb = d_bicb; bicb >= 0; bicb--) {
	      // get the rmlb array
	      Rmatrix::t_rmlb rmlb = rmcb[bicb];
	      // check for a valid block and reset index
	      if (rmlb == nullptr) {
		d_bilb = BLK_RMLB_SIZE - 1;
		continue;
	      }
	      // loop in the lb block at next position
	      for (long bilb = d_bilb -1; bilb >= 0; bilb--) {
		// get the value and check
		t_real val = rmlb[bilb];
		if (val != 0.0) {
		  d_bihb = bihb;
		  d_bilt = bilt;
		  d_bict = bict;
		  d_bicc = bicc;
		  d_bicb = bicb;
		  d_bilb = bilb;
		  return;
		}
	      }
	    }
	  }
	}
      }
    }
    // move back to the end to mark it
    d_bihb = d_hbsz;
    d_bilt = BLK_RMLT_SIZE;
    d_bict = BLK_RMCT_SIZE;
    d_bicc = BLK_RMCC_SIZE;
    d_bicb = BLK_RMCB_SIZE;
    d_bilb = BLK_RMLB_SIZE;
  }

  // move the iterator to the previous row position
  
  void Rmatrixit::mrprev (void) {
    // check for nil - double security
    if (p_mobj == nullptr) return;
    // get the array column size
    long csiz = rm_csiz_hb (p_mobj->d_csiz);
    if (csiz == 0) return;
    // loop in the hb block by columns
    for (long bihb = d_bihb; bihb >= 0; bihb-=csiz) {
      // get the rmlt array
      Rmatrix::t_rmlt rmlt = p_mobj->p_rmhb[bihb];
      // check for a valid block and reset index
      if (rmlt == nullptr) {
	d_bilt = rm_indx_lt (p_mobj->d_rsiz-1LL, d_mcol);
	d_bict = rm_indx_ct (p_mobj->d_rsiz-1LL, d_mcol);
	d_bicc = rm_indx_cc (p_mobj->d_rsiz-1LL, d_mcol);
	d_bicb = rm_indx_cb (p_mobj->d_rsiz-1LL, d_mcol);
	d_bilb = rm_indx_lb (p_mobj->d_rsiz-1LL, d_mcol);
	continue;
      }
      // loop in the lt block
      for (long bilt = d_bilt; bilt >= 0; 
	   bilt-=COL_RMLT_SIZE) {
	// get the rmct array
	Rmatrix::t_rmct rmct = rmlt[bilt];
	// check for a valid block and reset index
	if (rmct == nullptr) {
	  d_bict = rm_indx_ct (p_mobj->d_rsiz-1LL, d_mcol);
	  d_bicc = rm_indx_cc (p_mobj->d_rsiz-1LL, d_mcol);
	  d_bicb = rm_indx_cb (p_mobj->d_rsiz-1LL, d_mcol);
	  d_bilb = rm_indx_lb (p_mobj->d_rsiz-1LL, d_mcol);
	  continue;
	}
	// loop in the ct block
	for (long bict = d_bict; bict >= 0; 
	     bict-=COL_RMCT_SIZE) {
	  // get the rmcc array
	  Rmatrix::t_rmcc rmcc = rmct[bict];
	  // check for a valid block and reset index
	  if (rmcc == nullptr) {
	    d_bicc = rm_indx_cc (p_mobj->d_rsiz-1LL, d_mcol);
	    d_bicb = rm_indx_cb (p_mobj->d_rsiz-1LL, d_mcol);
	    d_bilb = rm_indx_lb (p_mobj->d_rsiz-1LL, d_mcol);
	    continue;
	  }
	  // loop in the cc block
	  for (long bicc = d_bicc; bicc >= 0;
	       bicc-=COL_RMCC_SIZE) {
	    // get the rmcb array
	    Rmatrix::t_rmcb rmcb = rmcc[bicc];
	    // check for a valid block and reset index
	    if (rmcb == nullptr) {
	      d_bicb = rm_indx_cb (p_mobj->d_rsiz-1LL, d_mcol);
	      d_bilb = rm_indx_lb (p_mobj->d_rsiz-1LL, d_mcol);
	      continue;
	    }
	    // loop in the cb block
	    for (long bicb = d_bicb; bicb >= 0;
		 bicb-=COL_RMCB_SIZE) {
	      // get the rmlb array
	      Rmatrix::t_rmlb rmlb = rmcb[bicb];
	      // check for a valid block and reset index
	      if (rmlb == nullptr) {
		d_bilb = rm_indx_lb (p_mobj->d_rsiz-1LL, d_mcol);
		continue;
	      }
	      // loop in the lb block at next position
	      for (long bilb = d_bilb - COL_RMLB_SIZE; bilb >= 0;
		   bilb-=COL_RMLB_SIZE) {
		// get the value and check
		t_real val = rmlb[bilb];
		if (val != 0.0) {
		  d_bihb = bihb;
		  d_bilt = bilt;
		  d_bict = bict;
		  d_bicc = bicc;
		  d_bicb = bicb;
		  d_bilb = bilb;
		  return;
		}
	      }
	    }
	  }
	}
      }
    }
    // move back to the end to mark it
    d_bihb = d_hbsz;
    d_bilt = BLK_RMLT_SIZE;
    d_bict = BLK_RMCT_SIZE;
    d_bicc = BLK_RMCC_SIZE;
    d_bicb = BLK_RMCB_SIZE;
    d_bilb = BLK_RMLB_SIZE;
  }

  // no lock - move the iterator to the previous column position
  
  void Rmatrixit::mcprev (void) {
    throw Exception ("unimplemented-error", "unimplemented mcprev method");
  }

  // no lock - get the row coordinate at the iterator position (no lock)
  
  t_long Rmatrixit::nlgrow (void) const {
    // check for nil first
    if (p_mobj == nullptr) return 0LL;
    // check for iterator end
    if (d_bihb >= d_hbsz) return p_mobj->d_rsiz;
    // check valid indexes
    if (d_bihb < 0) {
      throw Exception ("internal-error", "invalid hb index in getrow");
    }
    // check valid indexes
    if ((d_bilt < 0) || (d_bilt >= BLK_RMLT_SIZE)) {
      throw Exception ("internal-error", "invalid lt index in getrow");
    }
    if ((d_bict < 0) || (d_bict >= BLK_RMCT_SIZE)) {
      throw Exception ("internal-error", "invalid ct index in getrow");
    }
    if ((d_bicc < 0) || (d_bicc >= BLK_RMCC_SIZE)) {
      throw Exception ("internal-error", "invalid cc index in getrow");
    }
    if ((d_bicb < 0) || (d_bicb >= BLK_RMCB_SIZE)) {
      throw Exception ("internal-error", "invalid cb index in getrow");
    }
    if ((d_bilb < 0) || (d_bilb >= BLK_RMLB_SIZE)) {
      throw Exception ("internal-error", "invalid lb index in getrow");
    }
    // map the matrix hb col size and indexes
    t_long cshb = rm_csiz_hb (p_mobj->d_csiz);
    if (cshb == 0L) return 0LL;
    // map the matrix row indexes
    t_long rihb = d_bihb / cshb;
    t_long rilt = d_bilt / COL_RMLT_SIZE;
    t_long rict = d_bict / COL_RMCT_SIZE;
    t_long ricc = d_bicc / COL_RMCC_SIZE;
    t_long ricb = d_bicb / COL_RMCB_SIZE;
    t_long rilb = d_bilb / COL_RMLB_SIZE;
    // recompute the row index
    t_long result = (rihb << ROW_RMHB_SHFT) + (rilt << ROW_RMLT_SHFT) +
      (rict << ROW_RMCT_SHFT) + (ricc << ROW_RMCC_SHFT) +
      (ricb << ROW_RMCB_SHFT) + rilb;
    return result;
  }

  // no lock - get the column coordinate at the iterator position
  
  t_long Rmatrixit::nlgcol (void) const {
    // check for nil first
    if (p_mobj == nullptr) return 0;
    // check for iterator end
    if (d_bihb >= d_hbsz) return p_mobj->d_csiz;
    // check valid indexes
    if ((d_bihb < 0) || (d_bihb >= d_hbsz)) {
      throw Exception ("internal-error", "invalid hb index in getcol");
    }
    // check valid indexes
    if ((d_bilt < 0) || (d_bilt >= BLK_RMLT_SIZE)) {
      throw Exception ("internal-error", "invalid lt index in getcol");
    }
    if ((d_bict < 0) || (d_bict >= BLK_RMCT_SIZE)) {
      throw Exception ("internal-error", "invalid ct index in getcol");
    }
    if ((d_bicc < 0) || (d_bicc >= BLK_RMCC_SIZE)) {
      throw Exception ("internal-error", "invalid cc index in getcol");
    }
    if ((d_bicb < 0) || (d_bicb >= BLK_RMCB_SIZE)) {
      throw Exception ("internal-error", "invalid cb index in getcol");
    }
    if ((d_bilb < 0) || (d_bilb >= BLK_RMLB_SIZE)) {
      throw Exception ("internal-error", "invalid lb index in getcol");
    }
    // map the matrix hb col size and indexes
    t_long cshb = rm_csiz_hb (p_mobj->d_csiz);
    // map the matrix column indexes
    t_long cihb = d_bihb % cshb;
    t_long cilt = d_bilt % COL_RMLT_SIZE;
    t_long cict = d_bict % COL_RMCT_SIZE;
    t_long cicc = d_bicc % COL_RMCC_SIZE;
    t_long cicb = d_bicb % COL_RMCB_SIZE;
    t_long cilb = d_bilb % COL_RMLB_SIZE;
    // recompute the column index
    t_long result = (cihb << COL_RMHB_SHFT) + (cilt << COL_RMLT_SHFT) +
      (cict << COL_RMCT_SHFT) + (cicc << COL_RMCC_SHFT) +
      (cicb << COL_RMCB_SHFT) + cilb;
    return result;
  }

  // no lock - set the value at the current position

  void Rmatrixit::nlsval (const t_real val)  {
    // check valid hb index
    if ((d_bihb < 0) || (d_bihb >= d_hbsz)) return;
    // map the rmlt array
    Rmatrix::t_rmlt rmlt = p_mobj->p_rmhb[d_bihb];
    // check for valid block and rmlt index
    if ((rmlt == nullptr) || (d_bilt < 0) || (d_bilt >= BLK_RMLT_SIZE)) {
      t_long row = nlgrow ();
      t_long col = nlgcol ();
      p_mobj->nlset (row, col, val);
      return;
    }
    // map the rmct array
    Rmatrix::t_rmct rmct = rmlt[d_bilt];
    // check for valid block and rmct index
    if ((rmct == nullptr) || (d_bict < 0) || (d_bict >= BLK_RMCT_SIZE)) {
      t_long row = nlgrow ();
      t_long col = nlgcol ();
      p_mobj->nlset (row, col, val);
      return;
    }
    // map the rmcc array
    Rmatrix::t_rmcc rmcc = rmct[d_bict];
    // check for valid block and rmcc index
    if ((rmcc == nullptr) || (d_bicc < 0) || (d_bicc >= BLK_RMCC_SIZE)) {
      t_long row = nlgrow ();
      t_long col = nlgcol ();
      p_mobj->nlset (row, col, val);
      return;
    }
    // map the rmcb array
    Rmatrix::t_rmcb rmcb = rmcc[d_bicc];
    // check for valid block and rmcb index
    if ((rmcb == nullptr) || (d_bicb < 0) || (d_bicb >= BLK_RMCB_SIZE)) {
      t_long row = nlgrow ();
      t_long col = nlgcol ();
      p_mobj->nlset (row, col, val);
      return;
    }
    // map the rmlb array
    Rmatrix::t_rmlb rmlb = rmcb[d_bicb];
    // check for valid block and rmlb index
    if ((rmlb == nullptr) || (d_bilb < 0) || (d_bilb >= BLK_RMLB_SIZE)) {
      t_long row = nlgrow ();
      t_long col = nlgcol ();
      p_mobj->nlset (row, col, val);
      return;
    }
    // here it is
    rmlb[d_bilb] = val;
  }

  // no lock - get the value at the current position (no lock)

  t_real Rmatrixit::nlgval (void) const {
    // check valid hb index
    if ((d_bihb < 0) || (d_bihb >= d_hbsz)) return 0.0;
    // map the rmlt array
    Rmatrix::t_rmlt rmlt = p_mobj->p_rmhb[d_bihb];
    // check for valid block and rmlt index
    if ((rmlt == nullptr) || (d_bilt < 0) || (d_bilt >= BLK_RMLT_SIZE)) return 0.0;
    // map the rmct array
    Rmatrix::t_rmct rmct = rmlt[d_bilt];
    // check for valid block and rmct index
    if ((rmct == nullptr) || (d_bict < 0) || (d_bict >= BLK_RMCT_SIZE)) return 0.0;
    // map the rmcc array
    Rmatrix::t_rmcc rmcc = rmct[d_bict];
    // check for valid block and rmcc index
    if ((rmcc == nullptr) || (d_bicc < 0) || (d_bicc >= BLK_RMCC_SIZE)) return 0.0;
    // map the rmcb array
    Rmatrix::t_rmcb rmcb = rmcc[d_bicc];
    // check for valid block and rmcb index
    if ((rmcb == nullptr) || (d_bicb < 0) || (d_bicb >= BLK_RMCB_SIZE)) return 0.0;
    // map the rmlb array
    Rmatrix::t_rmlb rmlb = rmcb[d_bicb];
    // check for valid block and rmlb index
    if ((rmlb == nullptr) || (d_bilb < 0) || (d_bilb >= BLK_RMLB_SIZE)) return 0.0;
    // here it is
    return rmlb[d_bilb];
  }
}
