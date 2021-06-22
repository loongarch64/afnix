// ---------------------------------------------------------------------------
// - Form.cpp                                                                -
// - afnix engine - form class implementation                                -
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

#include "Form.hpp"
#include "Engsid.hxx"
#include "Integer.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  Form::Form (void) {
    d_lnum = 0;
  }

  // create a new form with a type

  Form::Form (const t_cctp cctp) : Cons (cctp) {
    d_lnum = 0;
  }

  // create a new form with a type and line number

  Form::Form (const t_cctp cctp, const long lnum) : Cons (cctp) {
    d_lnum = lnum;
  }

  // create a form with a car
  
  Form::Form (Object* car) : Cons (car) {
    d_lnum = 0;
  }

  // create a form with a type and a car
  
  Form::Form (const t_cctp cctp, Object* car) : Cons (cctp, car) {
    d_lnum = 0;
  }

  // copy constructor for this form

  Form::Form (const Form& that) {
    that.rdlock ();
    try {
      // assign the base cons cell
      Cons::operator = (that);
      // copy locally
      d_name = that.d_name;
      d_lnum = that.d_lnum;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // return the class name

  String Form::repr (void) const {
    return "Form";
  }

  // return a clone of this object

  Object* Form::clone (void) const {
    return new Form (*this);
  }

  // assign a form to another one

  Form& Form::operator = (const Form& that) {
    if (this == &that) return *this;
    wrlock ();
    that.rdlock ();
    try {
      Cons::operator = (that);
      d_name = that.d_name;
      d_lnum = that.d_lnum;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the serial did

  t_word Form::getdid (void) const {
    return SRL_DEOD_ENG;
  }

  // return the serial sid

  t_word Form::getsid (void) const {
    return SRL_FORM_SID;
  }

  // serialize this form

  void Form::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize base cell
      Cons::wrstream  (os);
      // serialize locally
      d_name.wrstream (os);
      Serial::wrlong (d_lnum, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this form

  void Form::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize base cell
      Cons::rdstream (is);
      // deserialize locally
      d_name.rdstream (is);
      d_lnum = Serial::rdlong (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the form information
  
  void Form::setinfo (const String& name, const long lnum) {
    wrlock ();
    try {
      d_name = name;
      d_lnum = lnum;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the form file name
  
  void Form::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the form file name

  String Form::getname (void) const {
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

  // set the form line number
  
  void Form::setlnum (const long lnum) {
    wrlock ();
    try {
      d_lnum = lnum;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the form line number

  long Form::getlnum (void) const {
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

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // evaluate this object in the current nameset

  Object* Form::eval (Evaluable* zobj, Nameset* nset) {
    try {
      return Cons::eval (zobj, nset);
    } catch (Exception& e) {
      e.updname (getname ());
      e.updlnum (getlnum ());
      throw;
    }
  }
}
