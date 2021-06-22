
// ---------------------------------------------------------------------------
// - Debugger.hpp                                                            -
// - afnix cross debugger - debugger class implementation                    -
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
#include "Module.hpp"
#include "Return.hpp"
#include "Resume.hpp"
#include "Integer.hpp"
#include "Function.hpp"
#include "AxdCalls.hpp"
#include "Debugger.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // debugger default prompts
  static const char* DEFAULT_PROMPT1 = "(axd) ";
  static const char* DEFAULT_PROMPT2 = "(...) ";
    
  // initialize the axd nameset
  static void axd_gset_init (Nameset* nset) {
    // always check for nil
    if (nset == nullptr) return;

    // bind axd commands
    nset->symcst ("run",        new Function (axd_run));
    nset->symcst ("load",       new Function (axd_ldf));
    nset->symcst ("exit",       new Function (axd_xit));
    nset->symcst ("quit",       new Function (axd_xit));
    nset->symcst ("next",       new Function (axd_nxt));
    nset->symcst ("break",      new Function (axd_bpt));
    nset->symcst ("list",       new Function (axd_lst));
    nset->symcst ("info",       new Function (axd_ifo));
    nset->symcst ("continue",   new Function (axd_cnt));
    nset->symcst ("break-info", new Function (axd_bfo));
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default debugger

  Debugger::Debugger (void) {
    // initialize the debugger mode
    d_emcs  = false;
    d_strt  = false;
    d_next   = false;
    d_fini = false;
    d_exit   = false;
    d_vflg  = true;
    d_lmax   = 10;
    p_form   = nullptr;
    // initialize the terminal
    if (p_term != nullptr) {
      p_term->setpp   (DEFAULT_PROMPT1);
      p_term->setsp   (DEFAULT_PROMPT2);
      p_term->setieos (true);
    }
    // initialize the reader
    Object::iref (p_dbrd = new Reader (getis ()));
    // bind the axd nameset
    Object::iref (p_aset = mknset ("axd"));
    // bind the i-files nameset
    Object::iref (p_iset = p_aset->mknset ("axd-i-files"));
    // bind the commands in the axd nameset
    axd_gset_init (p_aset);
  }

  // copy construct this debugger

  Debugger::Debugger (const Debugger& that) : Interp (that) {
    // copy debugger mode
    d_emcs  = that.d_emcs;
    d_strt  = that.d_strt;
    d_next   = that.d_next;
    d_fini = that.d_fini;
    d_exit   = that.d_exit;
    d_vflg  = that.d_vflg;
    d_lmax   = that.d_lmax;
    d_fnam  = that.d_fnam;
    d_brkp = that.d_brkp;
    // copy debugger object
    Object::iref (p_dbrd = that.p_dbrd);
    Object::iref (p_form   = that.p_form);
    Object::iref (p_aset   = that.p_aset);
    Object::iref (p_iset   = that.p_iset);
  }

  // destroy this debugger

  Debugger::~Debugger (void) {
    d_brkp.reset ();
    if (d_clon == false) {
      if (p_iset != nullptr) p_iset->reset ();
      if (p_aset != nullptr) p_aset->reset ();
    }
    Object::dref (p_form);
    Object::dref (p_iset);
    Object::dref (p_aset);
    Object::dref (p_dbrd);
  }

  // return the class name

  String Debugger::repr (void) const {
    return "Debugger";
  }

  // return a clone of this object

  Object* Debugger::clone (void) const {
    return new Debugger (*this);
  }

  // the debugger main loop

  bool Debugger::loop (void) {
    // initialize status
    bool status = true;
    // loop in the standard input
    try {
      status = loop ((Nameset*) nullptr);
    } catch (const Exception& e) {
      getes()->errorln (e);
    } catch (...) {
      status = false;
      getes()->errorln ("fatal: unknown exception trapped");
    }
    return status;
  }

  // loop on the standard input in a nameset context

  bool Debugger::loop (Nameset* nset) {
    // check if the nameset is defined
    Nameset* lset = (nset == nullptr) ? getgset () : nset;
    // loop until the exit flag is set
    while (d_exit == false) {
      Iform* form = nullptr;
      try {
	Form* rdfm = p_dbrd->parse ();
	if (rdfm == nullptr) break;
	form = new Iform (rdfm); delete rdfm;
	Object::cref (form->eval (this, lset));
	Object::dref (form);
      } catch (const Exception& e) {
	getes()->errorln (e);
	Object::dref (form);
      } catch (const Return& r) {
	this->post (r.getobj ());
	Object::dref (form);
      } catch (const Resume&) {
	Object::dref (form);
	if (nset != nullptr) break;
      } catch (...) {
	Object::dref (form);
	throw;
      }
    }
    return true;
  }

  // load a file name - first read the file - create a block form list
  // also updates the debugger tables - and finally execute it

  void Debugger::load (const String& fname) {
    wrlock ();
    try {
      // try to find the instrumented file
      Ifile* ifile = nullptr;
      if (p_iset->exists (fname) == false) {
	ifile = instrument (fname);
      } else {
	ifile = dynamic_cast <Ifile*> (p_iset->find (fname));
      }
      // evaluate the ifile
      if (ifile != nullptr) Object::cref (ifile->eval (this, getgset ()));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // break the debugger in a nameset with an object

  bool Debugger::bpoint (Nameset* nset, Object* object) {
    // check for a form
    auto form = dynamic_cast <Iform*> (object);
    if (form == nullptr) {
      if (object != nullptr) object->eval (this, nset);
      return true;
    }
    // break in the form
    String name = form->getname ();
    long   lnum = form->getlnum ();
    if (d_brkp.exists (form) == true) {
      long index = d_brkp.find (form);
      pmsg ("breakpoint", index, name, lnum);
    }
    // add additional information
    info (name, lnum);
    // set the form
    Object::iref (form);
    Object::dref (p_form);
    p_form = form;
    // loop in the nameset
    return loop (nset);
  }

  // set the next flag
  
  void Debugger::setnext (const bool flag) {
    wrlock ();
    try {
      d_next = flag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the next flag

  bool Debugger::getnext (void) const {
    rdlock ();
    try {
      bool result = d_next;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the emacs info mode

  void Debugger::setemacs (const bool mode) {
    wrlock ();
    try {
      d_emcs = mode;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the start flag

  bool Debugger::getstart (void) const {
    rdlock ();
    try {
      bool result = d_strt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the exit flag

  void Debugger::setexit (const bool flag) {
    wrlock ();
    try {
      d_exit = flag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the initial file name

  void Debugger::setinitial (const String& fname) {
    wrlock ();
    try {
      if (fname.length () != 0) {
	Ifile* ifile = instrument (fname);
	if ((ifile != nullptr) && (ifile->length () > 0)) {
	  Iform* form = ifile->getform(0);
	  if (form != nullptr) {
	    Object::iref (form);
	    Object::dref (p_form);
	    p_form = form;
	    String name = form->getname ();
	    long   lnum = form->getlnum ();
	    info (name, lnum);
	  }
	}
	d_fnam = fname;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // run the program as specified in the initial file name

  void Debugger::runinitial (void) {
    wrlock ();
    try {
      if (d_fnam.length () == 0) {
	throw Exception ("axd-error", "no initial file specified");
      }
      d_strt = true;
      load (d_fnam);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // instrument a file and return an instrumented file object

  Ifile* Debugger::instrument (const String& fname) {
    wrlock ();
    try {
      // bind the input stream
      InputStream* ms = p_rslv->tolookup  (fname);
      // get the interpreter encoding/transcoding modes
      if (ms != nullptr) {
	// check for forced encoding
	if (d_emod.isnil () == true) {
	  if (p_is == nullptr) {
	    ms->settmod (Encoding::getstm ());
	    ms->setemod (Encoding::getsem ());
	  } else {
	    ms->settmod (p_is->gettmod ());
	    ms->setemod (p_is->getemod ());
	  }
	} else {
	  ms->setemod (d_emod);
	}
      }
      // try to open the module
      String mn = p_rslv->tornam (fname);
      Module mp (ms, mn);
      // bind the instrumented file
      Ifile* ifile = new Ifile (mp);
      if (ifile != nullptr) p_iset->bind (mn, ifile);
      unlock ();
      return ifile;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // install a breakpoint at the current position

  void Debugger::setbpoint (void) {
    wrlock ();
    try {
      if (p_form == nullptr) {
	throw Exception ("debugger-error", "nothing to list - no form found");
      }
      String name = p_form->getname ();
      long   lnum = p_form->getlnum ();
      setbpoint (name, lnum);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // install a breakpoint by line number

  void Debugger::setbpoint (const long lnum) {
    wrlock ();
    try {
      if (p_form == nullptr) {
	throw Exception ("debugger-error", "nothing to list - no form found");
      }
      String name = p_form->getname ();
      setbpoint (name, lnum);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // install a breakpoint by file name and line number - if the line number
  // is 0 we use the first form

  void Debugger::setbpoint (const String& fname, const long lnum) {
    wrlock ();
    try {
      // check if the file name exist or instrument it
      Ifile* ifile = nullptr;
      if (p_iset->exists (fname) == false) {
	ifile = instrument (fname);
      } else {
	ifile = dynamic_cast <Ifile*> (p_iset->find (fname));
      }
      if (ifile == nullptr) {
	throw Exception ("debugger-error", "cannot find file", fname);
      }
      // we got the ifile - get the form by line number
      Iform* form = ifile->lookup (lnum);
      if (form == nullptr) {
	Integer lstr = lnum;
	throw Exception ("debugger-error", "cannot find form at line",
			 lstr.tostring ());
      }
      // check if the form already has a breakpoint
      if (d_brkp.exists (form) == false) d_brkp.add (form);
      // set the breakpoint
      form->setbrkp (true);
      // get the breakpoint index
      long index = d_brkp.find (form);
      pmsg ("setting breakpoint", index, fname, lnum);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // list a file at the current form

  void Debugger::flist (void) const {
    wrlock ();
    try {
      if (p_form == nullptr) {
	throw Exception ("debugger-error", "nothing to list - no form found");
      }
      String name = p_form->getname ();
      long   lnum = p_form->getlnum ();
      flist (name, lnum);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // list the current file at a certain line

  void Debugger::flist (const long lnum) const {
    wrlock ();
    try {
      if (p_form == nullptr) {
	throw Exception ("debugger-error", "nothing to list - no form found");
      }
      String name = p_form->getname ();
      flist (name, lnum);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // list a file content on the standard output by line number

  void Debugger::flist (const String& fname, const long lnum) const {
    wrlock ();
    try {
      // call the resolver to get an input stream
      InputStream* is = p_rslv->tolookup  (fname);
      // jump to the line
      long lc = 1;
      while (lc < lnum) {
	char c = is->read ();
	if (c == eolc) lc++;
	if (c == eosc) break;
      }
      // get the output stream
      OutputStream* os = getos ();
      // now print upto max lines
      for (long i = 0; i < d_lmax; i++) {
	Integer lval (lnum + i);
	String line = lval.tostring ();
	line = line.rfill (' ', 6);
	line = line + is->readln ();
	os->writeln (line);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write an info message based on callback or emacs mode

  void Debugger::info (const String& name, const long lnum) const {
    wrlock ();
    try {
      // check for emacs mode
      if (d_emcs == true) {
	OutputStream* os = getos ();
	if ((os == nullptr) || (name.length () == 0) || (lnum <= 0)) {
	  unlock ();
	  return;
	}
	*os << '\032' << '\032' << name << ':' << lnum << eolc;
	unlock ();
	return;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // print a message if the verbose flag is on

  void Debugger::pmsg (const String& msg,   const long data, 
		       const String& fname, const long lnum) const {
    wrlock ();
    try {
      // check if the verbose flag is on
      if ((d_vflg == false) || (msg.length () == 0)) return;
      // get the output stream
      OutputStream* os = getos ();
      *os << msg << ' ' << data;
      // check for file name
      if (fname.length () == 0) return;
      *os << " in file " << fname << " at line " << lnum << eolc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // print some info about the debugger

  void Debugger::dbginfo (void) const {
    wrlock ();
    try {
      OutputStream* os = getos ();
      String fname = (d_fnam.length () == 0) ? "not defined" : d_fnam;
      String vflag = d_vflg ? "true" : "false";
      long   blen  = d_brkp.length ();
      String sform = (p_form == nullptr) ? "not defined" : p_form->getname ();
      long   slnum = (p_form == nullptr) ? 0 : p_form->getlnum ();
      // print debugger information
      *os << "debugger version    : " << System::version () << eolc;
      *os << "os name             : " << System::osname  () << eolc;
      *os << "os type             : " << System::ostype  () << eolc;
      *os << "initial file        : " << fname              << eolc;
      *os << "form file name      : " << sform              << eolc;
      *os << "form line number    : " << slnum              << eolc;
      *os << "verbose mode        : " << vflag              << eolc;
      *os << "max line display    : " << d_lmax             << eolc;
      *os << "defined breakpoints : " << blen               << eolc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // print some information about the breakpoints

  void Debugger::brkinfo (void) const {
    wrlock ();
    try {
      OutputStream* os = getos ();
      long blen = d_brkp.length ();
      for (long i = 0; i < blen; i++) {
	auto form = dynamic_cast <Iform*> (d_brkp.get (i));
	if (form == nullptr) continue;
	String name = form->getname ();
	long   lnum = form->getlnum ();
	*os << "Breakpoint " << i    << " in file " << name;
	*os << " at line "   << lnum << eolc;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
