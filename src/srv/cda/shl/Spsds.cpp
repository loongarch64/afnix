// ---------------------------------------------------------------------------
// - Spsds.cpp                                                               -
// - afnix:sps module - sps data streamer class implementation               -
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

#include "Spsds.hpp"
#include "Utility.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default streamer

  Spsds::Spsds (void) {
    d_meth = METH_MRK;
    d_ridx = -1L;
    d_cidx = -1L;
    p_shto = nullptr;
    reset ();
  }

  // create a streamer by sheet

  Spsds::Spsds (Sheet* shto) {
    d_meth = METH_MRK;
    d_ridx = -1L;
    d_cidx = -1L;
    Object::iref (p_shto = shto);
    reset ();
  }
  
  // create a streamer by folio and sheet index

  Spsds::Spsds (Folio* flio, const long sidx) {
    d_meth = METH_MRK;
    d_ridx = -1L;
    d_cidx = -1L;
    p_shto = (flio == nullptr) ? nullptr : flio->get (sidx);
    Object::iref (p_shto);
    reset ();
  }
  
  // delete this streamer

  Spsds::~Spsds (void) {
    Object::dref (p_shto);
  }

  // return the object name

  String Spsds::repr (void) const {
    return "Spsds";
  }

  // reset the sheet streamer

  void Spsds::reset (void) {
    wrlock ();
    try {
      d_iidx = -1L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the streamer departure position

  t_real Spsds::departure (void) const {
    rdlock ();
    try {
      t_real result = 0.0;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer arrival position

  t_real Spsds::arrival (void) const {
    rdlock ();
    try {
      t_real result = Streamable::arrival ();
      if (p_shto == nullptr) {
	unlock ();
	return result;
      }
      if (d_meth == METH_ROW) {
	// get the number of rows
	long rows = p_shto->length ();
	// map it to length
	result = (t_real) (rows - 1L);
      }
      if (d_meth == METH_COL) {
	// get the number of columns
	long cols = p_shto->getcols ();
	// map it to a length
	result = (t_real) (cols - 1L);
      }
      if (d_meth == METH_BND) {
	// get a literal stack
	Literal* lobj = p_shto->map (d_ridx, d_cidx);
	Bundle*  lstk = dynamic_cast<Bundle*> (lobj);
	if (lstk == nullptr) {
	  throw Exception ("lstream-error", "invalid cell object as bundle",
			   Object::repr (lobj));
	}
	// get the stack depth
	long slen = lstk->length ();
	// map it to a length
	result = (t_real) (slen - 1L);
      }
      if (d_meth == METH_MRK) {
	// get the marker length
	long mlen = p_shto->marklen ();
	// map it to a length
	result = (t_real) (mlen - 1L);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer position

  t_real Spsds::locate (void) const {
    rdlock ();
    try {
      t_real result = Streamable::locate ();
      if (p_shto == nullptr) {
	unlock ();
	return result;
      }
      result = (t_real) d_iidx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the streamer to the next position
  
  t_real Spsds::next (void) {
    wrlock ();
    try {
      t_real result = Streamable::next ();
      if (p_shto == nullptr) {
	unlock ();
	return result;
      }
      result = move (++d_iidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the streamer to the previous position
  
  t_real Spsds::prev (void) {
    wrlock ();
    try {
      t_real result = Streamable::prev ();
      if (p_shto == nullptr) {
	unlock ();
	return result;
      }
      result = move (--d_iidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the streamer position

  t_real Spsds::move (const t_real pos) {
    wrlock ();
    try {
      t_real result = Streamable::move (pos);
      if (p_shto == nullptr) {
	unlock ();
	return result;
      }
      d_iidx = (long) pos;
      if (d_iidx < 0L) d_iidx = 0L;
      if (d_meth == METH_ROW) {
	long rows = p_shto->length ();
	if (d_iidx >= rows) d_iidx = rows - 1L;
      }
      if (d_meth == METH_COL) {
	long cols = p_shto->getcols ();
	if (d_iidx >= cols) d_iidx = cols - 1L;
      }
      if (d_meth == METH_BND) {
	Literal* lobj = p_shto->map (d_ridx, d_cidx);
	Bundle*  lstk = dynamic_cast<Bundle*> (lobj);
	if (lstk == nullptr) {
	  throw Exception ("lstream-error", "invalid cell object as bundle",
			   Object::repr (lobj));
	}
	long slen = lstk->length ();
	if (d_iidx >= slen) d_iidx = slen - 1L;
      }
      if (d_meth == METH_MRK) {
	long mlen = p_shto->marklen ();
	if (d_iidx >= mlen) d_iidx = mlen - 1L;
      }
      result = (t_real) d_iidx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer boolean value

  bool Spsds::getbool (void) const {
    rdlock ();
    try {
      bool result = Streamable::getbool ();
      if (p_shto == nullptr) {
	unlock ();
	return result;
      }
      if (d_meth == METH_ROW) {
	Literal* lobj = p_shto->map (d_iidx, d_cidx);
	if (lobj != nullptr) result = Utility::tobool (lobj);
      }
      if (d_meth == METH_COL) {
	Literal* lobj = p_shto->map (d_ridx, d_iidx);
	if (lobj != nullptr) result = Utility::tobool (lobj);
      }
      if (d_meth == METH_BND) {
	Literal* lobj = p_shto->map (d_ridx, d_cidx);
	Bundle*  lstk = dynamic_cast<Bundle*> (lobj);
	if (lstk == nullptr) {
	  throw Exception ("lstream-error", "invalid cell object as bundle",
			   Object::repr (lobj));
	}
	lobj = lstk->get (d_iidx);
	if (lobj != nullptr) result = Utility::tobool (lobj);
      }
      if (d_meth == METH_MRK) {
	Literal* lobj = p_shto->getmark (d_iidx);
	if (lobj != nullptr) result = Utility::tobool (lobj);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer integer value

  long Spsds::getlong (void) const {
    rdlock ();
    try {
      long result = Streamable::getlong ();
      if (p_shto == nullptr) {
	unlock ();
	return result;
      }
      if (d_meth == METH_ROW) {
	Literal* lobj = p_shto->map (d_iidx, d_cidx);
	if (lobj != nullptr) result = Utility::tolong (lobj);
      }
      if (d_meth == METH_COL) {
	Literal* lobj = p_shto->map (d_ridx, d_iidx);
	if (lobj != nullptr) result = Utility::tolong (lobj);
      }
      if (d_meth == METH_BND) {
	Literal* lobj = p_shto->map (d_ridx, d_cidx);
	Bundle*  lstk = dynamic_cast<Bundle*> (lobj);
	if (lstk == nullptr) {
	  throw Exception ("lstream-error", "invalid cell object as bundle",
			   Object::repr (lobj));
	}
	lobj = lstk->get (d_iidx);
	if (lobj != nullptr) result = Utility::tolong (lobj);
      }
      if (d_meth == METH_MRK) {
	Literal* lobj = p_shto->getmark (d_iidx);
	if (lobj != nullptr) result = Utility::tolong (lobj);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer real value

  t_real Spsds::getreal (void) const {
    rdlock ();
    try {
      t_real result = Streamable::getlong ();
      if (p_shto == nullptr) {
	unlock ();
	return result;
      }
      if (d_meth == METH_ROW) {
	Literal* lobj = p_shto->map (d_iidx, d_cidx);
	if (lobj != nullptr) result = Utility::torint (lobj);
      }
      if (d_meth == METH_COL) {
	Literal* lobj = p_shto->map (d_ridx, d_iidx);
	if (lobj != nullptr) result = Utility::torint (lobj);
      }
      if (d_meth == METH_BND) {
	Literal* lobj = p_shto->map (d_ridx, d_cidx);
	Bundle*  lstk = dynamic_cast<Bundle*> (lobj);
	if (lstk == nullptr) {
	  throw Exception ("lstream-error", "invalid cell object as bundle",
			   Object::repr (lobj));
	}
	lobj = lstk->get (d_iidx);
	if (lobj != nullptr) result = Utility::torint (lobj);
      }
      if (d_meth == METH_MRK) {
	Literal* lobj = p_shto->getmark (d_iidx);
	if (lobj != nullptr) result = Utility::torint (lobj);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the sheet row index

  void Spsds::setridx (const long ridx) {
    wrlock ();
    try {
      reset ();
      d_meth = METH_COL;
      d_ridx = ridx;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the sheet column index

  void Spsds::setcidx (const long cidx) {
    wrlock ();
    try {
      reset ();
      d_meth = METH_ROW;
      d_cidx = cidx;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the sheet indexes

  void Spsds::setindx (const long ridx, const long cidx) {
    wrlock ();
    try {
      reset ();
      if ((ridx >= 0L) && (cidx >= 0L)) {
	d_meth = METH_BND;
	d_ridx = ridx;
	d_cidx = cidx;
      } else {
	if ((ridx >= 0L) && (cidx < 0L)) setridx (ridx);
	if ((cidx >= 0L) && (ridx < 0L)) setcidx (cidx);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETIDX = zone.intern ("set-index");
  static const long QUARK_SETROW = zone.intern ("set-row-index");
  static const long QUARK_SETCOL = zone.intern ("set-col-index");

  // create a new object in a generic way

  Object* Spsds::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Spsds;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Sheet*  sht = dynamic_cast <Sheet*> (obj);
      if (sht != nullptr) return new Spsds (sht);
      throw Exception ("type-error", "invalid argument with sps streamer",
		       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      Folio*  flo = dynamic_cast <Folio*> (obj);
      long   sidx = argv->getlong (1);
      if (flo != nullptr) return new Spsds (flo, sidx);
      throw Exception ("type-error", "invalid argument with sps streamer",
		       Object::repr (obj));
    }
    throw Exception ("argument-error",
		     "too many arguments with literal streamer");
  }

  // return true if the given quark is defined

  bool Spsds::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
        unlock ();
        return true;
      }
      bool result = hflg ? Streamable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Spsds::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETROW) {
	long ridx = argv->getlong (0); 
        setridx (ridx);
	return nullptr;
      }
      if (quark == QUARK_SETCOL) {
	long cidx = argv->getlong (0); 
        setcidx (cidx);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETIDX) {
	long ridx = argv->getlong (0); 
	long cidx = argv->getlong (1); 
        setindx (ridx, cidx);
	return nullptr;
      }
    }
    // call the streamable method
    return Streamable::apply (zobj, nset, quark, argv);
  }
}
