// ---------------------------------------------------------------------------
// - LinearFactory.cpp                                                       -
// - afnix:mth module - linear solver factory class implementation           -
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

#include "Linear.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "LinearFactory.hpp"
 
namespace afnix {
 
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default linear factory

  LinearFactory::LinearFactory (void) {
    d_avf = Linear::AVF_DEF;
    d_pcf = Linear::PCF_DEF;
  }
  
  // create a linear factory by flag

  LinearFactory::LinearFactory (Logger* slg) : SolverFactory (slg) {
    d_avf = Linear::AVF_DEF;
    d_pcf = Linear::PCF_DEF;
  }

  // create a linear factory by type

  LinearFactory::LinearFactory (const String& lst) {
    d_avf = Linear::AVF_DEF;
    d_pcf = Linear::PCF_DEF;
    d_lst = lst;
  }

  // create a linear solver by flags

  LinearFactory::LinearFactory (const bool avf, const bool pcf) {
    d_avf = avf;
    d_pcf = pcf;
  }

  // create a linear solver by flags and type

  LinearFactory::LinearFactory (const bool avf, const bool pcf, 
				const String& lst) {
    d_avf = avf;
    d_pcf = pcf;
    d_lst = lst;
  }
  // copy construct this object

  LinearFactory::LinearFactory (const LinearFactory& that) {
    that.rdlock ();
    try {
      // assign base factory
      SolverFactory::operator = (that);
      // copy locally
      d_avf = that.d_avf;
      d_pcf = that.d_pcf;
      d_lst = that.d_lst;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name
  
  String LinearFactory::repr (void) const {
    return "LinearFactory";
  }
  
  // return a clone of this object
  
  Object* LinearFactory::clone (void) const {
    return new LinearFactory (*this);
  }

  // assign an object to this one

  LinearFactory& LinearFactory::operator = (const LinearFactory& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base factory
      SolverFactory::operator = (that);
      // assign locally
      d_avf = that.d_avf;
      d_pcf = that.d_pcf;
      d_lst = that.d_lst;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // create a new solver

  Solver* LinearFactory::newslv (void) const {
    rdlock ();
    try {
      // get the solver type
      Linear::t_styp styp = Linear::tostyp (d_lst);
      // create a linear solver
      Solver* result = new Linear (d_avf, d_pcf, styp);
      result->setslg (p_slg);
      // done
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

  // create a new object in a generic way
  
  Object* LinearFactory::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new LinearFactory;    
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a logger
      Logger* slg = dynamic_cast <Logger*> (obj);
      if (slg != nullptr) return new LinearFactory (slg);
      // check for a string
      String *sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new LinearFactory (*sobj);
      // invalid object
      throw Exception ("type-error", "invalid object for linear factory",
		       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      bool avf = argv->getbool (0);
      bool pcf = argv->getbool (1);
      return new LinearFactory (avf, pcf);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with linear factory");
  }
}
