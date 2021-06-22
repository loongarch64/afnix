// ---------------------------------------------------------------------------
// - Options.cpp                                                             -
// - standard object library - options class implementation                  -
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

#include "Vector.hpp"
#include "System.hpp"
#include "Global.hpp"
#include "Options.hpp"
#include "Boolean.hpp"
#include "Unicode.hpp"
#include "Loopable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default program name
  static const String DEF_PGM_NAME = "unknown";

  // option component
  struct s_optc {
    // the option message
    String  d_mesg;
    // the option flag
    bool    d_oflg;
    // the option string (if any)
    String  d_opts;
    // the option vector (if any)
    Strvec  d_optv;
    // the next record in list
    s_optc* p_next;
    // create an option component
    s_optc (const String mesg) {
      d_mesg = mesg;
      d_oflg = false;
      p_next = nullptr;
    }
    // create an option component by string
    s_optc (const String opts, const String& mesg) {
      d_mesg = mesg;
      d_oflg = false;
      d_opts = opts;
      p_next = nullptr;
    }
    // destroy this option component
    ~s_optc (void) {
      delete p_next;
    }
    // reset this option component
    void reset (void) {
      d_oflg = false;
      d_opts = "";
      d_optv.reset ();
    }
    // set the option flag
    void setoflg (const bool oflg) {
      d_oflg = oflg;
    }
    // get the option component flag
    bool getoflg (void) const {
      return d_oflg;
    }
    // set the option string
    void setopts (const String& opts) {
      d_oflg = true;
      d_opts = opts;
    }
    // return the option string
    String getopts (void) const {
      return d_opts;
    }
    // set the option vector
    void setoptv (const String& optv) {
      d_oflg = true;
      d_optv.add (optv);
    }
    // return the option vector
    Strvec getoptv (void) const {
      return d_optv;
    }
  };

  struct s_optd {
    // the option type
    Options::t_type d_type;
    // the option element
    t_quad  d_opte;
    // the option component
    s_optc* p_optc;
    // next descriptor
    s_optd* p_next;
    // create an option descriptor
    s_optd (Options::t_type type, const t_quad opte, const String& mesg) {
      d_type = type;
      d_opte = opte;
      p_optc = new s_optc (mesg);
      p_next = nullptr;
    }
    // create an option descriptor by element and string
    s_optd (const t_quad opte, const String& opts, const String& mesg) {
      d_type = Options::LIST;
      d_opte = opte;
      p_optc = new s_optc (opts, mesg);
      p_next = nullptr;
    }
    // destroy this option descriptor
    ~s_optd (void) {
      delete p_optc;
      delete p_next;
    }
    // reset this descriptor
    void reset (void) {
      // loop in the option component
      s_optc* optc = p_optc;
      while (optc != nullptr) {
	optc->reset ();
	optc = optc->p_next;
      }
    }
    // print the usage message for this descriptor
    void usage (OutputStream& os, const String& prefix) const {
      // loop in the option components
      s_optc* optc = p_optc;
      while (optc != nullptr) {
	os << prefix << optc->d_mesg << eolc;
	optc = optc->p_next;
      }
    }
    // add a new component in the descriptor chain
    void add (const String& opts, const String& mesg) {
      // check if the descriptor is of type list
      if (d_type != Options::LIST) return;
      // create the new component and link it
      s_optc* optc = new s_optc (opts, mesg);
      optc->p_next = p_optc;
      p_optc = optc;
    }
    // find an option descriptor by element
    s_optd* find (const t_quad opte) {
      if (d_opte == opte) return this;
      return (p_next == nullptr) ? nullptr : p_next->find (opte);
    }
    // find an option descriptor by element and string
    s_optd* find (const t_quad opte, const String& opts) {
      // check for right element
      if (d_opte != opte) {
	return (p_next == nullptr) ? nullptr : p_next->find (opte, opts);
      }
      // check for element string
      if (d_type != Options::LIST) return nullptr;
      // loop in the component list
      s_optc* optc = p_optc;
      while (optc != nullptr) {
	if (optc->getopts () == opts) return this;
	optc = optc->p_next;
      }
      return nullptr;
    }
    // look for an option component by string
    s_optc* look (const String& opts) {
      // check for element string
      if (d_type != Options::LIST) return nullptr;
      // loop in the component list
      s_optc* optc = p_optc;
      while (optc != nullptr) {
	if (optc->getopts () == opts) return optc;
	optc = optc->p_next;
      }
      return nullptr;
    }
    // look for an option component by element and string
    s_optc* look (const t_quad opte, const String& opts) {
      // check for right element
      if (d_opte != opte) {
	return (p_next == nullptr) ? nullptr : p_next->look (opte, opts);
      }
      return look (opts);
    }
    // set the option flag
    void setoflg (const bool oflg) {
      if (p_optc != nullptr) p_optc->setoflg (oflg);
    }
    // get the option flag if possible
    bool getoflg (void) const {
      return (p_optc == nullptr) ? false : p_optc->getoflg ();
    }
    // set the option string
    void setopts (const String& opts) {
      if (p_optc != nullptr) p_optc->setopts (opts);
    }
    // get the option string
    String getopts (void) {
      String result;
      if (p_optc != nullptr) result = p_optc->getopts ();
      return result;
    }
    // set the option vector
    void setoptv (const String& optv) {
      if (p_optc != nullptr) p_optc->setoptv (optv);
    }
    // get the option vector
    Strvec getoptv (void) {
      Strvec result;
      if (p_optc != nullptr) result = p_optc->getoptv ();
      return result;
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
 
  // create a default option object

  Options::Options (void) {
    d_umsg = "not defined";
    d_pgmn = DEF_PGM_NAME;
    p_optd = nullptr;
  }

  // create an option object with a usage message

  Options::Options (const String& umsg) {
    d_umsg = umsg;
    d_pgmn = DEF_PGM_NAME;
    p_optd = nullptr;
  }

  // destroy this option object

  Options::~Options (void) {
    delete p_optd;
  }

  // return the class name

  String Options::repr (void) const {
    return "Options";
  }

  // reset the options descriptors

  void Options::reset (void) {
    wrlock ();
    try {
      // reset the argument vector
      d_args.reset ();
      // reset the program name
      d_pgmn = DEF_PGM_NAME;
      // loop in the descriptors
      s_optd* optd = p_optd;
      while (optd != nullptr) {
	optd->reset ();
	optd = optd->p_next;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the usage message

  void Options::setumsg (const String& umsg) {
    wrlock ();
    try {
      d_umsg = umsg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the usage message

  String Options::getumsg (void) const {
    rdlock ();
    try {
      String result = d_umsg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // print a usage message to a stream

  void Options::usage (OutputStream& os) const {
    rdlock ();
    try {
      // print the usage message
      os << "usage: " << d_umsg << eolc;
      // loop in the descriptors and print the usage message
      s_optd* optd = p_optd;
      while (optd != nullptr) {
	optd->usage (os, "       ");
	optd = optd->p_next;
      }
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a uniq descriptor by character and message

  void Options::add (t_type type, const char opte, const String& mesg) {
    wrlock ();
    try {
      add (type, Unicode::toquad (opte), mesg);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a uniq descriptor by character and message

  void Options::add (t_type type, const t_quad opte, const String& mesg) {
    wrlock ();
    try {
      // check if the type is a uniq, string or vector option
      if ((type != UNIQ) && (type != SOPT) && (type != VOPT)) {
	throw Exception ("options-error", "invalid option type for add");
      }
      // check if the option already exists
      if ((p_optd != nullptr)  && (p_optd->find (opte) != nullptr)) {
	throw Exception ("option-error" , "option already exists", opte);
      }
      // create the new option and link it
      s_optd* optd = new s_optd (type, opte, mesg);
      optd->p_next = p_optd;
      p_optd = optd;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a list option by element and string

  void Options::add (const char opte, const String& opts, const String& mesg) {
    wrlock ();
    try {
      add (Unicode::toquad (opte), opts, mesg);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a list option by element and string

  void Options::add (const t_quad opte,const String& opts,const String& mesg) {
    wrlock ();
    try {
      // check if the option already exists
      if ((p_optd != nullptr) && (p_optd->look (opte, opts) != nullptr)) {
	throw Exception ("options-error", "option list alreay exists", opts);
      }
      // eventually find the option descriptor
      s_optd* optd = (p_optd == nullptr) ? nullptr : p_optd->find (opte);
      // create a new one or add into it
      if (optd == nullptr) {
	optd = new s_optd (opte, opts, mesg);
	optd->p_next = p_optd;
	p_optd = optd;
      } else {
	optd->add (opts, mesg);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the option flag if set

  bool Options::getoflg (const char opte) const {
    rdlock ();
    try {
      bool result = getoflg (Unicode::toquad (opte));
      unlock ();
      return result;	
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the option flag if set

  bool Options::getoflg (const t_quad opte) const {
    rdlock ();
    try {
      // check if we have some descriptors
      if (p_optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      // get the descriptor and check the type
      s_optd* optd = p_optd->find (opte);
      if(optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      t_type type = optd->d_type;
      if ((type != UNIQ) && (type != SOPT) && (type != VOPT)) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      bool result = optd->getoflg ();
      unlock ();
      return result;	
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the option flag if set

  bool Options::getoflg (const char opte, const String& opts) const {
    rdlock ();
    try {
      bool result = getoflg (Unicode::toquad (opte), opts);
      unlock ();
      return result;	
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the option flag if set

  bool Options::getoflg (const t_quad opte, const String& opts) const {
    rdlock ();
    try {
      // check if we have some descriptors
      if (p_optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      // get the descriptor and check the type
      s_optc* optc = p_optd->look (opte, opts);
      if(optc == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      bool result = optc->getoflg ();
      unlock ();
      return result;	
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an option string by element

  void Options::setopts (const char opte, const String& opts) {
    wrlock ();
    try {
      setopts (Unicode::toquad (opte), opts);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an option string by element

  void Options::setopts (const t_quad opte, const String& opts) {
    wrlock ();
    try {
      // check if we have some descriptors
      if (p_optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      // get the descriptor and check the type
      s_optd* optd = p_optd->find (opte);
      if(optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      t_type type = optd->d_type;
      if (type != SOPT) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      optd->setopts (opts);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the option string if set

  String Options::getopts (const char opte) const {
    rdlock ();
    try {
      String result = getopts (Unicode::toquad (opte));
      unlock ();
      return result;	
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the option string if set

  String Options::getopts (const t_quad opte) const {
    rdlock ();
    try {
      // check if we have some descriptors
      if (p_optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      // get the descriptor and check the type
      s_optd* optd = p_optd->find (opte);
      if(optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      t_type type = optd->d_type;
      if (type != SOPT) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      String result = optd->getopts ();
      unlock ();
      return result;	
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the option vector

  Strvec Options::getoptv (const char opte) const {
    rdlock ();
    try {
      Strvec result = getoptv (Unicode::toquad (opte));
      unlock ();
      return result;	
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the option vector

  Strvec Options::getoptv (const t_quad opte) const {
    rdlock ();
    try {
      // check if we have some descriptors
      if (p_optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      // get the descriptor and check the type
      s_optd* optd = p_optd->find (opte);
      if(optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      t_type type = optd->d_type;
      if (type != VOPT) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      Strvec result = optd->getoptv ();
      unlock ();
      return result;	
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the debug marking flag

  void Options::setdbug (const char opte) {
    wrlock ();
    try {
      setdbug (Unicode::toquad (opte));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the debug marking flag

  void Options::setdbug (const t_quad opte) {
    wrlock ();
    try {
      // check if we have some descriptors
      if (p_optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      // get the descriptor and check the type
      s_optd* optd = p_optd->find (opte);
      if(optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      t_type type = optd->d_type;
      if (type != UNIQ) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      // set the debug flag
      optd->setoflg (Global::getdbug ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the debug marking flag

  void Options::setdbug (const char opte, const String& opts) {
    wrlock ();
    try {
      setdbug (Unicode::toquad (opte), opts);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the debug marking flag

  void Options::setdbug (const t_quad opte, const String& opts) {
    wrlock ();
    try {
      // check if we have some descriptors
      if (p_optd == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      // get the descriptor and check the type
      s_optc* optc = p_optd->look (opte, opts);
      if(optc == nullptr) {
	throw Exception ("options-error", "invalid option flag access", opte);
      }
      optc->setoflg (Global::getdbug ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the argument vector is empty

  bool Options::empty (void) const {
    rdlock ();
    try {
      bool result = d_args.empty ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the terminal flag is set

  bool Options::istflg (void) const {
    rdlock ();
    try {
      // check for empty arguments
      bool result = d_args.empty ();
      // eventually add the optional module
      result = result && (!getoflg ('m'));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the string argument vector

  Strvec Options::getargs (void) const {
    rdlock ();
    try {
      Strvec result = d_args;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a vector option by option element

  Vector* Options::getvopt (const t_quad opte) const {
    rdlock ();
    try {
      // get the string vector
      Strvec optv = getoptv (opte);
      // map it to an object vector
      long size = optv.length ();
      Vector* result = new Vector (size);
      for (long i = 0; i < size; i++) {
	String opts = optv.get (i);
	result->add (new String (opts));
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }	   
  }

  // get a vector argument

  Vector* Options::getvarg (void) const {
    rdlock ();
    try {
      long    size   = d_args.length ();
      Vector* result = new Vector (size);
      for (long i = 0; i < size; i++) {
	String opts = d_args.get (i);
	result->add (new String (opts));
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }	   
  }

  // get the program name

  String Options::getpgmn (void) const {
    rdlock ();
    try {
      String result = d_pgmn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse an argument vector

  void Options::parse (const Strvec& argv) {
    wrlock ();
    try {
      // get the argument count
      long argc = argv.length ();
      // process the options
      t_quad optsep = System::optsep ();
      long   count  = 0;
      bool   fflag  = false;
      while (count < argc) {
	String arg = argv.get (count);
	// check if we have an option
	if ((fflag == false) && (arg[0] == optsep)) {
	  // check option length
	  if (arg.length () != 2) {
	    throw Exception ("options-error", "illegal option", arg);
	  }
	  // get the option element
	  t_quad opte = arg[1];
	  // look for the option end
	  if (opte == optsep) {
	    fflag = true;
	    count++;
	    continue;
	  }
	  // look for an option descriptor and get the optionb type
	  s_optd* optd = (p_optd == nullptr) ? nullptr : p_optd->find (opte);
	  if (optd == nullptr) {
	    throw Exception ("options-error", "invalid option request", arg);
	  }
	  t_type type = optd->d_type;
	  // process the uniq option type
	  if (type == UNIQ) {
	    optd->setoflg (true);
	    count++;
	    continue;
	  }
	  // process the string option type
	  if (type == SOPT) {
	    // check for enough argument
	    if (++count >= argc) {
	      throw Exception ("options-error", "missing argument option",arg);
	    }
	    optd->setopts (argv.get (count++));
	    continue;
	  }
	  // process the string vector type
	  if (type == VOPT) {
	    // check for enough argument
	    if (++count >= argc) {
	      throw Exception ("options-error", "missing argument option",arg);
	    }
	    optd->setoptv (argv.get (count++));
	    continue;
	  }
	  // process the list option type
	  if (type == LIST) {
	    // check for enough argument
	    if (++count >= argc) {
	      throw Exception ("options-error", "missing argument option",arg);
	    }
	    // look for the option component
	    String  opts = argv.get (count++);
	    s_optc* optc = optd->look (opts);
	    if (optc == nullptr) {
	      throw Exception ("options-error", "illegal option argument",arg);
	    }
	    optc->setopts (opts);
	    continue;
	  }
	} else {
	  fflag = true;
	  d_args.add (argv.get (count++));
	  continue;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // parse a system argument vector
  
  void Options::parse (const long argc, const char** argv) {
    wrlock ();
    try {
      // save the program name
      d_pgmn = argv[0];
      // convert the arguments
      Strvec args (argc-1);
      for (long i = 1; i < argc; i++) args.add (argv[i]);
      // parse the vector
      parse (args);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // parse an object vector
  
  void Options::parse (Vector* argv) {
    // check for nil vector
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc == 0) return;
    // convert the vector
    Strvec args (argc);
    for (long i = 0; i < argc; i++) args.add (argv->getstring (i));
    // parse the vector
    parse (args);
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 15;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_USAGE   = zone.intern ("usage");
  static const long QUARK_PARSE   = zone.intern ("parse");
  static const long QUARK_EMPTYP  = zone.intern ("empty-p");
  static const long QUARK_ADDLOPT = zone.intern ("add-list-option");
  static const long QUARK_ADDUNIQ = zone.intern ("add-unique-option");
  static const long QUARK_ADDSOPT = zone.intern ("add-string-option");
  static const long QUARK_ADDVOPT = zone.intern ("add-vector-option");
  static const long QUARK_SETUMSG = zone.intern ("set-user-message");
  static const long QUARK_GETUMSG = zone.intern ("get-user-message");
  static const long QUARK_GETOFLG = zone.intern ("get-unique-option");
  static const long QUARK_GETSOPT = zone.intern ("get-string-option");
  static const long QUARK_GETVOPT = zone.intern ("get-vector-option");
  static const long QUARK_SETDFLG = zone.intern ("set-debug-flag");
  static const long QUARK_GETVARG = zone.intern ("get-vector-arguments");

  // create a new input file in a generic way

  Object* Options::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Options;
    // check for 1 argument
    if (argc == 1) {
      String umsg = argv->getstring (0);
      return new Options (umsg);
    }
    throw Exception ("argument-error", 
		     "invalid arguments with with options"); 
  }

  // return true if the given quark is defined

  bool Options::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }      
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* Options::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_EMPTYP)  return new Boolean (empty   ());
      if (quark == QUARK_GETVARG) return getvarg ();
      if (quark == QUARK_GETUMSG) return new String  (getumsg ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_USAGE) {
	auto lobj = dynamic_cast<Loopable*>(zobj);
	OutputStream* os = (lobj == nullptr) ? nullptr : lobj->getos ();
	if (os != nullptr) usage (*os);
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETUMSG) {
	String umsg = argv->getstring (0);
	setumsg (umsg);
	return nullptr;
      }
      if (quark == QUARK_GETOFLG) {
	t_quad opte = argv->getchar (0);
	return new Boolean (getoflg (opte));
      }
      if (quark == QUARK_GETSOPT) {
	t_quad opte = argv->getchar (0);
	return new String (getopts (opte));
      }
      if (quark == QUARK_SETDFLG) {
	t_quad opte = argv->getchar (0);
	setdbug (opte);
	return nullptr;
      }
      if (quark == QUARK_GETVOPT) {
	t_quad opte = argv->getchar (0);
	return getvopt (opte);
      }
      if (quark == QUARK_USAGE) {
	Object* obj = argv->get (0);
	OutputStream*  os = dynamic_cast <OutputStream*> (obj);
	if (os == nullptr) {
	  throw Exception ("type-error", "invalid output stream object",
			   Object::repr (obj));
	}
	usage (*os);
	return nullptr;
      }
      if (quark == QUARK_PARSE) {
	Object*  obj = argv->get (0);
	Vector* vobj = dynamic_cast <Vector*> (obj);
	if (vobj == nullptr) {
	  throw Exception ("type-error", "invalid vector object",
			   Object::repr (obj));
	}
	parse (vobj);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADDUNIQ) {
	t_quad opte = argv->getchar   (0);
	String mesg = argv->getstring (1);
	add (UNIQ, opte, mesg);
	return nullptr;
      }
      if (quark == QUARK_ADDSOPT) {
	t_quad opte = argv->getchar   (0);
	String mesg = argv->getstring (1);
	add (SOPT, opte, mesg);
	return nullptr;
      }
      if (quark == QUARK_ADDVOPT) {
	t_quad opte = argv->getchar   (0);
	String mesg = argv->getstring (1);
	add (VOPT, opte, mesg);
	return nullptr;
      }
      if (quark == QUARK_GETOFLG) {
	t_quad opte = argv->getchar   (0);
	String opts = argv->getstring (1);
	return new Boolean (getoflg (opte, opts));
      }
      if (quark == QUARK_SETDFLG) {
	t_quad opte = argv->getchar   (0);
	String opts = argv->getstring (1);
	setdbug (opte, opts);
	return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_ADDLOPT) {
	t_quad opte = argv->getchar   (0);
	String opts = argv->getstring (1);
	String mesg = argv->getstring (2);
	add (opte, opts, mesg);
	return nullptr;
      }
    }
    // call the object method method
    return Object::apply (zobj, nset, quark, argv);
  }
}
