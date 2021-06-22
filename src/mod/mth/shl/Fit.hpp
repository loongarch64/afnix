// ---------------------------------------------------------------------------
// - Fit.hpp                                                                 -
// - afnix:mth module - Model fitting interface definitions                  -
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

#ifndef  AFNIX_FIT_HPP
#define  AFNIX_FIT_HPP

#ifndef  AFNIX_RVI_HPP
#include "Rvi.hpp"
#endif
 
namespace afnix {

  /// The Fit class is the interface class for the model fitting. The
  /// class defines the operations needed for the learning phase, the
  /// fitting as well as the methods for the estimation. The class is
  /// designed to operate with a predictor/response feeding mode. Adding
  /// predictor increases the model dimension while addig a response starts
  /// a new trial.
  /// @author amaury darsch

  class Fit : public Object {
  public:
    /// reset this model fitter
    virtual void reset (void) =0;

    /// clear this model fitter
    virtual void clear (void) =0;

    /// @return the number of predictors
    virtual long getsize (void) const =0;
    
    /// add a predictor value
    /// @param pv the predictor value
    virtual void addp (const t_real pv) =0;
    
    /// add a response value value
    /// @param rv the response value
    virtual void addr (const t_real rv) =0;

    /// factorize this model fitter
    virtual bool factorize (void) =0;
    
    /// push a regressor value and regress
    /// @param rv the regressor value
    virtual t_real regress (const t_real rv) const =0;
    
  public:
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
