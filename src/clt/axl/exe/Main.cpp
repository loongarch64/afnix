// ---------------------------------------------------------------------------
// - Main.cpp                                                                -
// - afnix cross librarian main program                                      -
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
#include "Options.hpp"
#include "Librarian.hpp"
#include "Exception.hpp"
#include "OutputFile.hpp"
#include "OutputTerm.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the options messages
  static const String U_CLS_MSG = "axl [options] [f name] files";
  static const String H_OPT_MSG = "    [h]        \t print this help message";
  static const String V_OPT_MSG = "    [v]        \t print system version";
  static const String C_OPT_MSG = "    [c]        \t create a new librarian";
  static const String X_OPT_MSG = "    [x]        \t extract from librarian";
  static const String S_OPT_MSG = "    [s]        \t short librarian list";
  static const String T_OPT_MSG = "    [t]        \t print librarian list";
  static const String F_OPT_MSG = "    [f name]   \t set librarian file name";
  static const String M_OPT_MSG = "    [m name]   \t set start module name";

  // this procedure returns the libarian option object
  static Options* get_options (OutputStream& os,
			       const long argc, const char** argv) {
    // create the option class
    Options* opts = new Options (U_CLS_MSG);
    // add the string options
    opts->add (Options::SOPT, 'm', M_OPT_MSG);
    opts->add (Options::SOPT, 'f', F_OPT_MSG);
    // add the uniq options
    opts->add (Options::UNIQ, 't', T_OPT_MSG);
    opts->add (Options::UNIQ, 's', S_OPT_MSG);
    opts->add (Options::UNIQ, 'x', X_OPT_MSG);
    opts->add (Options::UNIQ, 'c', C_OPT_MSG);
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
	os << "afnix cross librarian, " << System::osname ();
	os << ", revision " << System::version () << eolc;
	delete opts;
	System::exit (0);
      }
      // make sure the librarian file is set
      if (opts->getoflg ('f') == false) {
	opts->usage (os);
	os << "fatal: missing librarian name" << eolc;
	System::exit (1);
      }
      // make sure that not both extract and create are set
      if ((opts->getoflg ('c') == true) && (opts->getoflg ('x') == true)) {
	os << "fatal: both create and extract flags are set" << eolc;
	System::exit (1);
      }
      // make sure that the module flag is only set in create mode
      if ((opts->getoflg ('m') == true) && (opts->getoflg ('c') == false)) {
	os << "fatal: start module flag in non create mode" << eolc;
	System::exit (1);
      }
 
    } catch (...) {
      opts->usage (os);
      delete opts;
      System::exit (1);
    }
    return opts;
  }

  // this procedure does a librarian report
  static void axl_report (const Librarian& axl, bool tflg, bool sflg) {
    // do nothing without report mode
    if (tflg == false) return;
    // create output stream and report
    OutputTerm tout;
    if (sflg == true) {
      Strvec flst = axl.getlist ();
      long   llen = flst.length ();
      for (long i = 0; i < llen; i++) tout.writeln (flst.get (i));
    } else {
      axl.format (tout);
    }
  }
  
  // this procedure extract a file from the librarian
  static void axl_extract (const Librarian& axl, const String& name) {
    InputStream* is = axl.extract (name);
    if (is == nullptr) return;
    try {
      OutputFile os (name);
      while (is->iseos () == false) os.write (is->read ());
      Object::cref (is);
    } catch (...) {
      Object::cref (is);
      throw;
    }
  }

  // this procedure process the options
  static void run_options (const Options& opts) {
    // preset the afnix system
    System::preset (opts.getpgmn (), false);
    // get the options value
    bool   cflg = opts.getoflg ('c');
    bool   xflg = opts.getoflg ('x');
    bool   mflg = opts.getoflg ('m');
    bool   sflg = opts.getoflg ('s');
    bool   tflg = opts.getoflg ('t') || sflg;
    String name = opts.getopts ('f');
    Strvec args = opts.getargs ();

    // process the create flag
    if (cflg == true) {
      // create a librarian
      Librarian axl;
      // loop in the vector
      long argc = args.length ();
      for (long i = 0; i < argc; i++) axl.add (args.get (i));
      // eventuall set the start module
      if (mflg == true) axl.setstm (opts.getopts ('m'));
      // write the librarian
      axl.write (name);
      // check for report
      axl_report (axl, tflg, sflg);
    } else if (xflg == true) {
      // open the librarian
      Librarian axl (name);
      // check if we extract eveything
      long argc = args.length ();
      if (argc == 0) {
	Strvec flst = axl.getlist ();
	long   llen = flst.length ();
	for (long i = 0; i < llen; i++) {
	  String name  = flst.get (i);
	  axl_extract (axl, name);
	}
      } else {
	for (long i = 0; i < argc; i++) {
	  String name = args.get (i);
	  axl_extract (axl, name);
	}
      }
      // check for report
      axl_report (axl, tflg, sflg);
    } else {
      // open the librarian
      Librarian axl (name);
      // check for report
      axl_report (axl, tflg, sflg);
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
