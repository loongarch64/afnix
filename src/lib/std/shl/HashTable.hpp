// ---------------------------------------------------------------------------
// - HashTable.hpp                                                           -
// - standard object library - hash table object class definition            -
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

#ifndef  AFNIX_HASHTABLE_HPP
#define  AFNIX_HASHTABLE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The HashTable class is another container class which maps an object
  /// with a key. The hash table is dynamic and get resized automatically
  /// when needed. When an object is added, the object reference count is
  /// increased. When the object is retreived, the reference count is not
  /// touched. The lookup method throw an exception if the key is not found.
  /// The get method returns nullptr if the object is not found. The table can
  /// be configured to operate in a case insensitive way. If the case flag
  /// is changed, the table is automatically reset.
  /// @author amaury darsch

  class HashTable : public virtual Serial {
  private:
    /// the hash table size
    long d_size;
    /// the hash table length
    long d_hlen;
    /// the hash table threshold
    long d_thrs;
    /// the case insensitive flag
    bool d_cifg;
    /// array of buckets
    struct s_bucket** p_htbl;

  public:
    /// create a hash table with a default size
    HashTable (void);

    /// create a hash table with a case flag
    /// @param cifg the case insensitive flag
    HashTable (const bool cifg);

    /// create a hash table with an initial size
    /// @param size the hash table size
    HashTable (const long size);

    /// create a hash table with an initial size and case
    /// @param size the hash table size
    /// @param cifg the case insensitive flag
    HashTable (const long size, const bool cifg);

    /// destroy the hash table. 
    ~HashTable (void);

    /// return the class name
    String repr (void) const override;

    /// @return the hash table did
    t_word getdid (void) const override;

    /// @return the hash table sid
    t_word getsid (void) const override;

    /// serialize this hash table to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a hash table from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;
    
    /// reset the hash table
    void reset (void);

    /// @return the number of elements
    long length (void) const;

    /// @return true if the table is empty
    bool empty (void) const;

    /// set the case insensitive flag
    /// @param cifg the case flag to set
    void setcifg (const bool cifg);

    /// @return the case insensitive flag
    bool getcifg (void) const;

    /// @return the element key by index
    String getkey (const long index) const;

    /// @return the element object by index
    Object* getobj (const long index) const;
  
    /// @return true if the key exists in this table
    bool exists (const String& key) const;

    /// set or create a new object in the table.
    /// @param key the object key to set
    /// @param obj the object to set or add
    void add (const String& key, Object* obj);

    /// get an object by key. If the object is not found nil is returned.
    /// this means that this function always succeds. You can use lookup 
    /// instead to get an exception if the key is not found. You can also use
    /// the exists method to check for a key but exists and get is very slow.
    /// @param key the object key to search
    Object* get (const String& key) const;

    /// find an object by key or throw an exception
    /// @param key the object key to find
    Object* lookup (const String& key) const;

    /// remove an object in the hash table. 
    /// @param key the object key to remove
    void remove (const String& key);

    /// @return a vector of keys
    Vector* getkeys (void) const;

    /// @return a vector of objects
    Vector* getvobj (void) const;

    /// resize this hash table
    void resize (const long size);

  private:
    // make the copy constructor private
    HashTable (const HashTable&);
    // make the assignment operator private
    HashTable& operator = (const HashTable&);    

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
