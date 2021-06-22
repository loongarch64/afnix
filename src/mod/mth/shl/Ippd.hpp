// ---------------------------------------------------------------------------
// - Ippd.hpp                                                                -
// - afnix:mth module - plane point integer datum class definitions          -
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

#ifndef  AFNIX_IPPD_HPP
#define  AFNIX_IPPD_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Ippd class is a class that implements a two dimensional (plane)
  /// point with integer coordinates. The coordinates are denoted by the
  /// (x,y) tuple, thus making this class extremelly simple.
  /// @author amaury darsch

  class Ippd : public Object {
  protected:
    /// the x coordinate
    long d_x;
    /// the y coordinate
    long d_y;

  public:
    /// create a default point
    Ippd (void);

    /// create a point by coordinates
    /// @param x the x coordinate
    /// @param y the y coordinate
    Ippd (const long x, const long y);

    /// copy construct this point
    /// @param that the object to copy
    Ippd (const Ippd& that);

    /// assign a point to this one
    /// @param that the object to assign
    Ippd& operator = (const Ippd& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// set the point x coordinate
    /// @param x the x coordinate to set
    virtual void setx (const long x);

    /// @return the point x coordinate
    virtual long getx (void) const;

    /// set the point y coordinate
    /// @param y the y coordinate to set
    virtual void sety (const long y);

    /// @return the point y coordinate
    virtual long gety (void) const;

    /// set the point coordinates at once
    /// @param x the x coordinate
    /// @param y the y coordinate
    virtual void set (const long x, const long y);

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
