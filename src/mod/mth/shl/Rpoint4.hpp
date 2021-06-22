// ---------------------------------------------------------------------------
// - Rpoint4.hpp                                                             -
// - afnix:mth module - real point 4 definitions                             -
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

#ifndef  AFNIX_RPOINT4_HPP
#define  AFNIX_RPOINT4_HPP

#ifndef  AFNIX_RPOINT_HPP
#include "Rpoint.hpp"
#endif

namespace afnix {
  
  /// The Rpoint4 class is a 4 dimensional  point class for the afnix math
  /// module. A 4 dimensional point is defined by its (x,y,z,w) coordinates.
  /// @author amaury darsch

  class Rpoint4 : public Rpoint {
  public:
    /// create a default point
    Rpoint4 (void);

    /// create a point by coordinates
    /// @param x the x coordinate
    /// @param y the y coordinate
    /// @param z the z coordinate
    /// @param w the z coordinate
    Rpoint4 (const t_real x, const t_real y, const t_real z, const t_real w);

    /// copy construct this point
    /// @param that the point to copy
    Rpoint4 (const Rpoint4& that);

    /// assign a point to this one
    /// @param that the point to assign
    Rpoint4& operator = (const Rpoint4& that);
    
    /// @return the class name
    String repr (void) const override;
    
    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;

    /// serialize this material
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this material
    /// @param is the input stream
    void rdstream (InputStream& os) override;
    
    /// @return the point x coordinate
    virtual t_real getx (void) const;

    /// @return the point y coordinate
    virtual t_real gety (void) const;
    
    /// @return the point z coordinate
    virtual t_real getz (void) const;
    
    /// @return the point w coordinate
    virtual t_real getw (void) const;
    
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
