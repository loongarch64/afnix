// ---------------------------------------------------------------------------
// - Cov.hpp                                                                 -
// - afnix:mth module - samples covariance class definition                  -
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

#ifndef  AFNIX_COV_HPP
#define  AFNIX_COV_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// The Cov class is a class that implements the computation of the
  /// covariance between two sources. The class maintains its own state which
  /// provides support for a dynamic update policy. The default algorithm
  /// computes (E[xy] - E[x]E[y]) which under some circumstances can fail
  /// miserably due to catastropic cancellation. Due to its nature, the
  /// class also provides a direct access to the variance of each source.
  /// @author amaury darsch

  class Cov : public Object {
  protected:
    /// the cumulative x value
    t_real d_cxv;
    /// the cumulative y value
    t_real d_cyv;
    /// the cumulative xx value
    t_real d_cxxv;
    /// the cumulative xy value
    t_real d_cyyv;
    /// the cumulative xy value
    t_real d_cxyv;
    /// the number of samples
    long d_snum;
    
  public:
    /// create a default covariance
    Cov (void);

    /// copy construct this covariance
    /// @param that the object to copy
    Cov (const Cov& that);

    /// copy move this covariance
    /// @param that the object to move
    Cov (Cov&& that);
    
    /// assign a covariance to this one
    /// @param that the object to assign
    Cov& operator = (const Cov& that);

    /// move a covariance to this one
    /// @param that the object to assign
    Cov& operator = (Cov&& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this covariance
    void reset (void);

    /// @return the cumulative x value
    t_real getcxv (void) const;

    /// @return the cumulative y value
    t_real getcyv (void) const;

    /// @return the cumulative xx value
    t_real getcxxv (void) const;

    /// @return the cumulative yy value
    t_real getcyyv (void) const;

    /// @return the cumulative xy value
    t_real getcxyv (void) const;

    /// @return the number of samples
    long getsnum (void) const;

    /// @return the current x variance
    t_real getxv (void) const;

    /// @return the current y variance
    t_real getyv (void) const;

    /// @return the current covariance
    t_real getcov (void) const;

    /// push a new sample value
    /// @param xval the x value to push
    /// @param yval the y value to push
    void push (const t_real xval, const t_real yval);

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
