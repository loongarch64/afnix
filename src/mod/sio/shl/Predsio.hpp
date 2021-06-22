// ---------------------------------------------------------------------------
// - Predsio.hpp                                                             -
// - afnix:sio module - predicates declaration                               -
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

#ifndef  AFNIX_PREDSIO_HPP
#define  AFNIX_PREDSIO_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:sys
  /// standard module.
  /// @author amaury darsch

  /// the named fifo predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_fifop   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the pathname predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_pathp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the pathlist predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_plstp   (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the directory predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_dirp    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the file predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_filep   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the terminal object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_termp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the intercom object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_icomp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the logtee object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_logtp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the directory object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_odirp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the file info predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_infop   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the transcoder object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_trscp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the stream object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_strmp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the input object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_inputp  (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the input file object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_ifilep  (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the output file object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_ofilep  (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the input terminal object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_itermp  (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the output terminal object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_otermp  (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the selector object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_selectp (Evaluable* zobj, Nameset* nset, Cons* args); 

  /// the output object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_outputp (Evaluable* zobj, Nameset* nset, Cons* args); 

  /// the input string object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_inpstrp (Evaluable* zobj, Nameset* nset, Cons* args); 

  /// the nput map object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_inpmapp (Evaluable* zobj, Nameset* nset, Cons* args); 

  /// the output string object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_outstrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the output buffer object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_outbufp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the input output object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* sio_inpoutp (Evaluable* zobj, Nameset* nset, Cons* args);
}


#endif
