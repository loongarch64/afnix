// ---------------------------------------------------------------------------
// - HashTable.cpp                                                           -
// - standard object library - hash table class implementation               -
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
#include "Vector.hpp"
#include "Stdsid.hxx"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "HashTable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
 
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
   
  // the hash table bucket
  struct s_bucket {
    // the object key
    String  d_key;
    // the hash id value
    long    d_hvl;
    // the object 
    Object* p_obj;
    // next record in the list
    s_bucket* p_next;
    // simple constructor
    s_bucket (void) {
      d_hvl  = 0;
      p_obj  = nullptr;
      p_next = nullptr;
    }
    // simple destructor
    ~s_bucket (void) {
      Object::dref (p_obj);
      delete p_next;
    }
  };
  
  // find a bucket by key given its root bucket
  static inline s_bucket* getbucket (s_bucket*  bucket, 
				     const String& key, const bool cifg) {
    // simple check as fast as we can
    if (bucket == nullptr) return nullptr;
    // loop until we have a match
    if (cifg == false) {
      while (bucket != nullptr) {
	if (bucket->d_key == key) return bucket;
	bucket = bucket->p_next;
      }
    } else {
      while (bucket != nullptr) {
	if (String::strcic (bucket->d_key, key) == true) return bucket;
	bucket = bucket->p_next;
      }
    }
    // no bucket found
    return nullptr;
  }
  
  // extract a bucket by key given its root bucket . This procedure remove the
  // bucket if it is found and maintain the link list.
  static inline s_bucket* rmbucket (s_bucket**   root, 
				    const String& key, const bool cifg) {
    s_bucket* bucket = *root;
    // simple check as fast as we can
    if (bucket == nullptr) return nullptr;
    // first case for the root bucket
    if ((bucket->d_key == key) ||
	((cifg == true) && (String::strcic (bucket->d_key, key) == true))) {
      *root = bucket->p_next;
      bucket->p_next = nullptr;
      return bucket;
    }
    // loop until we have a match
    if (cifg == false) {
      while (bucket->p_next != nullptr) {
	if (bucket->p_next->d_key == key) {
	  s_bucket* result = bucket->p_next;
	  bucket->p_next = result->p_next;
	  result->p_next = nullptr;
	  return result;
	}
	bucket = bucket->p_next;
      }
    } else {
      while (bucket->p_next != nullptr) {
	if (String::strcic (bucket->p_next->d_key, key) == true) {
	  s_bucket* result = bucket->p_next;
	  bucket->p_next = result->p_next;
	  result->p_next = nullptr;
	  return result;
	}
	bucket = bucket->p_next;
      }
    }
    // no node found
    return nullptr;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
    
  // create a new hash table
  
  HashTable::HashTable (void) {
    // build the array
    d_size = Prime::mkthrp (0);
    d_hlen = 0;
    d_thrs = (d_size * 7) / 10;
    d_cifg = false;
    p_htbl = new s_bucket*[d_size];
    // initialize the table with null pointers
    for (long i = 0; i < d_size; i++)
      p_htbl[i] = nullptr;
  }
  
  // create a new hash table by case flag
  
  HashTable::HashTable (const bool cifg) {
    // build the array
    d_size = Prime::mkthrp (0);
    d_hlen = 0;
    d_thrs = (d_size * 7) / 10;
    d_cifg = cifg;
    p_htbl = new s_bucket*[d_size];
    // initialize the table with null pointers
    for (long i = 0; i < d_size; i++)
      p_htbl[i] = nullptr;
  }

  // create a new hash table with a predefined size
  
  HashTable::HashTable (const long size) {
    // build the array - threshold at 70%
    d_size = Prime::mkthrp (size);
    d_hlen = 0;
    d_thrs = (size * 7) / 10;
    d_cifg = false;
    p_htbl = new s_bucket*[d_size];
    // initialize the table with null pointers
    for (long i = 0; i < d_size; i++)
      p_htbl[i] = nullptr;
  }

  // create a new hash table by size and case flag
  
  HashTable::HashTable (const long size, const bool cifg) {
    // build the array - threshold at 70%
    d_size = Prime::mkthrp (size);
    d_hlen = 0;
    d_thrs = (size * 7) / 10;
    d_cifg = cifg;
    p_htbl = new s_bucket*[d_size];
    // initialize the table with null pointers
    for (long i = 0; i < d_size; i++)
      p_htbl[i] = nullptr;
  }
  
  // delete this hash table 
  
  HashTable::~HashTable (void) {
    if (p_htbl != nullptr) {
      for (long i = 0; i < d_size; i++) delete p_htbl[i];
      delete [] p_htbl;
    }
  }

  // return the class name

  String HashTable::repr (void) const {
    return "HashTable";
  }

  // return the hash table did

  t_word HashTable::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the hash table sid
  
  t_word HashTable::getsid (void) const {
    return SRL_HASH_SID;
  }

  // serialize this hash table

  void HashTable::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the table length
      Serial::wrlong (d_hlen, os);
      // write the name/objects
      for (long k = 0L; k < d_hlen; k++) {
	String name = getkey(k); name.wrstream(os);
	Object* obj = getobj (k);
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

  // deserialize this hash table

  void HashTable::rdstream (InputStream& is) {
    wrlock ();
    try {
      reset ();
      // get the table length
      long hlen = Serial::rdlong (is);
      // readin each name/object
      for (long k = 0L; k < hlen; k++) {
	// get the name/value pair
	String name; name.rdstream (is);
	Object* obj = Serial::deserialize (is);
	// add in the table
	add (name, obj);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this hash table
  
  void HashTable::reset (void) {
    wrlock ();
    try {
      if (p_htbl != nullptr) {
	for (long i = 0; i < d_size; i++) {
	  delete p_htbl[i];
	  p_htbl[i] = nullptr;
	}
      }
      d_hlen = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }  

  // get the number of elements

  long HashTable::length (void) const {
    rdlock ();
    try {
      long result = d_hlen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the table is empty

  bool HashTable::empty (void) const {
    rdlock ();
    try {
      bool result = (d_hlen == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the case flag

  void HashTable::setcifg (const bool cifg) {
    wrlock ();
    try {
      // reset the table
      reset ();
      // set the case flag
      d_cifg = cifg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
   
  // get the case flag

  bool HashTable::getcifg (void) const {
    rdlock ();
    try {
      bool result = d_cifg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the element key by index

  String HashTable::getkey (const long index) const {
    rdlock ();
    try {
      long npos = 0;
      for (long i = 0; i < d_size; i++) {
	s_bucket* bucket = p_htbl[i];
	while (bucket != nullptr) {
	  if (npos == index) {
	    String result = bucket->d_key;
	    unlock ();
	    return result;
	  }
	  npos++;
	  bucket = bucket->p_next;
	}
      }
      throw Exception ("index-error", "index is out of range");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the element object by index

  Object* HashTable::getobj (const long index) const {
    rdlock ();
    try {
      long npos = 0;
      for (long i = 0; i < d_size; i++) {
	s_bucket* bucket = p_htbl[i];
	while (bucket != nullptr) {
	  if (npos == index) {
	    Object* result = bucket->p_obj;
	    unlock ();
	    return result;
	  }
	  npos++;
	  bucket = bucket->p_next;
	}
      }
      throw Exception ("index-error", "index is out of range");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a key exists in this table

  bool HashTable::exists (const String& key) const {
    rdlock ();
    try {
      // compute hash id
      long hvl = d_cifg ? key.tolower().hashid () : key.hashid ();
      long hid = hvl % d_size;
      // look for the bucket
      s_bucket* bucket = getbucket (p_htbl[hid], key, d_cifg);
      bool result = (bucket != nullptr) ? true : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set or create an object in this table
  
  void HashTable::add (const String& key, Object* object) {
    wrlock ();
    try {
      // protect the object
      Object::iref (object);
      // compute the hash value
      long hvl = d_cifg ? key.tolower().hashid () : key.hashid ();
      long hid = hvl % d_size;
      // look for the bucket
      s_bucket* bucket = getbucket (p_htbl[hid], key, d_cifg);
      if (bucket != nullptr) {
	Object::dref (bucket->p_obj);
	bucket->p_obj = object;
	unlock ();
	return;
      }
      // the bucket does not exist, create it 
      bucket           = new s_bucket;
      bucket->d_key  = key;
      bucket->d_hvl  = hvl;
      bucket->p_obj  = object;
      bucket->p_next = p_htbl[hid];
      p_htbl[hid]    = bucket;
      if (++d_hlen > d_thrs) resize (Prime::mkthrp (d_size + 1));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get an object by key. If the key is not found, nullptr is returned
  
  Object* HashTable::get (const String& key) const {
    rdlock ();
    try {
      // compute hash id
      long hvl = d_cifg ? key.tolower().hashid () : key.hashid ();
      long hid = hvl % d_size;
      // look for the node and find symbol
      s_bucket* bucket = getbucket (p_htbl[hid], key, d_cifg);
      Object* result = (bucket == nullptr) ? nullptr : bucket->p_obj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object by key. If the key is not found an exception is raised

  Object* HashTable::lookup (const String& key) const {
    rdlock ();
    try {
      // compute hash id
      long hvl = d_cifg ? key.tolower().hashid () : key.hashid ();
      long hid = hvl % d_size;
      // look for the node and find symbol
      s_bucket* bucket = getbucket (p_htbl[hid], key, d_cifg);
      if (bucket != nullptr) {
	Object* result = bucket->p_obj;
	unlock ();
	return result;
      }
      throw Exception ("key-error", "key not found", key);
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // remove an object by key. 
  
  void HashTable::remove (const String& key) {
    wrlock ();
    try {
      // compute hash id
      long hvl = d_cifg ? key.tolower().hashid () : key.hashid ();
      long hid = hvl % d_size;   
      // extract the bucket
      s_bucket* bucket = rmbucket (&p_htbl[hid], key, d_cifg);
      if (bucket != nullptr) {
	delete bucket;
	d_hlen--;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return a vector of keys in this hash table

  Vector* HashTable::getkeys (void) const {
    rdlock ();
    try {
      Vector* result = new Vector;
      for (long i = 0; i < d_size; i++) {
	s_bucket* bucket = p_htbl[i];
	while (bucket != nullptr) {
	  result->add (new String(bucket->d_key));
	  bucket = bucket->p_next;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a vector of objects in this hash table

  Vector* HashTable::getvobj (void) const {
    rdlock ();
    try {
      Vector* result = new Vector;
      for (long i = 0; i < d_size; i++) {
	s_bucket* bucket = p_htbl[i];
	while (bucket != nullptr) {
	  Object* obj = bucket->p_obj;
	  if (obj != nullptr) result->add (obj);
	  bucket = bucket->p_next;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize the hash table by creating a new one
  
  void HashTable::resize (const long size) {
    wrlock ();
    try {
      // check for the size
      if (size < d_size) return;
      // initialize the new table
      s_bucket** table = new s_bucket*[size];
      for (long i = 0; i < size; i++) table[i] = nullptr;
      // rebuild the table
      for (long i = 0; i < d_size; i++) {
	s_bucket* bucket = p_htbl[i];
	while (bucket != nullptr) {
	  s_bucket* next = bucket->p_next;
	  bucket->p_next = nullptr;
	  long hid = bucket->d_hvl  % size;
	  bucket->p_next = table[hid];
	  table[hid]     = bucket;
	  bucket = next;
	}
      }
      // clean the old table
      delete [] p_htbl;
      // restore the new table
      d_size = size;
      d_thrs = (d_size * 7) / 10;
      p_htbl = table;
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 14;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ADD     = zone.intern ("add");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_LOOKUP  = zone.intern ("lookup");
  static const long QUARK_REMOVE  = zone.intern ("remove");
  static const long QUARK_EXISTP  = zone.intern ("exists-p");
  static const long QUARK_EMPTYP  = zone.intern ("empty-p");
  static const long QUARK_GETKEY  = zone.intern ("get-key");
  static const long QUARK_GETOBJ  = zone.intern ("get-object");
  static const long QUARK_GETKEYS = zone.intern ("get-keys");
  static const long QUARK_GETOBJS = zone.intern ("get-objects");
  static const long QUARK_SETCIFG = zone.intern ("set-case-flag");
  static const long QUARK_GETCIFG = zone.intern ("get-case-flag");

  // create a new object in a generic way

  Object* HashTable::mknew (Vector* argv) {
    // get tne number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check 0 argument
    if (argc == 0) return new HashTable;
    // check 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a boolean
      Boolean* bobj = dynamic_cast <Boolean*> (obj);
      if (bobj != nullptr) {
	bool cifg = bobj->tobool ();
	return new HashTable (cifg);
      }
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	long size = argv->getlong (0);
	return new HashTable (size);
      }
      // invalid object
      throw Exception ("type-error", 
		       "invalid object with hash table constructor",
		       Object::repr (obj));
    }
    // check 2 arguments
    if (argc == 2) {
      long size = argv->getlong (0);
      bool cifg = argv->getbool (1);
      return new HashTable (size, cifg);
    }
    throw Exception ("argument-error", "too many arguments for hash table");
  }

  // return true if the given quark is defined

  bool HashTable::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* HashTable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_EMPTYP)  return new Boolean (empty   ());
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_GETKEYS) return getkeys ();
      if (quark == QUARK_GETOBJS) return getvobj ();
      if (quark == QUARK_GETCIFG) return new Boolean (getcifg ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EXISTP) {
	String key = argv->getstring (0);
	return new Boolean (exists (key));
      }
      if (quark == QUARK_GET) {
	String key = argv->getstring (0);
	rdlock();
	try {
	  Object* result = get (key);
	  zobj->post (result);
	  unlock ();	    
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_LOOKUP) {
	String key = argv->getstring (0);
	rdlock();
	try {
	  Object* result = lookup (key);
	  zobj->post (result);
	  unlock ();	    
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETKEY) {
	long index = argv->getlong (0);
	return new String (getkey (index));
      }
      if (quark == QUARK_GETOBJ) {
	long index = argv->getlong (0);
	rdlock();
	try {
	  Object* result = getobj (index);
	  zobj->post (result);
	  unlock ();	    
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_REMOVE) {
	String key = argv->getstring (0);
	remove (key);
	return nullptr;
      }
      if (quark == QUARK_SETCIFG) {
	bool cifg = argv->getbool (0);
	setcifg (cifg);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADD) {
	String  key = argv->getstring (0);
	Object* obj  = argv->get (1);
	add (key, obj);
	return nullptr;
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
