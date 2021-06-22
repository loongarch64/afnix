// ---------------------------------------------------------------------------
// - cerr.cxx                                                                -
// - standard platform library - c error system function implementation      -
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

#include "cerr.hpp"
#include "csys.hpp"
#include "csio.hpp"
#include "cstr.hpp"
#include "cerr.hxx"

namespace afnix {
  // the static array of message
  static const char* afnix_err_list[] = {
    "generic system error",       // AFNIX_ERR_OTHR
    "interrupt received",         // AFNIX_ERR_INTR
    "privilege violations",       // AFNIX_ERR_PRIV
    "broken pipe",                // AFNIX_ERR_PIPE
    "name is too long",           // AFNIX_ERR_LONG
    "connection refused",         // AFNIX_ERR_CREF
    "network is unreachable",     // AFNIX_ERR_UNRE
    "resource has timed-out",     // AFNIX_ERR_TOUT
    "resource already in use",    // AFNIX_ERR_IUSE
    "cannot find protocol",       // AFNIX_ERR_POTO
    "invalid inet address",       // AFNIX_ERR_ADDR
    "end point is not connected", // AFNIX_ERR_UCON
    "connection reset by peer",   // AFNIX_ERR_PRST
    "invalid arguments",          // AFNIX_ERR_IARG
  };

  // return a string representation of the error

  const char* c_errmsg (const long eid) {
    if ((eid <= AFNIX_ERR_EMAX) || (eid >= 0)) return nullptr;
    return afnix_err_list[-eid - 1];
  }

  // this function print an bort message in case of unexpected exception
  static void abort_unexpected (void) {
    int eid         = c_stderr ();
    const char* msg = "fatal: unexpected exception - aborting\n";
    c_write (eid, msg, c_strlen (msg));
    c_abort ();
  }

  // set an unexpected handler

  void c_errsetexpt (t_errh func) {
    if (func == nullptr)
      std::set_unexpected (abort_unexpected);
    else
      std::set_unexpected (func);
  }
}

#ifdef AFNIX_HAVE_ERRNO
namespace afnix {
  // map the unix errno to afnix error
  long c_errmap (const long val) {
    long result = AFNIX_ERR_OTHR;
    switch (val) {
    case EINTR:
      result = AFNIX_ERR_INTR;
      break;
    case EACCES:
      result = AFNIX_ERR_PRIV;
      break;
    case EPIPE:
      result = AFNIX_ERR_PIPE;
      break;
    case ENAMETOOLONG:
      result = AFNIX_ERR_LONG;
      break;
    case ECONNREFUSED:
      result = AFNIX_ERR_CREF;
      break;
    case ENETUNREACH:
      result = AFNIX_ERR_UNRE;
      break;
    case ETIMEDOUT:
      result = AFNIX_ERR_TOUT;
      break;
    case EADDRINUSE:
      result = AFNIX_ERR_IUSE;
      break;
    case ENOTCONN:
      result = AFNIX_ERR_UCON;
      break;
    case ECONNRESET:
      result = AFNIX_ERR_PRST;
      break;
    default:
      result = AFNIX_ERR_OTHR;
      break;
    }
    return result;
  }
}
#else
namespace afnix {
  long c_errmap (const long val) {
    return AFNIX_ERR_IARG;
  }
}
#endif
