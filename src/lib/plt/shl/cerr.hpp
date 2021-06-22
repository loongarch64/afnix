// ---------------------------------------------------------------------------
// - cerr.hpp                                                                -
// - standard platform library - c system error functions                      -
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

#ifndef  AFNIX_CERR_HPP
#define  AFNIX_CERR_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the definitions for the platform independant error
  /// management procedures. Additionnaly, generic error code are provided
  /// with this implementation.
  /// @author amaury darsch

  /// type of error code
  static const long AFNIX_ERR_OTHR = -1;
  static const long AFNIX_ERR_INTR = -2;
  static const long AFNIX_ERR_PRIV = -3;
  static const long AFNIX_ERR_PIPE = -4;
  static const long AFNIX_ERR_LONG = -5;
  static const long AFNIX_ERR_CREF = -6;
  static const long AFNIX_ERR_UNRE = -7;
  static const long AFNIX_ERR_TOUT = -8;
  static const long AFNIX_ERR_IUSE = -9;
  static const long AFNIX_ERR_POTO = -10;
  static const long AFNIX_ERR_ADDR = -11;
  static const long AFNIX_ERR_UCON = -12;
  static const long AFNIX_ERR_PRST = -13;
  static const long AFNIX_ERR_IARG = -14;
  static const long AFNIX_ERR_EMAX = -15;

  // the exception error handler
  using t_errh = void (*) (void);

  /// install an unexpected exception handle
  /// @param func the handler function or nil
  void c_errsetexpt (t_errh func);

  /// map a system error code to an afnix error
  long c_errmap (const long val);

  /// @return a string representation of the error
  const char* c_errmsg (const long val);
}

#endif
