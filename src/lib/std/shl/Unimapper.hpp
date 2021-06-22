// ---------------------------------------------------------------------------
// - Unimapper.hpp                                                           -
// - standard object library - unicode character mapper class definition     -
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

#ifndef  AFNIX_UNIMAPPER_HPP
#define  AFNIX_UNIMAPPER_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Unimapper class is a container class which can be used to map
  /// a unicode character with another character. The primary use of the 
  /// mapping table is to perform character transformation. The chararacter
  /// mapper operates like a hash table.
  /// @author amaury darsch

  class Unimapper : public virtual Object {
  private:
    /// the table size
    long d_size;
    /// the number of elements
    long d_count;
    /// threshold before resizing
    long d_thrs;
    /// table of buckets
    struct s_umnode** p_table;

  public:
    /// create a unicode mapper with a default size
    Unimapper (void);
  
    /// create a unicode mapper with an initial size
    /// @param size the initial mapper size
    Unimapper (const long size);
  
    /// destroy the unicode mappewr. 
    ~Unimapper (void);

    /// return the class name
    String repr (void) const;

    /// reset the unicode mapping table
    void reset (void);

    /// @return the number of elements
    long length (void) const;

    /// @return true if the table is empty
    bool empty (void) const;

    /// @return the element key by index
    t_quad getkey (const long index) const;

    /// @return the element character by index
    t_quad getchr (const long index) const;

    /// set or create a new entry in the mapping table by key and character
    /// @param key the unicode key to use
    /// @param chr the character to map
    void add (const t_quad key, const t_quad chr);

    /// get a mapped character by key
    /// @param key the unicode key to search
    t_quad get (const t_quad key) const;

    /// get a mapped character by key or raise an exception
    /// @param key the unicode key to find
    t_quad lookup (const t_quad key) const;

    /// remove a mapping by key
    /// @param key the object key to remove
    void remove (const t_quad key);
  
    /// @return true if the key exists
    bool exists (const t_quad key) const;

  private:
    // make the copy constructor private
    Unimapper (const Unimapper&);
    // make the assignment operator private
    Unimapper& operator = (const Unimapper&);    
    // resize this hash table
    void resize (const long size);
  };
}

#endif
