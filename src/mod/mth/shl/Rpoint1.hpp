// ---------------------------------------------------------------------------
// - Rpoint1.hpp                                                             -
// - afnix:mth module - real point 1 definitions                             -
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

#ifndef  AFNIX_RPOINT1_HPP
#define  AFNIX_RPOINT1_HPP

#ifndef  AFNIX_RPOINT_HPP
#include "Rpoint.hpp"
#endif

namespace afnix {
  
  /// The Rpoint1 class is a 1 dimensional  point class for the afnix math
  /// module. A 1 dimensional point is defined by its (x) coordinate.
  /// @author amaury darsch

  class Rpoint1 : public Rpoint {
  public:
    /// create a default point
    Rpoint1 (void);

    /// create a point by coordinates
    /// @param x the x coordinate
    Rpoint1 (const t_real x);

    /// copy construct this point
    /// @param that the point to copy
    Rpoint1 (const Rpoint1& that);

    /// assign a point to this one
    /// @param that the point to assign
    Rpoint1& operator = (const Rpoint1& that);
    
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
