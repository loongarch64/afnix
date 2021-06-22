// ---------------------------------------------------------------------------
// - Transform.hpp                                                           -
// - afnix:svg service - svg transformation attribute class definition       -
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

#ifndef  AFNIX_TRANSFORM_HPP
#define  AFNIX_TRANSFORM_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Transform class is an object designed to represent a svg
  /// transformation. The transformation parameters are stored in the
  /// object and later used to generate a svg transform-list in the form
  /// of a string representation.
  /// @author amaury darsch

  class Transform : public Object {
  private:
    /// the transform list
    struct s_svgt* p_tlst;

  public:
    /// create a null tranformation
    Transform (void);

    /// destroy this transformation
    ~Transform (void);

    /// @return the class name
    String repr (void) const;

    /// reset this transformation
    void reset (void);

    /// @return true if the transformation is nil
    bool isnil (void) const;

    /// @return a string representation
    String tostring (void) const;

    /// add a translation transformation
    /// @param tx the x translation factor
    void addtr (const long tx);

    /// add a translation transformation
    /// @param tx the x translation factor
    /// @param ty the y translation factor
    void addtr (const long tx, const long ty);
    
    /// add a scaling transformation
    /// @param sx the x scaling factor
    void addsc (const t_real sx);

    /// add a scaling transformation
    /// @param sx the x scaling factor
    /// @param sy the y scaling factor
    void addsc (const t_real sx, const t_real sy);
    
    /// add a rotation transformation
    /// @param ra the rotation angle
    void addrt (const t_real ra);
    
    /// add a rotation transformation
    /// @param ra the rotation angle
    /// @param px the rotation x point
    /// @param py the rotation y point
    void addrt (const t_real ra, const long px, const long py);
    
    /// add a skew-x transformation
    /// @param sa the skew angle
    void addsx (const t_real sa);
    
    /// add a skew-y transformation
    /// @param sa the skew angle
    void addsy (const t_real sa);
    
    /// add a matrix transformation
    /// @param a the matrix a parameter
    /// @param b the matrix a parameter
    /// @param c the matrix a parameter
    /// @param d the matrix a parameter
    /// @param e the matrix a parameter
    /// @param f the matrix a parameter
    void addmt (const t_real a, const t_real b, const t_real c,
		const t_real d, const t_real e, const t_real f);

  private:
    // make the copy constructor private
    Transform (const Transform&);
    // make the assignment operator private
    Transform& operator = (const Transform&);

  public:
    /// create a new object in a generic object
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
