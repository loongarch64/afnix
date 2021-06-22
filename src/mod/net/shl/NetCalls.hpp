// ---------------------------------------------------------------------------
// - NetCalls.hpp                                                            -
// - afnix:net module - network system call definitions                      -
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

#ifndef  AFNIX_NETCALLS_HPP
#define  AFNIX_NETCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains specific networking procedures that require only
  /// arguments and that are not bounded to a class
  /// @author amaury darsch

  /// return the loopback name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* net_getloopback (Evaluable* zobj, Nameset* nset, Cons* args);

  /// return the tcp service
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* net_gettcpserv  (Evaluable* zobj, Nameset* nset, Cons* args);

  /// return the udp service
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* net_getudpserv  (Evaluable* zobj, Nameset* nset, Cons* args);

  /// check for a numerical ip address
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* net_isipn (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
