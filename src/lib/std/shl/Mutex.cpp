// ---------------------------------------------------------------------------
// - Mutex.cpp                                                               -
// - standard object library - mutex class implementation                    -
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

#include "Mutex.hpp"
#include "Exception.hpp"
#include "cthr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new unlocked mutex

  Mutex::Mutex (void) {
    p_mutex = c_mtxcreate ();
    if (p_mutex == nullptr) 
      throw Exception ("mutex-error", "cannot create mutex");
  }

  // destroy this mutex

  Mutex::~Mutex (void) {
    c_mtxdestroy (p_mutex);
  }

  // lock this mutex

  void Mutex::lock (void) const {
    if (c_mtxlock (p_mutex) == false)
      throw Exception ("mutex-error", "cannot lock mutex");
  }

  // unlock this mutex

  void Mutex::unlock (void) const {
    if (c_mtxunlock (p_mutex) == false)
      throw Exception ("mutex-error", "cannot unlock mutex");
  }

  // try to lock this mutex

  bool Mutex::trylock (void) const {
    return c_mtxtry (p_mutex);
  }
}
