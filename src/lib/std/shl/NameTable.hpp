// ---------------------------------------------------------------------------
// - NameTable.hpp                                                           -
// - standard object library - name table object class definition            -
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

#ifndef  AFNIX_NAMETABLE_HPP
#define  AFNIX_NAMETABLE_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

namespace afnix {

  /// The NameTable class is similar to the HashTable class except that it
  /// is designed to work with a small set of objects. The NameTable defines
  /// a binding between a name and an object. The class has the same methods
  /// like the HashTable class, but can works with quarks too.
  /// @author amaury darsch

  class NameTable : public virtual Serial {
  private:
    /// name table structure
    struct s_ntnode* p_table;

  public:
    /// create a name table
    NameTable (void);
  
    /// destroy the name table. 
    ~NameTable (void);

    /// return the class name
    String repr (void) const;

    /// @return the name table did
    t_word getdid (void) const;

    /// @return the name table sid
    t_word getsid (void) const;

    /// serialize this name table to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a name table from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// reset this name table
    void reset (void);

    /// @return the name table length
    long length (void) const;

    /// @return the element name by index
    String getname (const long index) const;

    /// @return the element object by index
    Object* getobj (const long index) const;

    /// set or create a new object in the table by quark
    /// @param quark  the object quark to set
    /// @param object the object to set or add
    void add (const long quark, Object* object);

    /// set or create a new object in the table by name
    /// @param name the object name to set
    /// @param object the object to set or add
    void add (const String& name, Object* object);

    /// get an object by quark. If the object is not found nil is returned.
    /// this means that this function always succeds. You can use lookup 
    /// instead to get an exception if the quark is not found. You can also use
    /// the exists method to check for a quark but exists and get is very slow.
    /// @param quark the object quark to search
    /// @return the object or nil
    Object* get (const long quark) const;

    /// get an object by name
    /// @param name the object name to search
    Object* get (const String& name) const;

    /// find an object by quark or throw an exception
    /// @param quark the object quark to find
    Object* lookup (const long quark) const;

    /// find an object by name or throw an exception
    /// @param the object name to find
    Object* lookup (const String& name) const;

    /// remove an object in the name table by quark
    /// @param quark the object quark to remove
    void remove (const long quark);

    /// remove an object in the name table by name
    /// @param name the object name to remove
    void remove (const String& name);
  
    /// @return true if the quark exists in this name table
    bool exists (const long quark) const;

    /// @return true if the name exists in this name table
    bool exists (const String& name) const;

  private:
    // make the copy constructor private
    NameTable (const NameTable&);
    // make the assignment operator private
    NameTable& operator = (const NameTable&);    
  };
}

#endif
