// ---------------------------------------------------------------------------
// - TlsShake.cpp                                                            -
// - afnix:tls service - tls handshake message class implementation          -
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

#include "Vector.hpp"
#include "Integer.hpp"
#include "TlsTypes.hxx"
#include "TlsShake.hpp"
#include "TlsHblock.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure computes the message length
  static long tls_msg_length (const TlsMessage* hobj, const long pos) {
    // check for valid arguments
    if ((hobj == nullptr) || (pos < 0L)) return 0L;
    // get the message byte
    t_byte bh = hobj->getbyte (pos);
    t_byte bm = hobj->getbyte (pos+1L);
    t_byte bl = hobj->getbyte (pos+2L);
    t_quad rl = (((t_quad) bh) << 16) + (((t_quad) bm) << 8) + ((t_quad) bl);
    // here it is - no sign extension
    return rl;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  TlsShake::TlsShake (void) {
    reset ();
  }

  // create a message by record

  TlsShake::TlsShake (TlsRecord* rcd) : TlsMessage (rcd) {
    // check valid type
    if (gettype () != TLS_TYPE_HSK) {
      throw Exception ("tls-error", "invalid type in handshake message");
    }
  }

  // create a message by type and version

  TlsShake::TlsShake (const t_byte vmaj, const t_byte vmin) :
    TlsMessage (TLS_TYPE_HSK ,vmaj, vmin) {
  }

  // copy construct this handshake

  TlsShake::TlsShake (const TlsShake& that) {
    that.rdlock ();
    try {
      // copy the message
      TlsMessage::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a handshake to this one

  TlsShake& TlsShake::operator = (const TlsShake& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the message
      TlsMessage::operator = (that);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // get the class name

  String TlsShake::repr (void) const {
    return "TlsShake";
  }

  // clone this object

  Object* TlsShake::clone (void) const {
    return new TlsShake (*this);
  }

  // reset the handshake

  void TlsShake::reset (void) {
    wrlock ();
    try {
      // reset base message
      TlsMessage::reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the message info as a plist

  Plist TlsShake::getinfo (void) const {
    rdlock ();
    try {
      // create a header result plist
      Plist plst = TlsHeader::getinfo ();
      // set the message info
      plst.add ("TLS-MSG-TYPE", "TLS HANDSHAKE", (t_long) gettype  ());
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a new tls iterator

  Iterator* TlsShake::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new TlsShakeit (this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // add chunk block by type
  
  void TlsShake::add (const t_byte type, const TlsChunk& chnk) {
    wrlock ();
    try {
      // create a working chunk
      TlsChunk wc;
      wc.add (type);
      // add the chunk length
      t_quad clen = chnk.length ();
      wc.add ((t_byte) ((clen & 0x00FF0000) >> 16));
      wc.add ((t_byte) ((clen & 0x0000FF00) >> 8));
      wc.add ((t_byte)  (clen & 0x000000FF));
      // merge the existing chunk
      wc.add (chnk);
      // add the chunk to the message
      TlsMessage::add (wc);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  /*
  // map the shake block as a cipher block

  TlsBlock* TlsShake::getblok (void) const
  {
    rdlock ();
    TlsHblock* result = nullptr;
    try {
      // create the data block
      t_byte type = getbyte (0L);
      long   mlen = tls_msg_length (this, 1L);
      result = new TlsHblock (p_brcd, type, 0L, mlen);
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }
  */
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* TlsShake::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsShake;
    // check for 1 argument
    if (argc == 1) {
      Object*    obj = argv->get (0);
      TlsRecord* rcd = dynamic_cast<TlsRecord*> (obj);
      if ((rcd == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid object as tls record",
			 Object::repr (obj));
      }
      return new TlsShake (rcd);
    }
    // too many arguments
    throw Exception ("argument-error", 
                     "too many argument with tls handshake constructor");
  }

  // return true if the given quark is defined

  bool TlsShake::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      bool result = hflg ? Iterable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? TlsMessage::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* TlsShake::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // check the iterable method
    if (Iterable::isquark (quark, true) == true) {
      return Iterable::apply (zobj, nset, quark, argv);
    }
    // call the tls message method
    return TlsMessage::apply (zobj, nset, quark, argv);
  }

  // -------------------------------------------------------------------------
  // - iterator section                                                      -
  // -------------------------------------------------------------------------

  // create a new handshake iterator

  TlsShakeit::TlsShakeit (TlsShake* hobj) {
    Object::iref (p_hobj = hobj);
    begin ();
  }

  // destroy this handshake iterator

  TlsShakeit::~TlsShakeit (void) {
    Object::dref (p_hobj);
  }

  // return the class name

  String TlsShakeit::repr (void) const {
    return "TlsShakeit";
  }  

  // reset the iterator to the begining

  void TlsShakeit::begin (void) {
    wrlock ();
    try {
      d_rpos = 0L;
      d_type = (p_hobj == nullptr) ? nilc : p_hobj->getbyte (d_rpos);
      d_mlen = tls_msg_length (p_hobj, d_rpos + 1L);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset the iterator to the end

  void TlsShakeit::end (void) {
    throw Exception ("tls-error", "unsupported tls message iterator end");
  }

  // go to the next object

  void TlsShakeit::next (void) {
    wrlock ();
    try {
      // get the record length and check
      long rlen = (p_hobj == nullptr) ? 0L : p_hobj->length ();
      if (d_rpos >= rlen) {
	unlock ();
	return;
      }
      // update to next position
      d_rpos += (4L + d_mlen);
      if (d_rpos < rlen) {
	d_type = (p_hobj == nullptr) ? nilc : p_hobj->getbyte (d_rpos);
	d_mlen = tls_msg_length (p_hobj, d_rpos + 1L);
	unlock ();
	return;
      }
      if (d_rpos > rlen) {
	throw Exception ("tls-error", "inconsistent message length");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // go to the previous object

  void TlsShakeit::prev (void) {
    throw Exception ("tls-error", "unsupported tls message iterator prev");
  }

  // get the object at the current position

  Object* TlsShakeit::getobj (void) const {
    rdlock ();
    try {
      Object* result = nullptr;
      if ((p_hobj != nullptr) && (isend () == false)) {
	result = new TlsHblock (p_hobj->p_brcd, d_type, d_rpos, d_mlen);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the iterator is at the end

  bool TlsShakeit::isend (void) const {
    rdlock ();
    try {
      // get the record length and check
      long rlen = (p_hobj == nullptr) ? 0L : p_hobj->length ();
      if (d_rpos > rlen) {
	throw Exception ("tls-error", "inconsistent message length");
      }
      bool result = (d_rpos == rlen) ? true : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }  
}
