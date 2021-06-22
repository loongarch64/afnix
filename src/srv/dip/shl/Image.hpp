// ---------------------------------------------------------------------------
// - Image.hpp                                                              -
// - afnix:dip service - image class definition                            -
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

#ifndef  AFNIX_IMAGE_HPP
#define  AFNIX_IMAGE_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

#ifndef  AFNIX_VIEWABLE_HPP
#include "Viewable.hpp"
#endif

namespace afnix {
  
  /// The Image class is a base class for single plane or multispectral image.
  /// At this stage, only the image geometry is defined.
  /// @author amaury darsch

  class Image : public virtual Serial, public Viewable {
  protected:
    /// the image width
    long d_wdth;
    /// the image height
    long d_hght;

  public:
    /// create a default image
    Image (void);

    /// copy construct this image
    /// @param that the image to copy
    Image (const Image& that);

    /// copy move this image
    /// @param that the image to move
    Image (Image&& that) noexcept;

    /// assign an image to this one
    /// @param that the image to assign
    Image& operator = (const Image& that);

    /// move an image to this one
    /// @param that the image to move
    Image& operator = (Image&& that) noexcept;

    // serialize this image
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this image
    /// @param is the input stream
    void rdstream (InputStream& os) override;
    
    /// reset this image
    virtual void reset (void);

    /// clear this image
    virtual void clear (void) =0;
    
    /// @return the image width
    virtual long getwdth (void) const;
    
    /// @return the image height
    virtual long gethght (void) const;

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
