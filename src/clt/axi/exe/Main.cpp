// ---------------------------------------------------------------------------
// - Main.cpp                                                                -
// - the afnix interpreter main program                                      -
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

#include "Interp.hpp"
#include "System.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the help option
  static const char  AXI_OPTS_HELP = 'h';
  // the version option
  static const char  AXI_OPTS_VERS = 'v';
  // the afnix messages
  static const char* AXI_BASE_MESG = "afnix cross interpreter, ";
  static const char* AXI_VERS_MESG = ", revision ";
  // the fatal exception trapped
  static const char* AXI_FOBJ_MESG = "fatal: object exception trapped";
  static const char* AXI_FERR_MESG = "fatal: unknown exception trapped";

  // get the interpreter options
  static Options* get_options (OutputStream& os,
			       const long argc, const char** argv) {
    // create a new option instance
    Options* opts = Interp::getopts ();
    
    // parse the options
    try {
      opts->parse (argc, argv);
      // check for verbose and version
      if (opts->getoflg (AXI_OPTS_HELP) == true) {
	opts->usage (os);
	delete opts;
	System::exit (0);
      }
      if (opts->getoflg (AXI_OPTS_VERS) == true) {
	os << AXI_BASE_MESG << System::osname  ();
	os << AXI_VERS_MESG << System::version () << eolc;
	delete opts;
	System::exit (0);
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
    // the running interpeter
    Interp* interp = nullptr;
    try {
      // get the terminal flag
      bool tflg = opts.istflg ();
      // create a new interpreter
      interp = new Interp (tflg);
      // process the standard options
      String name = interp->setopts (opts);
      // loop or execute on the standard input or a file
      bool status = tflg ? interp->loop () : interp->loop (name);
      // clean the interpreter and return
      delete interp;
      return status;
    } catch (...) {
      delete interp;
      throw;
    }
  }
}

// ---------------------------------------------------------------------------
// - main process section                                                    -
// ---------------------------------------------------------------------------

int main (int argc, const char** argv) {
  using namespace afnix;
  OutputTerm terr (OutputTerm::ERROR);
  
  // get the options and run them
  Options* opts = get_options (terr, argc, argv);

  // main processing loop
  try {
    // process the options
    bool status = run_options (*opts);
    // clean the options
    delete opts;
    // clean everything and return
    if (status == false) return 3;
  } catch (const Exception& e) {
    delete opts;
    terr.errorln (e);
    return 1;
  } catch (const Object& o) {
    delete opts;
    terr << AXI_FOBJ_MESG << o.repr() << eolc;
    return 2;
  } catch (...) {
    delete opts;
    terr << AXI_FERR_MESG << eolc;
    return 3;
  }
  // so far, so good
  return 0;
}
