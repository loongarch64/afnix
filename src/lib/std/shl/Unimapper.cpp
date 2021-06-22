// ---------------------------------------------------------------------------
// - UniMapper.cpp                                                           -
// - standard object library - unicode character mapper class implementation -
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
#include "Unimapper.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the unicode table node
  struct s_umnode {
    // the unicode key
    t_quad d_key;
    // the mapped character
    t_quad d_chr;
    // next record in the list
    s_umnode* p_next;
    // simple constructor
    s_umnode (void) {
      d_key  = nilq;
      d_chr  = nilq;
      p_next = nullptr;
    }
    // simple destructor
    ~s_umnode (void) {
      delete p_next;
    }
  };
  
  // find a node by character given its root qnode
  static inline s_umnode* getnode (s_umnode* node, const t_quad key) {
    // simple check as fast as we can
    if (node == nullptr) return nullptr;
    // loop until we have a match
    while (node != nullptr) {
      if (node->d_key == key) return node;
      node = node->p_next;
    }
    // no node found
    return nullptr;
  }
  
  // extract a node by key given its root node
  static inline s_umnode* rmnode (s_umnode** root, const t_quad key) {
    s_umnode* node = *root;
    // simple check as fast as we can
    if (node == nullptr) return nullptr;
    // first case for the root node
    if (node->d_key == key) {
      *root = node->p_next;
      node->p_next = nullptr;
      return node;
    }
    // loop until we have a match
    while (node->p_next != nullptr) {
      if (node->p_next->d_key == key) {
	s_umnode* result = node->p_next;
	node->p_next     = result->p_next;
	result->p_next   = nullptr;
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
  
  // create a new unicode table 
  
  Unimapper::Unimapper (void) {
    // build the array
    d_size   = Prime::mkthrp (0);
    d_thrs   = (d_size * 7) / 10;
    d_count  = 0;
    p_table  = new s_umnode*[d_size];
    for (long i = 0; i < d_size; i++) p_table[i] = nullptr;
  }
  
  // create a new unicode table with a predefined size
  
  Unimapper::Unimapper (const long size) {
    // build the array - threshold at 70%
    d_size   = Prime::mkthrp (size);
    d_thrs   = (d_size * 7) / 10;
    d_count  = 0;
    p_table  = new s_umnode*[d_size];    
    for (long i = 0; i < d_size; i++) p_table[i] = nullptr;
  }
  
  // delete this unicode table 
  
  Unimapper::~Unimapper (void) {
    // protect ourself
    Object::iref (this);
    // clean the table
    if (p_table != nullptr) {
      for (long i = 0; i < d_size; i++) delete p_table[i];
      delete [] p_table;
    }
  }

  // return the class name

  String Unimapper::repr (void) const {
    return "Unimapper";
  }

  // reset this unicode mapping table
  
  void Unimapper::reset (void) {
    wrlock ();
    // clear everything
    if (p_table != nullptr) {
      for (long i = 0; i < d_size; i++) {
	delete p_table[i];
	p_table[i] = nullptr;
      }
    }
    d_count = 0;
    unlock ();
  }  

  // get the number of elements

  long Unimapper::length (void) const {
    rdlock ();
    long result = d_count;
    unlock ();
    return result;
  }

  // return rtue if the table empty

  bool Unimapper::empty (void) const {
    rdlock ();
    bool result = (d_count == 0);
    unlock ();
    return result;
  }

  // get the element key by index

  t_quad Unimapper::getkey (const long index) const {
    rdlock ();
    long npos = 0;
    for (long i = 0; i < d_size; i++) {
      s_umnode* node = p_table[i];
      while (node != nullptr) {
	if (npos == index) {
	  t_quad result = node->d_key;
	  unlock ();
	  return result;
	}
	npos++;
	node = node->p_next;
      }
    }
    unlock ();
    throw Exception ("index-error", "index is out of range");
  }

  // get the element character by index

  t_quad Unimapper::getchr (const long index) const {
    rdlock ();
    long npos = 0;
    for (long i = 0; i < d_size; i++) {
      s_umnode* node = p_table[i];
      while (node != nullptr) {
	if (npos == index) {
	  t_quad result = node->d_chr;
	  unlock ();
	  return result;
	}
	npos++;
	node = node->p_next;
      }
    }
    unlock ();
    throw Exception ("index-error", "index is out of range");
  }

  // set or create a mapping character in this table
  
  void Unimapper::add (const t_quad key, const t_quad chr) {
    // get the write lock
    wrlock ();
    // compute the table id
    long hid = (long) key % d_size;
    if (hid < 0) {
      unlock ();
      throw Exception ("internal-error", "negative unicode table hid");
    }
    // look for existing symbol
    s_umnode* node = getnode (p_table[hid], key);
    if (node != nullptr) {
      node->d_chr = chr;
      unlock ();
      return;
    }
    // the node does not exist, create it 
    node         = new s_umnode;
    node->d_key  = key;
    node->d_chr  = chr;
    node->p_next = p_table[hid];
    p_table[hid] = node;
    if (++d_count > d_thrs) resize (Prime::mkthrp (d_size + 1));
    unlock ();
  }
  
  // get a mapping character by key
  
  t_quad Unimapper::get (const t_quad key) const {
    // get the read lock
    rdlock ();
    // compute table id
    long hid = (long) key % d_size;
    if (hid < 0) {
      unlock ();
      throw Exception ("internal-error", "negative unicode table hid");
    }
    // look for the node and get the object
    s_umnode*  node = getnode (p_table[hid], key);
    t_quad result = (node == nullptr) ? nilq : node->d_chr;
    // unlock and return
    unlock ();
    return result;
  }

  // get a mapping character by key or raise an exception

  t_quad Unimapper::lookup (const t_quad key) const {
    // get the read lock
    rdlock ();    
    // compute table id
    long hid = (long) key % d_size;
    if (hid < 0) {
      unlock ();
      throw Exception ("internal-error", "negative unicode table hid");
    }
    // look for the node and find symbol
    s_umnode* node = getnode (p_table[hid], key);
    if (node != nullptr) {
      t_quad result =  node->d_chr;
      unlock ();
      return result;
    }
    // unlock and notify
    unlock ();
    throw Exception ("key-error", "unicode key not found");
  }
  
  // return true if a key exists in this table

  bool Unimapper::exists (const t_quad key) const {
    // get the read lock
    rdlock ();
    // compute table id
    long hid = (long) key % d_size;
    if (hid < 0) {
      unlock ();
      throw Exception ("internal-error", "negative unicode table hid");
    }
    // look for the node and find symbol
    s_umnode* node = getnode (p_table[hid], key);
    unlock ();
    return (node != nullptr);
  }
  
  // remove an entry by key
  
  void Unimapper::remove (const t_quad key) {
    // get the write lock
    wrlock ();
    // compute table id
    long hid = (long) key % d_size;
    if (hid < 0) {
      unlock ();
      throw Exception ("internal-error", "negative unicode table hid");
    }
    // extract the qnode and delete
    s_umnode* node = rmnode (&p_table[hid], key);
    delete node;
    d_count--;
    // release the write lock
    unlock ();
  }
  
  // resize the table by creating a new one.
  // no need to lock here since the procedure is private and called
  // from the critical region
  
  void Unimapper::resize (const long size) {
    // check for the size
    if (size < d_size) return;
    // initialize the new table
    s_umnode** table = new s_umnode*[size];
    for (long i = 0; i < size; i++) table[i] = nullptr;
    // rebuild the table
    for (long i = 0; i < d_size; i++) {
      s_umnode* node = p_table[i];
      while (node != nullptr) {
	s_umnode* next = node->p_next;
	node->p_next   = nullptr;
	long hid = (long) node->d_key % size;
	node->p_next = table[hid];
	table[hid]   = node;
	node = next;
      }
    }
    // clean the old table
    delete [] p_table;
    // restore the new table
    d_size  = size;
    d_thrs  = (d_size * 7) / 10;
    p_table = table;
  }
}
