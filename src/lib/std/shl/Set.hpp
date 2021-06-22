// ---------------------------------------------------------------------------
// - Set.hpp                                                                 -
// - standard object library - set class definition                          -
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

#ifndef  AFNIX_SET_HPP
#define  AFNIX_SET_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

#ifndef  AFNIX_ITERABLE_HPP
#include "Iterable.hpp"
#endif

namespace afnix {

  /// The Set class is a base container class used to represent object set.
  /// Unlike other container, a set class store object uniquely using its
  /// pointer as a discriminator. Adding two times the same object result
  /// only in one object at the end. This property differentiate it from
  /// the other container.
  /// @author amaury darsch

  class Set : public virtual Serial, public Iterable {
  private:
    /// the set size
    long     d_size;
    /// the set cardinal
    long     d_slen;
    /// the set of objects
    Object** p_oset;
    
  public:
    /// create an empty set
    Set (void);

    /// create a set with an object
    /// @param obj the object to add in the set
    Set (Object* obj);

    /// copy construct this set
    /// @param that the set to copy 
    Set (const Set& that);

    /// copy move this set
    /// @param that the set to move 
    Set (Set&& that) noexcept;
    
    /// destroy this set
    ~Set (void);

    /// assign a set to this one
    /// @param that the set to assign
    Set& operator = (const Set& that);

    /// move a set to this one
    /// @param that the set to move
    Set& operator = (Set&& that) noexcept;

    /// @return the class name
    String repr (void) const;

    /// @return the set did
    t_word getdid (void) const;

    /// @return the set sid
    t_word getsid (void) const;

    /// serialize this set cell to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a set from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// reset this set
    void reset (void);

    /// @return the cardinal of this set
    long length (void) const;

    /// @return an object by index
    Object* get (const long index) const;

    /// @return true if the set is empty
    bool empty (void) const;

    /// @return true if the object exists in this set
    bool exists (Object* obj) const;

    /// add an object to the set
    /// @param obj the object to add
    void add (Object* obj);

    /// remove an object from this set
    /// @param  obj  the object ot remove
    /// @return true if the object was removed
    bool remove (Object* obj);

    /// replace an object in this set
    /// @param  tobj the target object
    /// @return sobj the source object to replace
    bool replace (Object* tobj, Object* sobj);

    /// merge a set into this one
    /// @param cset he child set to merge
    void merge (const Set& cset);

    /// mix this set with a certain number of passes
    /// @param pnum the number of passes
    void remix (const long pnum);

    /// get a random subset by size
    /// @param size the subset size
    Set* getrss (const long size) const;

    /// resize this set with a new size
    /// @para size the requested size
    void resize (const long size);

    /// @return a new iterator for this set
    Iterator* makeit (void);

  private:
    // make the set iterator a friend
    friend class Setit;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
  
  /// The Setit class is the iterator for the set class. Such iterator
  /// is constructed with the "makeit" set method. The iterator is reset to
  /// the beginning of the set.
  /// @author amaury darsch

  class Setit : public Iterator {
  private:
    /// the set to iterate
    Set* p_set;
    /// the current index
    long d_idx;

  public:
    /// create a new iterator from a set
    /// @param set the set to iterate
    Setit (Set* set);

    /// destroy this set iterator
    ~Setit (void);

    /// @return the class name
    String repr (void) const;

    /// reset the iterator to the begining
    void begin (void);

    /// reset the iterator to the end
    void end (void);

    /// move the iterator to the next position
    void next (void);

    /// move the iterator to the previous position
    void prev (void);

    /// @return the object at the current position
    Object* getobj (void) const;

    /// @return true if the iterator is at the end
    bool isend (void) const;

  private:
    // make the copy constructor private
    Setit (const Setit&);
    // make the assignment operator private
    Setit& operator = (const Setit&);
  };
}

#endif
