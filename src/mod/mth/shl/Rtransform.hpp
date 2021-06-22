// ---------------------------------------------------------------------------
// - Rtransform.hpp                                                          -
// - afnix:mth module - real transform class definitions                     -
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

#ifndef  AFNIX_RTRANSFORM_HPP
#define  AFNIX_RTRANSFORM_HPP

#ifndef  AFNIX_RMATRIX2_HPP
#include "Rmatrix2.hpp"
#endif

#ifndef  AFNIX_RMATRIX3_HPP
#include "Rmatrix3.hpp"
#endif

#ifndef  AFNIX_RMATRIX4_HPP
#include "Rmatrix4.hpp"
#endif

namespace afnix {
  
  /// The rtransform class is a collection of static affine rtransformation
  /// algorithms. This class provides a default algorithm implementation that
  /// can be optimized by a specialized class. The rtransformations depends on
  /// an axis orientation mode which should be set according to the underlying
  /// hardware platform.
  /// @author amaury darsch

  class Rtransform : public Object {
  public:
    /// create a vector from a point
    /// @param p the target point
    static Rvector2 tovector (const Rpoint2& p);

    /// create a vector from a point
    /// @param p the target point
    static Rvector3 tovector (const Rpoint3& p);

    /// create a vector from a point
    /// @param p the target point
    static Rvector4 tovector (const Rpoint4& p);

    /// create a vector from two points
    /// @param o the orgin point
    /// @param p the target point
    static Rvector2 tovector (const Rpoint2& o, const Rpoint2& p);

    /// create a vector from two points
    /// @param o the orgin point
    /// @param p the target point
    static Rvector3 tovector (const Rpoint3& o, const Rpoint3& p);

    /// create a vector from two points
    /// @param o the orgin point
    /// @param p the target point
    static Rvector4 tovector (const Rpoint4& o, const Rpoint4& p);

    /// translate a point with a vector
    /// @param p the origin point
    /// @param v the translation vector
    static Rpoint2 translate (const Rpoint2& p, const Rvector2& v);
    
    /// translate a point with a vector
    /// @param p the origin point
    /// @param v the translation vector
    static Rpoint3 translate (const Rpoint3& p, const Rvector3& v);
    
    /// translate a point with a vector
    /// @param p the origin point
    /// @param v the translation vector
    static Rpoint4 translate (const Rpoint4& p, const Rvector4& v);
    
    /// scale a vector by a real
    /// @param s the scalar argument
    /// @param v the vector argument
    static Rvector2 scale (const t_real s, const Rvector2& v);
    
    /// scale a vector by a real
    /// @param s the scalar argument
    /// @param v the vector argument
    static Rvector3 scale (const t_real s, const Rvector3& v);
    
    /// scale a vector by a real
    /// @param s the scalar argument
    /// @param v the vector argument
    static Rvector4 scale (const t_real s, const Rvector4& v);

    /// normalize a vector
    /// @param v the vector to normalize
    static Rvector2 normalize (const Rvector2& v);
    
    /// normalize a vector and return the norm
    /// @param v the vector to normalize
    static Rvector3 normalize (const Rvector3& v);
    
    /// normalize a vector and return the norm
    /// @param v the vector to normalize
    static Rvector4 normalize (const Rvector4& v);

    /// compute the vector cross product
    /// @param vl the left vector
    /// @param vr the right vector
    static Rvector3 cross (const Rvector3& vl, const Rvector3& vr);

  public:
    /// create a default rtransformation
    Rtransform (void) =default;

    /// @return the class name
    String repr (void) const override;
    
    /// create a 2 dimensional rotation matrix
    /// @param a the rotation angle
    virtual Rmatrix2 newrot2 (const t_real a);

    /// set a 2 dimensional rotation matrix
    /// @param m the rotation matrix
    /// @param a the rotation angle
    virtual Rmatrix2& setrot2 (Rmatrix2& m, const t_real a);

    /// create a 3 dimensional rotation matrix
    /// @param a the xyz rotation angle
    virtual Rmatrix3 newrot3 (const t_real a[3]);

    /// set a 3 dimensional rotation matrix
    /// @param m the rotation matrix
    /// @param a the xyz rotation angle
    virtual Rmatrix3& setrot3 (Rmatrix3& m, const t_real a[3]);

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
