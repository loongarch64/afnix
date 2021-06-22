// ---------------------------------------------------------------------------
// - Main.cpp                                                                -
// - the afnix debugger main program                                         -
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

#include "System.hpp"
#include "Debugger.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the options messages
  static const String U_CLS_MSG = "axd [options] [file] [arguments]";
  static const String F_EMC_MSG = "    [f  emacs]\t enable emacs mode";
  static const String F_INI_MSG = "    [f runini]\t run initial file";

  // get the interpreter options
  static Options* get_options (OutputStream& os,
			       const long argc, const char** argv) {
    // create a new option instance
    Options* opts = Interp::getopts ();
    // set debugger specific options
    opts->setumsg (U_CLS_MSG);
    opts->add ('f', "emacs",  F_EMC_MSG);
    opts->add ('f', "runini", F_INI_MSG);

    // parse the options
    try {
      opts->parse (argc, argv);
      // check for verbose and version
      if (opts->getoflg ('h') == true) {
	opts->usage (os);
	delete opts;
	System::exit (0);
      }
      if (opts->getoflg ('v') == true) {
	os << "afnix cross debugger, " << System::osname ();
	os << ", revision " << System::version () << eolc;
	delete opts;
	System::exit (0);
      }
      // check for empty arguments
      if (opts->empty () == true) {
	opts->usage (os);
	delete opts;
	System::exit (1);
      }
    } catch (...) {
      opts->usage (os);
      delete opts;
      System::exit (1);
    }
    return opts;
  }

  // this procedure process the options
  static bool run_options (const Options& opts) {
    // create a new debugger
    Debugger* interp = new Debugger;
    // process the standard options
    String name = interp->setopts (opts);
    // process the specific options
    interp->setemacs (opts.getoflg ('f', "emacs"));
    // get the initial flag or module flag
    bool iflg = opts.getoflg ('f', "runini") || opts.getoflg ('m');
    if (iflg == false) interp->setinitial (name);
    // loop depending on the initial status
    bool status = iflg ? interp->Interp::loop (name) : interp->loop ();
    // clean the interpreter and return
    delete interp;
    return status;
  }
}

// ---------------------------------------------------------------------------
// - main process section                                                    -
// ---------------------------------------------------------------------------

int main (int argc, const char** argv) {
  using namespace afnix;
  OutputTerm terr (OutputTerm::ERROR);

  // main processing loop
  try {
    // get the options and run them
    Options* opts = get_options (terr, argc, argv);
    // process the options
    bool status = run_options (*opts);
    // clean everything and return
    delete opts;
    if (status == false) return 3;
  } catch (const Exception& e) {
    terr.errorln (e);
    return 1;
  } catch (...) {
    terr << "fatal: unknown exception trapped\n";
    return 2;
  }
  // so far, so good
  return 0;
}
