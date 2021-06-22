// ---------------------------------------------------------------------------
// - Csv.cpp                                                                 -
// - afnix:sps module - csv importer class implementation                    -
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

#include "Csv.hpp"
#include "Lexer.hpp"
#include "Locale.hpp"
#include "Strvec.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "InputFile.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // -  private section                                                      -
  // -------------------------------------------------------------------------

  // default line number
  static const long   CSV_LNUM_DEF = 0L;
  // default break sequence
  static const String CSV_SBRK_DEF = ",;\t";

  // this procedure create a lexical object from a string
  static Object* csv_new_lobj (const String& lval) {
    // create a lexical anayzer
    Lexer lexr (lval);
    // get the next token
    Token tokn = lexr.get ();
    // check for nil token
    Token::t_token tid = tokn.gettid ();
    if (tid == Token::EOS) return nullptr;
    // check for standard token
    if ((tid == Token::REAL)    || (tid == Token::STRING)  ||
	(tid == Token::BOOLEAN) || (tid == Token::INTEGER) ||
	(tid == Token::RELATIF) || (tid == Token::CHARACTER)) {
      // check for full parsing
      Token ntok = lexr.get ();
      if (ntok.gettid () == Token::EOS) {
	return tokn.getobj ();
      } else {
	return new String (lval);
      }
    } 
    return new String (lval);
  }

  // this procedure creates a lexical vector a string vector
  static Vector* csv_new_lvec (const Strvec& sv, const Vector& lv) {
    // get length and and form
    long vlen = sv.length ();
    if (vlen == 0L) return nullptr;
    // extract the locale length
    long llen = lv.length ();
    // loop for vector building
    Vector* lvec = new Vector;
    for (long k = 0L; k < vlen; k++) {
      String lval = sv.get(k).strip ();
      if (lval.isnil () == true) {
	lvec->add (nullptr);
	continue;
      }
      // eventually get the locale
      Locale* lo = dynamic_cast <Locale*> ((k < llen) ? lv.get (k) : nullptr);
      // the literal object to add
      Object* lobj = (lo == nullptr) ?
	csv_new_lobj (lval) : lo->toobject (lval);
      // add the object to the vector
      lvec->add (lobj);
    }
    return lvec;
  }
  
  // this procedure creates a form from a string vector
  static Form* csv_new_form (const Strvec& sv, const Vector& lv) {
    // get length and and form
    long vlen = sv.length ();
    if (vlen == 0L) return nullptr;
    // extract the locale length
    long llen = lv.length ();
    // loop for form building
    Form* form = nullptr;
    for (long k = 0L; k < vlen; k++) {
      String lval = sv.get(k).strip ();
      if (lval.isnil () == true) continue;
      // eventually get the locale
      Locale* lo = dynamic_cast <Locale*> ((k < llen) ? lv.get (k) : nullptr);
      // the literal object to add
      Object* lobj = (lo == nullptr) ?
	csv_new_lobj (lval) : lo->toobject (lval);
      if (lobj == nullptr) continue;
      // add the object to the form
      if (form == nullptr) {
	form = new Form (lobj);
      } else {
	form->add (lobj);
      }
    }
    return form;
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a default csv reader

  Csv::Csv (void) {
    p_is   = nullptr;
    d_lnum = CSV_LNUM_DEF;    
    d_sbrk = CSV_SBRK_DEF;
  }

  // create a csv reader by stream

  Csv::Csv (InputStream* is) {
    Object::iref (p_is = is);
    d_lnum = CSV_LNUM_DEF;    
    d_sbrk = CSV_SBRK_DEF;
  }
  
  // create a csv reader by stream name
  
  Csv::Csv (const String& name) {
    Object::iref (p_is = new InputFile (name));
    d_name = name;
    d_lnum = CSV_LNUM_DEF;
    d_sbrk = CSV_SBRK_DEF;
  }

  // create a csv reader by stream and break sequence

  Csv::Csv (InputStream* is, const String& sbrk) {
    Object::iref (p_is = is);
    d_lnum = CSV_LNUM_DEF;    
    d_sbrk = sbrk;
  }

  // create a csv reader by stream, break sequence and locale

  Csv::Csv (InputStream* is, const String& sbrk, const Vector& locv) {
    Object::iref (p_is = is);
    d_lnum = CSV_LNUM_DEF;    
    d_sbrk = sbrk;
    d_locv = locv;
  }

  // create a csv reader by name and break sequence
  
  Csv::Csv (const String& name, const String& sbrk) {
    Object::iref (p_is = new InputFile (name));
    d_name = name;
    d_lnum = CSV_LNUM_DEF;
    d_sbrk = sbrk;
  }
  
  // destroy this csv reader

  Csv::~Csv (void) {
    Object::dref (p_is);
  }

  // return the object name

  String Csv::repr (void) const {
    return "Csv";
  }

  // check for a valid parser

  bool Csv::valid (void) const {
    wrlock ();
    try {
      bool result = (p_is == nullptr) ? false : p_is->valid ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // split a line from this reader

  Vector* Csv::split (void) {
    wrlock ();
    Vector* lvec = nullptr;
    try {
      // read a line from the stream
      String line = (p_is == nullptr) ? "" : p_is->readln ();
      if (line.isnil () == true) {
	unlock ();
	return nullptr;
      }
      d_lnum++;
      // split and parse
      Strvec sv = Strvec::split (line, d_sbrk);
      // create the form 
      lvec = csv_new_lvec (sv, d_locv);
      unlock ();
      return lvec;
    } catch (...) {
      delete lvec;
      unlock ();
      throw;
    }
  }

  // get a form from this reader

  Form* Csv::parse (void) {
    wrlock ();
    Form* form = nullptr;
    try {
      // read a line from the stream
      String line = (p_is == nullptr) ? "" : p_is->readln ();
      if (line.isnil () == true) {
	unlock ();
	return nullptr;
      }
      d_lnum++;
      // split and parse
      Strvec sv = Strvec::split (line, d_sbrk);
      // create the form 
      form = csv_new_form (sv, d_locv); form->setlnum (d_lnum);
      unlock ();
      return form;
    } catch (...) {
      delete form;
      unlock ();
      throw;
    }
  }
  
  // get the reader line number

  long Csv::getlnum (void) const {
    rdlock ();
    try {
      long result = d_lnum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the input stream name

  String Csv::getname (void) const {
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

  // set the reader break sequence

  void Csv::setsbrk (const String& sbrk) {
    wrlock ();
    try {
      d_sbrk = sbrk;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the reader line number
  
  String Csv::getsbrk (void) const {
    rdlock ();
    try {
      String result = d_sbrk;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the locale vector

  void Csv::setlocv (const Vector& locv) {
    wrlock ();
    try {
      d_locv = locv;
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SPLIT    = zone.intern ("split");
  static const long QUARK_VALIDP   = zone.intern ("valid-p");
  static const long QUARK_SETSBRK  = zone.intern ("set-break-sequence");
  static const long QUARK_GETSBRK  = zone.intern ("get-break-sequence");

  // create a new object in a generic way

  Object* Csv::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Csv;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new Csv (is);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Csv (*sobj);
      // invalid type
      throw Exception ("type-error", "invalid argument with csv reader");
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get (0);
      String sbrk = argv->getstring (1);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new Csv (is, sbrk);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Csv (*sobj, sbrk);
      // invalid type
      throw Exception ("type-error", "invalid argument with csv reader");
    }
    throw Exception ("argument-error", "too many arguments with csv reader");
  }

  // return true if the given quark is defined

  bool Csv::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      // check the nameable class
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      // check the former class
      if (result == false) {
	result = hflg ? Former::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Csv::apply (Evaluable* zobj, Nameset* nset, const long quark, 
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_SPLIT)   return split ();
      if (quark == QUARK_VALIDP)  return new Boolean (valid ());
      if (quark == QUARK_GETSBRK) return new String (getsbrk ());
    }    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETSBRK) {
	String sbrk = argv->getstring (0);
	setsbrk (sbrk);
	return nullptr;
      }
    }
    // check the nameable class
    if (Nameable::isquark (quark, true) == true) {
      return Nameable::apply (zobj, nset, quark, argv);
    }
    // fallback with the former method
    return Former::apply (zobj, nset, quark, argv);
  }
}
