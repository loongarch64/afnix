// ---------------------------------------------------------------------------
// - Message.cpp                                                             -
// - standard object library - message class implementation                  -
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
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Message.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default message

  Message::Message (void) {
    d_type = MESG_NONE;
    d_code = 0;
  }

  // create a message by name

  Message::Message (const String& name) : Taggable (name) {
    d_type = MESG_NONE;
    d_code = 0;
  }

  // create a message by name and info

  Message::Message (const String& name,
		    const String& info) : Taggable(name, info) {
    d_type = MESG_INFO;
    d_code = 0;
  }

  // create a message by name, info and code

  Message::Message (const String& name, const String& info, const long code) :
    Taggable (name, info) {
    d_type = MESG_NONE;
    d_code = code;
  }

  // create a message by type, name, info and code

  Message::Message (const t_mesg type, const String& name, const String& info,
		    const long code) : Taggable (name, info) {
    d_type = type;
    d_code = code;
  }


  // copy construct this message

  Message::Message (const Message& that) {
    that.rdlock ();
    try {
      // copy base taggable
      Taggable::operator = (that);
      // copy locally
      d_type = that.d_type;
      d_code = that.d_code;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this message

  Message::Message (Message&& that) noexcept :
    Taggable(static_cast<Taggable&&>(that)) {
    that.wrlock ();
    try {
      d_type = that.d_type; that.d_type = MESG_NONE;
      d_code = that.d_code; that.d_code = 0L;
      that.unlock ();
    } catch (...) {
      d_type = MESG_NONE;
      d_code = 0L;
    }
    that.unlock ();
  }

  // assign a message to this one

  Message& Message::operator = (const Message& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base taggable
      Taggable::operator = (that);
      // assign locally
      d_type = that.d_type;
      d_code = that.d_code;
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move a message to this one

  Message& Message::operator = (Message&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // move base taggable
      Taggable::operator = (static_cast<Taggable&&>(that));
      // assign locally
      d_type = that.d_type; that.d_type = MESG_NONE;
      d_code = that.d_code; that.d_code = 0L;
    } catch (...) {
      d_type = MESG_NONE;
      d_code = 0L;
    }
    unlock ();
    that.unlock ();
    return *this;
  }

  // return the class name

  String Message::repr (void) const {
    return "Message";
  }
 
  // return a clone of this object

  Object* Message::clone (void) const {
    return new Message (*this);
  }

  // return the message did

  t_word Message::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the message sid

  t_word Message::getsid (void) const {
    return SRL_MESG_SID;
  }

  // serialize this message

  void Message::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the taggable
      Taggable::wrstream (os);
      // serialize locally
      Serial::wrbyte (d_type, os);
      Serial::wrlong (d_code, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this message

  void Message::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the taggable
      Taggable::rdstream (is);
      // deserialize locally
      d_type = static_cast<t_mesg>(Serial::rdbyte (is));
      d_code = Serial::rdlong (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a message by type, info and code
  void Message::set (const t_mesg type, const String& name, const String& info,
		     const long   code) {
    wrlock ();
    try {
      d_type = type;
      d_name = name;
      d_info = info;
      d_code = code;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the message type

  void Message::settype (const t_mesg type) {
    wrlock ();
    try {
      d_type = type;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the message type

  Message::t_mesg Message::gettype (void) const {
    rdlock ();
    try {
      t_mesg result = d_type;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the message code

  void Message::setcode (const long code) {
    wrlock ();
    try {
      d_code = code;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the message code

  long Message::getcode (void) const {
    rdlock ();
    try {
      long result = d_code;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the message as a string

  String Message::format (void) const {
    rdlock ();
    try {
      String result;
      switch (d_type) {
      case MESG_NONE:
	result = "[N:";
	break;
      case MESG_INFO:
	result = "[I:";
	break;
      case MESG_WARN:
	result = "[W:";
	break;
      case MESG_MERR:
	result = "[E:";
	break;
      case MESG_FAIL:
	result = "[F:";
	break;
      }
      // add the message code
      result += Utility::tostring (d_code) + ']';
      // add the message name
      if (d_name.isnil () == false) {
	result += ' ' ;
	result += d_name;
      }
      // add the message info
      if (d_info.isnil () == false) {
	result += " (" ;
	result += d_info;
	result += ')';
      }
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

  // the object eval quarks
  static const long QUARK_NONE = String::intern ("NONE");
  static const long QUARK_INFO = String::intern ("INFO");
  static const long QUARK_WARN = String::intern ("WARNING");
  static const long QUARK_MERR = String::intern ("ERROR");
  static const long QUARK_FAIL = String::intern ("FATAL");
  static const long QUARK_MESG = String::intern ("Message");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_FORMAT  = zone.intern ("format");
  static const long QUARK_SETTYPE = zone.intern ("set-type");
  static const long QUARK_GETTYPE = zone.intern ("get-type");
  static const long QUARK_SETCODE = zone.intern ("set-code");
  static const long QUARK_GETCODE = zone.intern ("get-code");

  // map an item to a message type
  static inline Message::t_mesg item_to_mesg (const Item& item) {
    // check for a stream item 
    if (item.gettid () != QUARK_MESG) {
      throw Exception ("item-error", "item is not a message item");
    }
    // map the item to the enumeration
    long quark = item.getquark (); 
    if (quark == QUARK_NONE) return Message::MESG_NONE;
    if (quark == QUARK_INFO) return Message::MESG_INFO;
    if (quark == QUARK_WARN) return Message::MESG_WARN;
    if (quark == QUARK_MERR) return Message::MESG_MERR;
    if (quark == QUARK_FAIL) return Message::MESG_FAIL;
    throw Exception ("item-error", "cannot map item to message type");
  }

  // map a type to an item
  static inline Item* mesg_to_item (const Message::t_mesg type) {
    Item* result  = nullptr;
    switch (type) {
    case Message::MESG_NONE:
      result = new Item (QUARK_MESG, QUARK_NONE);
      break;
    case Message::MESG_INFO:
      result = new Item (QUARK_MESG, QUARK_INFO);
      break;
    case Message::MESG_WARN:
      result = new Item (QUARK_MESG, QUARK_WARN);
      break;
    case Message::MESG_MERR:
      result = new Item (QUARK_MESG, QUARK_MERR);
      break;
    case Message::MESG_FAIL:
      result = new Item (QUARK_MESG, QUARK_FAIL);
      break;
    }
    return result;
  }

  // create a new object in a generic way
  
  Object* Message::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Message;
    // check 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Message (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Message (name, info);
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      long   code = argv->getlong   (2);
      return new Message (name, info, code);
    }
    // check for 4 arguments
    if (argc == 4) {
      Object* obj = argv->get (0);
      Item*  iobj = dynamic_cast <Item*> (obj);
      if (iobj == nullptr) {
	throw Exception ("type-error", "invalid object for message type",
			 Object::repr (obj));
      }
      String name = argv->getstring (1);
      String info = argv->getstring (2);
      long   code = argv->getlong   (3);
      return new Message (item_to_mesg (*iobj), name, info, code);
    }
    throw Exception ("argument-error", "too many argument for message");
  }

  // evaluate a quark statically

  Object* Message::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_INFO)
      return new Item (QUARK_MESG, QUARK_INFO);
    if (quark == QUARK_WARN) 
      return new Item (QUARK_MESG, QUARK_WARN);
    if (quark == QUARK_MERR) 
      return new Item (QUARK_MESG, QUARK_MERR);
    if (quark == QUARK_FAIL) 
      return new Item (QUARK_MESG, QUARK_FAIL);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // return true if the given quark is defined

  bool Message::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Taggable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Message::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_FORMAT)  return new String   (format  ());
      if (quark == QUARK_GETTYPE) return mesg_to_item (gettype ());
      if (quark == QUARK_GETCODE) return new Integer  (getcode ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETTYPE) {
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object for message type",
			   Object::repr (obj));
	}
	settype (item_to_mesg (*iobj));
	return nullptr;
      }
      if (quark == QUARK_SETCODE) {
	long code = argv->getlong (0);
	setcode (code);
	return nullptr;
      }
    }
    // dispatch 4 arguments
    if (argc == 4) {
      if (quark == QUARK_SET) {
	Object* obj = argv->get (0);
	Item*  iobj = dynamic_cast <Item*> (obj);
	if (iobj == nullptr) {
	  throw Exception ("type-error", "invalid object for message type",
			   Object::repr (obj));
	}
	String name = argv->getstring (1);
	String info = argv->getstring (2);
	long   code = argv->getlong   (3);
	set (item_to_mesg (*iobj), name, info, code);
	return nullptr;
      }
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
