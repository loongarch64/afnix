// ---------------------------------------------------------------------------
// - Lockrw.cpp                                                              -
// - standard object library - read-write lock class implementation          -
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

#include "Lockrw.hpp"
#include "Exception.hpp"
#include "cthr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new read-write lock

  Lockrw::Lockrw (void) {
    p_tid = nullptr;
    p_mtx = c_mtxcreate ();
    if (p_mtx == nullptr) 
      throw Exception ("lockrw-error", "cannot create read-write lock");
    p_rcv = c_tcvcreate ();
    if (p_rcv == nullptr) {
      c_mtxdestroy (p_mtx);
      throw Exception ("lockrw-error", "cannot create read-write lock");
    }
    p_wcv   = c_tcvcreate ();
    if (p_wcv == nullptr) {
      c_mtxdestroy (p_mtx);
      c_tcvdestroy (p_rcv);
      throw Exception ("lockrw-error", "cannot create read-write lock");
    }
    d_rcount = 0;
    d_wcount = 0;
    d_waitrd = 0;
    d_waitwr = 0;
  }

  // destroy this read-write lock

  Lockrw::~Lockrw (void) {
    c_tcvdestroy (p_wcv);
    c_tcvdestroy (p_rcv);
    c_mtxdestroy (p_mtx);
  }

  // lock this read-write lock for reading

  void Lockrw::rdlock (void) {
    // lock this object
    c_mtxlock (p_mtx);

    // check if we have the write lock
    if ((d_wcount > 0) && (c_threqual (p_tid) == true)) {
      d_wcount++;
      c_mtxunlock (p_mtx);
      return;
    }

    // wait until the writers have finished
    while (d_wcount > 0) {
      d_waitrd++;
      c_tcvwait (p_rcv, p_mtx);
      d_waitrd--;
    }

    // here we have a new reader
    d_rcount++;
    c_mtxunlock (p_mtx);
  }

  // lock this read-write lock for writing

  void Lockrw::wrlock (void) {
    // lock this object
    c_mtxlock (p_mtx);

    // check if we have already the lock
    if ((d_wcount > 0) && (c_threqual (p_tid) == true)) {
      d_wcount++;
      c_mtxunlock (p_mtx);
      return;
    }

    // check if we have some readers or writers
    while ((d_rcount > 0) || (d_wcount > 0)) {
      d_waitwr++;
      c_tcvwait (p_wcv, p_mtx);
      d_waitwr--;
    }
    // here we have a new write lock
    d_wcount++;
    p_tid = c_thrself ();
    c_mtxunlock (p_mtx);
  }

  // adaptative read-write lock

  void Lockrw::arlock (void) {
    // lock this object
    c_mtxlock (p_mtx);

    // check the writers
    if (d_wcount > 0) {
      // check if we have already the lock
      if (c_threqual (p_tid) == true) {
	d_wcount++;
	c_mtxunlock (p_mtx);
	return;
      }
      
      // check if we have some readers or writers
      while ((d_rcount > 0) || (d_wcount > 0)) {
	d_waitwr++;
	c_tcvwait (p_wcv, p_mtx);
	d_waitwr--;
      }
      // here we have a new write lock
      d_wcount++;
      p_tid = c_thrself ();
      c_mtxunlock (p_mtx);
    } else {
      // allocate a new read lock
      d_rcount++;
      c_mtxunlock (p_mtx);
    }
  }

  // unlock a read-write lock

  void Lockrw::unlock (void) {
    // lock this object
    c_mtxlock (p_mtx);
    
    // adjust the counters - we can only have one writer - for recursive 
    // reader and writer, we simply decrement the counter
    if (d_wcount > 0) {
      if (--d_wcount > 0) {
	c_mtxunlock (p_mtx);
	return;
      }
      p_tid = nullptr;
    } else if (d_rcount > 0) {
      if (--d_rcount > 0) {
	c_mtxunlock (p_mtx);
	return;
      }
      p_tid = nullptr;
    }

    // give preference to waiting writers
    if (d_waitwr > 0) {
      c_tcvsignal (p_wcv);
    } else if (d_waitrd > 0) {
      c_tcvbdcast (p_rcv);
    }
    c_mtxunlock (p_mtx);
  }
}
