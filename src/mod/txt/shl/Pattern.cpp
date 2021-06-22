// ---------------------------------------------------------------------------
// - Pattern.hpp                                                             -
// - afnix:txt module - pattern description class implementation             -
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

#include "Item.hpp"
#include "Vector.hpp"
#include "Pattern.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputString.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this structure permits to operate with an input stream that
  // is prefixed by a string.
  struct s_pis {
    // the input stream
    InputStream* p_is;
    // the prefix string
    String d_ps;
    // the prefix stream
    InputString d_ip;
    // the escape character
    t_quad d_ec;
    // the read buffer
    String d_buf;
    // construct by stream
    s_pis (InputStream* is) {
      p_is = is;
      d_ps = "";
      d_ec = nilq;
      reset ();
    }
    // construct by stream and prefix
    s_pis (InputStream* is, const String ps) {
      p_is = is;
      d_ec = nilq;
      d_ps = ps;
      reset ();
    }
    // construct by stream, prefix and escape character
    s_pis (InputStream* is, const String ps, const t_quad ec) {
      p_is = is;
      d_ec = nilq;
      d_ps = ps;
      d_ec = ec;
      reset ();
    }
    // reset the prefixed stream
    void reset (void) {
      d_ip.set (d_ps);
      d_buf = "";
    }
    // check for an escape character
    bool isesc (const t_quad c) const {
      if (d_ec == nilq) return false;
      return (d_ec == c);
    }
    // read a character
    t_quad getu (void) {
      // check first the prefix
      if (d_ip.iseos () == false) {
	return d_ip.getu ();
      }
      // check the stream
      if (!p_is) return eosq;
      t_quad uc = p_is->getu ();
      if (uc == eosq) return uc;
      // save in the pushback buffer
      d_buf = d_buf + uc;
      return uc;
    }
    // restore the input stream
    void restore (void) {
      if (!p_is) return;
      if (d_buf.length () != 0) p_is->pushback (d_buf);
    }
    // check for a pattern and eventually consume
    bool check (const String& pat, const bool pflg) {
      // the check buffer
      String buf;
      // iterate in the pattern
      long  len = pat.length ();
      for (long  i = 0; i < len; i++) {
	// update string pattern
	t_quad uc = getu ();
	buf = buf + uc;
	// check for escape
	if (isesc (uc) == true) {
	  d_ip.InputStream::pushback (buf);
	  return false;
	}
	// check for position
	if (pat[i] != uc) {
	  d_ip.InputStream::pushback (buf);
	  return false;
	}
      }
      if (pflg == true) d_ip.InputStream::pushback (buf);
      return true;
    }
    // try to match with a start and end string patterns in balanced mode
    String bmode (const String& sbs, const String& ebs) {
      // check for initial matching
      if (check (sbs, false) == false) {
	restore ();
	return "";
      }
      // initialize result
      String result = sbs;
      // accumulate until end match
      while (check (ebs, true) == false) {
	// get next character or escape
	t_quad uc = getu ();
	// chekc for eos
	if (uc == eosq) {
	  restore ();
	  return "";
	}
	// check for escape
	if (isesc (uc) == true) {
	  t_quad nc = getu ();
	  if (nc == eosq) {
	    restore ();
	    return "";
	  }
	  result = result + uc;
	  uc = nc;
	}
	// add the character
	result = result + uc;
      }
      // the match occurs
      result = result + ebs;
      return result;
    }
    // try to match with a start and end string patterns in recursive mode
    String rmode (const String& sbs, const String& ebs) {
      // the recursive counter
      long count = 0;
      // check for initial matching
      if (check (sbs, false) == false) {
	restore ();
	return "";
      }
      count++;
      // initialize result
      String result = sbs;
      // loop for data
      while (true) {
	// check for start string
	if (check (sbs, false) == true) {
	  result = result + sbs;
	  count++;
	  continue;
	}
	// check for end string
	if (check (ebs, false) == true) {
	  result = result + ebs;
	  count--;
	  if (count == 0) break;
	  continue;
	}
	// get next character or escape
	t_quad uc = getu ();
	// chekc for eos
	if (uc == eosq) {
	  restore ();
	  return "";
	}
	// check for escape
	if (isesc (uc) == true) {
	  t_quad nc = getu ();
	  if (nc == eosq) {
	    restore ();
	    return "";
	  }
	  result = result + uc;
	  uc = nc;
	}
	// add the character
	result = result + uc;
      }
      // the match occurs
      return result;
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty pattern
  
  Pattern::Pattern (void) {
    d_mode  = REGEX;
    d_sbs   = "";
    d_ebs   = "";
    d_name  = "";
    d_rtag  = -1;
    d_escc  = nilq;
  }

  // create a regex pattern by string
  
  Pattern::Pattern (const String& re) {
    d_mode  = REGEX;
    d_regex = re;
    d_sbs   = "";
    d_ebs   = "";
    d_name  = "";
    d_rtag  = -1;
    d_escc  = nilq;
  }

  // create a regex pattern by name and string
  
  Pattern::Pattern (const String& name, const String& re) {
    d_mode  = REGEX;
    d_regex = re;
    d_sbs   = "";
    d_ebs   = "";
    d_name  = name;
    d_rtag  = -1;
    d_escc  = nilq;
  }

  // create a pattern by name and regex
  
  Pattern::Pattern (const String& name, const Regex& re) {
    d_mode  = REGEX;
    d_regex = re;
    d_sbs   = "";
    d_ebs   = "";
    d_name  = name;
    d_rtag  = -1;
    d_escc  = nilq;
  }

  // create a balanced pattern by name, control string and escape character
  
  Pattern::Pattern (const String& name, const String& cs, const t_quad escc) {
    d_mode  = BMODE;
    d_sbs   = cs;
    d_ebs   = cs;
    d_name  = name;
    d_rtag  = -1;
    d_escc  = escc;
  }

  // create a balanced pattern by name, control strings and escape character
  
  Pattern::Pattern (const String& name, const String& sbs, const String& ebs,
		    const t_quad escc) {
    d_mode  = BMODE;
    d_sbs   = sbs;
    d_ebs   = ebs;
    d_name  = name;
    d_rtag  = -1;
    d_escc  = escc;
  }

  // create a balanced pattern by name and control strings
  
  Pattern::Pattern (const String& name, const String& sbs, const String& ebs) {
    d_mode  = BMODE;
    d_sbs   = sbs;
    d_ebs   = ebs;
    d_name  = name;
    d_rtag  = -1;
    d_escc  = nilq;
  }

  // create a pattern by name, control strings and flags
  
  Pattern::Pattern (const String& name, const String& sbs, const String& ebs,
		    const bool rfl) {
    d_mode  = rfl ? RMODE : BMODE;
    d_sbs   = sbs;
    d_ebs   = ebs;
    d_name  = name;
    d_rtag  = -1;
    d_escc  = nilq;
  }

  // copy construct this regex element

  Pattern::Pattern (const Pattern& that) {
    that.rdlock ();
    try {
      d_mode  = that.d_mode;
      d_regex = that.d_regex;
      d_sbs   = that.d_sbs;
      d_ebs   = that.d_ebs;
      d_name  = that.d_name;
      d_rtag  = that.d_rtag;
      d_escc  = that.d_escc;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Pattern::repr (void) const {
    return "Pattern";
  }

  // return a clone of this object

  Object* Pattern::clone (void) const {
    return new Pattern (*this);
  }

  // assign a pattern to this one

  Pattern& Pattern::operator = (const Pattern& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    that.rdlock ();
    wrlock ();
    try {
      // assign the pattern to this one
      d_mode  = that.d_mode;
      d_regex = that.d_regex;
      d_sbs   = that.d_sbs;
      d_ebs   = that.d_ebs;
      d_name  = that.d_name;
      d_rtag  = that.d_rtag;
      d_escc  = that.d_escc;
      // unlock everything
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // set the pattern with a regex string

  void Pattern::setregex (const String& re) {
    wrlock ();
    try {
      d_mode  = REGEX;
      d_regex = re;
      d_sbs   = "";
      d_ebs   = "";
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the pattern with a regex object

  void Pattern::setregex (const Regex& re) {
    wrlock ();
    try {
      d_mode  = REGEX;
      d_regex = re;
      d_sbs   = "";
      d_ebs   = "";
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the pattern with the balanced control strings

  void Pattern::setbcs (const String& sbs, const String& ebs) {
    wrlock ();
    try {
      d_mode  = BMODE;
      d_regex = "";
      d_sbs   = sbs;
      d_ebs   = sbs;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the pattern with the escape character

  void Pattern::setesc (const t_quad escc) {
    wrlock ();
    try {
      d_escc = escc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the escape character

  t_quad Pattern::getesc (void) const {
    rdlock ();
    try {
      t_quad escc = d_escc;
      unlock ();
      return escc;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the pattern name
  
  void Pattern::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the pattern name
  
  String Pattern::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the pattern tag

  void Pattern::settag (const long rtag) {
    wrlock ();
    try {
      d_rtag = rtag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
   
  // return the pattern tag
  
  long Pattern::gettag (void) const {
    rdlock ();
    try {
      long result = d_rtag;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check a string with this pattern

  bool Pattern::check (const String& s) const {
    // lock and check for recursive consistency mode
    rdlock ();
    try {
      if ((d_mode == RMODE) && (d_sbs == d_ebs)) {
	throw Exception ("pattern-error", 
			 "recursive mode with same delimiters");
      }
      bool result = false;
      switch (d_mode) {
      case REGEX:
	result = (d_regex == s);
	break;
      case BMODE: 
	{
	  s_pis pis (nullptr, s, d_escc);
	  result = (pis.bmode (d_sbs, d_ebs) == s);
	}
	break;
      case RMODE: 
	{
	  s_pis pis (nullptr, s, d_escc);
	  result = (pis.rmode (d_sbs, d_ebs) == s);
	}
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // match an input stream with this pattern

  String Pattern::match (InputStream* is) const {
    rdlock ();
    try {
      String result = match (is, "");
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // match an input stream with this pattern and a prefix

  String Pattern::match (InputStream* is, const String& ps) const {
    // lock and check for recursive consistency mode
    rdlock ();
    try {
      if ((d_mode == RMODE) && (d_sbs == d_ebs)) {
	throw Exception ("pattern-error", 
			 "recursive mode with same delimiters");
      }
      String result = "";
      switch (d_mode) {
      case REGEX:
	result = d_regex.match (is, ps);
	break;
      case BMODE: 
	{
	  s_pis pis (is, ps, d_escc);
	  result = pis.bmode (d_sbs, d_ebs);
	}
	break;
      case RMODE: 
	{
	  s_pis pis (is, ps, d_escc);
	  result = pis.rmode (d_sbs, d_ebs);
	}
	break;
      }
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

  // the object eval quarks
  static const long QUARK_REGEX     = String::intern ("REGEX");
  static const long QUARK_PATTERN   = String::intern ("Pattern");
  static const long QUARK_BALANCED  = String::intern ("BALANCED");
  static const long QUARK_RECURSIVE = String::intern ("RECURSIVE");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 10;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_CHECK     = zone.intern ("check");
  static const long QUARK_MATCH     = zone.intern ("match");
  static const long QUARK_SETBCS    = zone.intern ("set-balanced");
  static const long QUARK_SETESC    = zone.intern ("set-escape");
  static const long QUARK_GETESC    = zone.intern ("get-escape");
  static const long QUARK_SETPTAG   = zone.intern ("set-tag");
  static const long QUARK_GETPTAG   = zone.intern ("get-tag");
  static const long QUARK_SETNAME   = zone.intern ("set-name");
  static const long QUARK_GETNAME   = zone.intern ("get-name");
  static const long QUARK_SETREGEX  = zone.intern ("set-regex");

  // evaluate a quark statically

  Object* Pattern::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_REGEX) 
      return new Item (QUARK_PATTERN, QUARK_REGEX);
    if (quark == QUARK_BALANCED) 
      return new Item (QUARK_PATTERN, QUARK_BALANCED);
    if (quark == QUARK_RECURSIVE) 
      return new Item (QUARK_PATTERN, QUARK_RECURSIVE);
    throw Exception ("eval-error", "cannot evaluate member",
		     String::qmap (quark));
  }

  // create a new object in a generic way

  Object* Pattern::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Pattern;
    // check for 1 argument
    if (argc == 1) {
      String re = argv->getstring (0);
      return new Pattern (re);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the regex name
      String name = argv->getstring (0);
      // get the object and dispatch
      Object* obj = argv->get (1);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Pattern (name, *sobj);
      // check for a regex
      Regex* zobj = dynamic_cast <Regex*> (obj);
      if (zobj != nullptr) return new Pattern (name, *zobj);
      throw Exception ("argument-error", "invalid arguments with pattern");
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);    
      String cs   = argv->getstring (1);
      // get the object and select
      Object* obj = argv->get (2);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Pattern (name, cs, *sobj);
      // check for a character
      Character* cobj = dynamic_cast <Character*> (obj);
      if (cobj != nullptr) {
	t_quad escc = cobj->toquad ();
	return new Pattern (name, cs, escc);
      }
      throw Exception ("argument-error", "invalid arguments with pattern");
    }
    // check for 4 arguments
    if (argc == 4) {
      String name = argv->getstring (0);    
      String sbs  = argv->getstring (1);
      String ebs  = argv->getstring (2);
      // get the object and select
      Object* obj = argv->get (3);
      // check for a character
      Character* cobj = dynamic_cast <Character*> (obj);
      if (cobj != nullptr) {
	t_quad escc = cobj->toquad ();
	return new Pattern (name, sbs, ebs, escc);
      }
      // check for a boolean
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj != nullptr) {
	bool rfl = bobj->tobool ();
	return new Pattern (name, sbs, ebs, rfl);
      }
      throw Exception ("argument-error", "invalid arguments with pattern");
    }
    throw Exception ("argument-error", "too many arguments with pattern");
  }

  // return true if the given quark is defined

  bool Pattern::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark
  
  Object* Pattern::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETESC)  return new Character (getesc ());
      if (quark == QUARK_GETNAME) return new String    (getname ());
      if (quark == QUARK_GETPTAG) return new Integer   (gettag  ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETPTAG) {
	long tag = argv->getlong (0);
	settag (tag);
	return nullptr;
      }
      if (quark == QUARK_SETESC) {
	t_quad escc = argv->getchar (0);
	setesc (escc);
	return nullptr;
      }
      if (quark == QUARK_CHECK) {
	String sval = argv->getstring (0);
	return new Boolean (check (sval));
      }
      if (quark == QUARK_MATCH) {
	Object*  obj = argv->get (0);
	// check for a stream
	InputStream*  iobj = dynamic_cast <InputStream*> (obj);
	if (iobj != nullptr) return new String (match (iobj));
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return new String (match (nullptr, *sobj));
	// argument error
	throw Exception ("type-error", "invalid object with match ", 
			 Object::repr (obj));
      }
      if (quark == QUARK_SETREGEX) {
	Object* obj = argv->get (0);
	String* sre = dynamic_cast <String*> (obj);
	if (sre != nullptr) {
	  setregex (*sre);
	  return nullptr;
	}
	Regex* ore = dynamic_cast <Regex*> (obj);
	if (ore != nullptr) {
	  setregex (*ore);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with set-regex ", 
			 Object::repr (obj));
      }
      if (quark == QUARK_SETBCS) {
	String bs = argv->getstring (0);
	setbcs (bs, bs);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_MATCH) {
	// get the input stream
	Object*  obj = argv->get (0);
	InputStream* iobj = dynamic_cast <InputStream*> (obj);
	// get the prefix
	String ps = argv->getstring (1);
	if (iobj != nullptr) return new String (match (iobj, ps));
	throw Exception ("type-error", "invalid object with match ", 
			 Object::repr (obj));
      }
      if (quark == QUARK_SETBCS) {
	String sbs = argv->getstring (0);
	String ebs = argv->getstring (1);
	setbcs (sbs, ebs);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
