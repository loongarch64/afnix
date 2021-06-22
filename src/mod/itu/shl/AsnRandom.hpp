// ---------------------------------------------------------------------------
// - AsnRandom.hpp                                                           -
// - afnix:itu module - asn random object generator definitions              -
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

#ifndef  AFNIX_ASNRANDOM_HPP
#define  AFNIX_ASNRANDOM_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// This file contains specific procedures that produce asn random object.
  /// @author amaury darsch

  /// get a random asn bit string
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* itu_asnbrnd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a random asn octet string
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* itu_asnornd (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
