// ---------------------------------------------------------------------------
// - XsmBuffer.hpp                                                           -
// - afnix:xml module - xsm character buffer class definition                -
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

#ifndef  AFNIX_XSMBUFFER_HPP
#define  AFNIX_XSMBUFFER_HPP

#ifndef  AFNIX_PROPERTY_HPP
#include "Property.hpp"
#endif

#ifndef  AFNIX_XSOBUFFER_HPP
#include "XsoBuffer.hpp"
#endif

namespace afnix {

  /// The XsmBuffer class is a character buffer used to accumulate
  /// characters during the xsm operations. The buffer provides the
  /// extensions to the xso buffer by being able to strip and extract
  /// field.
  /// @author amaury darsch

  class XsmBuffer : public XsoBuffer {
  public:
    /// create a new buffer
    XsmBuffer (void);

    /// create a new buffer by string
    /// @param xval the initial string
    XsmBuffer (const String& xval);

    /// copy construct this buffer
    /// @param that the buffer to copy
    XsmBuffer (const XsmBuffer& that);

    /// assign a buffer to this one
    /// @param that the buffer to assign
    XsmBuffer& operator = (const XsmBuffer& that);

    /// @return true if the buffer is empty after left striping
    bool isnext (void);

    /// @return true if the character is a space
    bool isspc (const t_quad c) const;

    /// strip the buffer with leading blank
    void stripl (void);

    /// strip the buffer with trailing blank
    void stripr (void);

    /// strip the buffer in the middle
    void stripm (void);

    /// strip the buffer with leading and trailing blank
    void strip (void);

    /// @return the next available string in this buffer
    String getnstr (void);

    /// @return the next available word
    String getword (void);

    /// @return the next available attribute
    Property getattr (void);
  };
}

#endif
