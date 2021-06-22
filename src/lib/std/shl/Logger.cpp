// ---------------------------------------------------------------------------
// - Logger.cpp                                                              -
// - standard object library - message logger class implementation           -
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

#include "Item.hpp"
#include "Date.hpp"
#include "Vector.hpp"
#include "Logger.hpp"
#include "Global.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputFile.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default stream operation mode
  static const String LOG_MODE_DEF = "UTF-8";

  // the message log structure
  struct s_mlog {
    // the message level
    Logger::t_mlvl d_mlvl;
    // the message time
    t_long d_time;
    // the message value
    String d_mesg;
    // create a default message log
    s_mlog (void) {
      reset ();
    }
    // assign a message log to this one
    s_mlog& operator = (const s_mlog& that) {
      if (this == &that) return *this;
      d_mlvl = that.d_mlvl;
      d_time = that.d_time;
      d_mesg = that.d_mesg;
      return *this;
    }
    // reset the message log
    void reset (void) {
      d_mlvl = Logger::MLVL_INFO;
      d_time = 0;
      d_mesg.clear();
    }
    // set a message with a log level
    void set (const Logger::t_mlvl mlvl, const String mesg) {
      d_mlvl = mlvl;
      d_time = Time::gettclk ();
      d_mesg = mesg;
    }
    // get a formated message
    String tostring (void) const {
      // format the message
      String result = "[";
      result += Date(d_time).toiso (true);
      result += "]";
      // add the level
      switch (d_mlvl) {
      case Logger::MLVL_FATL:
	result += "[F] ";
	break;
      case Logger::MLVL_XERR:
	result += "[E] ";
	break;
      case Logger::MLVL_WRNG:
	result += "[W] ";
	break;
      case Logger::MLVL_INFO:
	result += "[I] ";
	break;
      case Logger::MLVL_DBUG:
	result += "[D] ";
	break;
      }
      // add the message
      result += d_mesg;
      // here it is
      return result;
    }
  };

  // logger initial size
  static const long LOGGER_SIZE = 256L;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a new logger
  
  Logger::Logger (void) {
    p_os   = nullptr;
    d_size = LOGGER_SIZE;
    p_mlog = new s_mlog[d_size];
    d_ldbg = Global::getdbug ();
    reset ();
  }
  
  // create a new logger by size
  
  Logger::Logger (const long size) {
    p_os   = nullptr;
    d_size = (size > 0) ? size : LOGGER_SIZE;
    p_mlog = new s_mlog[d_size];
    d_ldbg = Global::getdbug ();
    reset ();
  }

  // destroy this logger

  Logger::~Logger (void) {
    Object::dref (p_os);
    delete [] p_mlog;
  }

  // return the class name

  String Logger::repr (void) const {
    return "Logger";
  }

  // reset this logger class

  void Logger::reset (void) {
    wrlock ();
    try {
      for (long k = 0L; k < d_size; k++) p_mlog[k].reset ();
      d_mcnt = 0;
      d_mpos = 0;
      d_base = 0;
      Object::dref (p_os); p_os = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the log length

  long Logger::length (void) const {
    rdlock ();
    try {
      long result = d_mcnt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a message in the logger by log level

  long Logger::add (const t_mlvl mlvl, const String& mesg) {
    wrlock ();
    try {
      // check for debug mode
      if ((mlvl == MLVL_DBUG) && (d_ldbg== false)) {
	unlock ();
	return -1L;
      }
      // save the message
      p_mlog[d_mpos].set (mlvl, mesg);
      // update position
      d_mpos = (d_mpos + 1) % d_size;
      // compute new indexes
      if (d_mcnt < d_size) {
	d_mcnt++;
      } else {
	d_base = (d_base + 1) % d_size;
      }
      // write on the output stream
      long result = d_mcnt - 1L;
      if (p_os != nullptr) p_os->writeln (getfull (result));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a message level by index

  Logger::t_mlvl Logger::getmlvl (const long index) const {
    rdlock ();
    try {
      if ((index < 0) || (index >= d_mcnt)) {
	throw Exception ("index-error", "index is out of range");
      }
      // compute position
      long pos = (index + d_base) % d_size;
      // get result
      t_mlvl result = p_mlog[pos].d_mlvl;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a message time by index

  t_long Logger::gettime (const long index) const {
    rdlock ();
    try {
      if ((index < 0) || (index >= d_mcnt)) {
	throw Exception ("index-error", "index is out of range");
      }
      // compute position
      long pos = (index + d_base) % d_size;
      // get result
      t_long result = p_mlog[pos].d_time;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a message by index

  String Logger::getmesg (const long index) const {
    rdlock ();
    try {
      if ((index < 0) || (index >= d_mcnt)) {
	throw Exception ("index-error", "index is out of range");
      }
      // compute position
      long pos = (index + d_base) % d_size;
      // get result
      String result = p_mlog[pos].d_mesg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a fully formatted message by index

  String Logger::getfull (const long index) const {
    rdlock ();
    try {
      if ((index < 0) || (index >= d_mcnt)) {
	throw Exception ("index-error", "index is out of range");
      }
      // compute position
      long pos = (index + d_base) % d_size;
      // get result
      String result = p_mlog[pos].tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the logger debug flag

  void Logger::setldbg (const bool ldbg) {
    wrlock ();
    try {
      d_ldbg = ldbg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the logger output stream

  void Logger::setos (OutputStream* os) {
    wrlock ();
    try {
      Object::iref (os);
      Object::dref (p_os);
      p_os = os;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the logger output by name

  void Logger::setos (const String& name) {
    wrlock ();
    try {
      // bind the output stream
      Object::dref (p_os); p_os = nullptr;
      Object::iref (p_os = new OutputFile (name));
      // set the default mode
      p_os->setemod (LOG_MODE_DEF);      
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this message log

  void Logger::resize (const long size) {
    wrlock ();
    try {
      // check the new size
      if (size < d_size) {
	unlock ();
	return;
      }
      // copy the old array in the new one
      s_mlog* mlog = new s_mlog[size];
      for (long i = 0; i < d_mcnt; i++) {
	long pos = (i + d_base) % d_size;
	mlog[i] = p_mlog[pos];
      }
      // finish the initialization
      for (long i = d_mcnt; i < size; i++) mlog[i].reset ();
      // clean and update indexes
      delete [] p_mlog;
      p_mlog = mlog;
      d_size = size;
      d_mpos = d_mcnt;
      d_base = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the object eval quarks
  static const long QUARK_LOGGER   = String::intern ("Logger");
  static const long QUARK_MLVLFATL = String::intern ("FATAL");
  static const long QUARK_MLVLXERR = String::intern ("ERROR");
  static const long QUARK_MLVLWRNG = String::intern ("WARNING");
  static const long QUARK_MLVLINFO = String::intern ("INFO");
  static const long QUARK_MLVLDBUG = String::intern ("DEBUG");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 11;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_PUSH    = zone.intern ("push");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_SETOS   = zone.intern ("set-output-stream");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_RESIZE  = zone.intern ("resize");
  static const long QUARK_SETLDBG = zone.intern ("set-debug");
  static const long QUARK_GETMESG = zone.intern ("get-message");
  static const long QUARK_GETFULL = zone.intern ("get-full-message");
  static const long QUARK_GETMTIM = zone.intern ("get-message-time");
  static const long QUARK_GETMLVL = zone.intern ("get-message-level");
  
  // map an item to a message level type
  static inline Logger::t_mlvl item_to_mlvl (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_LOGGER)
      throw Exception ("item-error", "item is not a logger item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_MLVLFATL) return Logger::MLVL_FATL;
    if (quark == QUARK_MLVLXERR) return Logger::MLVL_XERR;
    if (quark == QUARK_MLVLWRNG) return Logger::MLVL_WRNG;
    if (quark == QUARK_MLVLINFO) return Logger::MLVL_INFO;
    if (quark == QUARK_MLVLDBUG) return Logger::MLVL_DBUG;
    throw Exception ("item-error", "cannot map item to message level type");
  }
  
  // map a key type to an item
  static inline Item* mlvl_to_item (const Logger::t_mlvl type) {
    switch (type) {
    case Logger::MLVL_FATL:
      return new Item (QUARK_LOGGER, QUARK_MLVLFATL);
      break;
    case Logger::MLVL_XERR:
      return new Item (QUARK_LOGGER, QUARK_MLVLXERR);
      break;
    case Logger::MLVL_WRNG:
      return new Item (QUARK_LOGGER, QUARK_MLVLWRNG);
      break;
    case Logger::MLVL_INFO:
      return new Item (QUARK_LOGGER, QUARK_MLVLINFO);
      break;
    case Logger::MLVL_DBUG:
      return new Item (QUARK_LOGGER, QUARK_MLVLDBUG);
      break;
    }
    return nullptr;
  }
  
  // evaluate an object data member

  Object* Logger::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_MLVLFATL)
      return new Item (QUARK_LOGGER, QUARK_MLVLFATL);
    if (quark == QUARK_MLVLXERR)
      return new Item (QUARK_LOGGER, QUARK_MLVLXERR);
    if (quark == QUARK_MLVLWRNG)
      return new Item (QUARK_LOGGER, QUARK_MLVLWRNG);
    if (quark == QUARK_MLVLINFO)
      return new Item (QUARK_LOGGER, QUARK_MLVLINFO);
    if (quark == QUARK_MLVLDBUG)
      return new Item (QUARK_LOGGER, QUARK_MLVLDBUG);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // create a new object in a generic way
  
  Object* Logger::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Logger;
    // check 1 argument
    if (argc == 1) {
      long size = argv->getlong (0);
      return new Logger (size);
    }
    throw Exception ("argument-error", "too many arguments for logger");
  }

  // return true if the given quark is defined
  
  bool Logger::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object class with a set of arguments and a quark
  
  Object* Logger::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispath generic argument
    if (argc > 1) {
      if ((quark == QUARK_ADD) || (quark == QUARK_PUSH)) {
	// get an item type
	Object* obj = argv->get(0);
	auto item = dynamic_cast<Item*>(obj);
	if (item == nullptr) {
	  throw Exception ("type-error", "invalid object as logger level",
			   Object::repr (obj));
	}
	// map the message level
	t_mlvl mlvl = item_to_mlvl (*item);
	// loop in the literals
	String mesg;
	for (long k = 1L; k < argc; k++) {
	  // get the literal
	  obj = argv->get (k);
	  auto lobj = dynamic_cast<Literal*>(obj);
	  if ((lobj == nullptr) && (obj != nullptr)) {
	    throw Exception ("type-error", "invalid object as literal",
			     Object::repr (obj));
	  }
	  // rebuild the final message
	  if ((mesg.isnil () == false) && (quark == QUARK_PUSH)) mesg += ' ';
	  mesg += lobj->tostring ();
	}
	// add the message
	return new Integer (add (mlvl, mesg));
      }
    }
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETMESG) {
	long index = argv->getlong (0);
	return new String (getmesg (index));
      }
      if (quark == QUARK_GETFULL) {
	long index = argv->getlong (0);
	return new String (getfull (index));
      }
      if (quark == QUARK_GETMTIM) {
	long index = argv->getlong (0);
	return new Integer (gettime (index));
      }
      
      if (quark == QUARK_GETMLVL) {
	long index = argv->getlong (0);
	return mlvl_to_item(getmlvl(index));
      }
      if (quark == QUARK_RESIZE) {
	long size = argv->getlong (0);
	resize (size);
	return nullptr;
      }
      if (quark == QUARK_SETLDBG) {
	bool ldbg = argv->getbool (0);
	setldbg (ldbg);
	return nullptr;
      }
      if (quark == QUARK_SETOS) {
	Object* obj = argv->get (0);
	// check for an output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  setos (os);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  setos (*sobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object set-output-stream",
			 Object::repr (obj));
      }
    }
    // apply these arguments with the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
