// ---------------------------------------------------------------------------
// - Index.cpp                                                               -
// - afnix:sps module - cell index class implementation                      -
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

#include "Index.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the index structure
  struct s_indx {
    // the cell index
    long d_cidx;
    // the record index
    long d_ridx;
    // the sheet index
    long d_sidx;
    // create a default entry
    s_indx (void) {
      reset ();
    }
    // create an entry by coordinate
    s_indx (const long cidx) {
      set (cidx);
    }
    // create an entry by coordinate
    s_indx (const long cidx, const long ridx) {
      set (cidx, ridx);
    }
    // create an entry by coordinate
    s_indx (const long cidx, const long ridx, const long sidx) {
      set (cidx, ridx, sidx);
    }
    // copy construct this entry
    s_indx (const s_indx& that) {
      d_cidx = that.d_cidx;
      d_ridx = that.d_ridx;
      d_sidx = that.d_sidx;
    }
    // assign an entry to this one
    s_indx& operator = (const s_indx& that) {
      if (this == &that) return *this;
      d_cidx = that.d_cidx;
      d_ridx = that.d_ridx;
      d_sidx = that.d_sidx;
      return *this;
    }
    // reset this entry
    void reset (void) {
      d_cidx = -1;
      d_ridx = -1;
      d_sidx = -1;
    }
    // check if an entry is equal to the coordinate and name
    bool chk (const long cidx) {
      if (d_cidx != cidx) return false;
      if (d_ridx != -1)   return false;
      if (d_sidx != -1)   return false;
      return true;
    }
    // check if an entry is equal to the coordinate and name
    bool chk (const long cidx, const long ridx) {
      if (d_cidx != cidx) return false;
      if (d_ridx != ridx) return false;
      if (d_sidx != -1)   return false;
      return true;
    }
    // check if an entry is equal to the coordinate and name
    bool chk (const long cidx, const long ridx, const long sidx) {
      if (d_cidx != cidx) return false;
      if (d_ridx != ridx) return false;
      if (d_sidx != sidx) return false;
      return true;
    }
    // check if an entry is equal to the coordinate and name
    bool chk (const long cidx, const long ridx, const long sidx, 
	      const String& name) {
      if (d_cidx != cidx) return false;
      if (d_ridx != ridx) return false;
      if (d_sidx != sidx) return false;
      return true;
    }
    // set an entry by coordinate
    void set (const long cidx) {
      d_cidx = cidx;
      d_ridx = -1;
      d_sidx = -1;
    }
    // set an entry by coordinate
    void set (const long cidx, const long ridx) {
      d_cidx = cidx;
      d_ridx = ridx;
      d_sidx = -1;
    }
    // set an entry by coordinate
    void set (const long cidx, const long ridx, const long sidx) {
      d_cidx = cidx;
      d_ridx = ridx;
      d_sidx = sidx;
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
   
  // create an empty index
  
  Index::Index (void) {
    d_size = 0L;
    d_dpos = -1L;
    p_indx = nullptr;
  }

  // create an index by coordinates
  
  Index::Index (const long cidx) {
    d_size = 1;
    d_dpos = 0L;
    p_indx = new s_indx (cidx);
  }

  // create an index by coordinates
  
  Index::Index (const long cidx, const long ridx) {
    d_size = 1;
    d_dpos = 0L;
    p_indx = new s_indx (cidx, ridx);
  }

  // create an index by coordinates
  
  Index::Index (const long cidx, const long ridx, const long sidx) {
    d_size = 1;
    d_dpos = 0L;
    p_indx = new s_indx (cidx, ridx, sidx);
  }

  // copy construct this index

  Index::Index (const Index& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      d_dpos = that.d_dpos;
      p_indx = (d_size == 0) ? nullptr : new s_indx[d_size];
      for (long i = 0; i < d_size; i++) p_indx[i] = that.p_indx[i];
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this index

  Index::~Index (void) {
    delete [] p_indx;
  }

  // return the class name

  String Index::repr (void) const {
    return "Index";
  }

  // assign an index to this one

  Index& Index::operator = (const Index& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      delete [] p_indx;
      d_size = that.d_size;
      d_dpos = that.d_dpos;
      p_indx = (d_size == 0) ? nullptr : new s_indx[d_size];
      for (long i = 0; i < d_size; i++) p_indx[i] = that.p_indx[i];
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this index

  void Index::reset (void) {
    wrlock ();
    try {
      delete [] p_indx;
      d_size = 0L;
      d_dpos = -1L;
      p_indx = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the index length

  long Index::length (void) const {
    rdlock ();
    try {
      long result = d_size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if an entry exist

  bool Index::exists (const long cidx) const {
    rdlock ();
    try {
      for (long i = 0; i < d_size; i++) {
	if (p_indx[i].chk (cidx) == true) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if an entry exist

  bool Index::exists (const long cidx, const long ridx) const {
    rdlock ();
    try {
      for (long i = 0; i < d_size; i++) {
	if (p_indx[i].chk (cidx, ridx) == true) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if an entry exist

  bool Index::exists (const long cidx, const long ridx, 
		      const long sidx) const {
    rdlock ();
    try {
      for (long i = 0; i < d_size; i++) {
	if (p_indx[i].chk (cidx, ridx, sidx) == true) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an entry by coordinate

  void Index::add (const long cidx) {
    wrlock ();
    try {
      // check if the entry exists
      if (exists (cidx) == true) {
	unlock ();
	return;
      }
      // the entry does not exist - create and copy the array
      s_indx* indx = new s_indx[d_size+1];
      for (long i = 0; i < d_size; i++) indx[i] = p_indx[i];
      delete [] p_indx;
      p_indx = indx;
      // set the new entry and position
      d_dpos = d_size;
      p_indx[d_size++].set (cidx);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an entry by coordinate

  void Index::add (const long cidx, const long ridx) {
    wrlock ();
    try {
      // check if the entry exists
      if (exists (cidx, ridx) == true) {
	unlock ();
	return;
      }
      // the entry does not exist - create and copy the array
      s_indx* indx = new s_indx[d_size+1];
      for (long i = 0; i < d_size; i++) indx[i] = p_indx[i];
      delete [] p_indx;
      p_indx = indx;
      // set the new entry and position
      d_dpos = d_size;
      p_indx[d_size++].set (cidx, ridx);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an entry by coordinate

  void Index::add (const long cidx, const long ridx, const long sidx) {
    wrlock ();
    try {
      // check if the entry exists
      if (exists (cidx, ridx, sidx) == true) {
	unlock ();
	return;
      }
      // the entry does not exist - create and copy the array
      s_indx* indx = new s_indx[d_size+1];
      for (long i = 0; i < d_size; i++) indx[i] = p_indx[i];
      delete [] p_indx;
      p_indx = indx;
      // set the new entry and position
      d_dpos = d_size;
      p_indx[d_size++].set (cidx, ridx, sidx);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cell index

  void Index::setcidx (const long cidx) {
    wrlock ();
    try {
      if (d_dpos < 0L) add (cidx); else setcidx (d_dpos, cidx);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cell index by position

  void Index::setcidx (const long ipos, const long cidx) {
    wrlock ();
    try {
      // check for position
      if ((ipos < 0) || (ipos >= d_size)) {
	unlock ();
	throw Exception ("index-error", "out of range index position");
      }
      // set indexat position
      p_indx[ipos].d_cidx = cidx;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // update all cell indexes with a new value

  void Index::updcidx (const long cidx) {
    wrlock ();
    try {
      for (long i = 0; i < d_size; i++) {
	p_indx[i].d_cidx = cidx;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the default cell index 

  long Index::getcidx (void) const {
    rdlock ();
    try {
      long result = (d_dpos < 0L) ? -1L : getcidx (d_dpos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the cell index by position

  long Index::getcidx (const long ipos) const {
    rdlock ();
    try {
      // check for position
      if ((ipos < 0) || (ipos >= d_size)) {
	throw Exception ("index-error", "out of range index position");
      }
      // get index at position
      long result = p_indx[ipos].d_cidx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the default record index 

  void Index::setridx (const long ridx) {
    wrlock ();
    try {
      if (d_dpos < 0L) add (-1L, ridx); else setridx (d_dpos, ridx);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the record index by position

  void Index::setridx (const long ipos, const long ridx) {
    wrlock ();
    try {
      // check for position
      if ((ipos < 0) || (ipos >= d_size)) {
	throw Exception ("index-error", "out of range index position");
      }
      // set indexat position
      p_indx[ipos].d_ridx = ridx;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // update all record indexes with a new value

  void Index::updridx (const long ridx) {
    wrlock ();
    try {
      for (long i = 0; i < d_size; i++) {
	p_indx[i].d_ridx = ridx;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the default record index 

  long Index::getridx (void) const {
    rdlock ();
    try {
      long result = (d_dpos < 0L) ? -1L : getridx (d_dpos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the record index by position

  long Index::getridx (const long ipos) const {
    rdlock ();
    try {
      // check for position
      if ((ipos < 0) || (ipos >= d_size)) {
	throw Exception ("index-error", "out of range index position");
      }
      // get index at position
      long result = p_indx[ipos].d_ridx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the default sheet index

  void Index::setsidx (const long sidx) {
    wrlock ();
    try {
      if (d_dpos < 0L) add (-1L, -1L, sidx); else setsidx (d_dpos, sidx);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the sheet index by position

  void Index::setsidx (const long ipos, const long sidx) {
    wrlock ();
    try {
      // check for position
      if ((ipos < 0) || (ipos >= d_size)) {
	throw Exception ("index-error", "out of range index position");
      }
      // set indexat position
      p_indx[ipos].d_sidx = sidx;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // update all sheet indexes with a new value

  void Index::updsidx (const long sidx) {
    wrlock ();
    try {
      for (long i = 0; i < d_size; i++) {
	p_indx[i].d_sidx = sidx;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the default sheet index

  long Index::getsidx (void) const {
    rdlock ();
    try {
      long result = (d_dpos < 0L) ? -1L : getsidx (d_dpos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the sheet index by position

  long Index::getsidx (const long ipos) const {
    rdlock ();
    try {
      // check for position
      if ((ipos < 0) || (ipos >= d_size)) {
	throw Exception ("index-error", "out of range index position");
      }
      // get index at position
      long result = p_indx[ipos].d_sidx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------
  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 13;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_EXISTSP = zone.intern ("exists-p");
  static const long QUARK_SETCIDX = zone.intern ("set-index-cell");
  static const long QUARK_UPDCIDX = zone.intern ("update-index-cell");
  static const long QUARK_GETCIDX = zone.intern ("get-index-cell");
  static const long QUARK_SETRIDX = zone.intern ("set-index-record");
  static const long QUARK_UPDRIDX = zone.intern ("update-index-record");
  static const long QUARK_GETRIDX = zone.intern ("get-index-record");
  static const long QUARK_SETSIDX = zone.intern ("set-index-sheet");
  static const long QUARK_UPDSIDX = zone.intern ("update-index-sheet");
  static const long QUARK_GETSIDX = zone.intern ("get-index-sheet");

  // create a new object in a generic way

  Object* Index::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Index;
    // check for 1 argument
    if (argc == 1) {
      long cidx = argv->getlong (0);
      return new Index (cidx);
    }
    // check for 2 arguments
    if (argc == 2) {
      long cidx = argv->getlong (0);
      long ridx = argv->getlong (1);
      return new Index (cidx, ridx);
    }
    // check for 3 arguments
    if (argc == 3) {
      long cidx = argv->getlong (0);
      long ridx = argv->getlong (1);
      long sidx = argv->getlong (2);
      return new Index (cidx, ridx, sidx);
    }
    throw Exception ("argument-error", "too many arguments with index");
  }

  // return true if the given quark is defined

  bool Index::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply an object method with a set of arguments and a quark

  Object* Index::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	long cidx = argv->getlong (0);
	add (cidx);
	return nullptr;
      }
      if (quark == QUARK_EXISTSP) {
	long cidx = argv->getlong (0);
	return new Boolean (exists (cidx));
      }
      if (quark == QUARK_GETCIDX) {
	long ipos = argv->getlong (0);
	return new Integer (getcidx (ipos));
      }
      if (quark == QUARK_GETRIDX) {
	long ipos = argv->getlong (0);
	return new Integer (getridx (ipos));
      }
      if (quark == QUARK_GETSIDX) {
	long ipos = argv->getlong (0);
	return new Integer (getsidx (ipos));
      }
      if (quark == QUARK_UPDCIDX) {
	long cidx = argv->getlong (0);
	updcidx (cidx);
	return nullptr;
      }
      if (quark == QUARK_UPDRIDX) {
	long ridx = argv->getlong (0);
	updridx (ridx);
	return nullptr;
      }
      if (quark == QUARK_UPDSIDX) {
	long sidx = argv->getlong (0);
	updsidx (sidx);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADD) {
	long cidx = argv->getlong (0);
	long ridx = argv->getlong (1);
	add (cidx, ridx);
	return nullptr;
      }
      if (quark == QUARK_EXISTSP) {
	long cidx = argv->getlong (0);
	long ridx = argv->getlong (1);
	return new Boolean (exists (cidx, ridx));
      }
      if (quark == QUARK_SETCIDX) {
	long ipos = argv->getlong (0);
	long cidx = argv->getlong (1);
	setcidx (ipos, cidx);
	return nullptr;
      }
      if (quark == QUARK_SETRIDX) {
	long ipos = argv->getlong (0);
	long ridx = argv->getlong (1);
	setridx (ipos, ridx);
	return nullptr;
      }
      if (quark == QUARK_SETSIDX) {
	long ipos = argv->getlong (0);
	long sidx = argv->getlong (1);
	setsidx (ipos, sidx);
	return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_ADD) {
	long cidx = argv->getlong (0);
	long ridx = argv->getlong (1);
	long sidx = argv->getlong (2);
	add (cidx, ridx, sidx);
	return nullptr;
      }
      if (quark == QUARK_EXISTSP) {
	long cidx = argv->getlong (0);
	long ridx = argv->getlong (1);
	long sidx = argv->getlong (2);
	return new Boolean (exists (cidx, ridx, sidx));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
