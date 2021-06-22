// ---------------------------------------------------------------------------
// - Rgivens.cpp                                                             -
// - afnix:mth module - real givens matrix class implementation              -
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

#include "Real.hpp"
#include "Math.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Rgivens.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // default givens array size
  static const long GR_SIZE_DEF = 16;

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // create a default givens rotation

  Rgivens::s_givens::s_givens (void) {
    d_row = -1L;
    d_col = -1L;
    d_c   = Math::CV_NAN;
    d_s   = Math::CV_NAN;
  }

  // create a givens rotation by datum

  Rgivens::s_givens::s_givens (const Rmd& rmr, const Rmd& rmc) {
    set (rmr, rmc);
  }

  // create a givens rotation by values

  Rgivens::s_givens::s_givens (const t_long row, const t_long col, 
			       const t_real rcv, const t_real ccv) {
    set (row, col, rcv, ccv);
  }

  // set a givens rotation by datum

  void Rgivens::s_givens::set (const Rmd& rmr, const Rmd& rmc) {
    // process the row datum 
    d_row = rmr.getrow ();
    d_col = rmr.getcol ();
    if (d_row <= d_col) {
      throw Exception ("rgivens-error", "inconsistent row datum in set");
    }
    t_real rcv = rmr.getval ();
    // process the col datu
    if ((rmc.getrow () != d_col) || (rmc.getcol () != d_col)) {
      throw Exception ("rgivens-error", "inconsistent col datum in set");
    }
    t_real ccv = rmc.getval ();
    // compute the givens coefficients
    t_real nrm = Math::hypot (rcv, ccv);
    d_c = ccv / nrm;
    d_s = rcv / nrm;
  }

  // set a givens rotation by values

  void Rgivens::s_givens::set (const t_long row, const t_long col, 
			       const t_real rcv, const t_real ccv) {
    // values validation
    if ((row < 0LL) || (col < 0) || (Math::isnan (rcv) == true) ||
	(Math::isnan (ccv) == true)) {
      throw Exception ("rgivens-error", "invalid rotation values in set");
    }
    // set the givens values
    d_row = row;
    d_col = col;
    // compute the givens coefficients
    t_real nrm = Math::hypot (rcv, ccv);
    d_c = ccv / nrm;
    d_s = rcv / nrm;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default givens array

  Rgivens::Rgivens (void) {
    d_size = 0;
    d_glen = 0;
    p_grot = nullptr;
  }

  // destroy this object

  Rgivens::~Rgivens (void) {
    delete [] p_grot;
  }

  // return the class name
  
  String Rgivens::repr (void) const {
    return "Rgivens";
  }
  
  // get the array size

  long Rgivens::length (void) const {
    rdlock ();
    try {
      long result = d_glen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a givens rotation

  long Rgivens::add (const s_givens& gr) {
    wrlock ();
    try {
      // check for null size
      if (d_size == 0) {
	d_size = GR_SIZE_DEF;
	d_glen = 0;
	p_grot = new s_givens[d_size];
      }
      // check for resize
      if (d_glen == d_size) {
	// allocate new size
	long size = d_size * 2;
	s_givens* grot = new s_givens[size];
	// copy old array
	for (long k = 0; k < d_size; k++) grot[k] = p_grot[k];
	// update indexes
	d_size = size;
	delete [] p_grot; p_grot = grot;
      }
      // add new rotation
      long result = d_glen++;
      p_grot[result] = gr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a givens rotation by datum - assume here (row > col)

  long Rgivens::add (const Rmd& rmr, const Rmd& rmc) {
    wrlock ();
    try {
      // create a givens rotation
      s_givens gr (rmr, rmc);
      // add it and unlock
      long result = add (gr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a givens rotation by values

  long Rgivens::add (const t_long row, const t_long col, 
		     const t_real rcv, const t_real ccv) {
    wrlock ();
    try {
      // create a givens rotation
      s_givens gr (row, col, rcv, ccv);
      // add it and unlock
      long result = add (gr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a givens rotation by index

  Rgivens::s_givens Rgivens::get (const long index) const {
    rdlock ();
    try {
      s_givens result = nlget (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the givens row by index

  t_long Rgivens::getrow (const long index) const {
    rdlock ();
    try {
      t_long result = nlgrow (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the givens matrix column

  t_long Rgivens::getcol (const long index) const {
    rdlock ();
    try {
      t_long result = nlgcol (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the givens matrix c coefficient

  t_real Rgivens::getc (const long index) const {
    rdlock ();
    try {
      t_real result = nlgc (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the givens matrix s coefficient

  t_real Rgivens::gets (const long index) const {
    rdlock ();
    try {
      t_real result = nlgs (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - no lock section                                                       -
  // -------------------------------------------------------------------------

  // no lock - get a givens rotation by index

  Rgivens::s_givens Rgivens::nlget (const long index) const {
    // check boundary
    if ((index < 0) || (index >= d_glen)) {
      throw Exception ("index-error", "invalid index in real givens array");
    }
    // get by index
    return p_grot[index];
  }

  // get the givens row by index

  t_long Rgivens::nlgrow (const long index) const {
    // get the givens rotation
    s_givens gr = get (index);
    // get result
    return gr.d_row;
  }

  // get the givens matrix column

  t_long Rgivens::nlgcol (const long index) const {
    // get the givens rotation
    s_givens gr = get (index);
    // get result
    return gr.d_col;
  }

  // get the givens matrix c coefficient

  t_real Rgivens::nlgc (const long index) const {
    // get the givens rotation
    s_givens gr = get (index);
    // get result
    return gr.d_c;
  }

  // get the givens matrix s coefficient

  t_real Rgivens::nlgs (const long index) const {
    // get the givens rotation
    s_givens gr = get (index);
    // get result
    return gr.d_s;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GETC   = zone.intern ("get-c");
  static const long QUARK_GETS   = zone.intern ("get-s");
  static const long QUARK_GETROW = zone.intern ("get-row");
  static const long QUARK_GETCOL = zone.intern ("get-col");
  static const long QUARK_LENGTH = zone.intern ("length");

  // create a new object in a generic way
  
  Object* Rgivens::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Rgivens;
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with real givens matrix");
  }

  // return true if the given quark is defined

  bool Rgivens::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Rgivens::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETC) {
	long index = argv->getlong (0);
	return new Real (getc (index));
      }
      if (quark == QUARK_GETS) {
	long index = argv->getlong (0);
	return new Real (gets (index));
      }
      if (quark == QUARK_GETROW) {
	long index = argv->getlong (0);
	return new Integer (getrow (index));
      }
      if (quark == QUARK_GETCOL) {
	long index = argv->getlong (0);
	return new Integer (getcol (index));
      }
    }
    // dispatch 4 arguments
    if (argc == 4) {
      if (quark == QUARK_ADD) {
	t_long row = argv->getlong (0);
	t_long col = argv->getlong (1);
	t_real rcv = argv->getrint (2);
	t_real ccv = argv->getrint (3);
	long result = add (row, col, rcv, ccv);
	return new Integer (result);
      }
    }
    // call the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
