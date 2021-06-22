// ---------------------------------------------------------------------------
// - Periodic.hpp                                                            -
// - afnix:phy service - periodic elements class definition                  -
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

#ifndef  AFNIX_PERIODIC_HPP
#define  AFNIX_PERIODIC_HPP

#ifndef  AFNIX_ATOM_HPP
#include "Atom.hpp"
#endif

namespace afnix {

  /// The Periodic class is a simple class that holds the fundamental 
  /// properties of the elements. Ths classificaton is based on the
  /// standard periodic element table.
  /// @author amaury darsch

  class Periodic : public Object {
  public:
    /// create a default table
    /// @param name the symbol name
    Periodic (void) =default;

    /// @return the class name
    String repr (void) const;

    /// @return true if an element exists
    virtual bool exists (const String& name) const;

    /// @return an element by atomic number
    virtual Atom getatom (const long anum) const;
    
    /// @return an element by name
    virtual Atom getatom (const String& name) const;
    
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
