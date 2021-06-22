// ---------------------------------------------------------------------------
// - Tranche.hpp                                                               -
// - afnix:dip service - image tranche class definition                        -
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

#ifndef  AFNIX_TRANCHE_HPP
#define  AFNIX_TRANCHE_HPP

#ifndef  AFNIX_IMAGE_HPP
#include "Image.hpp"
#endif

#ifndef  AFNIX_PIXEL_HPP
#include "Pixel.hpp"
#endif

namespace afnix {
  
  /// The Tranche class is a single or multiple band image class which
  /// aggregates a slice of pixels. The tranche is characterized by its
  /// depth. The tranche format is defined by the pixel format which is
  /// the same across the slices.
  /// @author amaury darsch

  class Tranche : public Image {
  protected:
    /// the pixel format
    Pixel::t_pfmt d_pfmt;
    /// the tranche depth
    long d_dpth;
    
  public:
    /// create a default tranche
    Tranche (void);

    /// copy construct this tranche
    /// @param that the tranche to copy
    Tranche (const Tranche& that);

    /// copy move this tranche
    /// @param that the tranche to move
    Tranche (Tranche&& that) noexcept;

    /// assign an tranche to this one
    /// @param that the tranche to assign
    Tranche& operator = (const Tranche& that);

    /// move an tranche to this one
    /// @param that the tranche to move
    Tranche& operator = (Tranche&& that) noexcept;

    // serialize this slice
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this slice
    /// @param is the input stream
    void rdstream (InputStream& os);
    
    /// reset this tranche
    void reset (void);
    
    /// @return the pixel format
    virtual Pixel::t_pfmt getpfmt (void) const;
    
    /// @return the tranche depth
    virtual long getdpth (void) const;
    
    /// set an tranche component by position and pixel
    /// @param x the tranche x position
    /// @param y the tranche y position
    /// @param d the tranche d position
    /// @param pixl the pixel value
    virtual void setpixl (const long x, const long y, const long d,
			  const Pixel& pixl) =0;

    /// @return a pixel by position
    virtual Pixel getpixl (const long x, const long y, const long d) const =0;
    
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
