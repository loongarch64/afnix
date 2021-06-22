// ---------------------------------------------------------------------------
// - Monitor.cpp                                                             -
// - standard object library - monitor class implementation                  -
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

#include "Monitor.hpp"
#include "Exception.hpp"
#include "cthr.hpp"
#include "csys.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new monitor

  Monitor::Monitor (void) {
    d_cnt = 0;
    p_tid = nullptr;
    p_mtx = c_mtxcreate ();
    p_mcv = c_tcvcreate ();
  }

  // destroy this monitor

  Monitor::~Monitor (void) {
    c_tcvdestroy (p_mcv);
    c_mtxdestroy (p_mtx);
  }

  // enter this monitor

  void Monitor::enter (void) {
    // get the monitor lock
    c_mtxlock (p_mtx);
    // take the monitor if null
    if (d_cnt == 0) {
      d_cnt++;
      p_tid = c_thrself ();
      c_mtxunlock (p_mtx);
      return;
    }
    // check if we are the thread
    if (c_threqual (p_tid) == true) {
      d_cnt++;
      c_mtxunlock (p_mtx);
      return;
    }
    // we do not own this monitor so go to wait
    while (d_cnt != 0) c_tcvwait (p_mcv, p_mtx);
    // here we have got the lock again
    if (d_cnt != 0) {
      c_mtxunlock (p_mtx);
      throw Exception ("internal-error", "monitor enter violation");
    }
    d_cnt++;
    p_tid = c_thrself ();
    c_mtxunlock (p_mtx);
  }

  // leave this monitor

  void Monitor::leave (void) {
    // get the monitor lock
    c_mtxlock (p_mtx);
    // check if we own the monitor
    if ((d_cnt > 0) && (c_threqual (p_tid) == false)) {
      c_mtxunlock (p_mtx);
      throw Exception ("internal-error", "monitor leave violation");
    }
    // check for recursive leave
    if (--d_cnt > 0) {
      c_mtxunlock (p_mtx);
      return;
    }
    // last leave call
    p_tid = nullptr;
    d_cnt = 0;
    c_tcvsignal (p_mcv);
    c_mtxunlock (p_mtx);
  }
}
