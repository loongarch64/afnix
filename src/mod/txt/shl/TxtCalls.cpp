// ---------------------------------------------------------------------------
// - TxtCalls.cpp                                                            -
// - afnix:txt module - txt specific calls implementation                    -
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

#include "Cons.hpp"
#include "Vector.hpp"
#include "Sorter.hpp"
#include "Boolean.hpp"
#include "TxtCalls.hpp"
#include "TxtUtils.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function compare an object with another one in ascent mode
  static bool qsort_cmplth (Object* ref, Object* slv) {
    if (ref == nullptr) return true;
    Object*   obj = ref->oper (Object::OPER_LTH, slv);
    Boolean* bobj = dynamic_cast <Boolean*> (obj);
    bool result   = (bobj == nullptr) ? false : bobj->tobool ();
    Object::cref (obj);
    return result;
  }

  // this function compare an object with another one in descent mode
  static bool qsort_cmpgth (Object* ref, Object* slv) {
    if ((ref == nullptr) && (slv == nullptr)) return true;
    if ((ref == nullptr) && (slv != nullptr)) return false;
    Object*   obj = ref->oper (Object::OPER_GTH, slv);
    Boolean* bobj = dynamic_cast <Boolean*> (obj);
    bool result   = (bobj == nullptr) ? false : bobj->tobool ();
    Object::cref (obj);
    return result;
  }
 
  // this function compare an object with another one in lexicographic mode
  static bool qsort_cmplex (Object* ref, Object* slv) {
    if ((ref == nullptr) && (slv == nullptr)) return true;
    if ((ref == nullptr) && (slv != nullptr)) return false;
    // try to get a literal object
    Literal* rlit = dynamic_cast <Literal*> (ref);
    if (rlit == nullptr) 
      throw Exception ("type-error", "cannot map object to literal");
    Literal* slit = dynamic_cast <Literal*> (slv);
    if (slit == nullptr) 
      throw Exception ("type-error", "cannot map object to literal");
    // map it to string and compare
    String rstr = rlit->tostring ();
    String sstr = slit->tostring ();
    return rstr < sstr;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // indent a string with start and end controls

  Object* txt_indent (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 4) {
	String src = argv->getstring (0);
	String scc = argv->getstring (1);
	String ecc = argv->getstring (2);
	String ibs = argv->getstring (3);
	delete argv; argv = nullptr;
	return new String (TxtUtils::indent (src, scc, ecc, ibs));
      }
      throw Exception ("argument-error", "too many arguments with indent");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // quick sort ascending function

  Object* txt_qslth (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) {
      delete argv;
      throw Exception ("argument-error", "invalid arguments with sort-ascent");
    }
    // get the argument vector
    Vector* vobj = dynamic_cast <Vector*> (argv->get (0));
    if (vobj == nullptr) {
      delete argv;
      throw Exception ("type-error", "invalid object with sort-ascent");
    }
    // build the sorter object
    Sorter sorter (qsort_cmplth);
    // sort the vector
    try {
      sorter.qsort (vobj);
      delete argv;
      return nullptr;
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // quick sort ascending function

  Object* txt_qsgth (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) {
      delete argv;
      throw Exception ("argument-error", 
		       "invalid arguments with sort-descent");
    }
    // get the argument vector
    Vector* vobj = dynamic_cast <Vector*> (argv->get (0));
    if (vobj == nullptr) {
      delete argv;
      throw Exception ("type-error", "invalid object with sort-descent");
    }
    // build the sorter object
    Sorter sorter (qsort_cmpgth);
    // sort the vector
    try {
      sorter.qsort (vobj);
      delete argv;
      return nullptr;
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // quick sort lexicographic function

  Object* txt_qslex (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) {
      delete argv;
      throw Exception ("argument-error", 
		       "invalid arguments with sort-lexical");
    }
    // get the argument vector
    Vector* vobj = dynamic_cast <Vector*> (argv->get (0));
    if (vobj == nullptr) {
      delete argv;
      throw Exception ("type-error", "invalid object with sort-lexical");
    }
    // build the sorter object
    Sorter sorter (qsort_cmplex);
    // sort the vector
    try {
      sorter.qsort (vobj);
      delete argv;
      return nullptr;
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
