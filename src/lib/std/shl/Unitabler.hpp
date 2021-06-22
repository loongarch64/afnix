// ---------------------------------------------------------------------------
// - Unitabler.hpp                                                           -
// - standard object library - unicode table class definition                -
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

#ifndef  AFNIX_UNITABLER_HPP
#define  AFNIX_UNITABLER_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Unitabler class is a container class which can be used to map
  /// a unicode character with an object. The table is similar to the quark
  /// table and provides the same functionnality. A nil object can be 
  /// associated with a character making the table operating as a set when
  /// using the exists method only.
  /// @author amaury darsch

  class Unitabler : public virtual Object {
  private:
    /// the table size
    long d_size;
    /// the number of elements
    long d_count;
    /// threshold before resizing
    long d_thrs;
    /// table of buckets
    struct s_utnode** p_table;

  public:
    /// create a unicode table with a default size
    Unitabler (void);
  
    /// create a unicode table with an initial size
    Unitabler (const long size);
  
    /// destroy the unicode table. 
    ~Unitabler (void);

    /// return the class name
    String repr (void) const;

    /// reset the unicode table
    void reset (void);

    /// @return the number of elements
    long length (void) const;

    /// @return the element key by index
    t_quad getkey (const long index) const;

    /// @return the element object by index
    Object* getobj (const long index) const;

    /// set or create a new entry in the table by key and object
    /// @param key the unicode key to use
    /// @param object the object to map
    void add (const t_quad key, Object* object);

    /// get an object by key
    /// @param key the unicode key to search
    Object* get (const t_quad key) const;

    /// get an object by key or raise an exception
    /// @param key the unicode key to find
    Object* lookup (const t_quad key) const;

    /// remove an object by key
    /// @param key the object key to remove
    void remove (const t_quad key);
  
    /// @return true if the key exists
    bool exists (const t_quad key) const;

  private:
    // make the copy constructor private
    Unitabler (const Unitabler&);
    // make the assignment operator private
    Unitabler& operator = (const Unitabler&);    
    // resize this hash table
    void resize (const long size);
  };
}

#endif
