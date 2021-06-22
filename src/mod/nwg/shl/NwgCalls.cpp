// ---------------------------------------------------------------------------
// - NwgCalls.cpp                                                            -
// - afnix:nwg module - specific calls implementation                        -
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

#include "Uri.hpp"
#include "Cons.hpp"
#include "Mime.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "NwgCalls.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // check if a mime extension is defined

  Object* nwg_mextp (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String mext = argv->getstring (0);
	delete argv; argv = nullptr;
	return new Boolean (Mime::ismext (mext));
      }
      throw Exception ("argument-error", 
		       "too many arguments with mime-extension-p");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // check if a mime value is defined

  Object* nwg_mvalp (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String mval = argv->getstring (0);
	delete argv; argv = nullptr;
	return new Boolean (Mime::ismval (mval));
      }
      throw Exception ("argument-error", 
		       "too many arguments with mime-value-p");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // check for a valid xml mime value

  Object* nwg_xmlmp (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String mval = argv->getstring (0);
	delete argv; argv = nullptr;
	return new Boolean (Mime::isxmlm (mval));
      }
      throw Exception ("argument-error", 
		       "too many arguments with mime-xml-p");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // get a mime value by extension

  Object* nwg_tomime (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String mext = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (Mime::tomime (mext, false));
      }
      if (argc == 2) {
	String mext = argv->getstring (0);
	bool   dflg = argv->getbool   (1);
	delete argv; argv = nullptr;
	return new String (Mime::tomime (mext, dflg));
      }
      throw Exception ("argument-error", 
		       "too many arguments with extension-to-mime");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // get an extension by mime value

  Object* nwg_tomext (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String mime = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (Mime::tomext (mime));
      }
      throw Exception ("argument-error", 
		       "too many arguments with extension-to-mime");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // percent encode a string

  Object* nwg_pencd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String sval = argv->getstring (0);
	delete argv; argv= nullptr;
	return new String (Uri::pencode (sval));
      }
      throw Exception ("argument-error", 
		       "too many arguments with percent-encode");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // uri encode a string

  Object* nwg_uencd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String sval = argv->getstring (0);
	delete argv; argv= nullptr;
	return new String (Uri::uencode (sval));
      }
      throw Exception ("argument-error", 
		       "too many arguments with uri-encode");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // component encode a string

  Object* nwg_cencd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String sval = argv->getstring (0);
	delete argv; argv= nullptr;
	return new String (Uri::cencode (sval));
      }
      throw Exception ("argument-error", 
		       "too many arguments with component-encode");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // percent decode a string

  Object* nwg_pdecd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String sval = argv->getstring (0);
	delete argv; argv= nullptr;
	return new String (Uri::pdecode (sval));
      }
      throw Exception ("argument-error", 
		       "too many arguments with percent-decode");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // check for a normal uri string

  Object* nwg_surip (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String suri = argv->getstring (0);
	delete argv; argv= nullptr;
	return new Boolean (Uri::isuri (suri));
      }
      throw Exception ("argument-error", 
		       "too many arguments with string-uri-p");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // create a string uri by scheme, host and port

  Object* nwg_tosuri (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 2) {
	String schm = argv->getstring (0);
	String host = argv->getstring (1);
	delete argv; argv= nullptr;
	return new String (Uri::tosuri (schm, host, 0));
      }
      if (argc == 3) {
	String schm = argv->getstring (0);
	String host = argv->getstring (1);
	long   port = argv->getlong   (2);
	delete argv; argv= nullptr;
	return new String (Uri::tosuri (schm, host, port));
      }
      throw Exception ("argument-error", 
		       "too many arguments with to-string-uri");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // normalize a uri name by adding a scheme if any

  Object* nwg_nrmunm (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String name = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (Uri::nrmname (name));
      }
      if (argc == 2) {
	String name = argv->getstring (0);
	bool   flag = argv->getbool   (1);
	delete argv; argv = nullptr;
	Uri uri = Uri::nrmname (name);
	if (flag == true) uri.nrmauth ();
	return new String (uri.getanam ());
      }
      throw Exception ("argument-error", 
		       "too many arguments with normalize-uri-name");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // normalize a uri name by prioritizing the system path

  Object* nwg_sysunm (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String path = argv->getstring (0);
	delete argv; argv= nullptr;
	return new String (Uri::sysname (path));
      }
      throw Exception ("argument-error", 
		       "too many arguments with system-uri-name");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // normalize a path by uri name

  Object* nwg_pthunm (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String path = argv->getstring (0);
	delete argv; argv= nullptr;
	return new String (Uri::pthname (path));
      }
      throw Exception ("argument-error", 
		       "too many arguments with path-uri-name");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // normalize a host by authority

  Object* nwg_nrmhost (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String auth = argv->getstring (0);
	delete argv; argv= nullptr;
	return new String (Uri::nrmhost (auth));
      }
      throw Exception ("argument-error", 
		       "too many arguments with normalize-uri-host");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // normalize a port by authority

  Object* nwg_nrmport (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String auth = argv->getstring (0);
	delete argv; argv= nullptr;
	return new Integer (Uri::nrmport (auth));
      }
      throw Exception ("argument-error", 
		       "too many arguments with normalize-uri-port");
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
