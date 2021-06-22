// ---------------------------------------------------------------------------
// - QuarkTable.hpp                                                          -
// - standard object library - quark table object class definition           -
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

#ifndef  AFNIX_QUARKTABLE_HPP
#define  AFNIX_QUARKTABLE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The QuarkTable class is a container class which maps an object
  /// with a quark. The quark table is dynamic and get resized automatically
  /// when needed. When an object is added, the object reference count is
  /// increased. When the object is retreived, the reference count is not
  /// touched. The lookup method throw an exception if the quark is not found.
  /// The get method returns nullptr if the object is not found. The quark table
  /// is similar to the hash table except it works with quarks.
  /// @author amaury darsch

  class QuarkTable : public virtual Object {
  private:
    /// the quark table size
    long d_size;
    /// the number of elements
    long d_count;
    /// threshold before resizing
    long d_thrs;
    /// table of quark bucket
    struct s_quanode** p_table;

  public:
    /// create a quark table with a default size
    QuarkTable (void);
  
    /// create a quark table with an initial size
    QuarkTable (const long size);
  
    /// destroy the quark table. 
    ~QuarkTable (void);

    /// return the class name
    String repr (void) const;

    /// reset the quark table
    void reset (void);

    /// @return the number of elements
    long length (void) const;

    /// @return the element name by index
    String getname (const long index) const;

    /// @return the element object by index
    Object* getobj (const long index) const;

    /// set or create a new object in the table.
    /// @param quark the object quark to set
    /// @param object the object to set or add
    void add (const long quark, Object* object);

    /// get an object by quark. If the object is not found nil is returned.
    /// this means that this function always succeds. You can use lookup 
    /// instead to get an exception if the quark is not found. You can also use
    /// the exists method to check for a quark but exists and get is very slow.
    /// @param qaurk the object quark to search
    Object* get (const long quark) const;

    /// find an object by quark or throw an exception
    /// @param quark the object quark to find
    Object* lookup (const long quark) const;

    /// remove an object by quark
    /// @param quark the object quark to remove
    void remove (const long quark);
  
    /// @return true if the quark exists
    bool exists (const long quark) const;

  private:
    // make the copy constructor private
    QuarkTable (const QuarkTable&);
    // make the assignment operator private
    QuarkTable& operator = (const QuarkTable&);    
    // resize this hash table
    void resize (const long size);
  };
}

#endif
