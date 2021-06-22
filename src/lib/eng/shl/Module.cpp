// ---------------------------------------------------------------------------
// - Module.cpp                                                              -
// - afnix engine - module class implementation                              -
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

#include "Module.hpp"
#include "Reader.hpp"
#include "Extracter.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the afnix magic number
  const long AXC_MSIZE   = 4;
  const char AXC_MAGIC[] = {'\177', 'A', 'X', 'C'};

  // this function write the module header to an output stream
  static void write_module_magic (OutputStream& os) {
    for (long i = 0; i < AXC_MSIZE; i++) os.write (AXC_MAGIC[i]);
  }

  // this function check that the header matches the axc magic number
  static Module::t_mtype get_module_type (InputStream* is) {
    // check for nil and reset
    if (is == nullptr) return Module::REGULAR;
    // read in the magic number
    char mbuf[AXC_MSIZE];
    for (long i = 0; i < AXC_MSIZE; i++) {
      mbuf[i] = is->read ();
      if (mbuf[i] != AXC_MAGIC[i]) {
	is->pushback (mbuf, i+1);
	return Module::REGULAR;
      }
    }
    return Module::COMPILED;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a compiled module

  Module::Module (void) {
    d_type   = COMPILED;
    p_is     = nullptr;
    p_former = nullptr;
  }

  // open a new module by name

  Module::Module (const String& name) {
    Module (new InputFile (name), name);
  }

  // open a module by stream

  Module::Module (InputStream* is, const String& name) {
    // bind the input stream
    Object::iref (p_is = is);
    // find out the file type
    d_type = get_module_type (p_is);
    // create a new form reader
    if (d_type == REGULAR) {
      Reader* rd = new Reader (p_is);
      p_former   = rd;
      rd->setname (name);
    } else {
      // force the natural encoding in compiled mode
      if (p_is != nullptr) p_is->setemod (Encoding::getnem ());
      // create the form extractor
      p_former = new Extracter (p_is);
    }
    // save the module name
    d_name = name;
  }
  
  // delete this module

  Module::~Module (void) {
    Object::dref (p_is);
    delete p_former;
  }

  // return the class name

  String Module::repr (void) const {
    return "Module";
  }

  // get the next form available in this module

  Form* Module::parse (void) {
    rdlock ();
    try {
      // check that we have a form reader
      if (p_former == nullptr) {
	throw Exception ("module-error", "no former installed");
      }
      Form* result = p_former->parse ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the module is regular

  bool Module::isregular (void) const {
    rdlock ();
    try {
      bool result = (d_type == REGULAR);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the module type

  Module::t_mtype Module::gettype (void) const {
    rdlock ();
    try {
      t_mtype result = d_type;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the module name

  String Module::getname (void) const {
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

  // return the reader line number

  long Module::getlnum (void) const {
    rdlock ();
    try {
      // check that we have a form reader
      if (p_former == nullptr) {
	throw Exception ("module-error", "no former installed");
      }
      // get the line number
      long result = p_former->getlnum ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a module to an output stream

  void Module::write (OutputStream& os) {
    rdlock ();
    try {
      // write the module header
      write_module_magic (os);
      // parse and serialize
      while (true) {
	Cons* cons = parse ();
	if (cons == nullptr) break;
	cons->serialize (os);
	Object::dref (cons);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
