// ---------------------------------------------------------------------------
// - Xref.hpp                                                                -
// - afnix:sps module - cross reference class definition                     -
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

#ifndef  AFNIX_XREF_HPP
#define  AFNIX_XREF_HPP

#ifndef  AFNIX_INDEX_HPP
#include "Index.hpp"
#endif

#ifndef  AFNIX_HASHTABLE_HPP
#include "HashTable.hpp"
#endif

namespace afnix {

  /// The Xref is a cross-reference class. The class maintains the
  /// association between a name and an index. with a particular name,
  /// an index entry is created if it does not exists. Such entry can be
  /// later used to access the cell content by index.
  /// @author amaury darsch

  class Xref : public Object {
  private:
    /// the Object table
    HashTable d_htbl;

  public:
    /// create a default xref table
    Xref (void);

    /// @return the class name
    String repr (void) const;

    /// reset the xref table
    void reset (void);

    /// @return the table length
    long length (void) const;

    /// @return the xref name by index
    String getname (const long index) const;

    /// @return true if an index exists
    bool exists (const String& name) const;

    /// @return an index by index
    Index* get (const long index) const;

    /// @return an index by name
    Index* get (const String& name) const;

    /// @return an index by name or throw an exception
    Index* lookup (const String& name) const;
    
    /// add an entry by name and coordinate
    /// @param name the index name
    /// @param cidx the cell index
    void add (const String& name, const long cidx);

    /// add an entry by name and coordinate
    /// @param name the index name
    /// @param cidx the cell index
    /// @param ridx the record index
    void add (const String& name, const long cidx, const long ridx);

    /// add an entry by name and coordinate
    /// @param name the index name
    /// @param cidx the cell index
    /// @param ridx the record index
    /// @param sidx the sheet index
    void add (const String& name, const long cidx, const long ridx,
	      const long sidx);

  private:
    // make the copy constructor private
    Xref (const Xref&);
    // make the assignement operator private
    Xref& operator = (const Xref&);

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
}

#endif
