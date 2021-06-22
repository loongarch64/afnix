// ---------------------------------------------------------------------------
// - Structure.cpp                                                           -
// - standard object library - structure class implementation                -
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
#include "Stdsid.hxx"
#include "Strvec.hpp"
#include "Vector.hpp"
#include "Locale.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "Structure.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the structure descriptor
  struct s_sdsc {
    // the locale to map
    Locale d_locl;
    // the buffer size
    long d_size;
    // the buffer data
    t_byte* p_data;
    // the next descriptor
    s_sdsc* p_next;
    // create a default descriptor
    s_sdsc (void) {
      d_size = 0L;
      p_data = nullptr;
      p_next = nullptr;
    }
    // destroy this descriptor
    ~s_sdsc (void) {
      // clean the data buffer
      delete [] p_data;
      // clean next buffer
      delete p_next;
    }
    // get the number of descriptor
    long getdsiz (void) const {
      return 1L + ((p_next == nullptr) ? 0L : p_next->getdsiz ());
    }
    // get the descriptor offset by index
    long getdoff (const long didx) {
      long doff = 0L;
      long kidx = 0L;
      s_sdsc* sdsc = this;
      while (sdsc != nullptr) {
	if (kidx++ == didx) return doff;
	doff += sdsc->d_size;
	sdsc  = sdsc->p_next;
      }
      throw Exception ("structure-error", "out-of-bound descriptor index");
    }
    // get the descriptor locale by index
    Locale getmloc (const long didx) {
      long kidx = 0L;
      s_sdsc* sdsc = this;
      while (sdsc != nullptr) {
	if (kidx++ == didx) return sdsc->d_locl;
	sdsc = sdsc->p_next;
      }
      throw Exception ("structure-error", "out-of-bound descriptor index");
    }
    // get the locale index
    long getlidx (void) const {
      return d_locl.getlidx ();
    }
    // add a new locale by string
    bool addlocl (const String& ldef) {
      if (p_next == nullptr) {
        s_sdsc* desc = new s_sdsc;
	if (desc->setlocl (ldef) == true) {
	  p_next = desc;
	  return true;
	} else {
	  delete desc;
	  return false;
	}
      }
      return p_next->addlocl (ldef);
    }
    // add a new locale to the list
    bool addlocl (const Locale& locl) {
      if (p_next == nullptr) {
        s_sdsc* desc = new s_sdsc;
	if (desc->setlocl (locl) == true) {
	  p_next = desc;
	  return true;
	} else {
	  delete desc;
	  return false;
	}
      }
      return p_next->addlocl (locl);
    }
    // set the locale by string
    bool setlocl (const String& ldef) {
      // create a locale
      Locale locl (ldef);
      // process by value
      return setlocl (locl);
    }
    // set the locale value
    bool setlocl (const Locale& locl) {
      // get the locale format and modifier
      Locale::t_ofmt ofmt = Locale::toofmt (locl.getofmt ());
      Locale::t_omod omod = Locale::toomod (locl.getomod ());
      // check for an integer
      if (ofmt == Locale::OFMT_INTG) {
	if (omod == Locale::OMOD_NONE) {
	  d_locl = locl;
	  d_size = sizeof (t_long);
	  return true;
	}	    
	if (omod == Locale::OMOD_SHRT) {
	  d_locl = locl;
	  d_size = sizeof (int);
	  return true;
	}
      }
      if (ofmt == Locale::OFMT_REAL) {
	if (omod == Locale::OMOD_NONE) {
	  d_locl = locl;
	  d_size = sizeof (t_real);
	  return true;
	}
	if (omod == Locale::OMOD_SHRT) {
	  d_locl = locl;
	  d_size = sizeof (float);
	  return true;
	}
      }
      return false;
    }
    // set a buffer by integer value
    long setxbuf (t_byte* xbuf, const t_long ival, const t_real rval) const {
      // get the locale format and modifier
      Locale::t_ofmt ofmt = Locale::toofmt (d_locl.getofmt ());
      Locale::t_omod omod = Locale::toomod (d_locl.getomod ());
      // check for integer
      if (ofmt == Locale::OFMT_INTG) {
	// check without a modifier
	if (omod == Locale::OMOD_NONE) {
	  union {
	    t_long d_ival;
	    t_byte d_bval[sizeof(t_long)];
	  };
	  d_ival = ival;
	  Utility::tobcpy (xbuf, sizeof(t_long), d_bval);
	  return sizeof(t_long);
	}
	// check with a short modifier
	if (omod == Locale::OMOD_SHRT) {
	  union {
	    int d_ival;
	    t_byte d_bval[sizeof(int)];
	  };
	  d_ival = (int) ival;
	  Utility::tobcpy (xbuf, sizeof(int), d_bval);
	  return sizeof(int);
	}
      }
      // check for real
      if (ofmt == Locale::OFMT_REAL) {
	// check without a modifier
	if (omod == Locale::OMOD_NONE) {
	  union {
	    t_real d_rval;
	    t_byte d_bval[sizeof(t_real)];
	  };
	  d_rval = (t_real) rval;
	  Utility::tobcpy (xbuf, sizeof(t_real), d_bval);
	  return sizeof(t_real);
	}
	// check with a short modifier
	if (omod == Locale::OMOD_SHRT) {
	  union {
	    float d_fval;
	    t_byte d_bval[sizeof(float)];
	  };
	  d_fval = (float) rval;
	  Utility::tobcpy (xbuf, sizeof(float), d_bval);
	  return sizeof(float);
	}
      }
      // no transform
      return 0L;
    }
    // serialize the descriptor list
    static void wrstream (s_sdsc* desc, OutputStream& os) {
      // the vector of locales
      Vector locv;
      while (desc != nullptr) {
	// add the locale to the vector
	locv.add (new Locale(desc->d_locl));
	// next descriptor
	desc = desc->p_next;
      }
      // serialize the vector
      locv.wrstream (os);
    }
    // deserialize a descriptor list
    static s_sdsc* rdstream (InputStream& is) {
      // deserialize the vector of locales
      Vector locv; locv.rdstream (is);
      // prepare result
      s_sdsc* desc = nullptr;
      // loop in the vector
      long llen = locv.length ();
      for (long k = 0L; k < llen; k++) {
	// get the locale object
	auto locl = dynamic_cast<Locale*>(locv.get (k));
	// check for a descriptor
	if (desc == nullptr) {
	  desc = new s_sdsc; desc->setlocl (*locl);
	} else {
	  desc->addlocl (*locl);
	}
      }
      return desc;
    }
  };

  // the structure block
  struct s_sblk {
    /// the structure padding
    long d_spad;
    /// the structure size
    long d_ssiz;
    /// the structure offset
    long d_soff;
    /// the structure block
    t_byte* p_sblk;
    // the open descriptor
    s_sdsc* p_desc;
    // create a default block
    s_sblk (void) {
      d_spad = 0L;
      p_sblk = nullptr;
      p_desc = nullptr;
      reset ();
    }
    // destroy this block
    ~s_sblk (void) {
      delete [] p_sblk;
    }
    // reset this block
    void reset (void) {
      d_spad = 0L;
      d_ssiz = 0L;
      d_soff = 0l;
      p_desc = nullptr;
      delete [] p_sblk; p_sblk = nullptr;
    }
    // recompute the block size
    void update (s_sdsc* desc) {
      // reset the structure block
      d_ssiz = 0L;
      delete [] p_sblk; p_sblk = nullptr;      
      // loop in the descriptor
      while (desc != nullptr) {
	// update the structure size
	d_ssiz += desc->d_size;
	// move to the next descriptor
	desc = desc->p_next;
      }
      // update the size with the padding
      if (d_spad > 0L) {
	d_ssiz = d_ssiz + d_spad - (d_ssiz % d_spad);
      }
      // bind the structure block
      if (d_ssiz > 0L) {
	p_sblk = new t_byte[d_ssiz];
	for (long k = 0L; k < d_ssiz; k++) p_sblk[k] = nilb;
      }
      // mark the block as closed
      d_soff = 0L;
      p_desc = nullptr;
    }
    // open the structure block
    bool open (s_sdsc* desc) {
      // check if the block is open
      if ((d_soff > 0L) || (p_desc != nullptr)) return false;
      // reset the block
      if (p_sblk != nullptr) {
	for (long k = 0L; k < d_ssiz; k++) p_sblk[k] = nilb;
      }
      p_desc = desc;
      return true;
    }
    // close the structure block
    bool close (void) {
      // check if the block is closed
      if ((d_soff == 0L) && (p_desc == nullptr)) return false;
      // reset offset and descriptor
      d_soff = 0L;
      p_desc = nullptr;
      return true;
    }
    // add a buffer to the block
    bool setxbuf (const t_byte* xbuf, const long xsiz) {
      // check if the structure is open
      if (p_desc == nullptr) return false;
      // check for valid target size
      if ((d_soff + xsiz) > d_ssiz) return false;
      // copy the buffer
      Utility::tobcpy (&p_sblk[d_soff], xsiz, xbuf);
      // update index and descriptor
      d_soff += xsiz;
      p_desc  = p_desc->p_next;
      return true;
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default structure

  Structure::Structure (void) {
    p_desc = nullptr;
    p_blok = nullptr;
    reset ();
  }

  // copy move this structure

  Structure::Structure (Structure&& that) noexcept  {
    that.wrlock ();
    try {
      // copy move the base buffer
      Buffer::operator = (static_cast<Buffer&&>(that));
      // copy move locally
      p_desc = that.p_desc; that.p_desc = nullptr;
      p_blok = that.p_blok; that.p_blok = nullptr;
      that.unlock ();
    } catch (...) {
      reset ();
    }
    that.unlock ();
  }

  // destroy this structure

  Structure::~Structure (void) {
    delete p_desc;
    delete p_blok;
  }
    
  // move a structure to this one

  Structure& Structure::operator = (Structure&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // delete old structure
      delete p_desc; p_desc = nullptr;
      delete p_blok; p_blok = nullptr;
      // move the base buffer
      Buffer::operator = (static_cast<Buffer&&>(that));
      // move locally
      p_desc = that.p_desc; that.p_desc = nullptr;
      p_blok = that.p_blok; that.p_blok = nullptr;
    } catch (...) {
      reset ();
    }
    unlock ();
    that.unlock ();
    return *this;
  }

  // return the class name

  String Structure::repr (void) const {
    return "Structure";
  }
 
  // return the structure did

  t_word Structure::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the structure sid

  t_word Structure::getsid (void) const {
    return SRL_STRC_SID;
  }

  // serialize this structure

  void Structure::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the buffer
      Buffer::wrstream (os);
      // serialize locally
      s_sdsc::wrstream (p_desc, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this structure

  void Structure::rdstream (InputStream& is) {
    wrlock ();
    try {
      // reset the structure
      reset ();
      // deserialize the buffer
      Buffer::rdstream (is);
      // deserialize locally
      p_desc = s_sdsc::rdstream (is);
      // eventually rebuild the structure block
      if (p_desc != nullptr) {
	// check for a structure block
	if (p_blok == nullptr) p_blok = new s_sblk;
	// update the strcuture block
	p_blok->update (p_desc);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this structure

  void Structure::reset (void) {
    wrlock ();
    try {
      // reset the base buffer
      Buffer::reset ();
      // reset locally
      delete p_desc; p_desc = nullptr;
      delete p_blok; p_blok = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the structure size

  long Structure::getssiz (void) const {
    rdlock ();
    try {
      long ssiz = (p_blok == nullptr) ? 0L : length() / p_blok->d_ssiz;
      unlock ();
      return ssiz;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the locale size

  long Structure::getdsiz (void) const {
    rdlock ();
    try {
      long dsiz = (p_desc == nullptr) ? 0L : p_desc->getdsiz ();
      unlock ();
      return dsiz;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add a locale definition

  bool Structure::addldef (const String& ldef) {
    wrlock ();
    try {
      // preset result
      bool result = false;
      // split the locale definition
      Strvec lvec = Strvec::split (ldef, ":");
      long llen = lvec.length ();
      for (long k = 0L; k < llen; k++) {
	// extract locale value
	String lval = lvec.get (k);
	if (lval.isnil () == true) continue;
	// check for descriptor
	if (p_desc == nullptr) {
	  p_desc = new s_sdsc;
	  result = p_desc->setlocl (lval);
	} else {
	  result = p_desc->addlocl (lval);
	}
	// check for valid result
	if (result == false) {
	  delete p_desc; p_desc= nullptr;
	  break;
	}
      }
      // eventually rebuild the structure block
      if (result == true) {
	// check for a structure block
	if (p_blok == nullptr) p_blok = new s_sblk;
	// update the strcuture block
	p_blok->update (p_desc);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the structure padding

  void Structure::setspad (const long spad) {
    wrlock ();
    try {
      // check for a structure block
      if (p_blok == nullptr) p_blok = new s_sblk;
      // set the structure padding
      p_blok->d_spad = (spad <= 0L) ? 0L : spad;
      // update the structure block
      p_blok->update (p_desc);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the locale indexes

  long* Structure::getlidx (void) const {
    rdlock ();
    try {
      // get the number of descriptors
      long dnum = (p_desc == nullptr) ? 0L : p_desc->getdsiz ();
      // create the indexes array
      long* lidx = (dnum == 0L) ? nullptr : new long[dnum];
      // loop in the descriptors
      long    kidx = 0L;
      s_sdsc* desc = p_desc;
      while (desc != nullptr) {
	lidx[kidx++] = desc->getlidx ();
	desc = desc->p_next;
      }
      unlock ();
      return lidx;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // open a structure block

  void Structure::open (void) {
    wrlock ();
    try {
      // check for a structure block
      if (p_blok == nullptr) {
	p_blok = new s_sblk;
	// update the structure block
	p_blok->update (p_desc);
      }
      // open the block
      if (p_blok->open (p_desc) == false) {
	throw Exception ("structure-error", "cannot open structure");
      }
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // close a structure block

  void Structure::close (void) {
    wrlock ();
    try {
      // check for a structure block
      if (p_blok == nullptr) {
	p_blok = new s_sblk;
	// update the structure block
	p_blok->update (p_desc);
      }
      // close the structure block
      if (p_blok->close () == true) {
	Buffer::add ((char*) p_blok->p_sblk, p_blok->d_ssiz);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an object to the structure

  bool Structure::add (Object* xobj) {
    // check for a valid object
    if (xobj == nullptr) return false;
    // lock and add
    wrlock ();
    try {
      // check if the structure is open
      s_sdsc* desc = (p_blok == nullptr) ? nullptr : p_blok->p_desc;
      if (desc == nullptr) {
	throw Exception ("structure-error", "closed structure in add");
      }
      // the mapping buffer
      t_byte xbuf[256]; long xsiz = 0L;
      // check for an integer
      auto iobj = dynamic_cast<Integer*>(xobj);
      if (iobj != nullptr) {
	// map the integer value
	t_long ival = iobj->tolong ();
	t_real rval = (t_real) ival;
	// map the transfer buffer
	xsiz = desc->setxbuf(xbuf, ival, rval);
      }
      // check for a real
      auto dobj = dynamic_cast<Real*>(xobj);
      if (dobj != nullptr) {
	// map the real value
	t_real rval = dobj->toreal ();
	t_long ival = (t_long) rval;
	// map the transfer buffer
	xsiz = desc->setxbuf(xbuf, ival, rval);
      }
      // check for a string
      auto sobj = dynamic_cast<String*>(xobj);
      if (sobj != nullptr) {
	// map the string value
	t_long ival = Utility::tolong (sobj);
	t_real rval = Utility::toreal (sobj);
	// map the transfer buffer
	xsiz = desc->setxbuf(xbuf, ival, rval);
      }
      if (xsiz == 0L) {
	throw Exception ("structure-error", "invalid object to add",
			 Object::repr (xobj));
      }
      // update the block by buffer
      bool result = p_blok->setxbuf(xbuf, xsiz);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object by coordinates

  Object* Structure::get (const long sidx, const long didx) const {
    rdlock ();
    try {
      // check for valid structure
      if ((p_desc == nullptr) || (p_blok == nullptr)) {
	throw Exception ("structure-error", "invalid null structure in get");
      }
      // check for valid index
      if ((sidx < 0L) || (didx < 0L)) {
	throw Exception ("structure-error", "invalid structure coordinates");
      }
      // compute the structure offset
      long soff = sidx * p_blok->d_ssiz + p_desc->getdoff(didx);
      // get the locale by index
      Locale locl = p_desc->getmloc (didx);
      // get the locale format and modifier
      Locale::t_ofmt ofmt = Locale::toofmt (locl.getofmt ());
      Locale::t_omod omod = Locale::toomod (locl.getomod ());
      // preset result
      Object* result = nullptr;
      // check for an integer
      if (ofmt == Locale::OFMT_INTG) {
	if (omod == Locale::OMOD_NONE) {
	  union {
	    t_long ival;
	    t_byte bval[sizeof(t_long)];
	  };
	  for (long k = 0; k < (long) sizeof(bval); k++) {
	    bval[k] = Buffer::get(soff+k);
	  }
	  result = new Integer(ival);
	}	    
	if (omod == Locale::OMOD_SHRT) {
	  union {
	    int    ival;
	    t_byte bval[sizeof(int)];
	  };
	  for (long k = 0; k < (long) sizeof(bval); k++) {
	    bval[k] = Buffer::get(soff+k);
	  }
	  result = new Integer((long) ival);
	}
      }
      if (ofmt == Locale::OFMT_REAL) {
	if (omod == Locale::OMOD_NONE) {
	  union {
	    t_real rval;
	    t_byte bval[sizeof(t_real)];
	  };
	  for (long k = 0; k < (long) sizeof(bval); k++) {
	    bval[k] = Buffer::get(soff+k);
	  }
	  result = new Real(rval);
	}
	if (omod == Locale::OMOD_SHRT) {
	  union {
	    float  fval;
	    t_byte bval[sizeof(float)];
	  };
	  for (long k = 0; k < (long) sizeof(bval); k++) {
	    bval[k] = Buffer::get(soff+k);
	  }
	  result = new Real((t_real) fval);
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // fill a structure by record size

  bool Structure::fill (const long size) {
    wrlock ();
    char* xbuf = nullptr;
    try {
      // check for a structure block
      if (p_blok == nullptr) {
	p_blok = new s_sblk;
	// update the structure block
	p_blok->update (p_desc);
      }
      if (p_blok->d_ssiz == 0L) {
	unlock ();
	return false;
      }
      xbuf = new char[p_blok->d_ssiz];
      for (long k = 0L; k < p_blok->d_ssiz; k++) xbuf[k] = nilc;
      for (long k = 0L; k < size; k++) {
	Buffer::add (xbuf, p_blok->d_ssiz);
      }
      delete [] xbuf;
      unlock ();
      return true;
    } catch (...) {
      delete [] xbuf;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 8;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_FILL    = zone.intern ("fill");
  static const long QUARK_OPEN    = zone.intern ("open");
  static const long QUARK_CLOSE   = zone.intern ("close");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_ADDLDEF = zone.intern ("add-locale");
  static const long QUARK_SETSPAD = zone.intern ("set-padding");

  // create a new object in a generic way
  
  Object* Structure::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Structure;
    // invalid arguments
    throw Exception ("argument-error", "too many argument for structure");
  }

  // return true if the given quark is defined

  bool Structure::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Buffer::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Structure::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_OPEN) {
	open ();
	return nullptr;
      }
      if (quark == QUARK_CLOSE) {
	close ();
	return nullptr;
      }
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	Object* xobj = argv->get (0);
        return new Boolean (add (xobj));
      }
      if (quark == QUARK_FILL) {
	long size = argv->getlong (0);
        return new Boolean (fill (size));
      }
      if (quark == QUARK_ADDLDEF) {
        String ldef = argv->getstring (0);
        return new Boolean (addldef (ldef));
      }
      if (quark == QUARK_SETSPAD) {
        long spad = argv->getlong (0);
	setspad (spad);
	return nullptr;
      }
    }
    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_GET) {
	long sidx = argv->getlong (0);
	long didx = argv->getlong (1);
	return get (sidx, didx);
      }
    }
    // call the buffer method
    return Buffer::apply (zobj, nset, quark, argv);
  }
}
