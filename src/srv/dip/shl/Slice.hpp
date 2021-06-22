// ---------------------------------------------------------------------------
// - Slice.hpp                                                               -
// - afnix:dip service - image slice class definition                        -
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

#ifndef  AFNIX_SLICE_HPP
#define  AFNIX_SLICE_HPP

#ifndef  AFNIX_IMAGE_HPP
#include "Image.hpp"
#endif

#ifndef  AFNIX_PIXEL_HPP
#include "Pixel.hpp"
#endif

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

namespace afnix {
  
  /// The Slice class is an image class which aggregates a block of pixels.
  /// The slice format is defined by the pixel format which is the same
  /// across the block, with data accessor/mutator that operates by pixel.
  /// @author amaury darsch

  class Slice : public Image {
  protected:
    /// the pixel format
    Pixel::t_pfmt d_pfmt;

  public:
    /// create a default slice
    Slice (void);

    /// copy construct this slice
    /// @param that the slice to copy
    Slice (const Slice& that);

    /// copy move this slice
    /// @param that the slice to move
    Slice (Slice&& that) noexcept;

    /// assign an slice to this one
    /// @param that the slice to assign
    Slice& operator = (const Slice& that);

    /// move an slice to this one
    /// @param that the slice to move
    Slice& operator = (Slice&& that) noexcept;

    // serialize this slice
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this slice
    /// @param is the input stream
    void rdstream (InputStream& os) override;
    
    /// reset this slice
    void reset (void) override;
    
    /// @return the pixel format
    virtual Pixel::t_pfmt getpfmt (void) const;

    /// set a slice by buffer and format
    /// @param pbuf the pixel buffer
    /// @param pfmt the pixel format
    virtual void setpixl (const Buffer& pbuf, const Pixel::t_pfmt pfmt);
    
    /// set a slice component by position and pixel
    /// @param x the slice x position
    /// @param y the slice y position
    /// @param pixl the pixel value
    virtual void setpixl (const long x, const long y, const Pixel& pixl) =0;

    /// @return a pixel by position
    virtual Pixel getpixl (const long x, const long y) const =0;

    /// convert this slice to another format
    /// @param pfmt the pixel format
    virtual Slice* convert (const Pixel::t_pfmt pfmt) const =0;
    
  public:
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
