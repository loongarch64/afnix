// ---------------------------------------------------------------------------
// - Pixmap.hpp                                                              -
// - afnix:dip service - pixmap class definition                             -
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

#ifndef  AFNIX_PIXMAP_HPP
#define  AFNIX_PIXMAP_HPP

#ifndef  AFNIX_SLICE_HPP
#include "Slice.hpp"
#endif

namespace afnix {
  
  /// The Pixmap class is a pixel block image stored as a continuous pixel
  /// stride. The pixmap structure follows the standard implementation with
  /// a line stride directly computed from the image width.
  /// @author amaury darsch

  class Pixmap : public Slice {
  protected:
    /// the line stride
    long d_strd;
    /// the pixmap data size
    long d_size;
    /// the pixmap data
    t_byte* p_data;
    
  public:
    /// create a default pixmap
    Pixmap (void);

    /// create a pixmap by format and geometry
    /// @param pfmt the pixmap format
    /// @param wdth the pixmap width
    /// @param hght the pixmap height
    Pixmap (const Pixel::t_pfmt pfmt, const long wdth, const long hght);

    /// copy construct this pixmap
    /// @param that the pixmap to copy
    Pixmap (const Pixmap& that);

    /// copy move this pixmap
    /// @param that the pixmap to move
    Pixmap (Pixmap&& that) noexcept;

    /// destroy this pixmap
    ~Pixmap (void);
    
    /// assign an pixmap to this one
    /// @param that the pixmap to assign
    Pixmap& operator = (const Pixmap& that);

    /// move an pixmap to this one
    /// @param that the pixmap to move
    Pixmap& operator = (Pixmap&& that) noexcept;

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

    /// reset this pixmap
    void reset (void) override;

    /// clear this pixmap
    void clear (void) override;

    /// set a pixmap by buffer and format
    /// @param pbuf the pixel buffer
    /// @param pfmt the pixel format
    void setpixl (const Buffer& pbuf, const Pixel::t_pfmt pfmt) override;
    
    /// set an image component by position and pixel
    /// @param x the image x position
    /// @param y the image y position
    /// @param pixl the pixel value
    void setpixl (const long x, const long y, const Pixel& pixl) override;

    /// @return a pixel by position
    Pixel getpixl (const long x, const long y) const override;

    /// convert this pixmap to another format
    /// @param pfmt the pixel format
    Slice* convert (const Pixel::t_pfmt pfmt) const override;
    
    /// @return the pixmap byte size
    long tosize (void) const override;

    /// @return the pixmap byte data
    t_byte* tobyte (void) override;

    /// @return the pixmap byte data
    const t_byte* tobyte (void) const override;
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
