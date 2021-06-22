// ---------------------------------------------------------------------------
// - NetCalls.cpp                                                            -
// - afnix:net module - network system call implementation                   -
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
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Address.hpp"
#include "NetCalls.hpp"
#include "Exception.hpp"

namespace afnix {

  // get the loopback name

  Object* net_getloopback (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    long argc = (args == nullptr) ? 0 : args->length ();
    if (argc != 0) 
      throw Exception ("argument-error", 
		       "invalid number of arguments with get-loopback");
    return new String (Address::loopname ());
  }

  // get a tcp service port by name

  Object* net_gettcpserv (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) 
      throw Exception ("argument-error", 
		       "invalid number of arguments with get-tcp-service");
    try {
      String name = argv->getstring (0);
      t_word port = Address::tservice (name);
      if (port == 0) {
	throw Exception ("service-error", "cannot find tcp service", name);
      }
      delete argv; argv = nullptr;
      return new Integer (port);
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // get a udp service port by name

  Object* net_getudpserv (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 1) 
      throw Exception ("argument-error", 
		       "invalid number of arguments with get-udp-service");
    try {
      String name = argv->getstring (0);
      t_word port = Address::uservice (name);
      if (port == 0) {
	throw Exception ("service-error", "cannot find udp service", name);
      }
      delete argv; argv = nullptr;
      return new Integer (port);
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // check for a numerical ip address

  Object* net_isipn (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 2) 
      throw Exception ("argument-error", 
		       "invalid number of arguments with isipn-p");
    try {
      String name = argv->getstring (0);
      String sapf = argv->getstring (1);
      bool result = Address::isipn (name, sapf);
      delete argv; argv = nullptr;
      return new Boolean (result);
    } catch (...) {
      delete argv;
      throw;
    }
  }
}

