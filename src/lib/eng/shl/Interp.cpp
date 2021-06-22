// ---------------------------------------------------------------------------
// - Interp.cpp                                                              -
// - afnix engine - interpreter class implementation                         -
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

#include "Set.hpp"
#include "Enum.hpp"
#include "Byte.hpp"
#include "Math.hpp"
#include "Real.hpp"
#include "Meta.hpp"
#include "List.hpp"
#include "Fifo.hpp"
#include "Heap.hpp"
#include "Tree.hpp"
#include "Queue.hpp"
#include "Mutex.hpp"
#include "Regex.hpp"
#include "Plist.hpp"
#include "Lockf.hpp"
#include "Engsrl.hxx"
#include "Global.hpp"
#include "Locale.hpp"
#include "Logger.hpp"
#include "Interp.hpp"
#include "Thrset.hpp"
#include "Return.hpp"
#include "Reader.hpp"
#include "Module.hpp"
#include "System.hpp"
#include "Bitset.hpp"
#include "Symbol.hpp"
#include "Numeral.hpp"
#include "Message.hpp"
#include "Lexical.hpp"
#include "Relatif.hpp"
#include "Condvar.hpp"
#include "Library.hpp"
#include "Printer.hpp"
#include "Builtin.hpp"
#include "Boolean.hpp"
#include "Closure.hpp"
#include "Counter.hpp"
#include "Function.hpp"
#include "Instance.hpp"
#include "Structure.hpp"
#include "Librarian.hpp"
#include "Qualified.hpp"
#include "Character.hpp"
#include "HashTable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AliasTable.hpp"
#include "PrintTable.hpp"
#include "BlockBuffer.hpp"

#include "transient.tcc"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure initialize the interpreter global nameset
  static void axi_gset_init (Interp* interp) {
    // always check for nil
    if (interp == nullptr) return;
    // bind the serial dispatcher
    if (Serial::isvdid (SRL_DEOD_DID) == false) {
      throw Exception ("interp-error", "invalid eng serial dispatcher");
    }
    // get the global nameset
    Nameset* nset = interp->getgset ();
    if (nset == nullptr) return;

    // bind standard constants
    nset->symcst  ("...",            nset);
    nset->symcst  ("nil",            nullptr);
    nset->symcst  ("interp",         interp);

    //  builtin functions
    interp->mkrsv ("if",             new Function (Builtin::sfif));
    interp->mkrsv ("do",             new Function (Builtin::sfdo));
    interp->mkrsv ("for",            new Function (Builtin::sffor));
    interp->mkrsv ("try",            new Function (Builtin::sftry));
    interp->mkrsv ("eval",           new Function (Builtin::sfeval));
    interp->mkrsv ("sync",           new Function (Builtin::sfsync));
    interp->mkrsv ("loop",           new Function (Builtin::sfloop));
    interp->mkrsv ("enum",           new Function (Builtin::sfenum));
    interp->mkrsv ("const",          new Function (Builtin::sfconst));
    interp->mkrsv ("trans",          new Function (Builtin::sftrans));
    interp->mkrsv ("unref",          new Function (Builtin::sfunref));
    interp->mkrsv ("class",          new Function (Builtin::sfclass));
    interp->mkrsv ("block",          new Function (Builtin::sfblock));
    interp->mkrsv ("while",          new Function (Builtin::sfwhile));
    interp->mkrsv ("gamma",          new Function (Builtin::sfgamma));
    interp->mkrsv ("throw",          new Function (Builtin::sfthrow));
    interp->mkrsv ("force",          new Function (Builtin::sfforce));
    interp->mkrsv ("delay",          new Function (Builtin::sfdelay));
    interp->mkrsv ("future",         new Function (Builtin::sffuture));
    interp->mkrsv ("lambda",         new Function (Builtin::sflambda));
    interp->mkrsv ("switch",         new Function (Builtin::sfswitch));
    interp->mkrsv ("return",         new Function (Builtin::sfreturn));
    interp->mkrsv ("launch",         new Function (Builtin::sflaunch));
    interp->mkrsv ("protect",        new Function (Builtin::sfprotect));
    interp->mkrsv ("nameset",        new Function (Builtin::sfnameset));

    // builtin operators
    interp->mkrsv ("+",              new Function (Builtin::sfadd));
    interp->mkrsv ("-",              new Function (Builtin::sfsub));
    interp->mkrsv ("*",              new Function (Builtin::sfmul));
    interp->mkrsv ("/",              new Function (Builtin::sfdiv));
    interp->mkrsv ("==",             new Function (Builtin::sfeql));
    interp->mkrsv ("!=",             new Function (Builtin::sfneq));
    interp->mkrsv (">=",             new Function (Builtin::sfgeq));
    interp->mkrsv (">",              new Function (Builtin::sfgth));
    interp->mkrsv ("<=",             new Function (Builtin::sfleq));
    interp->mkrsv ("<",              new Function (Builtin::sflth));
    interp->mkrsv ("assert",         new Function (Builtin::sfassert)); 

    // logical operators
    interp->mkrsv ("or",             new Function (Builtin::sfor));
    interp->mkrsv ("not",            new Function (Builtin::sfnot));
    interp->mkrsv ("and",            new Function (Builtin::sfand));
    
    // standard printer objects
    interp->mkrsv ("print",          new Printer  (Printer::OUTPUT));
    interp->mkrsv ("println",        new Printer  (Printer::OUTPUTLN));
    interp->mkrsv ("error",          new Printer  (Printer::ERROR));
    interp->mkrsv ("errorln",        new Printer  (Printer::ERRORLN));

    // standard predicates
    interp->mkrsv ("nil-p",          new Function (Builtin::sfnilp));
    interp->mkrsv ("set-p",          new Function (Builtin::sfsetp));
    interp->mkrsv ("eval-p",         new Function (Builtin::sfevlp));
    interp->mkrsv ("cons-p",         new Function (Builtin::sfconsp));
    interp->mkrsv ("form-p",         new Function (Builtin::sfformp));
    interp->mkrsv ("list-p",         new Function (Builtin::sflistp));
    interp->mkrsv ("byte-p",         new Function (Builtin::sfbytep));
    interp->mkrsv ("real-p",         new Function (Builtin::sfrealp));
    interp->mkrsv ("enum-p",         new Function (Builtin::sfenump));
    interp->mkrsv ("item-p",         new Function (Builtin::sfitemp));
    interp->mkrsv ("hash-p",         new Function (Builtin::sfhashp));
    interp->mkrsv ("fifo-p",         new Function (Builtin::sffifop));
    interp->mkrsv ("heap-p",         new Function (Builtin::sfheapp));
    interp->mkrsv ("tree-p",         new Function (Builtin::sftreep));
    interp->mkrsv ("regex-p",        new Function (Builtin::sfregexp));
    interp->mkrsv ("queue-p",        new Function (Builtin::sfqueuep));
    interp->mkrsv ("class-p",        new Function (Builtin::sfclsp));
    interp->mkrsv ("plist-p",        new Function (Builtin::sfplistp));
    interp->mkrsv ("combo-p",        new Function (Builtin::sfcmbp));
    interp->mkrsv ("lexer-p",        new Function (Builtin::sflexerp));
    interp->mkrsv ("style-p",        new Function (Builtin::sfstylep));
    interp->mkrsv ("lockf-p",        new Function (Builtin::sflockfp));
    interp->mkrsv ("object-p",       new Function (Builtin::sfobjp));
    interp->mkrsv ("serial-p",       new Function (Builtin::sfsrlp));
    interp->mkrsv ("string-p",       new Function (Builtin::sfstrp));
    interp->mkrsv ("buffer-p",       new Function (Builtin::sfbufp));
    interp->mkrsv ("thread-p",       new Function (Builtin::sfthrp));
    interp->mkrsv ("thrset-p",       new Function (Builtin::sfthrsp));
    interp->mkrsv ("strvec-p",       new Function (Builtin::sfstvp));
    interp->mkrsv ("vector-p",       new Function (Builtin::sfvecp));
    interp->mkrsv ("bitset-p",       new Function (Builtin::sfbitsp));
    interp->mkrsv ("number-p",       new Function (Builtin::sfnump));
    interp->mkrsv ("symbol-p",       new Function (Builtin::sfsymp));
    interp->mkrsv ("loader-p",       new Function (Builtin::sfloadp));
    interp->mkrsv ("locale-p",       new Function (Builtin::sflocalp));
    interp->mkrsv ("logger-p",       new Function (Builtin::sflogp));
    interp->mkrsv ("former-p",       new Function (Builtin::sfformrp));
    interp->mkrsv ("reader-p",       new Function (Builtin::sfreadrp));
    interp->mkrsv ("future-p",       new Function (Builtin::sffuturp));
    interp->mkrsv ("lexical-p",      new Function (Builtin::sflexp));
    interp->mkrsv ("condvar-p",      new Function (Builtin::sfcondp));
    interp->mkrsv ("promise-p",      new Function (Builtin::sfprmp));
    interp->mkrsv ("boolean-p",      new Function (Builtin::sfboolp));
    interp->mkrsv ("integer-p",      new Function (Builtin::sfintp));
    interp->mkrsv ("relatif-p",      new Function (Builtin::sfrltp));
    interp->mkrsv ("numeral-p",      new Function (Builtin::sfnumrp));
    interp->mkrsv ("literal-p",      new Function (Builtin::sflitp));
    interp->mkrsv ("closure-p",      new Function (Builtin::sfclop));
    interp->mkrsv ("nameset-p",      new Function (Builtin::sfnstp));
    interp->mkrsv ("counter-p",      new Function (Builtin::sfcntrp));
    interp->mkrsv ("message-p",      new Function (Builtin::sfmesgp));
    interp->mkrsv ("constant-p",     new Function (Builtin::sfcstp));
    interp->mkrsv ("nameable-p",     new Function (Builtin::sfnblp));
    interp->mkrsv ("viewable-p",     new Function (Builtin::sfviewp));
    interp->mkrsv ("instance-p",     new Function (Builtin::sfinstp));
    interp->mkrsv ("resolver-p",     new Function (Builtin::sfrslvp));
    interp->mkrsv ("property-p",     new Function (Builtin::sfpropp));
    interp->mkrsv ("structure-p",    new Function (Builtin::sfstrcp));
    interp->mkrsv ("hashtable-p",    new Function (Builtin::sfashp));
    interp->mkrsv ("character-p",    new Function (Builtin::sfcharp));
    interp->mkrsv ("qualified-p",    new Function (Builtin::sfqualp));
    interp->mkrsv ("librarian-p",    new Function (Builtin::sflbrnp));
    interp->mkrsv ("collectable-p",  new Function (Builtin::sfcblp));
    interp->mkrsv ("alias-table-p",  new Function (Builtin::sfatblp));
    interp->mkrsv ("print-table-p",  new Function (Builtin::sfptblp));
    interp->mkrsv ("block-buffer-p", new Function (Builtin::sfbbufp));

    // standard objects
    interp->mkrsv ("Set",            new Meta (Set::mknew));
    interp->mkrsv ("Enum",           new Meta (Enum::mknew));
    interp->mkrsv ("Byte",           new Meta (Byte::mknew));
    interp->mkrsv ("Real",           new Meta (Real::mknew));
    interp->mkrsv ("Cons",           new Meta (Cons::mknew));
    interp->mkrsv ("List",           new Meta (List::mknew));
    interp->mkrsv ("Fifo",           new Meta (Fifo::mknew));
    interp->mkrsv ("Heap",           new Meta (Heap::mknew));
    interp->mkrsv ("Tree",           new Meta (Tree::mknew));
    interp->mkrsv ("Regex",          new Meta (Regex::mknew));
    interp->mkrsv ("Queue",          new Meta (Queue::mknew));
    interp->mkrsv ("Plist",          new Meta (Plist::mknew));
    interp->mkrsv ("Lexer",          new Meta (Lexer::mknew));
    interp->mkrsv ("Style",          new Meta (Style::meval, Style::mknew));
    interp->mkrsv ("Lockf",          new Meta (Lockf::mknew));
    interp->mkrsv ("String",         new Meta (String::mknew));
    interp->mkrsv ("Buffer",         new Meta (Buffer::mknew));
    interp->mkrsv ("Strvec",         new Meta (Strvec::mknew));
    interp->mkrsv ("Vector",         new Meta (Vector::mknew)); 
    interp->mkrsv ("Thread",         new Meta (Thread::mknew));
    interp->mkrsv ("Thrset",         new Meta (Thrset::mknew));
    interp->mkrsv ("Bitset",         new Meta (Bitset::mknew));
    interp->mkrsv ("Symbol",         new Meta (Symbol::mknew));
    interp->mkrsv ("Locale",         new Meta (Locale::mknew));
    interp->mkrsv ("Logger",         new Meta (Logger::meval, Logger::mknew));
    interp->mkrsv ("Lexer",          new Meta (Lexer::mknew));
    interp->mkrsv ("Reader",         new Meta (Reader::mknew));
    interp->mkrsv ("Lexical",        new Meta (Lexical::mknew));
    interp->mkrsv ("Condvar",        new Meta (Condvar::mknew));    
    interp->mkrsv ("Relatif",        new Meta (Relatif::mknew));    
    interp->mkrsv ("Integer",        new Meta (Integer::mknew));
    interp->mkrsv ("Numeral",        new Meta (Numeral::meval, Numeral::mknew));
    interp->mkrsv ("Boolean",        new Meta (Boolean::mknew));
    interp->mkrsv ("Closure",        new Meta (Closure::mknew));
    interp->mkrsv ("Counter",        new Meta (Counter::mknew));
    interp->mkrsv ("Message",        new Meta (Message::meval, Message::mknew));
    interp->mkrsv ("Instance",       new Meta (Instance::mknew));
    interp->mkrsv ("Resolver",       new Meta (Resolver::mknew));
    interp->mkrsv ("Property",       new Meta (Property::mknew));
    interp->mkrsv ("Structure",      new Meta (Structure::mknew));
    interp->mkrsv ("Character",      new Meta (Character::mknew));
    interp->mkrsv ("HashTable",      new Meta (HashTable::mknew));
    interp->mkrsv ("Librarian",      new Meta (Librarian::mknew));
    interp->mkrsv ("Qualified",      new Meta (Qualified::mknew));
    interp->mkrsv ("AliasTable",     new Meta (AliasTable::mknew));
    interp->mkrsv ("PrintTable",     new Meta (PrintTable::mknew));
    interp->mkrsv ("BlockBuffer",    new Meta (BlockBuffer::mknew));
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // the options messages
  static const String U_CLS_MSG = "axi [options] [file] [arguments]";
  static const String H_OPT_MSG = "    [h]       \t print this help message";
  static const String V_OPT_MSG = "    [v]       \t print system version";
  static const String M_OPT_MSG = "    [m]       \t enable the start module";
  static const String I_OPT_MSG = "    [i   path]\t add a resolver path";
  static const String E_OPT_MSG = "    [e   mode]\t force the encoding mode";
  static const String F_ASR_MSG = "    [f assert]\t enable assertion checks";
  static const String F_NOP_MSG = "    [f nopath]\t do not set initial path";
  static const String F_SRE_MSG = "    [f   seed]\t seed random engine";
  static const String F_NOS_MSG = "    [f noseed]\t do not seed random engine";

  // get an interpreter option class

  Options* Interp::getopts (void) {
    // create the option class
    Options* opts = new Options (U_CLS_MSG);

    // add the list options
    opts->add ('f', "seed",   F_SRE_MSG);
    opts->add ('f', "noseed", F_NOS_MSG);
    opts->add ('f', "nopath", F_NOP_MSG);
    opts->add ('f', "assert", F_ASR_MSG);

    // add the string options
    opts->add (Options::SOPT, 'e', E_OPT_MSG);
    opts->add (Options::VOPT, 'i', I_OPT_MSG);

    // add the uniq options
    opts->add (Options::UNIQ, 'm', M_OPT_MSG);
    opts->add (Options::UNIQ, 'v', V_OPT_MSG);
    opts->add (Options::UNIQ, 'h', H_OPT_MSG);

    // preset the debug flags
    opts->setdbug ('f', "assert");
    opts->setdbug ('f', "noseed");
    return opts;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default interpreter
  
  Interp::Interp (void) {
    // initialize default values
    d_clon = false;
    // create a default terminal
    Object::iref (p_term = new Terminal);
    // save streams
    Object::iref (p_is = p_term);
    Object::iref (p_os = p_term);
    Object::iref (p_es = new OutputTerm (OutputTerm::ERROR));
    // clear the post object
    p_post = nullptr;
    // initialize the arguments vector
    Object::iref (p_argv = new Vector);
    // initialize the resolver
    Object::iref (p_rslv = new Resolver);
    // initialize the global nameset
    Object::iref (p_gset = new Superset); axi_gset_init (this);
    // reset the evaluable form
    p_rfrm = nullptr;
    // create the library loader
    Object::iref (p_shld = new Loader);
  }

  // create a new interpreter with or without a terminal

  Interp::Interp (const bool tflg) {
    // initialize default values
    d_clon = false;
    // set the streams
    if (tflg == true) {
      Object::iref (p_term = new Terminal);
      Object::iref (p_is = p_term);
      Object::iref (p_os = p_term);
      Object::iref (p_es = new OutputTerm (OutputTerm::ERROR));
    } else {
      p_term = nullptr;
      p_is   = nullptr;
      p_os   = nullptr;
      p_es   = nullptr;
    }
    // clear the post object
    p_post = nullptr;
    // initialize the arguments vector
    Object::iref (p_argv = new Vector);
    // initialize the resolver
    Object::iref (p_rslv = new Resolver);
    // initialize the global nameset
    Object::iref (p_gset = new Superset); axi_gset_init (this);
    // reset the evaluable form
    p_rfrm = nullptr;
    // create the libary loader
    Object::iref (p_shld = new Loader);
  }

  // create a new interpreter with the specified streams

  Interp::Interp (InputStream* is, OutputStream* os, OutputStream* es) {
    // initialize default values
    d_clon = false;
    p_term = nullptr;
    // save streams
    Object::iref (p_is = is);
    Object::iref (p_os = os);
    Object::iref (p_es = es);
    // clear the post object
    p_post = nullptr;
    // initialize the arguments vector
    Object::iref (p_argv = new Vector);
    // initialize the resolver
    Object::iref (p_rslv = new Resolver);
    // initialize the global nameset
    Object::iref (p_gset = new Superset); axi_gset_init (this);
    // reset the evaluable form
    p_rfrm = nullptr;
    // create the libary loader
    Object::iref (p_shld = new Loader);
  }

  // copy constructor

  Interp::Interp (const Interp& that) {
    // initialize default value
    d_clon = true;
    // copy the terminal
    Object::iref (p_term = that.p_term);
    // copy the streams
    Object::iref (p_is = that.p_is);
    Object::iref (p_os = that.p_os);
    Object::iref (p_es = that.p_es);
    // clear the post object
    p_post = nullptr;
    // copy the vector arguments
    Object::iref (p_argv = that.p_argv);
    // copy the file path
    Object::iref (p_rslv = that.p_rslv);
    // copy the global nameset
    Object::iref (p_gset = that.p_gset);
    // reset the evaluable form
    p_rfrm = nullptr;
    // copy the library loader
    Object::iref (p_shld = that.p_shld);
  }

  // delete this interpreter

  Interp::~Interp (void) {
    // protect us
    Object::iref (this);
    // clear the posted object
    Object::dref (p_post); p_post = nullptr;
    // eventually reset the globalset
    if (d_clon == false) p_gset->reset ();
    // clean the objects first
    Object::dref (p_is);
    Object::dref (p_os);
    Object::dref (p_es);
    Object::dref (p_term);
    Object::dref (p_gset);
    Object::dref (p_argv);
    Object::dref (p_rslv);
    Object::dref (p_rfrm);
    Object::dref (p_shld);
    // check for a nil thread list - this is a tricky one here
    // it is designed to avoid a race condition between the thread
    // system and the cleanup handler where all threads might be marked
    // finished but have not yet release the lock and the global
    // destructors are called when the interpreter is released...
    if (d_clon == false) {
      // wait for the nil thread list
      Thread::wnil ();
      // check if the thread list is nil
      if (Thread::isnil () == false) {
	if (p_os != nullptr) p_os->panic ("thread list is not nil at exit");
	System::abort ();
      }
    }
  }

  // return the class name

  String Interp::repr (void) const {
    return "Interp";
  }

  // return a clone of this object

  Object* Interp::clone (void) const {
    return new Interp (*this);
  }

  // evaluate the evaluable form

  Object* Interp::run (void) {
    Object* result = nullptr;
    try {
      result = (p_rfrm == nullptr) ? nullptr : p_rfrm->eval (this, p_gset);
    } catch (const Exception& e) {
      getes()->errorln (e);
      if (e.getabf () == true) Thread::exit ();
    } catch (const Return& r) {
      result = r.getobj ();
    } catch (...) {
      getes()->errorln ("fatal: unknown exception trapped");
      result = nullptr;
    }
    this->post (result);
    return result;
  }

  // execute a form in a thread by cloning this interpreter

  Object* Interp::launch (Object* form) {
    return launch (new Thread, form);
  }

  // execute a form in a thread by cloning this interpreter

  Object* Interp::launch (Object* tobj, Object* form) {
    // check for a thread object
    Thread* thread = dynamic_cast <Thread*> (tobj);
    if (thread == nullptr) {
      throw Exception ("type-error", "invalid object as thread launcher",
		       Object::repr (tobj));
    }
    // duplicate the interpreter
    Interp* interp = dup (form);
    Object::iref (interp);
    // create a new thread
    try {
      thread->start (interp);
      this->post (thread);
      Object::dref (interp);
      return thread;
    } catch (...) {
      Object::dref (interp);
      throw;
    }
  }
  
  // loop on the standard input by doing a read-eval loop
  // this procedure return false is something bad happen

  bool Interp::loop (void) {
    // initialize status
    bool status = true;
    // loop in the standard input
    try {
      status = loop (p_gset, getis ());
    } catch (const Exception& e) {
      getes()->errorln (e);
      if (e.getabf () == true) System::exit (1);
    } catch (...) {
      status = false;
      getes()->errorln ("fatal: unknown exception trapped");
    }
    // wait for all threads to terminate - this works only for the
    // master thread - for the other threads, this does nothing
    Thread::wall ();
    return status;
  }

  // loop on the standard input in a nameset context

  bool Interp::loop (Nameset* nset, InputStream* is) {
    // initialize the status
    bool status = true;
    // create a new reader 
    t_transient<Reader> rd = new Reader (is);
    // loop until we have an eos
    while (true) {
      Form* form = nullptr;
      try {
	form = rd->parse ();
	if (form == nullptr) break;
	Object::cref (form->eval (this, nset));
	Object::dref (form);
      } catch (const Exception& e) {
	if (e.getabf () == true) throw;
	status = false;
	getes()->errorln (e);
	Object::dref (form);
      } catch (const Return& r) {
	this->post (r.getobj ());
	Object::dref (form);
      } catch (...) {
	Object::dref (form);
	throw;
      }
    }
    return status;
  }

  // loop with an input file by doing a read-eval loop
  // this procedure return false is something bad happen
  
  bool Interp::loop (const String& fname) {
    // load the file
    bool status = true;
    try {
      load (fname);
    } catch (const Exception& e) {
      status = false;
      getes()->errorln (e);
      if (e.getabf () == true) System::exit (1);
    } catch (...) {
      status = false;
      getes()->errorln ("fatal: unknown exception trapped");
    }
    // wait for all threads to terminate
    if (status == true) Thread::wall ();
    return status;
  }

  // loop with an input file by doing a read-eval loop - no thread wait
  
  void Interp::load (const String& fname) {
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
    // loop until we have an eos
    while (true) {
      Form* form = nullptr;
      try {
	form = mp.parse ();
	if (form == nullptr) break;
	Object::cref (form->eval (this, p_gset));
	Object::dref (form);
      } catch (Exception& e) {
	e.updname (fname);
	if (form == nullptr) {
	  e.updlnum (mp.getlnum ());
	} else {
	  e.updlnum (form->getlnum ());
	}
	Object::dref (form);
	throw;
      } catch (const Return& r) {
	Object::dref (form);
	this->post (r.getobj ());
      } catch (...) {
	Object::dref (form);
	throw;
      }
    }
  }

  // return the interpreter input stream

  InputStream* Interp::getis (void) const {
    wrlock ();
    try {
      if (p_is != nullptr) {
	unlock ();
	return p_is;
      }
      Object::iref (p_is = new InputTerm);
      p_is->setemod (d_emod); 
      unlock ();
      return p_is;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the interpreter output stream

  OutputStream* Interp::getos (void) const {
    wrlock ();
    try {
      if (p_os != nullptr) {
	unlock ();
	return p_os;
      }
      Object::iref (p_os = new OutputTerm (OutputTerm::OUTPUT));
      p_os->setemod (d_emod); 
      unlock ();
      return p_os;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the interpreter error stream

  OutputStream* Interp::getes (void) const {
    wrlock ();
    try {
      if (p_es != nullptr) {
	unlock ();
	return p_es;
      }
      Object::iref (p_es = new OutputTerm (OutputTerm::ERROR));
      p_es->setemod (d_emod); 
      unlock ();
      return p_es;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the interpreter global nameset

  Nameset* Interp::getgset (void) const {
    return p_gset;
  }

  // post an object in this interpreter

  void Interp::post (Object* object) {
    if (object == p_post) return;
    Object::iref (object); Object::dref (p_post); p_post = object;
  }

  // duplicate this interpreter and set the evaluable form

  Interp* Interp::dup (Object* form) const {
    auto interp = dynamic_cast<Interp*>(clone());
    interp->p_rfrm = Object::iref (form);
    return interp;
  }

  // duplicate this stream by setting the terminal

  Interp* Interp::dup (Terminal* term) const {
    // create a new interpreter
    auto interp = dynamic_cast<Interp*>(clone());
    // check that we have a valid terminal
    if (term == nullptr) term = new Terminal;
    // update the terminal and the other stream
    Object::iref (term);
    Object::dref (interp->p_term);
    interp->p_term = term;
    // update the input stream
    Object::iref (term);
    Object::dref (interp->p_is);
    interp->p_is = term;
    // update the output stream
    Object::iref (term);
    Object::dref (interp->p_os);
    interp->p_os = term;
    // update the error stream
    Object::iref (term);
    Object::dref (interp->p_es);
    interp->p_es = term;
    // the new interpeter
    return interp;
  }

  // duplicate this interpreter with the associated streams

  Interp* Interp::dup (InputStream* is, 
		       OutputStream* os, OutputStream* es) const {
    // create a new interpreter
    auto interp = dynamic_cast<Interp*>(clone());
    // update the input stream
    if (is != nullptr) {
      Object::iref (is);
      Object::dref (interp->p_is);
      interp->p_is = is;
    }
    // update the output stream
    if (os != nullptr) {
      Object::iref (os);
      Object::dref (interp->p_os);
      interp->p_os = os;
    }
    // update the output stream
    if (es != nullptr) {
      Object::iref (es);
      Object::dref (interp->p_es);
      interp->p_es = es;
    }
    return interp;
  }

  // create a new reserved entry in the global nameset
  
  void Interp::mkrsv (const String& name, Object* object) {
    wrlock ();
    try {
      p_gset->symcst (name, object);
      Token::mkrsv   (name);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a child nameset by name

  Nameset* Interp::mknset (const String& name) {
    wrlock ();
    try {
      Nameset* result = p_gset->mknset (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the primary prompt

  void Interp::setpp (const String& value) {
    wrlock ();
    try {
      if (p_term != nullptr) p_term->setpp (value);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the primary prompt

  String Interp::getpp (void) const {
    rdlock ();
    try {
      String result;
      if (p_term != nullptr) result = p_term->getpp ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the secondary prompt

  void Interp::setsp (const String& value) {
    wrlock ();
    try {
      if (p_term != nullptr) p_term->setsp (value);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the secondary prompt

  String Interp::getsp (void) const {
    rdlock ();
    try {
      String result;
      if (p_term != nullptr) result = p_term->getsp ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a line from the interpreter terminal

  String Interp::readline (const bool pflg) {
    wrlock ();
    try {
      String result = (p_term != nullptr) ? p_term->readline (pflg) : "";
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a credential from the interpreter terminal

  String Interp::readcred (void) {
    wrlock ();
    try {
      String result = (p_term != nullptr) ? p_term->readcred () : "";
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a credential from the interpreter terminal

  String Interp::readcred (const String& prompt) {
    wrlock ();
    try {
      // eventually prit the prompt
      if ((prompt.isnil () == false) && (p_term != nullptr)) {
	p_term->OutputStream::write (prompt);
      }
      // read the credential
      String result = (p_term != nullptr) ? p_term->readcred () : "";
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the interpreter encoding mode

  void Interp::setemod (const String& emod) {
    wrlock ();
    try {
      // save the encodig mode
      d_emod = emod;
      // set the terminal
      if (p_term != nullptr) p_term->InputStream::setemod  (emod);
      if (p_term != nullptr) p_term->OutputStream::setemod (emod);
      // set the base streams
      if (p_is != nullptr) p_is->setemod (emod);
      if (p_os != nullptr) p_os->setemod (emod);
      if (p_es != nullptr) p_es->setemod (emod);
      // unlock done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the interpreter options

  String Interp::setopts (const Options& opts) {
    wrlock ();
    try {
      // initialize the name result
      String result = "";
      // get the program name
      String pgmn = opts.getpgmn ();
      // get the seed flag
      bool sflg = !opts.getoflg ('f', "noseed");
      sflg |= opts.getoflg ('f', "seed");
      // preset the afnix system
      System::preset (pgmn, sflg);
      // get the options arguments
      Strvec args = opts.getargs ();
      // check for an optional file name
      bool tflg = args.empty ();
      bool mflg = opts.getoflg ('m');
      bool pflg = opts.getoflg ('f', "nopath");
      // eventually extract the module name
      if ((tflg == false) && (mflg == false) && (pflg == false)) {
	// get the path name
	result = args.pop ();
	// add the directory path
	addpath (System::xdir (result));
      }
      // set the arguments
      setargs (args);
      // set the assert flag
      Global::setasrt (opts.getoflg ('f', "assert"));
      // get the resolver path
      setpath (opts.getoptv ('i'));
      // set the encoding mode
      if (opts.getoflg ('e') == true) setemod (opts.getopts ('e'));
      // eventually extract the start module since the resolver is set
      if (mflg == true) {
	result = p_rslv->getstm ();
	if (result.isnil () == true) {
	  throw Exception ("option-error", "cannot resolve implicit module");
	}
      }
      // unlock and return file name to execute if any
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the interpreter arguments

  void Interp::setargs (const Strvec& args) {
    wrlock ();
    try {
      // reset the vector
      p_argv->reset ();
      // fill in the arguments
      long len = args.length ();
      for (long i = 0; i < len; i++) {
	p_argv->add (new String (args.get (i)));
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the interpreter arguments

  Strvec Interp::getargs (void) const {
    rdlock ();
    try {
      Strvec result;
      long len = p_argv->length ();
      for (long i = 0; i < len; i++) {
	String* str = dynamic_cast <String*> (p_argv->get (i));
	if (str == nullptr) continue;
	result.add (*str);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a new path entry in the resolver

  void Interp::addpath (const String& path) {
    wrlock ();
    try {
      p_rslv->add (path);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the path resolver

  void Interp::setpath (const Strvec& path) {
    wrlock ();
    try {
      long len = path.length ();
      for (long i = 0; i < len; i++) p_rslv->add (path.get (i));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the interpreter loader

  Loader* Interp::getld (void) const {
    rdlock ();
    try {
      Loader* result = p_shld;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a library has been loaded

  bool Interp::islib (const String& name) const {
    rdlock ();
    try {
      bool result = (p_shld == nullptr) ? false : p_shld->exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // register a library by name and handle
  
  void Interp::reglib (const String& name, void* hand) {
    wrlock ();
    try {
      p_shld->add (name, hand);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // open a new dynamic library by name
 
  Object* Interp::library (const String& name, Vector* argv) {
    wrlock ();
    try {
      // optionnal add the libary and find it
      p_shld->add (name);
      Library* lib = p_shld->lookup (name);
      // call the initialize procedure now
      Object::cref (lib->dlinit (this, argv));
      // post and return the library
      post (lib);
      unlock ();
      return lib;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compile from an input stream into an output stream
  
  void Interp::compile (const String& name, OutputStream& os) {
    // bind the input stream
    InputStream*  ms = p_rslv->lookup  (name);
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
    String mn = p_rslv->getpath (name);
    Module mp (ms, mn);
    // write the compiled code
    try {
      mp.write (os);
    } catch (const Exception& e) {
      Integer line = mp.getlnum ();
      String  msg  = name + ':' + line.tostring ();
      throw Exception (msg, e.getval ());
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                         -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 37;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the interpreter supported quarks
  static const long QUARK_DUP      = zone.intern ("dup");
  static const long QUARK_URI      = zone.intern ("afnix-uri");
  static const long QUARK_ISBE     = zone.intern ("big-endian-p");
  static const long QUARK_IS64     = zone.intern ("64-bits-p");
  static const long QUARK_LOAD     = zone.intern ("load");
  static const long QUARK_ROLL     = zone.intern ("roll");
  static const long QUARK_ARGV     = zone.intern ("argv");
  static const long QUARK_WAIT     = zone.intern ("wait");
  static const long QUARK_RSLV     = zone.intern ("resolver");
  static const long QUARK_MACHS    = zone.intern ("machine-size");
  static const long QUARK_MAJOR    = zone.intern ("major-version");
  static const long QUARK_MINOR    = zone.intern ("minor-version");
  static const long QUARK_PATCH    = zone.intern ("patch-version");
  static const long QUARK_GETIS    = zone.intern ("get-input-stream");
  static const long QUARK_GETOS    = zone.intern ("get-output-stream");
  static const long QUARK_GETES    = zone.intern ("get-error-stream");
  static const long QUARK_GETPP    = zone.intern ("get-primary-prompt");
  static const long QUARK_GETSP    = zone.intern ("get-secondary-prompt");
  static const long QUARK_SETPP    = zone.intern ("set-primary-prompt");
  static const long QUARK_SETSP    = zone.intern ("set-secondary-prompt");
  static const long QUARK_DAEMON   = zone.intern ("daemon");
  static const long QUARK_LOADER   = zone.intern ("loader");
  static const long QUARK_OSNAME   = zone.intern ("os-name");
  static const long QUARK_OSTYPE   = zone.intern ("os-type");
  static const long QUARK_GETAEPS  = zone.intern ("get-absolute-precision");
  static const long QUARK_GETREPS  = zone.intern ("get-relative-precision");
  static const long QUARK_SETAEPS  = zone.intern ("set-absolute-precision");
  static const long QUARK_SETREPS  = zone.intern ("set-relative-precision");
  static const long QUARK_VERSION  = zone.intern ("version");
  static const long QUARK_PGMNAME  = zone.intern ("program-name");
  static const long QUARK_LIBRARY  = zone.intern ("library");
  static const long QUARK_SETMDBG  = zone.intern ("set-memory-debug");
  static const long QUARK_SETMLBL  = zone.intern ("set-memory-label");
  static const long QUARK_SIGWAIT  = zone.intern ("wait-kill-signal");
  static const long QUARK_READLINE = zone.intern ("read-line");
  static const long QUARK_READCRED = zone.intern ("read-credential");
  static const long QUARK_DESERIAL = zone.intern ("deserialize");

  // return true if the given quark is defined

  bool Interp::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Runnable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate an object in this interpreter

  Object* Interp::eval (Object* object) {
    if (object == nullptr) return nullptr;
    rdlock ();
    try {
      Object* result = object->eval (this, p_gset);
      post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // evaluate an object in this interpreter by quark

  Object* Interp::eval (Object* object, const long quark) {
    if (object == nullptr) return nullptr;
    rdlock ();
    try {
      Object* result = object->eval (this, p_gset, quark);
      post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // evaluate an interpreter method by quark

  Object* Interp::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    // standard builtin name
    if (quark == QUARK_ISBE)    return new Boolean (System::isbe    ());
    if (quark == QUARK_IS64)    return new Boolean (System::is64    ());
    if (quark == QUARK_URI)     return new String  (System::geturi  ());
    if (quark == QUARK_MACHS)   return new String  (System::machs   ());
    if (quark == QUARK_MAJOR)   return new String  (System::major   ());
    if (quark == QUARK_MINOR)   return new String  (System::minor   ());
    if (quark == QUARK_PATCH)   return new String  (System::patch   ());
    if (quark == QUARK_OSTYPE)  return new String  (System::ostype  ());
    if (quark == QUARK_OSNAME)  return new String  (System::osname  ());
    if (quark == QUARK_VERSION) return new String  (System::version ());
    if (quark == QUARK_PGMNAME) return new String  (System::getpgm  ());
    if (quark == QUARK_ARGV) {
      Object* result = p_argv;
      post (result);
      return result;
    }
    if (quark == QUARK_RSLV) {
      Object* result = p_rslv;
      post (result);
      return result;
    }
    if (quark == QUARK_LOADER) {
      Object* result = p_shld;
      post (result);
      return result;
    }
    // look the object
    return Object::eval (zobj, nset, quark);
  }

  // apply this object with a set of arguments and a quark

  Object* Interp::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // special case for library with any arguments
    if ((quark == QUARK_LIBRARY) && (argc > 0)) {
      String lname = argv->getstring (0);
      return library (lname, argv);
    }

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_ROLL)     return new Boolean (loop (p_gset, p_is));
      if (quark == QUARK_GETPP)    return new String  (getpp ());
      if (quark == QUARK_GETSP)    return new String  (getsp ());
      if (quark == QUARK_DAEMON)   return new Boolean (System::daemon ());
      if (quark == QUARK_GETAEPS)  return new Real    (Math::d_aeps);
      if (quark == QUARK_GETREPS)  return new Real    (Math::d_reps);
      if (quark == QUARK_READLINE) return new String (readline (true));
      if (quark == QUARK_READCRED) return new String (readcred ());
      if (quark == QUARK_WAIT) {
	Thread::wall ();
	return nullptr;
      }
      if (quark == QUARK_SIGWAIT) {
	System::sigwait ();
	return nullptr;
      }
      if (quark == QUARK_GETIS) {
	wrlock ();
	try {
	  Object* result = getis ();
	  post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETOS) {
	wrlock ();
	try {
	  Object* result = getos ();
	  post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETES) {
	wrlock ();
	try {
	  Object* result = getes ();
	  post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_DUP){
	Terminal* term = nullptr;
	return dup (term);
      }
    }

    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_READLINE) {
	bool flag = argv->getbool (0);
        String* result = new String (readline (flag));
        return result;
      }
      if (quark == QUARK_READCRED) {
	String  prompt = argv->getstring (0);
        String* result = new String (readcred (prompt));
        return result;
      }
      if (quark == QUARK_WAIT) {
	String tgn = argv->getstring (0);
	Thread::wall (tgn);
	return nullptr;
      }
      if (quark == QUARK_SETAEPS) {
	Math::d_aeps = argv->getreal (0);
	return nullptr;
      }
      if (quark == QUARK_SETREPS) {
	Math::d_reps = argv->getreal (0);
	return nullptr;
      }
      if (quark == QUARK_LOAD) {
	String name = argv->getstring (0);
	load (name);
	return nullptr;
      }
      if (quark == QUARK_DUP) {
	Object*    obj = argv->get (0);
	Terminal* term = dynamic_cast <Terminal*> (obj);
	if (term == nullptr) {
	  throw Exception ("type-error", "invalid terminal object",
			   Object::repr (obj));
	}
	return dup (term);
      }
      if (quark == QUARK_SETPP) {
        String val = argv->getstring (0);
        setpp (val);
        return nullptr;
      }
      if (quark == QUARK_SETSP) {
        String val = argv->getstring (0);
        setsp (val);
        return nullptr;
      }
      if (quark == QUARK_SETMDBG) {
        bool flag = argv->getbool (0);
        Object::setmdbg (flag);
        return nullptr;
      }
      if (quark == QUARK_SETMLBL) {
        String label = argv->getstring (0);
        Object::setmlbl (label);
        return nullptr;
      }
      if (quark == QUARK_DESERIAL) {
	Object* obj = argv->get (0);
	if (obj == nullptr) return nullptr;
	auto* is = dynamic_cast<InputStream*>(obj);
	if (is == nullptr) {
	  throw Exception ("interp-error", "invalid object for deserialize",
			   Object::repr (obj));
	}
	return Serial::deserialize (*is);
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_DUP) {
	// get the input stream
	Object* iobj = argv->get (0);
	InputStream* is = dynamic_cast <InputStream*> (iobj);
	if (is == nullptr) {
	  throw Exception ("type-error", "invalid input stream object",
			   Object::repr (iobj));
	}
	// get the output/error stream
	Object* oobj = argv->get (1);
	OutputStream* os = dynamic_cast <OutputStream*> (oobj);
	if (os == nullptr) {
	  throw Exception ("type-error", "invalid output stream object",
			   Object::repr (oobj));
	}
	return dup (is, os, os);
      }
    }
    // check for 3 arguments
    if (argc == 3) {
      if (quark == QUARK_DUP) {
	// get the input stream
	Object* iobj = argv->get (0);
	InputStream* is = dynamic_cast <InputStream*> (iobj);
	if (is == nullptr) {
	  throw Exception ("type-error", "invalid input stream object",
			   Object::repr (iobj));
	}
	// get the output stream
	Object* oobj = argv->get (1);
	OutputStream* os = dynamic_cast <OutputStream*> (oobj);
	if (os == nullptr) {
	  throw Exception ("type-error", "invalid output stream object",
			   Object::repr (oobj));
	}
	// get the error stream
	Object* eobj = argv->get (2);
	OutputStream* es = dynamic_cast <OutputStream*> (eobj);
	if (es == nullptr) {
	  throw Exception ("type-error", "invalid error stream object",
			   Object::repr (eobj));
	}
	return dup (is, os, es);
      }
    }
    // call the runnable method
    return Runnable::apply (zobj, nset, quark, argv);
  }
}
