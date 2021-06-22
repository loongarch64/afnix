// ---------------------------------------------------------------------------
// - NwgCalls.hpp                                                            -
// - afnix:nwg module - specific calls definitions                           -
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

#ifndef  AFNIX_NWGCALLS_HPP
#define  AFNIX_NWGCALLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains specific system procedures that require only
  /// arguments and that are not bounded to a class
  /// @author amaury darsch

  /// check if a mime extension is defined
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_mextp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// check if a mime value is defined
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_mvalp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// check for a valid xml mime value
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_xmlmp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get a mime value by extension
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_tomime (Evaluable* zobj, Nameset* nset, Cons* args);

  /// get an extension by mime value
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_tomext (Evaluable* zobj, Nameset* nset, Cons* args);

  /// percent encode a string
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_pencd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// uri encode a string
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_uencd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// component encode a string
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_cencd (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// percent decode a string
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_pdecd (Evaluable* zobj, Nameset* nset, Cons* args);

  /// check for a valid uri string
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_surip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// create a uri string by scheme, host and port
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_tosuri (Evaluable* zobj, Nameset* nset, Cons* args);

  /// normalize a uri name by adding a missing scheme if any
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_nrmunm (Evaluable* zobj, Nameset* nset, Cons* args);

  /// normalize a uri name by prioritizing the system path
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_sysunm (Evaluable* zobj, Nameset* nset, Cons* args);

  /// normalize a path by uri name
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_pthunm (Evaluable* zobj, Nameset* nset, Cons* args);

  /// normalize a host by authority
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_nrmhost (Evaluable* zobj, Nameset* nset, Cons* args);

  /// normalize a port by authority
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the argument list
  Object* nwg_nrmport (Evaluable* zobj, Nameset* nset, Cons* args);
}


#endif
