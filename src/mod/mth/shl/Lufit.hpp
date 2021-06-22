// ---------------------------------------------------------------------------
// - Lufit.hpp                                                               -
// - afnix:mth module - Linear univariate model fitting class definition     -
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

#ifndef  AFNIX_LUFIT_HPP
#define  AFNIX_LUFIT_HPP

#ifndef  AFNIX_FIT_HPP
#include "Fit.hpp"
#endif

#ifndef  AFNIX_COV_HPP
#include "Cov.hpp"
#endif 
namespace afnix {

  /// The Lufit class is the univariate linear model implementation of the
  /// model fitting interface. Simply said, the class implements a linear
  /// regression with both learning and estimation processes. This model
  /// is the simplest one with a (1,1) dimension.
  /// @author amaury darsch

  class Lufit : public Fit {
  private:
    /// the covariance data
    Cov d_dcov;
    /// the cached predictor value
    t_real d_pv;
    /// the cached slope
    t_real d_cs;
    /// the cached intercept
    t_real d_ci;
    
  public:
    /// create a default fitter
    Lufit (void);

    /// copy construct this fitter
    /// @param that the object to copy
    Lufit (const Lufit& that);

    /// copy move this fitter
    /// @param that the object to move
    Lufit (Lufit&& that);
    
    /// assign a fitter to this one
    /// @param that the object to assign
    Lufit& operator = (const Lufit& that);

    /// move a fitter to this one
    /// @param that the object to assign
    Lufit& operator = (Lufit&& that);

    /// @return the class name
    String repr (void) const;
    
    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this fitter
    void reset (void);

    /// clear this fitter
    void clear (void);
    
    /// @return the number of predictors
    long getsize (void) const;

    /// add a predictor value
    /// @param pv the predictor value
    void addp (const t_real pv);
    
    /// add a response value value
    /// @param rv the response value
    void addr (const t_real rv);

    /// factorize this model fitter
    bool factorize (void);
    
    /// push a regressor value and regress
    /// @param rv the regressor value
    t_real regress (const t_real rv) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);    
  };
}

#endif
