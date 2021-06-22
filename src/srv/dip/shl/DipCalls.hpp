// ---------------------------------------------------------------------------
// - DipCalls.hpp                                                            -
// - afnix:dip service - image processing specific calls definitions         -
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

#ifndef  AFNIX_DIPCALLS_HPP
#define  AFNIX_DIPCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// This file contains specific image processing procedures that require
  /// only arguments and that are not bounded to a class
  /// @author amaury darsch

  /// read a netpbm image
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* dip_rdpbm (Evaluable* zobj, Nameset* nset, Cons* args);

  /// write a netpbm image
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* dip_wrpbm (Evaluable* zobj, Nameset* nset, Cons* args);
  
}

#endif
