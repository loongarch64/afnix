// ---------------------------------------------------------------------------
// - Axdcalls.cpp                                                            -
// - afnix cross debugger - commands call implementation                     -
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
#include "Vector.hpp"
#include "Resume.hpp"
#include "AxdCalls.hpp"
#include "Debugger.hpp"
#include "Exception.hpp"

namespace afnix {

  // print a file listing

  Object* axd_lst (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    // get the debuger
    Debugger* dbg = dynamic_cast <Debugger*> (zobj);
    // check for 0 arguemnt
    if (argc == 0) {
      dbg->flist ();
      return nullptr;
    }
    // check for 1 argument
    if (argc == 1) {
      long lnum = argv->getlong (0);
      dbg->flist (lnum);
      delete argv;
      return nullptr;
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      long   lnum = argv->getlong (1);
      dbg->flist (name, lnum);
      delete argv;
      return nullptr;
    }
    delete argv;
    throw Exception ("argument-error", "invalid arguemnts with list");
  }

  // print some debugger information

  Object* axd_ifo (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the debugger
    Debugger* dbg = dynamic_cast <Debugger*> (zobj);
    // get the debugger information
    if (dbg != nullptr) dbg->dbginfo ();
    return nullptr;
  }

  // print some breakpoint information

  Object* axd_bfo (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the debugger
    Debugger* dbg = dynamic_cast <Debugger*> (zobj);
    // get the break information
    if (dbg != nullptr) dbg->brkinfo ();
    return nullptr;
  }

  // set the exit flag

  Object* axd_xit (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the debugger
    Debugger* dbg = dynamic_cast <Debugger*> (zobj);
    // set the exit flag
    if (dbg != nullptr) dbg->setexit (true);
    return nullptr;
  }

  // load a file and make it the initial one

  Object* axd_ldf (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // get the debugger
    Debugger* dbg = dynamic_cast <Debugger*> (zobj);
    if (argc == 1) {
      String name = argv->getstring (0);
      delete argv;
      dbg->setinitial (name);
      return nullptr;
    }
    delete argv;
    throw Exception ("argument-error", "too many arguments with run");
  }

  // run the debugger

  Object* axd_run (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // get the debugger
    Debugger* dbg = dynamic_cast <Debugger*> (zobj);
    // run with or without a file
    if (argc == 0) {
      delete argv;
      dbg->runinitial ();
      return nullptr;
    }
    if (argc == 1) {
      String name = argv->getstring (0);
      delete argv;
      dbg->setinitial (name);
      dbg->runinitial ();
      return nullptr;
    }
    delete argv;
    throw Exception ("argument-error", "too many arguments with run");
  }

  // set a breakpoint by name and file number

  Object* axd_bpt (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // get the debugger
    auto dbg = dynamic_cast <Debugger*> (zobj);
    // check for 0 argument
    if (argc == 0) {
      delete argv;
      if (dbg != nullptr) dbg->setbpoint ();
      return nullptr;
    }
    // check for 1 argument
    if (argc == 1) {
      long lnum = argv->getlong (0);
      delete argv;
      if (dbg != nullptr) dbg->setbpoint (lnum);
      return nullptr;
    }
    // check for 2 arguments
    if (argc == 2) {
     String fname = argv->getstring (0);
     long   lnum  = argv->getlong (1);
     delete argv;
     if (dbg != nullptr) dbg->setbpoint (fname, lnum);
     return nullptr;
    }
    delete argv;
    throw Exception ("breakpoint-error", "invalid number of arguments");
  }

  // continue withing this debugger

  Object* axd_cnt (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the debugger
    Debugger* dbg = dynamic_cast <Debugger*> (zobj);
    if ((dbg != nullptr) && (dbg->getstart () == false)) {
      throw Exception ("debugger-error", "the program is not started");
    }
    throw Resume ();
  }

  // continue to the next form

  Object* axd_nxt (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the debugger
    Debugger* dbg = dynamic_cast <Debugger*> (zobj);
    if ((dbg != nullptr) && (dbg->getstart () == false)) {
      throw Exception ("debugger-error", "the program is not started");
    }
    if (dbg != nullptr) dbg->setnext (true);
    throw Resume ();
  }
}
