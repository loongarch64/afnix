// ---------------------------------------------------------------------------
// - SpsTransit.hpp                                                          -
// - afnix:sps module - sps objects transit class definition                 -
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

#ifndef  AFNIX_SPSTRANSIT_HPP
#define  AFNIX_SPSTRANSIT_HPP

#ifndef  AFNIX_FOLIO_HPP
#include "Folio.hpp"
#endif

#ifndef  AFNIX_STRUCTURE_HPP
#include "Structure.hpp"
#endif

namespace afnix {

  /// The SpsTransit class is a base class for the spreadsheet import and
  /// export processes. The class mostly defines the import and export
  /// methods which are used to transfer data from a record, sheet or folio.
  /// @author amaury darch

  class SpsTransit : public virtual Object {
  public:
    /// the transit mode
    enum t_tmod
      {
       TMOD_AUTO, // mode auto
       TMOD_XALS, // mode als
       TMOD_XCSV, // mode csv
      };

  protected:
    /// the transit mode
    t_tmod d_tmod;
    /// the break sequence
    String d_sbrk;
    /// the locale vector
    Vector d_locv;
    /// the transit sheet
    Sheet* p_tsht;
    /// the output stream
    OutputStream* p_os;

  public:
    /// create a default transit
    SpsTransit (void);

    /// create a transit by output stream
    /// @param os the output stream
    SpsTransit (OutputStream* os);
    
    /// create a transit by mode
    /// @param xmod the transit mode
    /// @param sbrk the break sequence
    SpsTransit (const String& xmod, const String& sbrk);

    /// destroy this transit
    ~SpsTransit (void);

    /// @return the object name
    String repr (void) const;
    
    /// set the transit mode
    /// @param xmod the transit mode
    virtual void setxmod (const String& xmod);

    /// set the break sequence
    /// @param sbrk the break sequence
    virtual void setsbrk (const String& sbrk);

    /// add a locale definition
    /// @param ldef the locale descriptor
    virtual long addldef (const String& ldef);

    /// @return the transit sheet
    virtual Sheet* gettsht (void) const;

    /// set the transit sheet
    /// @param sht the sheet to set
    virtual void settsht (Sheet* sht);

    /// @return a detached transit sheet
    virtual Sheet* detach (void);

    /// fill a structure with a sheet
    /// @param strc the structure to fill
    virtual bool fill (Structure* strc) const;
    
    /// import cons data
    /// @param cons the cons cell to use
    virtual void import (Cons* cons);

    /// import stream data into the record
    /// @param is the input stream to use
    virtual void import (InputStream* is);

    /// import data from a file
    /// @param name the input file to use
    virtual void import (const String& name);

    /// export a structure to an output stream
    /// @param s the structure to export
    virtual bool xport (const Structure& s);
    
  private:
    // make the copy constructor private
    SpsTransit (const SpsTransit&) =delete;
    // make the assignement operator private
    SpsTransit& operator = (const SpsTransit&) =delete;

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
