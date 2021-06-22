// ---------------------------------------------------------------------------
// - XhtmlScript.hpp                                                         -
// - afnix:wax service - xhtml script node class definition                  -
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

#ifndef  AFNIX_XHTMLSCRIPT_HPP
#define  AFNIX_XHTMLSCRIPT_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlScript class is a xhtml script node used in the head or body 
  /// node. The script node is built with a xml tag node that holds the 
  /// script content. Sometimes it is recommended to place the script inside
  /// a cdata node that is stored as a child node of the script node. The
  /// node is automatically created when the parse method is called and if
  /// cdata flag is set. By default, the cdata flag is false, making the
  /// script stored inside a text node.
  /// @author amaury darsch

  class XhtmlScript : public XmlTag {
  private:
    /// the xml cdata flag
    bool d_cflg;
    /// the xml data node
    XmlNode* p_data;

  public:
    /// create a xhtml script node with a type
    /// @param type the script type
    XhtmlScript (const String& type);

    /// create a xhtml script node with a type and a cdata flag
    /// @param type the script type
    /// @param cflg the script type
    XhtmlScript (const String& type, const bool cflg);

    /// create a xhtml script node with a type and url
    /// @param type the script type
    /// @param url  the url of the script
    XhtmlScript (const String& type, const String& url);

    /// destroy this xhtml script node
    ~XhtmlScript (void);

    /// @return the class name
    String repr (void) const;

    /// parse a string and add it to a cdata node
    /// @param s the string to parse
    void parse (const String& s);

  private:
    // make the copy constructor private
    XhtmlScript (const XhtmlScript&);
    // make the assignment operator private
    XhtmlScript& operator = (const XhtmlScript&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
