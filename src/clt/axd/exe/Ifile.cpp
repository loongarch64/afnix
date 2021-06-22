// ---------------------------------------------------------------------------
// - Ifile.cpp                                                               -
// - afnix cross debugger - instrumented file class implementation           -
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

#include "Ifile.hpp"
#include "Breakable.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // this procedure add a form in the line map table if and only if
  // the line is not binded
  static void add_form (QuarkTable& lmap, Iform* form) {
    if (form == nullptr) return;
    // get the line number and check
    long lnum = form->getlnum ();
    if (lmap.exists (lnum) == true) return;
    lmap.add (lnum, form);
  }

  // this procedure take a form, traverse it for other form and add them
  // in the line map - of course, the first form wins
  static void map_form (QuarkTable& lmap, Iform* form) {
    // add this top level form
    add_form (lmap, form);
    // start to traverse the form
    Cons* cons = form;
    while (cons != nullptr) {
      // get the car
      auto car = cons->getcar ();
      // check for an iform
      auto ifrm = dynamic_cast<Iform*>(car);
      if (ifrm != nullptr) {
	// continue with the cdr
	cons = cons->getcdr ();
	continue;
      }
      // check for a form
      auto form = dynamic_cast <Form*> (car);
      if (car != nullptr) map_form (lmap, new Iform (form));
      // continue with the cdr
      cons = cons->getcdr ();
    }
  }

  // create a new ifile by name with a module
  
  Ifile::Ifile (Module& mp) {
    d_fname = mp.getname ();
    while (true) {
      Form* form = mp.parse ();
      if (form == nullptr) break;
      Iform* ifrm = new Iform (form); delete form;
      map_form (d_lmap, ifrm);
      d_fvec.add (ifrm);
    }
  }

  // destroy this ifile

  Ifile::~Ifile (void) {
    d_lmap.reset ();
    d_fvec.reset ();
  }

  // return the class name
  
  String Ifile::repr (void) const {
    return "Ifile";
  }

  // return the i-file file name

  String Ifile::getfname (void) const {
    rdlock ();
    String result = d_fname;
    unlock ();
    return result;
  }

  // return the form vector length

  long Ifile::length (void) const {
    return d_fvec.length ();
  }

  // get a form by index

  Iform* Ifile::getform (const long index) const {
    rdlock ();
    try {
      auto form = dynamic_cast <Iform*> (d_fvec.get (index));
      if (form == nullptr) throw Exception ("ifile-error", "cannot get form");
      unlock ();
      return form;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // find a form by line number

  Iform* Ifile::lookup (const long lnum) const {
    if (lnum <  0) throw Exception ("lookup-error", "invalid line number");
    if (lnum == 0) return getform (0);
    rdlock ();
    try {
      // check if the form exists
      if (d_lmap.exists (lnum) == false) {
	unlock ();
	return nullptr;
      }
      auto form = dynamic_cast <Iform*> (d_lmap.get (lnum));
      unlock ();
      return form;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate this ifile withing the calling nameset
  
  Object* Ifile::eval (Evaluable* zobj, Nameset* nset) {
    // collect the beakable
    auto bobj = dynamic_cast<Breakable*>(zobj);
    // loop in the form vector
    long len = d_fvec.length ();
    Object* result = nullptr;
    for (long i = 0; i < len; i++) {
      Object* obj = d_fvec.get (i);
      if (obj == nullptr) continue;
      if ((bobj != nullptr) && (bobj->getnext () == true)) {
	bobj->setnext (false);
	bobj->bpoint  (nset, obj);
      }
      result = obj->eval (zobj, nset);
      zobj->post (result);
    }
    return result;
  }
}
