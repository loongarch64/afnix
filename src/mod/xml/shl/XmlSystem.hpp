// ---------------------------------------------------------------------------
// - XmlSystem.hpp                                                           -
// - afnix:xml module - xml system class definition                          -
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

#ifndef  AFNIX_XMLSYSTEM_HPP
#define  AFNIX_XMLSYSTEM_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

namespace afnix {

  /// The XmlSystem class is  an information class as well as utility
  /// procedure container. The class provides also the supported version.
  /// @author amaury darsch

  class XmlSystem {
  public:
    /// the supported version
    enum t_xmlv {
      XML_1_0, // xml version 1.0
      XML_1_1  // xml version 1.1
    };

  public:
    /// @return the default xml system version string
    static String getxvid (void);

    /// @return the default xml system version id
    static t_xmlv getxmlv (void);

    /// @return the default xml system encoding mode
    static String getemod (void);

    /// @return true if the version id is valid
    static bool isxvid (const String& xvid);

    /// convert a string into a version type
    /// @param xvid the string version to convert
    static t_xmlv toxmlv (const String& xvid);

    /// convert a version to a string
    /// @param xmlv the xml version
    static String toxvid (const t_xmlv xmlv);

    /// @return true for a valid standard entity reference
    static bool issref (const String& xref);
    
    /// convert a standard entity reference to a character form
    /// @param eref the the entity reference to convert
    static t_quad tosref (const String& xref);

    /// convert a character to a character entity
    /// @param c the character to convert
    static String tocent (const t_quad c);

    /// format a character to an escaped entity
    /// @param c the character to escape
    static String tocesc (const t_quad c);

    /// format a text string with escaped entities
    /// @param text the text to escape
    static String totesc (const String& text);

    /// format a value string with escaped entities
    /// @param text the text to escape
    static String tovesc (const String& text);

    /// completly escape a string with unicode characters
    /// @param text the text to escape
    static String touesc (const String& text);

    /// prenormalize a text by keeping the eol characters
    /// @param text the text to normalize
    static String prenorm (const String& text);

    /// normalize a text by fixing the spacing characters
    /// @param text the text to normalize
    static String tonorm (const String& text);

    /// convert a property into an attribute string
    /// @param prop the property to convert
    static String toattr (const Property& prop);

    /// convert a property list into an attribute string
    /// @param plst the property list to convert
    static String toattr (const Plist& plst);
  };
}

#endif
