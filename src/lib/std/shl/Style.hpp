// ---------------------------------------------------------------------------
// - Style.hpp                                                               -
// - Standard object library - style class definition                        -
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

#ifndef  AFNIX_STYLE_HPP
#define  AFNIX_STYLE_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

namespace afnix {

  /// The Style class is a generic placeholder for text style. The styling
  /// mechanism enables the customization of the literal rendering for various
  /// targets such like print table.
  /// @author amaury darsch

  class Style : public virtual Serial {
  public:
    /// the text weight
    enum t_wght {
      WGHT_N, // normal
      WGHT_B, // bold
      WGHT_I  // italic
    };
    /// the text alignment
    enum t_algn {
      ALGN_L, // left
      ALGN_C, // center
      ALGN_R  // right 
    };

  protected:
    /// the text weight
    t_wght d_wght;
    /// the text alignment
    t_algn d_algn;
    /// the text size
    long d_tsiz;
    /// the filling character
    t_quad d_fill;
    /// the number precision
    long d_psiz;
    /// the scientific flag
    bool d_sflg;
    /// the truncate flag
    bool d_tflg;
    /// the truncate left size
    long d_lsiz;
    /// the truncate right size
    long d_rsiz;
    
  public:
    /// create a default style
    Style (void);

    /// copy construct this style
    /// @param that the style to copy
    Style (const Style& that);

    /// @return the object name
    String repr (void) const;

    /// @ return a clone of this object
    Object* clone (void) const;

    /// @return the style did
    t_word getdid (void) const;

    /// @return the style sid
    t_word getsid (void) const;

    /// serialize this style
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this style
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// assign a style to this one
    /// @param that the style to assign
    Style& operator = (const Style& that);

    /// reset this style
    void reset (void);

    /// set the text weight
    /// @param wght the text weight 
    virtual void setwght (const t_wght wght);

    /// @return the text weight
    virtual t_wght getwght (void) const;

    /// set the text alignment
    /// @param algn the text alignment 
    virtual void setalgn (const t_algn algn);

    /// @return the text alignment
    virtual t_algn getalgn (void) const;

    /// set the text size
    /// @param tsiz the text size 
    virtual void settsiz (const long tsiz);

    /// @return the text size
    virtual long gettsiz (void) const;

    /// set the filling character
    /// @param fill the filling character 
    virtual void setfill (const t_quad fill);

    /// @return the filling character
    virtual t_quad getfill (void) const;

    /// set the number precision
    /// @param psiz the number precision 
    virtual void setpsiz (const long psiz);

    /// @return the number precision
    virtual long getpsiz (void) const;

    /// set the scientific flag
    /// @param sflg the scientific flag 
    virtual void setsflg (const bool sflg );

    /// @return the scientific flag
    virtual bool getsflg (void) const;

    /// set the truncate mode
    /// @param lsiz the left size to keep
    /// @param rsiz the right size to keep
    virtual void settrunc (const long lsiz, const long rsiz);
    
    /// format an integer to a string
    /// @param ival the integer to convert
    virtual class String format (const long ival) const;

    /// format an integer to a string
    /// @param ival the integer to convert
    virtual class String format (const t_long ival) const;

    /// format a real to a string - no extra style
    /// @param rval the real to convert
    virtual class String format (const t_real rval) const;

    /// format a string with style
    /// @param sval the string to format
    /// @param size the maximum size
    virtual class String format (const class String& sval, 
				 const long size) const;

    /// truncate a string by style
    /// @param sval the string to truncate
    virtual class String truncate (const class String& sval) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
    /// evaluate an object data member
    /// @param zobj  zobj the current evaluable
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
