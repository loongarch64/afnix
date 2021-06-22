// ---------------------------------------------------------------------------
// - Atom.hpp                                                                -
// - afnix:phy service - atomic element class definition                     -
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

#ifndef  AFNIX_ATOM_HPP
#define  AFNIX_ATOM_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

namespace afnix {

  /// The Periodic class is a simple class that holds the fundamental 
  /// properties of an atomic element.
  /// @author amaury darsch

  class Atom : public Infoable {
  protected:
    /// the atomic number
    long   d_anum;
    /// the atomic weight
    t_real d_wght;
    /// the ionization energy
    t_real d_eion;

  public:
    /// create a null atom
    Atom (void);

    /// create an atom by name, info, number and properties
    /// @param name the atom name
    /// @param info the atom info
    /// @param prop the atom properties
    Atom (const String& name, const String& info, const t_real prop[3]);
    Atom (const String& name, const String& info, const long anum);

    /// copy construct this object
    /// @param that the object to copy
    Atom (const Atom& that);

    /// assign an object to this one
    /// @param that the object to assign
    Atom& operator = (const Atom& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the atomic number
    virtual long getanum (void) const;

    /// @return the atomic weight
    virtual t_real getwght (void) const;

    /// @return the atomic ionization energy
    virtual t_real geteion (void) const;

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
