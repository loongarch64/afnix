// ---------------------------------------------------------------------------
// - Primality.cpp                                                           -
// - afnix:mth module - primality testing function implementations           -
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
#include "Real.hpp"
#include "Prime.hpp"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Relatif.hpp"
#include "Primality.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // check the fermat theorem

  Object* mth_frmtp (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc == 2) {
	// initialise test number
	Relatif a; Relatif p;
	// get the base number
	Object*  oa = argv->get (0);
	// check for an integer
	Integer* ia = dynamic_cast <Integer*> (oa);
	if (ia != nullptr) {
	  a = ia->tolong ();
	}
	// check for a relatif
	Relatif* ra = dynamic_cast <Relatif*> (oa);
	if (ra != nullptr) a = *ra;
	// type error with base
	if ((ia == nullptr) && (ra == nullptr)) {
	  throw Exception ("type-error", "invalid object with fermat-p",
			   Object::repr (oa));
	}
	// get the prime number to validate
	Object*  op = argv->get (1);
	// check for an integer
	Integer* ip = dynamic_cast <Integer*> (op);
	if (ip != nullptr) p = ip->tolong ();
	// check for a relatif
	Relatif* rp = dynamic_cast <Relatif*> (op);
	if (rp != nullptr) p = *rp;
	// type error with prime
	if ((ip == nullptr) && (rp == nullptr)) {
	  throw Exception ("type-error", "invalid object with fermat-p",
			   Object::repr (op));
	}
	// clean arguments
	delete argv; argv = nullptr;
	// compute result
	bool result = Prime::fermat (a, p);
	return new Boolean (result);
      }
      throw Exception ("argument-error", 
		       "too many arguments with fermat-p");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // check the miller-rabin primality test

  Object* mth_mrptp (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc == 2) {
	// initialise test number
	Relatif a; Relatif p;
	// get the base number
	Object*  oa = argv->get (0);
	// check for an integer
	Integer* ia = dynamic_cast <Integer*> (oa);
	if (ia != nullptr) {
	  a = ia->tolong ();
	}
	// check for a relatif
	Relatif* ra = dynamic_cast <Relatif*> (oa);
	if (ra != nullptr) a = *ra;
	// type error with base
	if ((ia == nullptr) && (ra == nullptr)) {
	  throw Exception ("type-error", "invalid object with miller-rabin-p",
			   Object::repr (oa));
	}
	// get the prime number to validate
	Object*  op = argv->get (1);
	// check for an integer
	Integer* ip = dynamic_cast <Integer*> (op);
	if (ip != nullptr) p = ip->tolong ();
	// check for a relatif
	Relatif* rp = dynamic_cast <Relatif*> (op);
	if (rp != nullptr) p = *rp;
	// type error with prime
	if ((ip == nullptr) && (rp == nullptr)) {
	  throw Exception ("type-error", "invalid object with miller-rabin-p",
			   Object::repr (op));
	}
	// clean arguments
	delete argv; argv = nullptr;
	// compute result
	bool result = Prime::mrpt (a, p);
	return new Boolean (result);
      }
      throw Exception ("argument-error", 
		       "too many arguments with miller-rabin-p");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // check if a number is a probable prime

  Object* mth_ppntp (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 1 argument
      if (argc == 1) {
	// initialise test number
	Relatif p;
	// get the base number
	Object*  op = argv->get (0);
	// check for an integer
	Integer* ip = dynamic_cast <Integer*> (op);
	if (ip != nullptr) p = ip->tolong ();
	// check for a relatif
	Relatif* rp = dynamic_cast <Relatif*> (op);
	if (rp != nullptr) p = *rp;
	// type error with prime number
	if ((ip == nullptr) && (rp == nullptr)) {
	  throw Exception ("type-error", "invalid object with prime-probable-p",
			   Object::repr (op));
	}
	// clean arguments
	delete argv; argv = nullptr;
	// compute result
	bool result = Prime::isprobable (p);
	return new Boolean (result);
      }
      // check for 2 arguments
      if (argc == 2) {
	// initialise test number
	Relatif p;
	// get the base number
	Object*  op = argv->get (0);
	// check for an integer
	Integer* ip = dynamic_cast <Integer*> (op);
	if (ip != nullptr) p = ip->tolong ();
	// check for a relatif
	Relatif* rp = dynamic_cast <Relatif*> (op);
	if (rp != nullptr) p = *rp;
	// type error with prime number
	if ((ip == nullptr) && (rp == nullptr)) {
	  throw Exception ("type-error", "invalid object with prime-probable-p",
			   Object::repr (op));
	}
	// get the number of iterations
	long tlvl = argv->getlong (1);
	// clean arguments
	delete argv; argv = nullptr;
	// compute result
	bool result = Prime::isprobable (p, tlvl);
	return new Boolean (result);
      }
      throw Exception ("argument-error", 
		       "too many arguments with prime-probable-p");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // check if a number is a safe prime

  Object* mth_safep (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 1 argument
      if (argc == 1) {
	// initialise test number
	Relatif p;
	// get the base number
	Object*  op = argv->get (0);
	// check for an integer
	Integer* ip = dynamic_cast <Integer*> (op);
	if (ip != nullptr) p = ip->tolong ();
	// check for a relatif
	Relatif* rp = dynamic_cast <Relatif*> (op);
	if (rp != nullptr) p = *rp;
	// type error with prime number
	if ((ip == nullptr) && (rp == nullptr)) {
	  throw Exception ("type-error", "invalid object with prime-safe-p",
			   Object::repr (op));
	}
	// clean arguments
	delete argv; argv = nullptr;
	// compute result
	bool result = Prime::issafe (p);
	return new Boolean (result);
      }
      throw Exception ("argument-error", 
		       "too many arguments with prime-safe-p");
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
