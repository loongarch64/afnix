// ---------------------------------------------------------------------------
// - Mixmap.hpp                                                              -
// - afnix:dip service - mixmap class definition                             -
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

#ifndef  AFNIX_MIXMAP_HPP
#define  AFNIX_MIXMAP_HPP

#ifndef  AFNIX_TRANCHE_HPP
#include "Tranche.hpp"
#endif

namespace afnix {
  
  /// The Mixmap class is a tranche image stored as a continuous pixel data
  /// block. The mixmap is designed to store multi/hyper spectral image
  /// with a (optionally) a reference image and subsequent band images. For
  /// multi-component pixel format, the band format is automatically derived
  /// by the implementation. Note that a multi-component pixel format
  /// enable the storage of only one reference image with this format
  /// at depth index 0.
  /// @author amaury darsch

  class Mixmap : public Tranche {
  protected:
    /// the reference stride
    long d_rtrd;
    /// the band stride
    long d_btrd;
    /// the mixmap data size
    long d_size;
    /// the mixmap data
    t_byte* p_data;
    
  public:
    /// create a default mixmap
    Mixmap (void);

    /// create a mixmap by format and geometry
    /// @param pfmt the mixmap format
    /// @param wdth the mixmap width
    /// @param hght the mixmap height
    /// @param dpth the mixmap depth
    Mixmap (const Pixel::t_pfmt pfmt, const long wdth, const long hght,
	    const long dpth);

    /// copy construct this mixmap
    /// @param that the mixmap to copy
    Mixmap (const Mixmap& that);

    /// copy move this mixmap
    /// @param that the mixmap to move
    Mixmap (Mixmap&& that) noexcept;

    /// destroy this mixmap
    ~Mixmap (void);
    
    /// assign an mixmap to this one
    /// @param that the mixmap to assign
    Mixmap& operator = (const Mixmap& that);

    /// move an mixmap to this one
    /// @param that the mixmap to move
    Mixmap& operator = (Mixmap&& that) noexcept;

    /// @return the class name
    String repr (void) const;
    
    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// serialize this mixmap
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this mixmap
    /// @param is the input stream
    void rdstream (InputStream& os);
    
    /// reset this mixmap
    void reset (void);

    /// clear this mixmap
    void clear (void);

    /// set an image component by position and pixel
    /// @param x the image x position
    /// @param y the image y position
    /// @param pixl the pixel value
    void setpixl (const long x, const long y, const long d,
		  const Pixel& pixl);

    /// @return a pixel by position
    Pixel getpixl (const long x, const long y, const long d) const;
    
    /// @return the mixmap byte size
    long tosize (void) const;

    /// @return the mixmap byte data
    t_byte* tobyte (void);

    /// @return the mixmap byte data
    const t_byte* tobyte (void) const;
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
