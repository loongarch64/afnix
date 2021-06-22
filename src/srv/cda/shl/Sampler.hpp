// ---------------------------------------------------------------------------
// - Sampler.hpp                                                             -
// - afnix:cda service - data sampler interface definitions                  -
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

#ifndef  AFNIX_SAMPLER_HPP
#define  AFNIX_SAMPLER_HPP

#ifndef  AFNIX_LOCALIZER_HPP
#include "Localizer.hpp"
#endif

#ifndef  AFNIX_STREAMABLE_HPP
#include "Streamable.hpp"
#endif

namespace afnix {

  /// This Sampler class is an abstract class that provides an interface
  /// for data sampling with the help of a localizer object and a streamable.
  /// In other words, the data sampling is performed by localization and
  /// extraction.
  /// @author amaury darsch

  class Sampler : public virtual Object {
  protected:
    /// the localizer object
    Localizer* p_lobj;
    /// the streamable object
    Streamable* p_sobj;
    
  public:
    /// create a default sampler
    Sampler (void);

    /// create a sampler by objects
    /// @param lobj the localizer object
    /// @param sobj the streamable object
    Sampler (Localizer* lobj, Streamable* sobj);

    /// destroy this sampler
    ~Sampler (void);

    /// @return the object name
    String repr (void) const;

    /// reset this sampler
    virtual void reset (void);
    
    /// @return the localizer object
    virtual Localizer* getlobj (void) const;

    /// @return the streamable object
    virtual Streamable* getsobj (void) const;

    /// validate a localized position
    /// @param pval the position value
    virtual bool validate (const t_real pval) const;
    
    /// sample a boolean by boolean
    /// @param pval the position value
    virtual bool mapbool (const bool pval);
    
    /// sample a boolean by integer
    /// @param pval the position value
    virtual bool mapbool (const long pval);
    
    /// sample a boolean by real
    /// @param pval the position value
    virtual bool mapbool (const t_real pval);

    /// sample an integer by boolean
    /// @param pval the position value
    virtual long maplong (const bool pval);
    
    /// sample an integer by integer
    /// @param pval the position value
    virtual long maplong (const long pval);
    
    /// sample an integer by real
    /// @param pval the position value
    virtual long maplong (const t_real pval);
    
    /// sample a real by boolean
    /// @param pval the position value
    virtual t_real mapreal (const bool pval);
    
    /// sample a real by integer
    /// @param pval the position value
    virtual t_real mapreal (const long pval);
    
    /// sample a real by real
    /// @param pval the position value
    virtual t_real mapreal (const t_real pval);
    
  private:
    // make the copy constructor private
    Sampler (const Sampler&) =delete;
    // make the assignement operator private
    Sampler& operator = (const Sampler&) =delete;

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
