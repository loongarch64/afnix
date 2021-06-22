// ---------------------------------------------------------------------------
// - Central.cpp                                                             -
// - standard object library - central object control class implementation   -
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

#include "Prime.hpp"
#include "Strvec.hpp"
#include "Central.hpp"
#include "Monitor.hpp"
#include "cmem.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - tokamak section                                                       -
  // -------------------------------------------------------------------------

  // the tokamak internal class
  class Tokamak {
  private:
    // the tokamak hash table node
    struct s_tnod {
      // the quark name
      String d_name;
      // the hashid value
      long   d_hvl;
      // the quark value 
      long  d_quark;
      // next record in the list
      s_tnod* p_next;
      // simple constructor
      s_tnod (void) {
	d_hvl   = 0;
	d_quark = 0;
      }
      // simple destructor
      ~s_tnod (void) {
	delete p_next;
      }
    };
    
    // find a node by name given its root node
    static inline s_tnod* getnode (s_tnod* node, const String& name) {
      // simple check as fast as we can
      if (node == nullptr) return nullptr;
      // loop until we have a match
      while (node != nullptr) {
	if (node->d_name == name) return node;
	node = node->p_next;
      }
      // no node found
      return nullptr;
    }

  private:
    // the hash table size
    long d_size;
    // the number of elements
    long d_tlen;
    // the threshold before resizing
    long d_thrs;
    // the hash table is an array of quark node
    s_tnod** p_tnod;
    // the string vector which remap the quark
    Strvec* p_svec;
    // the controlling monitor
    mutable Monitor d_cmon;

  public:
    // create a new tokamak  
    Tokamak (void) {
      // build the array
      d_size = Prime::mkthrp (0);
      d_thrs = (d_size * 7) / 10;
      d_tlen = 0;
      p_tnod = new s_tnod*[d_size];
      for (long i = 0; i < d_size; i++) p_tnod[i] = nullptr;
      p_svec = new Strvec (d_size);
      p_svec->add ("");
    }
    
    // delete this tokamak
    ~Tokamak (void) {
      if (p_tnod != nullptr) {
	for (long i = 0; i < d_size; i++) delete p_tnod[i];
	delete [] p_tnod;
      }
      delete p_svec;
    }
    
    // intern a name in this tokamak
    long intern (const String& name) {
      // check for nil
      if (name.isnil () == true) return 0;
      // lock the monitor
      d_cmon.enter ();
      try {
	// compute the hash value
	long hvl = name.hashid ();
	long hid = hvl % d_size;
	// look for existing node
	s_tnod* node = getnode (p_tnod[hid],name);
	if (node != nullptr) {
	  long result = node->d_quark;
	  d_cmon.leave ();
	  return result;
	}
	// the node does not exist, create it 
	node          = new s_tnod;
	node->d_name  = name;
	node->d_hvl   = hvl;
	node->d_quark = ++d_tlen;
	node->p_next  = p_tnod[hid];
	p_tnod[hid]   = node;
	if (d_tlen > d_thrs) resize (Prime::mkthrp (d_size + 1));
	p_svec->add (name);
	// leave and return
	d_cmon.leave ();
	return d_tlen;
      } catch (...) {
	d_cmon.leave ();
	throw;
      }
    }
    
    // remap a quark to a string
    String qmap (const long quark) const {
      d_cmon.enter ();
      try {
	String result = p_svec->get (quark);
	d_cmon.leave ();
	return result;
      } catch (...) {
	d_cmon.leave ();
	throw;
      }
    }
    
    // resize the hash table by creating a new one
    void resize (const long size) {
      // check for the size
      if (size < d_size) return;
      // initialize the table
      s_tnod** table = new s_tnod*[size];
      for (long i = 0; i < size; i++) table[i] = nullptr;
      // rebuild the table
      for (long i = 0; i < d_size; i++) {
	s_tnod* node = p_tnod[i];
	while (node != nullptr) {
	  s_tnod* next = node->p_next;
	  node->p_next = nullptr;
	  long hid = node->d_hvl % size;
	  node->p_next = table[hid];
	  table[hid]   = node;
	  node = next;
	}
      }
      // clean the old table
      delete [] p_tnod;
      // restore the new table
      d_size = size;
      d_thrs = (d_size * 7) / 10;
      p_tnod = table;
    }
    
  private:
    // make the copy constructor private
    Tokamak (const Tokamak&);
    // make the assignment operator private
    Tokamak& operator = (const Tokamak&);    
  };

  // -------------------------------------------------------------------------
  // - confiner section                                                       -
  // -------------------------------------------------------------------------

  class Confiner {
  private:
    // the confinement node
    struct s_cnod {
      Collectable* p_cobj;
      s_cnod* p_next;
      s_cnod* p_prev;
      // create a default node
      s_cnod (void) {
	p_cobj = nullptr;
	p_next = nullptr;
	p_prev = nullptr;
      }
      // create a node by collectable and root
      s_cnod (Collectable* cobj, s_cnod* root) {
	p_cobj = cobj;
	p_next = root;
	p_prev = nullptr;
	if (root != nullptr) root->p_prev = this;
      }
      // destroy the node
      ~s_cnod (void) {
	if (p_next != nullptr) delete p_next;
      }
    };

  private:
    // the root node
    s_cnod* p_root;
    // the verify flag
    bool d_vflg;
    // the controllng monitor
    mutable Monitor d_cmon;

  public:
    // create a confiner
    Confiner (void) {
      p_root = nullptr;
      d_vflg = false;
    }
    
    // destroy this object
    ~Confiner (void) {
      if (p_root != nullptr) delete p_root;
    }

    // confine a collectable object
    void* confine (Collectable* cobj) {
      // do nothing with nil
      if (cobj == nullptr) return nullptr;
      // lock and confine
      d_cmon.enter ();
      try {
	// check for existence
	if (d_vflg == true) {
	  void* cptr = find (cobj);
	  if (cptr != nullptr) {
	    d_cmon.leave ();
	    return cptr;
	  }
	}
	// create a new node with root
	s_cnod* node = new s_cnod (cobj, p_root);
	p_root = node;
	// leave and return
	d_cmon.leave ();
	return node;
      } catch (...) {
	d_cmon.leave ();
	throw;
      }
    }

    // untangle a confined object
    bool untangle (Collectable* cobj, void* cptr) {
      // check for nil
      if (cptr == nullptr) return false;
      // lock and untangle
      d_cmon.enter ();
      try {
	// map to a node
	s_cnod* node = reinterpret_cast <s_cnod*> (cptr);
	// evntually double check and leave
	if ((cobj != nullptr) && (node->p_cobj != cobj)) {
	  d_cmon.leave ();
	  return false;
	}
	// untangle the node
	s_cnod* next = node->p_next;
	s_cnod* prev = node->p_prev;
	if (next != nullptr) next->p_prev = prev;
	if (prev != nullptr) prev->p_next = next;
	// adjust root
	if (p_root == node) p_root = next;
	// free the node
	node->p_next = nullptr;
	node->p_prev = nullptr;
	delete node;
	// leave and return
	d_cmon.leave ();
	return true;
      } catch (...) {
	d_cmon.leave ();
	throw;
      }
    }

  private:
    // find a collectable object
    void* find (Collectable* cobj) const {
      // do nothing with nil
      if ((cobj == nullptr) || (p_root == nullptr)) return nullptr;
      // loop in the list
      s_cnod* node = p_root;
      while (node != nullptr) {
	if (node->p_cobj == cobj) return node;
	node = node->p_next;
      }
      // not found
      return nullptr;
    }
  };


  // -------------------------------------------------------------------------
  // - private section                                                         -
  // -------------------------------------------------------------------------

  // the tokamak reactor for quark mapping
  static Tokamak* p_reactor = nullptr;
  // the confiner for collectable object
  static Confiner* p_confiner = nullptr;

  // the central reactor deallocator
  static void central_del_reactor (void) {
    delete p_reactor;
    p_reactor = nullptr;
  }
  
  // the central confiner deallocator
  static void central_del_confiner (void) {
    delete p_confiner;
    p_confiner = nullptr;
  }

  // the central reactor allocator
  static void central_new_reactor (void) {
    if (p_reactor == nullptr) {
      p_reactor = new Tokamak;
      c_gcleanup (central_del_reactor);
    }
  }

  // the central confiner allocator
  static void central_new_confiner (void) {
    if (p_confiner == nullptr) {
      p_confiner = new Confiner;
      c_gcleanup (central_del_confiner);
    }
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // intern a string and return a quark
  
  long Central::intern (const String& name) {
    // create the reactor
    central_new_reactor ();
    // try to intern
    return p_reactor->intern (name);
  }
  
  // remap a string from a quark
  
  String Central::qmap (const long quark) {
    // create the reactor
    central_new_reactor ();
    // try to map
    return p_reactor->qmap (quark);
  }

  // confine a collectable object
  
  void* Central::confine (Collectable* cobj) {
    // create the confiner
    central_new_confiner ();
    // try to confine
    return p_confiner->confine (cobj);
  }

  // untangle a confined object
  
  bool Central::untangle (Collectable* cobj, void* cptr) {
    // create the confiner
    central_new_confiner ();
    // try to untangle
    return p_confiner->untangle (cobj, cptr);
  }
}
