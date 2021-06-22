// ---------------------------------------------------------------------------
// - Locale.hpp                                                              -
// - standard object library - locale class definition                       -
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

#ifndef  AFNIX_LOCALE_HPP
#define  AFNIX_LOCALE_HPP

#ifndef  AFNIX_REGEX_HPP
#include "Regex.hpp"
#endif

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Locale class is a class design to support the formating of
  /// various objects such like literals and/or date and time. The class
  /// is bidirectional as can format an object into a string or the other
  /// way around. This is achieved by parsing the string with respect to
  /// a certain specification.
  /// @author amaury darsch

  class Locale : public Taggable {
  public:
    /// the date format
    enum t_dfmt : t_byte
      {
       DFMT_YYMMDD   = 0x00U,
       DFMT_DDMMYY   = 0x01U,
       DFMT_YYYYMMDD = 0x02U,
       DFMT_DDMMYYYY = 0x03U
      };

    /// the object format
    enum t_ofmt : t_byte
      {
       OFMT_IDEM = 0x00U,
       OFMT_INTG = 0x01U,
       OFMT_REAL = 0x02U,
       OFMT_DATE = 0x03U
      };

    /// the object modifier
    enum t_omod : t_byte
      {
       OMOD_NONE = 0x00U,
       OMOD_SHRT = 0x01U
      };

    /// @return an object format by string
    static t_ofmt toofmt (const String& sval);

    /// @return an object modifier by string
    static t_omod toomod (const String& sval);
    
  protected:
    /// the locale index
    long d_lidx;
    /// the decimal point
    t_quad d_dpnt;
    /// the decimal separator
    t_quad d_dsep;
    /// the date format
    t_dfmt d_dfmt;
    /// the date marker
    t_quad d_dmrk;
    /// the date regex
    Regex  d_drgx;
    /// the object format
    t_ofmt d_ofmt;
    /// the object modifier
    t_omod d_omod;
    
  public:
    /// create a default locale
    Locale (void);

    /// create a locale by format
    /// @param s the locale parameters
    Locale (const String& s);

    /// copy construct this locale
    /// @param that the object to copy
    Locale (const Locale& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// assign a locale to this one
    /// @param that the obejct to assign
    Locale& operator = (const Locale& that);

    /// @return the serial id
    t_word getdid (void) const override;

    /// @return the serial id
    t_word getsid (void) const override;

    /// serialize this object to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize an object from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;
    
    /// reset this locale
    virtual void reset (void);

    /// set the locale index
    /// @paran lidx the locale index to set
    virtual void setlidx (const long lidx);
    
    /// @return the locale info
    virtual long getlidx (void) const;
    
    /// set the locale decimal point
    /// @paran pnt the decimal point to set
    virtual void setdpnt (const t_quad dpnt);
    
    /// @return the locale decimal point
    virtual t_quad getdpnt (void) const;
    
    /// set the locale decimal separator
    /// @paran dsep the decimal separator to set
    virtual void setdsep (const t_quad dsep);
    
    /// @return the locale decimal separator
    virtual t_quad getdsep (void) const;

    /// set the locale date format
    /// @paran dfmt the date format to set
    virtual void setdfmt (const String& dfmt);
    
    /// @return the locale date format
    virtual String getdfmt (void) const;

    /// set the locale date marker
    /// @paran dmrk the date marker to set
    virtual void setdmrk (const t_quad dmrk);
    
    /// @return the locale date marker
    virtual t_quad getdmrk (void) const;

    /// set the locale object format
    /// @paran ofmt the object format to set
    virtual void setofmt (const String& ofmt);
    
    /// @return the locale object format
    virtual String getofmt (void) const;
    
    /// set the locale object modifier
    /// @paran omod the object modifier to set
    virtual void setomod (const String& omod);
    
    /// @return the locale object modifier
    virtual String getomod (void) const;
    
    /// set the locale parameters
    /// @param lp the parameter string
    virtual void setprm (const String& lp);

    /// @return the locale parameters
    virtual String getprm (void) const;
    
    /// convert a locale string to a real
    /// @param s the string to convert
    virtual t_real toreal (const String& s) const;

    /// convert a locale string to a date
    /// @param s the string to convert
    virtual t_long todate (const String& s) const;

    /// convert a string to an object
    /// @param s the string to convert
    virtual Object* toobject (const String& s) const;
    
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
