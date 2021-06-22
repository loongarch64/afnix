// ---------------------------------------------------------------------------
// - Spsds.hpp                                                               -
// - afnix:sps module - sps data streamer class definition                   -
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

#ifndef  AFNIX_SPSDS_HPP
#define  AFNIX_SPSDS_HPP

#ifndef  AFNIX_FOLIO_HPP
#include "Folio.hpp"
#endif

#ifndef  AFNIX_STREAMABLE_HPP
#include "Streamable.hpp"
#endif

namespace afnix {

  /// The Spsds class is a literal streamer which is used to deliver sps
  /// content by creating a literal linear view of a sheet element. By default
  /// a marker streamer is used for data streaming. Other streamers are row
  /// based, column based or bundle based streamer.
  /// @author amaury darch

  class Spsds : public Streamable {
  public:
    /// the streamer method format
    enum t_meth {
      METH_ROW, // method row
      METH_COL, // method colum
      METH_BND, // method bundle
      METH_MRK  // method mark
    };

  protected:
    /// the streamer method
    t_meth d_meth;
    /// the row index
    long d_ridx;
    /// the column index
    long d_cidx;
    /// the iteration index
    long d_iidx;
    /// the sheet object
    Sheet* p_shto;
    
  public:
    /// create a streamer
    Spsds (void);

    /// create a streamer with a sheet
    /// @param shto the sheet object
    Spsds (Sheet* shto);
    
    /// create a streamer with a folio and sheet index
    /// @param flio the folio object
    /// @param sidx the sheet index
    Spsds (Folio* flio, const long sidx);
    
    /// destroy this streamer
    ~Spsds (void);

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

    /// @return the streamer boolean data
    bool getbool (void) const;

    /// @return the streamer integer data
    long getlong (void) const;

    /// @return the streamer real data
    t_real getreal (void) const;

    /// set the streamer row index
    /// @param ridx the row index
    virtual void setridx (const long ridx);

    /// set the streamer column index
    /// @param cidx the column index
    virtual void setcidx (const long cidx);

    /// set the streamer indexes
    /// @param ridx the row index
    /// @param cidx the column index
    virtual void setindx (const long ridx, const long cidx);
    
  private:
    // make the copy constructor private
    Spsds (const Spsds&) =delete;
    // make the assignement operator private
    Spsds& operator = (const Spsds&) =delete;

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
