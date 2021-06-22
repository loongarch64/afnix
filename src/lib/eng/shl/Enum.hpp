// ---------------------------------------------------------------------------
// - Enum.hpp                                                               -
// - afnix engine - enumeration class definition                            -
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

#ifndef  AFNIX_ENUM_HPP
#define  AFNIX_ENUM_HPP

#ifndef  AFNIX_QARRAY_HPP
#include "QuarkArray.hpp"
#endif

namespace afnix {

  /// The Enum class is a container object for an enumeration. By default
  /// an enumeration is empty but can be filled with symbol. Evaluating an
  /// enumeration returns an enumeration item. Such item encapsulate the
  /// enumeration object and the item index.
  /// @author amaury darsch

  class Enum : public Object {
  private:
    /// the enum quark array
    QuarkArray d_enum;

  public:
    /// create a default enumaration
    Enum (void);

    /// @return the class name
    String repr (void) const;

    /// reset this enumeration
    void reset (void);

    /// @return the enumeration length
    long length (void) const;

    /// @return true if an item exists by quark
    bool exists (const long quark) const;

    /// @return true if an item exists by string
    bool exists (const String& name) const;

    /// add an item by name to this enumeration
    /// @param name the item name to add
    void add (const String& name);

    /// add an item to this enumeration
    /// @param quark the quark to add
    void add (const long quark);

    /// @return a item string by index
    String get (const long index) const;

  private:
    // make the copy constructor private
    Enum (const Enum&);
    // make the assignment operator private
    Enum& operator = (const Enum&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// evaluate an object member by quark
    /// @param zobj  zobj the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to evaluate
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark);
  
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
