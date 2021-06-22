// ---------------------------------------------------------------------------
// - Lnrds.hpp                                                               -
// - afnix:sps module - linear interpolation streamer class definition       -
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

#ifndef  AFNIX_LNRDS_HPP
#define  AFNIX_LNRDS_HPP

#ifndef  AFNIX_STREAMABLE_HPP
#include "Streamable.hpp"
#endif

namespace afnix {

  /// The Lnrds class is a linear interpolation streamer which is used to
  /// deliver data as a streambable anywhere between the departure and
  /// arrival position. if a position is given outside the streamable bounds
  /// the position is reset to the closest boundary.
  /// @author amaury darch

  class Lnrds : public Streamable {
  protected:
    /// the streamer position
    t_real d_spos;
    /// the streamable object
    Streamable* p_strm;
    
  public:
    /// create a linear streamer
    Lnrds (void);

    /// create a streamer with a stremable
    /// @param strm the streamable object
    Lnrds (Streamable* strm);
    
    /// destroy this streamer
    ~Lnrds (void);

    /// @return the object name
    String repr (void) const;

    /// reset the sheet streamer
    void reset (void);

    /// @return the streamer departure position
    t_real departure (void) const;

    /// @return the streamer arrival position
    t_real arrival (void) const;

    /// @return the streamer position
    t_real locate (void) const;

    /// move the streamer to the next position
    t_real next (void);

    /// move the streamer to the previous position
    t_real prev (void);
    
    /// set the streamer position
    /// @param pos the position to set
    t_real move (const t_real pos);

    /// @return the streamer integer data
    long getlong (void) const;

    /// @return the streamer real data
    t_real getreal (void) const;

    /// forward the streamer to the next integer
    /// @param pval the integer value to find
    t_real forward (const long pval);

    /// forward the streamer to the next real
    /// @param pval the real value to find
    t_real forward (const t_real pval);

    /// backward the streamer to the previous real
    /// @param pval the real value to find
    t_real backward (const t_real pval);

  private:
    // make the copy constructor private
    Lnrds (const Lnrds&) =delete;
    // make the assignement operator private
    Lnrds& operator = (const Lnrds&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

  };
}

#endif
