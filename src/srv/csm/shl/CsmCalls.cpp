// ---------------------------------------------------------------------------
// - CsmCalls.cpp                                                            -
// - afnix:csm service - specific calls implementation                       -
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
#include "CsmCalls.hpp"
#include "WorkSpace.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // convert a name to a workspace zone uri

  Object* csm_tozuri (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String name = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (WorkZone::tozuri (name));
      }
      throw Exception ("argument-error", 
		       "too many arguments with to-workspace-uri");
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
