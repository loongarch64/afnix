// ---------------------------------------------------------------------------
// - Localizer.hpp                                                           -
// - afnix:cda service - localizer interface definitions                     -
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

#ifndef  AFNIX_LOCALIZER_HPP
#define  AFNIX_LOCALIZER_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {

  /// This Localizer class is an abstract class that provides an interface
  /// to the streamable position. The most important method is the 'locate'
  /// method which returns a streamer position.
  /// @author amaury darsch

  class Localizer : public virtual Object {
  public:
    /// the localization method
    enum t_lmth {
      LMTH_AFWD, // absolute forward
      LMTH_ABWD, // absolute backward
      LMTH_RFWD, // relative forward
      LMTH_RBWD  // relative backward
    };

  protected:
    /// the localizer method
    t_lmth d_lmth;
    
  public:
    /// create a default localizer
    Localizer (void);

    /// copy move this localizer
    /// @param that the localizer to move
    Localizer (Localizer&& that) noexcept;

    /// move an localizer into this one
    /// @param that the localizer to move
    Localizer& operator = (Localizer&& that) noexcept;

    /// reset this localizer
    virtual void reset (void) =0;

    /// set the localization method
    /// @param lmth the method to set
    virtual void setlmth (const t_lmth lmth);

    /// @return the localization method
    virtual t_lmth getlmth (void) const;

    /// validate a localized position
    /// @param pval the position value
    virtual bool validate (const t_real pval) const;
    
    /// locate by boolean argument
    /// @param pval the position value
    virtual t_real locate (const bool pval) const;
    
    /// locate by integer argument
    /// @param pval the position value
    virtual t_real locate (const long pval) const;
    
    /// locate by real argument
    /// @param pval the position value
    virtual t_real locate (const t_real pval) const;

    /// @return the localized boolean by position
    virtual bool getbool (const t_real pos) const;

    /// @return the localized integer by position
    virtual long getlong (const t_real pos) const;
    
    /// @return the localized real by position
    virtual t_real getreal (const t_real pos) const;
    
  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
