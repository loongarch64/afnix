// ---------------------------------------------------------------------------
// - Random.cpp                                                              -
// - afnix:mth module - random number generator implementation               -
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

#include "Byte.hpp"
#include "Cons.hpp"
#include "Real.hpp"
#include "Prime.hpp"
#include "Random.hpp"
#include "Vector.hpp"
#include "Bitset.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Relatif.hpp"
#include "Ivector.hpp"
#include "Nblock.hpp"
#include "Rblock.hpp"
#include "Rmatrix.hpp"
#include "Deviate.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // return a random byte

  Object* mth_byternd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 0 argument
      if (argc == 0) {
	delete argv; argv = nullptr;
	return new Byte (Utility::byternd ());
      }
      throw Exception ("argument-error", 
		       "too many arguments with get-random-integer");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // return a long random number

  Object* mth_longrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 0 argument
      if (argc == 0) {
	delete argv; argv = nullptr;
	return new Integer (Utility::longrnd ());
      }
      // check for 1 argument
      if (argc == 1) {
	long max = argv->getlong (0);
	delete argv; argv = nullptr;
	return new Integer (Utility::longrnd (max));
      }
      throw Exception ("argument-error", 
		       "too many arguments with get-random-integer");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return a real random number between 0.0 and 1.0

  Object* mth_realrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 0 argument
      if (argc == 0) {
	delete argv; argv = nullptr;
	return new Real (Utility::realrnd (true));
      }
      // check for 1 argument
      if (argc == 1) {
	bool iflg = argv->getbool (0);
	delete argv; argv = nullptr;
	return new Real (Utility::realrnd (iflg));
      }
      throw Exception ("argument-error", 
		       "too many arguments with get-random-real");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return a large random relatif number

  Object* mth_relnrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 1 argument
      if (argc == 1) {
	long bits = argv->getlong (0);
	delete argv; argv = nullptr;
	return new Relatif (Relatif::random (bits));
      }
      // check for 2 arguments
      if (argc == 2) {
	long bits = argv->getlong (0);
	bool oddf = argv->getbool (1);
	delete argv; argv = nullptr;
	return new Relatif (Relatif::random (bits, oddf));
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-relatif");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return a prime random relatif number

  Object* mth_primrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 1 argument
      if (argc == 1) {
	long bits = argv->getlong (0);
	delete argv; argv = nullptr;
	return new Relatif (Prime::random (bits));
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-prime");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // return a safe prime random relatif number

  Object* mth_safernd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 1 argument
      if (argc == 1) {
	long bits = argv->getlong (0);
	delete argv; argv = nullptr;
	return new Relatif (Prime::safe (bits));
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-safe");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // get a random bitset by size

  Object* mth_bitsrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 1 argument
      if (argc == 1) {
	long bits = argv->getlong (0);
	delete argv; argv = nullptr;
	return new Bitset (Bitset::random (bits));
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-bitset");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // get a random byte buffer by size

  Object* mth_bbufrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 1 argument
      if (argc == 1) {
	long size = argv->getlong (0);
	delete argv; argv = nullptr;
	return Buffer::random (size);
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-buffer");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // get a random integer vector by size

  Object* mth_ivecrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc == 2) {
	long size = argv->getlong (0);
	long lmax = argv->getlong (1);
	delete argv; argv = nullptr;
	return Ivector::random (size, 0, lmax);
      }
      // check for 3 arguments
      if (argc == 3) {
	long size = argv->getlong (0);
        long lmin = argv->getlong (1);
	long lmax = argv->getlong (2);
	delete argv; argv = nullptr;
	return Ivector::random (size, lmin, lmax);
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-i-vector");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // get a random numeral vector

  Object* mth_nvecrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc == 2) {
	long   size = argv->getlong (0);
	Object* obj = argv->get (1);
	// check for a numeral
	auto nval = dynamic_cast<Numeral*>(obj);
	if (nval != nullptr) {
	  Numeral rmax = *nval;
	  Numeral zero (rmax.getnumt ());
	  delete argv; argv = nullptr;
	  return Nvector::random (size, zero, rmax);
	}
	// check for an integer
	auto ival = dynamic_cast<Integer*>(obj);
	if (ival != nullptr) {
	  Numeral rmax = ival->tolong();
	  Numeral zero (rmax.getnumt ());
	  delete argv; argv = nullptr;
	  return Nvector::random (size, zero, rmax);
	}
	// check for a real
	auto rval = dynamic_cast<Real*>(obj);
	if (rval != nullptr) {
	  Numeral rmax = rval->toreal();
	  Numeral zero (rmax.getnumt ());
	  delete argv; argv = nullptr;
	  return Nvector::random (size, zero, rmax);
	}
	throw Exception ("type-error", "invalid object as numeral",
			 Object::repr (argv->get(1)));
      }
      // check for 3 arguments
      if (argc == 3) {
	long   size = argv->getlong (0);
	auto rval = dynamic_cast<Numeral*>(argv->get(1));
	if (rval == nullptr) {
	  throw Exception ("type-error", "invalid object as numeral",
			   Object::repr (argv->get(1)));
	}
	Numeral rmin = *rval;
        rval = dynamic_cast<Numeral*>(argv->get(2));
	if (rval == nullptr) {
	  throw Exception ("type-error", "invalid object as numeral",
			   Object::repr (argv->get(2)));
	}
	Numeral rmax = *rval;
	delete argv; argv = nullptr;
	return Nvector::random (size, rmin, rmax);
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-n-vector");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // get a random real vector

  Object* mth_rvecrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc == 2) {
	long   size = argv->getlong (0);
	t_real rmax = argv->getreal (1);
	delete argv; argv = nullptr;
	return Rvector::random (size, 0.0, rmax);
      }
      // check for 3 arguments
      if (argc == 3) {
	long   size = argv->getlong (0);
	t_real rmin = argv->getreal (1);
	t_real rmax = argv->getreal (2);
	delete argv; argv = nullptr;
	return Rvector::random (size, rmin, rmax);
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-r-vector");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // return a random numeral block

  Object* mth_nblkrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc == 2) {
	long   size = argv->getlong (0);
	auto rval = dynamic_cast<Numeral*>(argv->get(1));
	if (rval == nullptr) {
	  throw Exception ("type-error", "invalid object as numeral",
			   Object::repr (argv->get(1)));
	}
	Numeral rmax = *rval;
	Numeral zero (rmax.getnumt ());
	delete argv; argv = nullptr;
	return Nblock::random (size, zero, rmax);
      }
      // check for 4 arguments
      if (argc == 5) {
	long   rows = argv->getlong (0);
	long   cols = argv->getlong (1);
	auto rval = dynamic_cast<Numeral*>(argv->get(2));
	if (rval == nullptr) {
	  throw Exception ("type-error", "invalid object as numeral",
			   Object::repr (argv->get(2)));
	}
	Numeral rmin = *rval;
        rval = dynamic_cast<Numeral*>(argv->get(3));
	if (rval == nullptr) {
	  throw Exception ("type-error", "invalid object as numeral",
			   Object::repr (argv->get(3)));
	}
	Numeral rmax = *rval;
	delete argv; argv = nullptr;
	return Nblock::random (rows, cols, rmin, rmax);
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-n-block");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  
  // return a random real block

  Object* mth_rblkrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc == 2) {
	long   size = argv->getlong (0);
	t_real rmax = argv->getreal (1);
	delete argv; argv = nullptr;
	return Rblock::random (size, 0.0, rmax);
      }
      // check for 3 arguments
      if (argc == 3) {
	long   size = argv->getlong (0);
	t_real rmax = argv->getreal (1);
	bool   ddom = argv->getbool (2);
	delete argv; argv = nullptr;
	return Rblock::random (size, 0.0, rmax, ddom);
      }
      // check for 4 arguments
      if (argc == 4) {
	long   size = argv->getlong (0);
	t_real rmin = argv->getreal (1);
	t_real rmax = argv->getreal (2);
	bool   ddom = argv->getbool (3);
	delete argv; argv = nullptr;
	return Rblock::random (size, rmin, rmax, ddom);
      }
      // check for 5 arguments
      if (argc == 5) {
	long   rows = argv->getlong (0);
	long   cols = argv->getlong (1);
	t_real rmin = argv->getreal (2);
	t_real rmax = argv->getreal (3);
	bool   ddom = argv->getbool (4);
	delete argv; argv = nullptr;
	return Rblock::random (rows, cols, rmin, rmax, ddom);
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-r-block");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return a random matrix by size

  Object* mth_rmtxrnd (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 2 arguments
      if (argc == 2) {
	long   size = argv->getlong (0);
	t_real rmax = argv->getreal (1);
	delete argv; argv = nullptr;
	return Rmatrix::random (size, 0.0, rmax);
      }
      // check for 3 arguments
      if (argc == 3) {
	long   size = argv->getlong (0);
	t_real rmax = argv->getreal (1);
	bool   ddom = argv->getbool (2);
	delete argv; argv = nullptr;
	return Rmatrix::random (size, 0.0, rmax, ddom);
      }
      // check for 4 arguments
      if (argc == 4) {
	long   size = argv->getlong (0);
	t_real rmin = argv->getreal (1);
	t_real rmax = argv->getreal (2);
	bool   ddom = argv->getbool (3);
	delete argv; argv = nullptr;
	return Rmatrix::random (size, rmin, rmax, ddom);
      }
      throw Exception ("argument-error", 
		       "too many argument with get-random-r-matrix");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return a block matrix by size

  Object* mth_rblksps (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 3 arguments
      if (argc == 3) {
	long   size = argv->getlong (0);
	t_real rmax = argv->getreal (1);
	t_long nzsz = argv->getlong (2);
	delete argv; argv = nullptr;
	return Rblock::sparse (size, 0.0, rmax, nzsz);
      }
      // check for 4 arguments
      if (argc == 4) {
	long   size = argv->getlong (0);
	t_real rmin = argv->getreal (1);
	t_real rmax = argv->getreal (2);
	t_long nzsz = argv->getlong (3);
	delete argv; argv = nullptr;
	return Rblock::random (size, rmin, rmax, nzsz);
      }
      throw Exception ("argument-error", 
		       "too many argument with get-sparse-r-block");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return a sparse matrix by size

  Object* mth_rmtxsps (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 3 arguments
      if (argc == 3) {
	long   size = argv->getlong (0);
	t_real rmax = argv->getreal (1);
	t_long nzsz = argv->getlong (2);
	delete argv; argv = nullptr;
	return Rmatrix::sparse (size, 0.0, rmax, nzsz);
      }
      // check for 4 arguments
      if (argc == 4) {
	long   size = argv->getlong (0);
	t_real rmin = argv->getreal (1);
	t_real rmax = argv->getreal (2);
	t_long nzsz = argv->getlong (3);
	delete argv; argv = nullptr;
	return Rmatrix::random (size, rmin, rmax, nzsz);
      }
      throw Exception ("argument-error", 
		       "too many argument with get-sparse-r-matrix");
    } catch (...) {
      delete argv;
      throw;
    }
  }

  // return a uniform deviate

  Object* mth_unidev (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 0 argument
      if (argc == 0) {
	delete argv; argv = nullptr;
	return new Real (Deviate::uniform ());
      }
      throw Exception ("argument-error", 
		       "too many argument with get-uniform-deviate");
    } catch (...) {
      delete argv;
      throw;
    }
  }
  // return a normal deviate

  Object* mth_nrmdev (Evaluable* zobj, Nameset* nset, Cons* args) {
    // get the arguments
    Vector* argv = Vector::eval (zobj, nset, args);
    long    argc = (argv == nullptr) ? 0 : argv->length ();
    try {
      // check for 0 argument
      if (argc == 0) {
	delete argv; argv = nullptr;
	return new Real (Deviate::normal ());
      }
      throw Exception ("argument-error", 
		       "too many argument with get-normal-deviate");
    } catch (...) {
      delete argv;
      throw;
    }
  }
}
