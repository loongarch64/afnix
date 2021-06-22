// ---------------------------------------------------------------------------
// - AliasTable.hpp                                                          -
// - standard object library - alias table class definition                  -
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

#ifndef  AFNIX_ALIASTABLE_HPP
#define  AFNIX_ALIASTABLE_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_HASHTABLE_HPP
#include "HashTable.hpp"
#endif

namespace afnix {

  /// The AliasTable class is a bi-directionnal alias table designed to
  /// map a set of name aliases. The alias table can operate on property
  /// and property list. Since the table is bi-drectionnal the name to alias
  /// can be reversed as necessary. In the direct mode, an original name is
  /// mapped by its alias. In the reverse mode, an alias is mapped by its
  /// original name.
  /// @author amaury darsch

  class AliasTable : public Object {
  private:
    /// the direct table
    HashTable d_dtbl;
    /// the reverse table
    HashTable d_rtbl;

  public:
    /// create an empty alias table
    AliasTable (void);

    /// create an alias table by data
    /// @param size the data size
    /// @param data the data list
    AliasTable (const long size, const char** data);
    
    /// @return the class name
    String repr (void) const override;

    /// reset this table
    void reset (void);

    /// @return true if the table is empty
    bool empty (void) const;

    /// check if a name exists
    bool exists (const String& name) const;

    /// check if an alias name exists
    bool isalias (const String& name) const;

    /// add an alias by name
    /// @param name the name to alias
    /// @param anam the alias name
    void add (const String& name, const String& anam);

    /// map a name to an alias
    /// @param name the name to map
    String map (const String& name) const;

    /// map a property name to an alias
    /// @param prop the property to map
    Property map (const Property& name) const;

    /// map a property list by name
    /// @param plst the property list to map
    Plist map (const Plist& plst) const;

    /// reverse map an alias to a name
    /// @param name the alias name to remap
    String remap (const String& name) const;

    /// reverse map a property alias to a name
    /// @param prop the property to remap
    Property remap (const Property& name) const;

    /// reverse map a property list by alias
    /// @param plst the property list to remap
    Plist remap (const Plist& plst) const;

  private:
    // make the copy constructor private
    AliasTable (const AliasTable&);
    // make the assignment operator private
    AliasTable& operator = (const AliasTable&);

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
