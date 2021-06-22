// ---------------------------------------------------------------------------
// - UserSpace.cpp                                                           -
// - afnix:csm module - user space class implementation                      -
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
#include "System.hpp"
#include "Exception.hpp"
#include "UserSpace.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default user space

  UserSpace::UserSpace (void) : LocalSpace (System::userdata()) {
  }
  
  // create a user space by root directory

  UserSpace::UserSpace (const String& root) :
    LocalSpace (System::userdata(root)){
  }
  
  // create a user space by root and name

  UserSpace::UserSpace (const String& root, const String& name) :
    LocalSpace (System::userdata(root), name) {
  }
  
  // create a user space by root, name and info

  UserSpace::UserSpace (const String& root,
			const String& name, const String& info) :
    LocalSpace (System::userdata(root), name, info) {
  }
  
  // return the class name

  String UserSpace::repr (void) const {
    return "UserSpace";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* UserSpace::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new UserSpace;
    // check for 1 argument
    if (argc == 1) {
      String root = argv->getstring (0);
      return new UserSpace (root);
    }
    // check for 2 arguments
    if (argc == 2) {
      String root = argv->getstring (0);
      String name = argv->getstring (1);
      return new UserSpace (root, name);
    }
    // check for 3 arguments
    if (argc == 3) {
      String root = argv->getstring (0);
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      return new UserSpace (root, name, info);
    }
    throw Exception ("argument-error",
		     "too many argument with user space ");
  }
}
