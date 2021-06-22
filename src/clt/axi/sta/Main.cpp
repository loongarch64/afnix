// ---------------------------------------------------------------------------
// - Main.cpp                                                                -
// - the afnix interpreter main program  (static version)                    -
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
#include "Options.hpp"
#include "Libgfx.hpp"
#include "Libitu.hpp"
#include "Libmth.hpp"
#include "Libnet.hpp"
#include "Libnwg.hpp"
#include "Libsec.hpp"
#include "Libsio.hpp"
#include "Libsps.hpp"
#include "Libsys.hpp"
#include "Libtxt.hpp"
#include "Libwgt.hpp"
#include "Libxml.hpp"
#include "Libbce.hpp"
#include "Libcda.hpp"
#include "Libcsm.hpp"
#include "Libdip.hpp"
#include "Libphy.hpp"
#include "Libsvg.hpp"
#include "Libtls.hpp"
#include "Libwax.hpp"
#include "Libxpe.hpp"
#include "Library.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // get the interpreter options
  static Options* get_options (OutputStream& os,
			       const long argc, const char** argv) {
    // create a new option instance
    Options* opts = Interp::getopts ();
    
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
	os << "afnix cross interpreter, " << System::osname ();
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

  // this procedure process the options
  static bool run_options (const Options& opts) {
   // the running interpeter
    Interp* interp = nullptr;
    try {
      // get the terminal flag
      bool tflg = opts.empty ();
      // create a new interpreter
      interp = new Interp (tflg);
      // register the standard modules
      interp->reglib ("afnix-gfx", (void*) init_afnix_gfx);
      interp->reglib ("afnix-itu", (void*) init_afnix_itu);
      interp->reglib ("afnix-mth", (void*) init_afnix_mth);
      interp->reglib ("afnix-net", (void*) init_afnix_net);
      interp->reglib ("afnix-nwg", (void*) init_afnix_nwg);
      interp->reglib ("afnix-sec", (void*) init_afnix_sec);
      interp->reglib ("afnix-sio", (void*) init_afnix_sio);
      interp->reglib ("afnix-sps", (void*) init_afnix_sps);
      interp->reglib ("afnix-sys", (void*) init_afnix_sys);
      interp->reglib ("afnix-txt", (void*) init_afnix_txt);
      interp->reglib ("afnix-wgt", (void*) init_afnix_wgt);
      interp->reglib ("afnix-xml", (void*) init_afnix_xml);
      // register the standard services
      interp->reglib ("afnix-bce", (void*) init_afnix_bce);
      interp->reglib ("afnix-cda", (void*) init_afnix_cda);
      interp->reglib ("afnix-csm", (void*) init_afnix_csm);
      interp->reglib ("afnix-dip", (void*) init_afnix_dip);
      interp->reglib ("afnix-phy", (void*) init_afnix_phy);
      interp->reglib ("afnix-svg", (void*) init_afnix_svg);
      interp->reglib ("afnix-tls", (void*) init_afnix_tls);
      interp->reglib ("afnix-wax", (void*) init_afnix_wax);
      interp->reglib ("afnix-xpe", (void*) init_afnix_xpe);
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
  } catch (...) {
    delete opts;
    terr << "fatal: unknown exception trapped\n";
    return 2;
  }
  // so far, so good
  return 0;
}
