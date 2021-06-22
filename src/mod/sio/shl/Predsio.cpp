// ---------------------------------------------------------------------------
// - Predsio.cpp                                                             -
// - afnix:sio module - predicates implementation                            -
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

#include "Cons.hpp"
#include "System.hpp"
#include "Logtee.hpp"
#include "Predsio.hpp"
#include "Boolean.hpp"
#include "Pathname.hpp"
#include "Pathlist.hpp"
#include "FileInfo.hpp"
#include "Selector.hpp"
#include "Terminal.hpp"
#include "Intercom.hpp"
#include "NamedFifo.hpp"
#include "Directory.hpp"
#include "InputFile.hpp"
#include "Exception.hpp"
#include "OutputFile.hpp"
#include "InputMapped.hpp"
#include "InputString.hpp"
#include "InputOutput.hpp"
#include "OutputString.hpp"
#include "OutputBuffer.hpp"

namespace afnix {

  // this procedure checks that we have one argument only and returns
  // the evaluated object
  static inline Object* get_obj (Evaluable* zobj, Nameset* nset, Cons* args,
                                 const String& pname) {
    Object* car = nullptr;
    if ((args == nullptr) || (args->length () != 1))
      throw Exception ("argument-error", "illegal arguments with predicate",
                       pname);
    car = args->getcar ();
    return (car == nullptr) ? nullptr : car->eval (zobj,nset);
  }

  // fifop: named fifo object predicate

  Object* sio_fifop (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "named-fifo-p");
    bool result = (dynamic_cast <NamedFifo*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pathp: pathname object predicate

  Object* sio_pathp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pathname-p");
    bool result = (dynamic_cast <Pathname*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // plstp: pathlist object predicate

  Object* sio_plstp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pathlist-p");
    bool result = (dynamic_cast <Pathlist*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // dirp: check for a directory

  Object* sio_dirp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj  = get_obj (zobj, nset, args, "dir-p");
    String* sval = dynamic_cast <String*> (obj);
    if (sval == nullptr) 
      throw Exception ("argument-error", "invalid object with dir-p",
		       Object::repr (obj));
    bool result = System::isdir (*sval);
    Object::cref (obj);
    return new Boolean (result);
  }

  // filep: check for a regular file

  Object* sio_filep  (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get object and check for nil
    Object* obj  = get_obj (zobj, nset, args, "file-p");
    if (obj == nullptr) return new Boolean (false);
    // bind to a string and check
    String* sval = dynamic_cast <String*> (obj);
    if (sval == nullptr) 
      throw Exception ("argument-error", "invalid object with file-p",
		       Object::repr (obj));
    bool result = System::isfile (*sval);
    Object::cref (obj);
    return new Boolean (result);
  }

  // termp: terminal predicate

  Object* sio_termp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "terminal-p");
    bool result = (dynamic_cast <Terminal*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // icomp: intercom predicate

  Object* sio_icomp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "intercom-p");
    bool result = (dynamic_cast <Intercom*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // logtp: logtee predicate

  Object* sio_logtp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "logtee-p");
    bool result = (dynamic_cast <Logtee*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // odirp: directory object predicate

  Object* sio_odirp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "directory-p");
    bool result = (dynamic_cast <Directory*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // infop: file information predicate

  Object* sio_infop  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "file-info-p");
    bool result = (dynamic_cast <FileInfo*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // trscp: transcoder predicate

  Object* sio_trscp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "transcoder-p");
    bool result =  (dynamic_cast <Transcoder*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // strmp: stream predicate

  Object* sio_strmp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "stream-p");
    bool result =  (dynamic_cast <Stream*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // inputp: input stream predicate

  Object* sio_inputp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "input-stream-p");
    bool result =  (dynamic_cast <InputStream*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ifilep: input file predicate

  Object* sio_ifilep  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "input-file-p");
    bool result = (dynamic_cast <InputFile*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ofilep: output file predicate

  Object* sio_ofilep  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "output-file-p");
    bool result = (dynamic_cast <OutputFile*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }


  // itermp: input term predicate

  Object* sio_itermp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "input-term-p");
    bool result = (dynamic_cast <InputTerm*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // otermp: output term predicate

  Object* sio_otermp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "output-term-p");
    bool result = (dynamic_cast <OutputTerm*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  } 

  // selectp: select stream predicate

  Object* sio_selectp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "selector-p");
    bool result = (dynamic_cast <Selector*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // outputp: output stream predicate

  Object* sio_outputp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "output-stream-p");
    bool result = (dynamic_cast <OutputStream*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // inpstrp: input string predicate

  Object* sio_inpstrp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "input-string-p");
    bool result = (dynamic_cast <InputString*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // inpmapp: input mapped predicate

  Object* sio_inpmapp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "input-mapped-p");
    bool result = (dynamic_cast <InputMapped*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // outstrp: output string predicate

  Object* sio_outstrp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "output-string-p");
    bool result = (dynamic_cast <OutputString*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // outbufp: output buffer predicate

  Object* sio_outbufp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "output-buffer-p");
    bool result = (dynamic_cast <OutputBuffer*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // inpoutp: input/output predicate

  Object* sio_inpoutp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "input-output-p");
    bool result = (dynamic_cast <InputOutput*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
