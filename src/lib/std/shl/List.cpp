// ---------------------------------------------------------------------------
// - List.cpp                                                                -
// - standard object library - doubly linked list class implementation       -
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

#include "List.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the private list structure
  struct s_list {
    // the object to store
    Object* p_object;
    // the previous element
    s_list* p_prev;
    // the next element;
    s_list* p_next;
    // simple constructor
    s_list (void) {
      p_object = nullptr;
      p_prev   = nullptr;
      p_next   = nullptr;
    }
    // simple destructor
    ~s_list (void) {
      Object::dref (p_object);
      delete p_next;
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty list

  List::List (void) {
    p_root = nullptr;
    p_last = nullptr;
  }

  // copy constructor for this list

  List::List (const List& that) {
    that.rdlock ();
    try {
      p_root = nullptr;
      p_last = nullptr;
      s_list* node = that.p_root;
      while (node != nullptr) {
	add (node->p_object);
	node = node->p_next;
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
    
  // destroy this list

  List::~List (void) {
    delete p_root;
  }

  // return the class name
  
  String List::repr (void) const {
    return "List";
  }

  // return the list did

  t_word List::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the list sid

  t_word List::getsid (void) const {
    return SRL_LIST_SID;
  }

  // serialize this list

  void List::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the list length
      long len = length ();
      Serial::wrlong (len, os);
      // loop in list
      s_list* node = p_root;
      while (node != nullptr) {
	// get the object
	Object* obj = node->p_object;
	if (obj == nullptr) {
	  Serial::wrnilid (os);
	} else {
	  // try to serialize the objects
	  Serial* sobj = dynamic_cast <Serial*> (obj);
	  if (sobj == nullptr) {
	    throw Exception ("serial-error", "cannot serialize object",
			     obj->repr ());
	  }
	  sobj->serialize (os);
	}
	// get the next node
	node = node->p_next;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this list

  void List::rdstream (InputStream& is) {
    wrlock ();
    try {
      // get the list length
      long len = Serial::rdlong (is);
      // read in each object
      for (long i = 0; i < len; i++) add (Serial::deserialize (is));
      unlock ();
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // assign a list to this one

  List& List::operator = (const List& that) {
    if (this == &that) return *this;
    wrlock ();
    that.rdlock ();
    try {
      delete p_root;
      p_root = nullptr;
      p_last = nullptr;
      s_list* node = that.p_root;
      while (node != nullptr) {
	add (node->p_object);
	node = node->p_next;
      }
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }

  // insert an object at the beginning of the list

  void List::insert (Object* object) {
    wrlock ();
    try {
      // insert in the list
      s_list* node   = new s_list;
      node->p_object = Object::iref (object);
      node->p_next   = p_root;
      if (p_root == nullptr) {
	p_root = node;
	p_last = node;
	unlock ();
	return;
      }
      p_root->p_prev = node;
      p_root = node;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an object to the end of the list

  void List::add (Object* object) {
    wrlock ();
    try {
      // add in the list
      s_list* node   = new s_list;
      node->p_object = Object::iref (object);
      if (p_root == nullptr) {
	p_root = node;
	p_last = node;
	unlock ();
	return;
      }
      p_last->p_next = node;
      node->p_prev = p_last;
      p_last = node;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of elements in the list

  long List::length (void) const {
    rdlock ();
    try {
      s_list* node = p_root;
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

  // return an object by index

  Object* List::get (const long index) const {
    rdlock ();
    try {
      long count   = 0;
      s_list* node = p_root;
      if (index < 0) {
	throw Exception ("index-error", "invalid negative index in list get");
      }
      // loop in the list
      while (node != nullptr) {
	if (count == index) {
	  Object* result = node->p_object;
	  unlock ();
	  return result;
	}
	count++;
	node = node->p_next;
      }
      unlock ();
      throw Exception ("index-error", "invalid index in list get method");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a new iterator for this list
  
  Iterator* List::makeit (void) {
    rdlock ();
    try {
      Iterator* result = new Listit (this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an iterator at the beginning

  Listit List::begin (void) {
    rdlock ();
    try {
      Listit result = this;
      result.begin ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get an iterator at the end

  Listit List::end (void) {
    rdlock ();
    try {
      Listit result = this;
      result.end ();
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

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_LENGTH = zone.intern ("length");
  static const long QUARK_INSERT = zone.intern ("insert");

  // create a new object in a generic way

  Object* List::mknew (Vector* argv) {
    long len = 0;
    if ((argv == nullptr) || ((len = argv->length ()) == 0)) return new List;
    // build the list
    List* result = new List;
    for (long i = 0; i < len; i++)
      result->add (argv->get (i));
    return result;
  }

  // return true if the given quark is defined

  bool List::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Iterable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Serial::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* List::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADD) {
	Object* result = argv->get (0);
	add (result);
	zobj->post (result);
	return result;
      }

      if (quark == QUARK_INSERT) {
	Object* result = argv->get (0);
	insert (result);
	zobj->post (result);
	return result;
      }

      if (quark == QUARK_GET) {
	rdlock ();
	try {
	  long val = argv->getlong (0);
	  Object* result = get (val);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check the iterable method
    if (Iterable::isquark (quark, true) == true) {
      return Iterable::apply (zobj, nset, quark, argv);
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }

  // -------------------------------------------------------------------------
  // - iterator section                                                      -
  // -------------------------------------------------------------------------

  // create a new list iterator

  Listit::Listit (List* lst) {
    Object::iref (p_list = lst);
    p_node = nullptr;
    begin ();
  }

  // copy construct this iterator

  Listit::Listit (const Listit& that) {
    that.rdlock ();
    try {
      Object::iref (p_list = that.p_list);
      p_node = that.p_node;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign an iterator to this one

  Listit& Listit::operator = (const Listit& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Object::iref (that.p_list);
      Object::dref (p_list);
      p_list = that.p_list;
      p_node = that.p_node;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // compare two iterators

  bool Listit::operator == (const Listit& it) const {
    rdlock ();
    try {
      bool result = (p_list == it.p_list) && (p_node == it.p_node);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare two iterators

  bool Listit::operator != (const Listit& it) const {
    rdlock ();
    try {
      bool result = (p_list != it.p_list) || (p_node != it.p_node);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the iterator to the next position

  Listit& Listit::operator ++ (void) {
    wrlock ();
    try {
      next ();
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // move the iterator to the previous position

  Listit& Listit::operator -- (void) {
    wrlock ();
    try {
      prev ();
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the iterator object

  Object* Listit::operator * (void) const {
    rdlock ();
    try {
      Object* result = getobj ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // destroy this list iterator

  Listit::~Listit (void) {
    Object::dref (p_list);
  }

  // return the class name

  String Listit::repr (void) const {
    return "Listit";
  }

  // reset the iterator to the begining

  void Listit::begin (void) {
    wrlock ();
    if (p_list != nullptr) p_list->rdlock ();
    try {
      p_node = (p_list == nullptr) ? nullptr : p_list->p_root;
      if (p_list != nullptr) p_list->unlock ();
      unlock ();
    } catch (...) {
      if (p_list != nullptr) p_list->unlock ();
      unlock ();
      throw;
    }
  }

  // reset the iterator to the end

  void Listit::end (void) {
    wrlock ();
    if (p_list != nullptr) p_list->rdlock ();
    try {
      p_node = (p_list == nullptr) ? nullptr : p_list->p_last;
      if (p_list != nullptr) p_list->unlock ();
      unlock ();
    } catch (...) {
      if (p_list != nullptr) p_list->unlock ();
      unlock ();
      throw;
    }      
  }

  // go to the next object

  void Listit::next (void) {
    wrlock ();
    if (p_list != nullptr) p_list->rdlock ();
    try {
      if (p_node != nullptr) p_node = p_node->p_next;
      if (p_list != nullptr) p_list->unlock ();
      unlock ();
    } catch (...) {
      if (p_list != nullptr) p_list->unlock ();
      unlock ();
      throw;
    }
  }

  // go to the previous object
  void Listit::prev (void) {
    wrlock ();
    if (p_list != nullptr) p_list->rdlock ();
    try {
      if (p_node != nullptr) p_node = p_node->p_prev;
      if (p_list != nullptr) p_list->unlock ();
      unlock ();
    } catch (...) {
      if (p_list != nullptr) p_list->unlock ();
      unlock ();
      throw;
    }
  }

  // get the object at the current position

  Object* Listit::getobj (void) const {
    rdlock ();
    try {
      Object* result = (p_node == nullptr) ? nullptr : p_node->p_object;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the iterator is at the end
  
  bool Listit::isend (void) const {
    rdlock ();
    try {
      bool result = (p_node == nullptr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
