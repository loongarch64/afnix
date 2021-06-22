// ---------------------------------------------------------------------------
// - Reader.cpp                                                              -
// - afnix engine - reader class implementation                              -
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
#include "Reader.hpp"
#include "Terminal.hpp"
#include "Exception.hpp"
#include "InputString.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure sets a line and eventually feeds the input stream
  static inline bool rd_read_term (InputStream* is, bool pflag) {
    // check for a valid stream
    if (is == nullptr) return false;
    // read the terminal line
    Terminal* term = dynamic_cast <Terminal*> (is);
    if (term == nullptr) return true;
    String line = term->readline (pflag);
    is->pushback (line);
    return true;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default reader
  
  Reader::Reader (void) {
    p_is  = nullptr;  
    p_lex = new Lexer;
  }

  // create a new reader by string

  Reader::Reader (const String& sval) {
    Object::iref (p_is = new InputString (sval));
    p_lex = new Lexer (p_is);
  }

  // create a new reader class 
  
  Reader::Reader (InputStream* is) {
    Object::iref (p_is = is);  
    p_lex = new Lexer (p_is);
  }

  // destroy this reader

  Reader::~Reader (void) {
    Object::dref (p_is);
    delete p_lex;
  }

  // return the class name

  String Reader::repr (void) const {
    return "Reader";
  }

  // parse the input stream and return a cons cell

  Form* Reader::parse (void) {
    wrlock ();
    try {
      // read a line from the terminal if nay
      if (rd_read_term (p_is, true) == false) {
	unlock ();
	return nullptr;
      }
      // loop until we have an eol or eos
      Form* result = nullptr;
      while (true) {
	Token token = p_lex->get ();
	switch (token.gettid ()) {
	case Token::ERROR:
	  delete result;
	  throw Exception ("syntax-error", "illegal token found", 
			   token.getval ());
	case Token::EOL:
	  if (result == nullptr) {
	    if (rd_read_term (p_is, true) == false) {
	      unlock ();
	      return nullptr;
	    }
	    continue;
	  }
	  unlock ();
	  return result;
	case Token::EOS:
	  unlock ();
	  return result;
	case Token::RFB:
	  if (result == nullptr) {
	    long lnum = getlnum ();
	    result    = new Form (rform (true));
	    result->setinfo (d_name, lnum);
	  } else result->add (rform (true));
	  continue;
	case Token::BFB:
	  if (result == nullptr) {
	    long lnum = getlnum ();
	    result    = new Form (bform (true));
	    result->setinfo (d_name, lnum);
	  } else result->add (bform (true));
	  continue;
	case Token::REAL:
	case Token::REGEX:
	case Token::STRING:
	case Token::LEXICAL:
	case Token::BOOLEAN:
	case Token::INTEGER:
	case Token::RELATIF:
	case Token::QUALIFIED:
	case Token::CHARACTER:
	  if (result == nullptr) {
	    long lnum = getlnum ();
	    result    = new Form (token.getobj ());
	    result->setinfo (d_name, lnum);
	  } else result->add (token.getobj ());
	  continue;
	default:
	  delete result;
	  throw Exception ("syntax-error", "illegal token found", 
			   token.getval ());
	}
      }
      // if we are here, we have an internal error
      delete result;
      throw Exception ("internal-error", "reader loop error");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  
  // read a form and return a cons cell - the rfb charcter is consumed
  
  Form* Reader::rform (bool pflag) {
    wrlock ();
    try {
      Form* result = nullptr;
      // loop until we have a rfe
      while (true) {
	Token token = p_lex->get ();
	switch (token.gettid ()) {
	case Token::ERROR:
	  delete result;
	  throw Exception ("syntax-error", "illegal token found", 
			   token.getval ());
	case Token::EOL:
	  if (rd_read_term (p_is, false) == false) {
	    unlock ();
	    return nullptr;
	  }
	  continue;
	case Token::EOS:
	  delete result;
	  throw Exception ("eos-error", "eos unexpected while parsing form");
	case Token::RFB:
	  if (result == nullptr) {
	    long lnum = getlnum ();
	    result    = new Form (rform (pflag));
	    result->setinfo (d_name, lnum);
	  } else result->add (rform (pflag));
	  continue;
	case Token::BFB:
	  if (result == nullptr) {
	    long lnum = getlnum ();
	    result    = new Form (bform (pflag));
	    result->setinfo (d_name, lnum);
	  } else result->add (bform (pflag));
	  continue;
	case Token::RFE:
	  unlock ();
	  return result;
	case Token::BFE:
	  delete result;
	  throw Exception ("reader-error", "illegal character } in form");
	default:
	  if (result == nullptr) {
	    long lnum = getlnum ();
	    result    = new Form (token.getobj ());
	    result->setinfo (d_name, lnum);
	  } else result->add (token.getobj ());
	  continue;
	}
      }
      // if we are here, we have an internal error
      delete result;
      throw Exception ("internal-error", "reader loop error");  
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a block form and return a cons cell - the bfb character is consumed

  Form* Reader::bform (bool pflag) {
    wrlock ();
    try {
      Form* form   = nullptr;
      Form* result = new Form (Cons::CCTP_BLOK, getlnum ());
      // loop until we have a bfe
      while (true) {
	Token token = p_lex->get ();
	switch (token.gettid ()) {
	case Token::ERROR:
	  delete form;
	  delete result;
	  throw Exception ("syntax-error", "illegal token found", 
			   token.getval ());
	case Token::EOL:
	  if (form == nullptr) {
	    if (rd_read_term (p_is, false) == false) {
	      unlock ();
	      return nullptr;
	    }
	    continue;
	  }
	  result->add (form);
	  form = nullptr;
	  if (rd_read_term (p_is, false) == false) {
	    unlock ();
	    return nullptr;
	  }
	  continue;
	case Token::EOS:
	  delete result;
	  delete form;
	  throw Exception ("eos-error", "eos unexpected while parsing form");
	case Token::RFB:
	  if (form == nullptr) {
	    long lnum = getlnum ();
	    form      = new Form (rform (pflag));
	    form->setinfo (d_name, lnum);
	  } else form->add (rform (pflag));
	  continue;
	case Token::BFB:
	  if (form == nullptr)
	    form = bform (pflag);
	  else
	    form->add (bform (pflag));
	  continue;
	case Token::BFE:
	  if (form != nullptr) result->add (form);
	  form = nullptr;
	  unlock ();
	  return result;
	case Token::RFE:
	  delete result;
	  throw Exception ("reader-error", "illegal character in block form");
	default:
	  if (form == nullptr) {
	    long lnum = getlnum ();
	    form      = new Form (token.getobj ());
	    form->setinfo (d_name, lnum);
	  } else form->add (token.getobj ());
	  continue;
	}
      }
      // if we are here, we have an internal error
      delete result;
      throw Exception ("internal-error", "reader loop error");  
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the reader file name

  void Reader::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the reader file name

  String Reader::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the lexer line number

  long Reader::getlnum (void) const {
    rdlock ();
    try {
      long result = p_lex->getlnum ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Reader::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Reader;
    // check 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new Reader (*sobj);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new Reader (is);
      // invalid object
      throw Exception ("type-error", "invalid object for reader",
		       Object::repr (obj));
    }
    throw Exception ("argument-error", "too many argument for reader");
  }

  // return true if the given quark is defined

  bool Reader::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      // check the nameable class
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      // check the former class
      if (result == false) {
	result = hflg ? Former::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Reader::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // check the nameable class
    if (Nameable::isquark (quark, true) == true) {
      return Nameable::apply (zobj, nset, quark, argv);
    }
    // fallback with the former method
    return Former::apply (zobj, nset, quark, argv);
  }
}
