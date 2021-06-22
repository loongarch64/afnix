// ---------------------------------------------------------------------------
// - XmlBuffer.hpp                                                           -
// - afnix:xml module - xml character buffer class definition                -
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

#ifndef  AFNIX_XMLBUFFER_HPP
#define  AFNIX_XMLBUFFER_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

#ifndef  AFNIX_XSOBUFFER_HPP
#include "XsoBuffer.hpp"
#endif

#ifndef  AFNIX_XMLSYSTEM_HPP
#include "XmlSystem.hpp"
#endif

namespace afnix {

  /// The XmlBuffer class is a character buffer used to accumulate
  /// characters during the xml parsing operation. The buffer provides the
  /// extensions to the xso buffer by beeing able to strip and extract
  /// field. The buffer also provides the verification in accordance to
  /// the xml version.
  /// @author amaury darsch

  class XmlBuffer : public XsoBuffer {
  private:
    ///the preserve space flag
    bool d_prsv;
    /// the xml version
    XmlSystem::t_xmlv d_xmlv;

  public:
    /// create a new buffer
    XmlBuffer (void);

    /// create a new buffer by version
    /// @param xmlv the xml version
    XmlBuffer (const XmlSystem::t_xmlv xmlv);

    /// create a new buffer by version and string
    /// @param xmlv the xml version
    /// @param xval the initial string
    XmlBuffer (const XmlSystem::t_xmlv xmlv, const String& xval);

    /// copy construct this buffer
    /// @param that the buffer to copy
    XmlBuffer (const XmlBuffer& that);

    /// assign a buffer to this one
    /// @param that the buffer to assign
    XmlBuffer& operator = (const XmlBuffer& that);

    /// reset this buffer 
    void reset (void);

    /// @return the xml buffer version id
    String getxvid (void) const;

    /// create a new buffer with the same properties
    XmlBuffer dup (void) const;

    /// @return true if the buffer is empty after left striping
    bool isnext (void);

    /// @return true if the buffer has a quoted string starter
    bool isqstr (void);

    /// @return true if the buffer has a subset declaration starter
    bool issubs (void);

    /// set the preserve space flag
    /// @param prsv the preserve flag to set
    void setprsv (const bool prsv);

    /// @return the preserve space flag
    bool getprsv (void) const;

    /// set the xml version
    /// @param xvid the xml version id to set
    void setxmlv (const String& xvid);

    /// @return true if the character is a space
    bool isspc (const t_quad c) const;

    /// @return true if the character can be skipped
    bool isskip (const t_quad c) const;

    /// strip the buffer with leading blank
    void stripl (void);

    /// strip the buffer with trailing blank
    void stripr (void);

    /// strip the buffer with leading and trailing blank
    void strip (void);

    /// @return the buffer contents as a text
    String totext (void) const;

    /// @return the buffer contents as a string
    String tostring (void) const;

    /// @return the buffer content as a name
    String toname (void) const;

    /// @return the buffer content as a name or a pe reference
    String topnam (void) const;

    /// @return the buffer content as a character reference
    t_quad tocref (void) const;

    /// @return the next available string in this buffer
    String getnstr (void);

    /// @return the next available name in this buffer
    String getname (void);

    /// @return the next available name or pe reference in this buffer
    String getpnam (void);

    /// @return the next available quoted string in this buffer
    String getqstr (void);

    /// @return the next available enumeration value
    String getnenm (void);

    /// @return the next available enumeration
    String getenum (void);

    /// @return the next available enumeration name
    String getenam (void);

    /// @return the attribute default value
    String getxdef (void);

    /// @return a subset delaration string
    String getsubs (void);

    /// @return the next available attribute
    Property getattr (void);

    /// @return true if the string is an enumeration
    bool isenum (const String& xval);

    /// return a string vector from an enumeration
    Strvec getxenm (const String& xval);
  };
}

#endif
