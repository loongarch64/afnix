// ---------------------------------------------------------------------------
// - XmlCalls.hpp                                                            -
// - afnix:xml module - xml specific calls definitions                       -
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

#ifndef  AFNIX_XMLCALLS_HPP
#define  AFNIX_XMLCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains specific system procedures that require only
  /// arguments and that are not bounded to a class
  /// @author amaury darsch

  /// prenormaize a text by keeping the eol characters
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* xml_prenorm (Evaluable* zobj, Nameset* nset, Cons* args);

  /// normalize a text by keeping only one blank character
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* xml_tonorm (Evaluable* zobj, Nameset* nset, Cons* args);
}


#endif
