// ---------------------------------------------------------------------------
// - Pixel.hpp                                                               -
// - afnix:dip service - pixel class definition                              -
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

#ifndef  AFNIX_PIXEL_HPP
#define  AFNIX_PIXEL_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {
  
  /// The Pixel class is a simple class which holds the pixel components
  /// in the form of various formats with up to four components. In the
  /// simplest form, the pixel is stored as a unique byte band. Variations
  /// around the single band format are provided. Standard multiband rgba
  /// format is also supported with a variety of representations. Note that
  /// in the band format, all components are the same.
  /// are supported as well.
  /// @author amaury darsch

  class Pixel : public virtual Serial {
  public:
    /// the pixel format
    enum t_pfmt : t_byte {
      PFMT_NONE = 0x00U, // no format
      PFMT_BYTE,         // band  8 bits
      PFMT_WORD,         // band 16 bits
      PFMT_REAL,         // band real
      PFMT_FLOT,         // band float
      PFMT_RGBA,         // rgba 32 bits
      PFMT_BGRA,         // bgra 32 bits
      PFMT_RGBO,         // rgba 64 bits
      PFMT_BGRO,         // bgra 64 bits
      PFMT_RGBR,         // rgba real
      PFMT_RGBF          // rgba float
    };
    /// the pixel components
    using t_pixl = union {
      t_byte d_b[4];
      t_word d_w[4];
      t_real d_r[4];
      float  d_f[4];
    };
      
    /// convert a string to a pixel format
    static t_pfmt topfmt (const String& pfmt);
    
  protected:
    /// the pixel format
    t_pfmt d_pfmt;
    /// the pixel data
    t_pixl d_pixl;
    
  public:
    /// create a default pixel
    Pixel (void);

    /// create a pixel by format and components
    /// @param pfmt the pixel format
    /// @param pixl the pixel components
    Pixel (const t_pfmt pfmt, const t_pixl pixl);
       
    /// copy construct this pixel
    /// @param that the pixel to copy
    Pixel (const Pixel& that);

    /// assign a pixel to this one
    /// @param that the pixel to assign
    Pixel& operator = (const Pixel& that);

    /// @return the class name
    String repr (void) const override;
    
    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;

    /// serialize this pixmap
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this pixmap
    /// @param is the input stream
    void rdstream (InputStream& os) override;
    
    /// reset this pixel
    virtual void reset (void);

    /// clear this pixel
    virtual void clear (void);

    /// set the pixel format
    /// @param pfmt the format to set
    virtual void setpfmt (const t_pfmt pfmt);

    /// @return the pixel format
    virtual t_pfmt getpfmt (void) const;
    
    /// set a pixel value
    /// @param pixl the component value
    virtual void setpixl (const t_pixl pixl);

    /// @return a pixel components
    virtual t_pixl getpixl (void) const;

    /// set a pixel by format and value
    /// @param pixl the component value
    virtual void setpixl (const t_pfmt pfmt, const t_pixl pixl);

    /// set a component value by object
    /// @param cobj the component object
    virtual void setcval (Object* cobj);
    
    /// set a component value by object
    /// @param cidx the component index
    /// @param cobj the component object
    virtual void setcval (const long cidx, Object* cobj);
    
    /// @return a component object
    virtual Object* getcval (void) const;

    /// @return a component object
    virtual Object* getcval (const long cidx) const;

    /// convert a pixel format to another one
    /// @param pfmt the target format
    virtual Pixel convert (const t_pfmt pfmt) const;
    
  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
