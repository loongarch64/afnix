// ---------------------------------------------------------------------------
// - Main.cpp                                                                -
// - the afnix cross compiler main program                                   -
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
#include "Interp.hpp"
#include "Exception.hpp"
#include "InputFile.hpp"
#include "OutputFile.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the options messages
  static const String U_CLS_MSG = "axc [options] files";
  static const String H_OPT_MSG = "    [h]        \t print this help message";
  static const String V_OPT_MSG = "    [v]        \t print system version";
  static const String I_OPT_MSG = "    [i  path]  \t add a resolver path";
  static const String E_OPT_MSG = "    [e  mode]  \t force the encoding mode";

  // this procedure returns the compiler option object
  static Options* get_options (OutputStream& os,
			       const long argc, const char** argv) {
    // create the option class
    Options* opts = new Options (U_CLS_MSG);
    // add the string options
    opts->add (Options::SOPT, 'e', E_OPT_MSG);
    opts->add (Options::VOPT, 'i', I_OPT_MSG);
    // add the uniq options
    opts->add (Options::UNIQ, 'v', V_OPT_MSG);
    opts->add (Options::UNIQ, 'h', H_OPT_MSG);

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
	os << "afnix cross compiler, " << System::osname ();
	os << ", revision " << System::version () << eolc;
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

  // this procedure compile a file by name
  static void compile_file (Interp* interp, const String& name) {
    // get the output file name
    String oname = System::xname (System::rmext (name)) + ".axc";
    try {
      // try to open the output file
      OutputFile os (oname);
      // force the default encoding mode
      os.setemod (Encoding::getnem ());
      // compile the module
      interp->compile (name, os);
    } catch (const Exception& e) {
      if (System::isfile (oname) == true) System::rmfile (oname);
      throw;
    } 
  }

  // this procedure process the options
  static void run_options (const Options& opts) {
    // register the program name
    System::preset (opts.getpgmn (), false);
    // create a new interpreter
    Interp* interp = new Interp;
    try {
      // set the interpreter options
      interp->setpath (opts.getoptv ('i'));
      if (opts.getoflg ('e') == true) {
	interp->setemod (opts.getopts ('e'));
      }
      // get the argument file vector
      Strvec args = opts.getargs ();
      long   argc = args.length ();
      // compile one by one
      for (long i = 0; i < argc; i++) {
	compile_file (interp, args.get (i));
      }
      delete interp;
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

  // main processing loop
  try {
    // get the options
    Options* opts = get_options (terr, argc, argv);
    // process the options
    run_options (*opts);
    // clean everything
    delete opts;
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
