// ---------------------------------------------------------------------------
// - NameTable.cpp                                                           -
// - standard object library - name table class implementation               -
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

#include "Stdsid.hxx"
#include "Integer.hpp"
#include "NameTable.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the name table node
  struct s_ntnode {
    // the object quark
    long d_quark;
    // the object 
    Object* p_object;
    // next record in the list
    s_ntnode* p_next;
    // simple constructor
    s_ntnode (void) {
      d_quark  = 0;
      p_object = nullptr;
      p_next   = nullptr;
    }
    // simple destructor
    ~s_ntnode (void) {
      Object::dref (p_object);
      delete p_next;
    }
  };
  
  // find a node by quark given its root node
  static inline s_ntnode* getnode (s_ntnode* node, const long quark) {
    // simple check as fast as we can
    if (node == nullptr) return nullptr;
    // loop until we have a match
    while (node != nullptr) {
      if (node->d_quark == quark) return node;
      node = node->p_next;
    }
    // no node found
    return nullptr;
  }
  
  // extract a node by quark given its root node . This procedure remove the
  // node if it is found and maintain the link list.
  static inline s_ntnode* rmnode (s_ntnode** root, const long quark) {
    s_ntnode* node = *root;
    // simple check as fast as we can
    if (node == nullptr) return nullptr;
    // first case for the root node
    if (node->d_quark == quark) {
      *root = node->p_next;
      node->p_next = nullptr;
      return node;
    }
    // loop until we have a match
    while (node->p_next != nullptr) {
      if (node->p_next->d_quark == quark) {
	s_ntnode* result = node->p_next;
	node->p_next = result->p_next;
	result->p_next = nullptr;
	return result;
      }
      node = node->p_next;
    } 
    // no node found
    return nullptr;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a new name table
  
  NameTable::NameTable (void) {
    p_table = nullptr;
  }
  
  // delete this name table but not the objects, norr the parent
  
  NameTable::~NameTable (void) {
    delete p_table;
  }

  // return the class name

  String NameTable::repr (void) const {
    return "NameTable";
  }

  // return the name table did

  t_word NameTable::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the name table sid

  t_word NameTable::getsid (void) const {
    return SRL_NTBL_SID;
  }

  // serialize this name table

  void NameTable::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the vector length
      long len = length ();
      Serial::wrlong (len, os);
      // write the objects
      for (long i = 0; i < len; i++) {
	// serialize the name
	String name = getname (i);
	name.wrstream (os);
	// serialize the object
	Object* obj = get (i);
	if (obj == nullptr) {
	  Serial::wrnilid (os);
	} else {
	  Serial* sobj = dynamic_cast <Serial*> (obj);
	  if (sobj == nullptr) {
	    throw Exception ("serial-error", "cannot serialize object", 
			     obj->repr ());
	  }
	  sobj->serialize (os);
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this name table

  void NameTable::rdstream (InputStream& is) {
    wrlock ();
    try {
      reset  ();
      // get the vector length
      long len = Serial::rdlong (is);
      // read in each object
      for (long i = 0; i < len; i++) {
	// deserialize the name
	String name;
	name.rdstream (is);
	// deserialize the object
	Object* obj = Serial::deserialize (is);
	// add the pair
	add (name, obj);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this name table

  void NameTable::reset (void) {
    wrlock ();
    try {
      delete p_table;
      p_table = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the name table size

  long NameTable::length (void) const {
    rdlock ();
    try {
      s_ntnode* node = p_table;
      long result  = 0;
      while (node != nullptr) {
	result++;
	node = node->p_next;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }


  // return an element name by index

  String NameTable::getname (const long index) const {
    rdlock ();
    try {
      s_ntnode* node = p_table;
      long    npos = 0;
      while (node != nullptr) {
	if (npos == index) {
	  String result = String::qmap (node->d_quark);
	  unlock ();
	  return result;
	}
	npos++;
	node = node->p_next;
      }
      throw Exception ("index-error", "index is out of range");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return an element object by index

  Object* NameTable::getobj (const long index) const {
    rdlock ();
    try {
      s_ntnode* node = p_table;
      long      npos = 0;
      while (node != nullptr) {
	if (npos == index) {
	  Object* result = node->p_object;
	  unlock ();
	  return result;
	}
	npos++;
	node = node->p_next;
      }
      throw Exception ("index-error", "index is out of range");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set or create an object in this table
  
  void NameTable::add (const long quark, Object* object) {
    // lock and check for shared
    wrlock ();
    try {
      // protect the object
      Object::iref (object);
      // look for existing symbol
      s_ntnode* node = getnode (p_table,quark);
      if (node != nullptr) {
	Object::dref (node->p_object);
	node->p_object = object;
	unlock ();
	return;
      }
      // the node does not exist, create it 
      node           = new s_ntnode;
      node->d_quark  = quark;
      node->p_object = object;
      node->p_next   = p_table;
      p_table        = node;
      unlock ();
    } catch (...) {
      Object::tref (object);
      unlock ();
      throw;
    }
  }
  
  // add an object by name

  void NameTable::add (const String& name, Object* object) {
    wrlock ();
    try {
      add (name.toquark (), object);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object by name. If the name is not found, nullptr is returned
  
  Object* NameTable::get (const long quark) const {
    rdlock ();
    try {
      // look for the node and find symbol
      s_ntnode* node = getnode (p_table, quark);
      Object* result = (node!= nullptr) ? node->p_object : nullptr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object by quark, if not found nil is returned
  
  Object* NameTable::get (const String& name) const {
    rdlock ();
    try {
      Object* result = get (name.toquark ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object by quark, if not found an exception is raised

  Object* NameTable::lookup (const long quark) const {
    rdlock ();
    try {
      // look for the node and find symbol
      s_ntnode* node = getnode (p_table,quark);
      if (node != nullptr) {
	Object* result = node->p_object;
	unlock ();
	return result;
      }
      throw Exception ("name-error", "name not found", String::qmap (quark));
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get an object by name or throw an exception

  Object* NameTable::lookup (const String& name) const {
    rdlock ();
    try {
      Object* result = lookup (name.toquark ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a quark exists in this table

  bool NameTable::exists (const long quark) const {
    rdlock ();
    try {
      // look for the node and find symbol
      s_ntnode* node = getnode (p_table,quark);
      bool result = (node != nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if the name exists in the table

  bool NameTable::exists (const String& name) const {
    rdlock ();
    try {
      bool result = exists (name.toquark ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // remove an object by quark
  
  void NameTable::remove (const long quark) {
    wrlock ();
    try {
      s_ntnode* node = rmnode (&p_table,quark);
      delete node;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove an object by name
  
  void NameTable::remove (const String& name) {
    wrlock ();
    try {
      remove (name.toquark ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }
}
