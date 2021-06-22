// ---------------------------------------------------------------------------
// - SioCalls.cpp                                                            -
// - afnix:sio module - i/o specific calls implementation                    -
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

#include "Cons.hpp"
#include "Vector.hpp"
#include "System.hpp"
#include "Boolean.hpp"
#include "SioCalls.hpp"
#include "Exception.hpp"
#include "InputFile.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // identify a serial object

  Object* sio_ident (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for one argument
      if (argc != 1) {
	throw Exception ("argument-error", "invalid arguments with ident");
      }
      Object* obj = argv->get (0);
      String* result = nullptr;
      // check for a string
      auto sobj = dynamic_cast<String*>(obj);
      if (sobj != nullptr) {
	InputFile is (*sobj);
	result = new String (Serial::identify (is));
      }
      // check for an input stream
      auto is = dynamic_cast<InputStream*>(obj);
      if (is != nullptr) {
	result = new String (Serial::identify (*is));
      }
      delete argv; argv = nullptr;
      return result;
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // rename a file by name

  Object* sio_mvfile (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 2) {
	String name = argv->getstring (0);
	String path = argv->getstring (1);
	delete argv; argv = nullptr;
	return new Boolean (System::mvfile (name, path));
      }
      throw Exception ("argument-error", 
		       "too many arguments with mvfile");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // remove a list of files by name

  Object* sio_rmfile (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      for (long i = 0; i < argc; i++) {
	String name = argv->getstring (i);
	if (System::rmfile (name) == false) {
	  throw Exception ("rmfile-error", "cannot remove file", name);
	}
      }
      delete argv;
      return nullptr;
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // create a list of directories by name

  Object* sio_mkdir (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      for (long i = 0; i < argc; i++) {
	String name = argv->getstring (i);
	if (System::mkdir (name) == false) {
	  throw Exception ("mkdir-error", "cannot create directory", name);
	}
      }
      delete argv;
      return nullptr;
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // create hierarchicaly a list of directories by name

  Object* sio_mhdir (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      for (long i = 0; i < argc; i++) {
	String name = argv->getstring (i);
	if (name.isnil () == true) continue;
	if (System::mhdir (name) == false) {
	  throw Exception ("mkdir-error", "cannot create directory", name);
	}
      }
      delete argv;
      return nullptr;
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // get a base name from a path

  Object* sio_xname (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String path = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (System::xname (path));
      }
      throw Exception ("argument-error", 
		       "too many arguments with get-base-name");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // get a directory base path from a path

  Object* sio_xdir (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String path = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (System::xdir (path));
      }
      throw Exception ("argument-error", 
		       "too many arguments with get-base-path");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // get the extension from a path

  Object* sio_xext (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String path = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (System::xext (path));
      }
      throw Exception ("argument-error", 
		       "too many arguments with get-extension");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // remove an extension from a path

  Object* sio_rmext (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 1) {
	String path = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (System::rmext (path));
      }
      throw Exception ("argument-error", 
		       "too many arguments with remove-extension");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // remove a list of directories by name

  Object* sio_rmdir (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      for (long i = 0; i < argc; i++) {
	String name = argv->getstring (i);
	if (System::rmdir (name) == false) {
	  throw Exception ("rmdir-error", "cannot remove directory", name);
	}
      }
      delete argv;
      return nullptr;
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // create a user data directory name

  Object* sio_usr_ddir (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 0) {
	delete argv; argv = nullptr;
	return new String (System::userdata ());
      }
      if (argc == 1) {
	String name = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (System::userdata (name));
      }
      throw Exception ("argument-error",
		       "too many arguments with user-data-directory");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  // create a temporary name

  Object* sio_tmp_name (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 0) {
	delete argv; argv = nullptr;
	return new String (System::tempnam ());
      }
      if (argc == 1) {
	String prefix = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (System::tempnam (prefix));
      }
      throw Exception ("argument-error", "too many arguments with tmp-name");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // create a temporary path

  Object* sio_tmp_path (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      if (argc == 0) {
	delete argv; argv = nullptr;
	return new String (System::temppth ());
      }
      if (argc == 1) {
	String prefix = argv->getstring (0);
	delete argv; argv = nullptr;
	return new String (System::temppth (prefix));
      }
      throw Exception ("argument-error", "too many arguments with tmp-path");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // create an absolute path name

  Object* sio_abs_path (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // root directory first
      if (argc == 0) {
	delete argv; argv = nullptr;
	return new String (System::getrdir ());
      }
      // first argument build root
      String result = System::getrdir ();
      result = result + argv->getstring (0);
      if (argc == 1) {
	delete argv; argv = nullptr;
	return new String (result);
      }
      // loop with the rest
      for (long i = 1; i < argc; i++) {
	result = System::join (result, argv->getstring (i));
      }
      delete argv; argv = nullptr;
      return new String (result);
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // create a relative path name

  Object* sio_rel_path (Evaluable* zobj, Nameset* nset, Cons* args) {
    // evaluate the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for at least one argument
      if (argc == 0) {
	throw Exception ("argument-error", 
			 "missing arguments with absolute-path");
      }
      // first argument starts the path
      String result = argv->getstring (0);
      // loop with the rest
      for (long i = 1; i < argc; i++)
	result = System::join (result, argv->getstring (i));
      delete argv; argv = nullptr;
      return new String (result);
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
