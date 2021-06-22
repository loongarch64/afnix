// ---------------------------------------------------------------------------
// - Stmdl.hpp                                                               -
// - afnix:sps module - streamable data localizer class definition           -
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

#ifndef  AFNIX_STMDL_HPP
#define  AFNIX_STMDL_HPP

#ifndef  AFNIX_LOCALIZER_HPP
#include "Localizer.hpp"
#endif

#ifndef  AFNIX_STREAMABLE_HPP
#include "Streamable.hpp"
#endif

namespace afnix {

  /// The Stmdl class is a streamable data localizer which is used to locate
  /// a position with the use of the forward/backward streamable methods.
  /// Normally a locate operation is carried in forward mode. A backward flag
  /// can be used to reverse this mode.
  /// @author amaury darch

  class Stmdl : public Localizer {
  protected:
    /// the relative position
    t_real d_rpos;
    /// the localized departure
    t_real d_locd;
    /// the localized arrival
    t_real d_loca;
    /// the streamable object
    mutable Streamable* p_strm;
    
  public:
    /// create a localizer
    Stmdl (void);

    /// create a localizer with a streamable
    /// @param strm the streamable object
    Stmdl (Streamable* strm);
    
    /// destroy this localizer
    ~Stmdl (void);

    /// @return the object name
    String repr (void) const;

    /// reset the localizer
    void reset (void);

    /// validate a localized position 
    /// @param pval the position value
    bool validate (const t_real pval) const;
    
    /// locate by boolean argument
    /// @param pval the position value
    t_real locate (const bool pval) const;
    
    /// locate by integer argument
    /// @param pval the position value
    t_real locate (const long pval) const;
    
    /// locate by real argument
    /// @param pval the position value
    t_real locate (const t_real pval) const;

    /// @return the localized boolean by position
    bool getbool (const t_real pos) const;

    /// @return the localized integer by position
    long getlong (const t_real pos) const;
    
    /// @return the localized real by position
    t_real getreal (const t_real pos) const;
    
  private:
    // make the copy constructor private
    Stmdl (const Stmdl&) =delete;
    // make the assignement operator private
    Stmdl& operator = (const Stmdl&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
