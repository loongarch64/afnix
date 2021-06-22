// ---------------------------------------------------------------------------
// - XhtmlRoot.hpp                                                           -
// - afnix:wax service - xhtml root node class definition                    -
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

#ifndef  AFNIX_XHTMLROOT_HPP
#define  AFNIX_XHTMLROOT_HPP

#ifndef  AFNIX_MIME_HPP
#include "Mime.hpp"
#endif

#ifndef  AFNIX_XMLROOT_HPP
#include "XmlRoot.hpp"
#endif

#ifndef  AFNIX_XHTMLHTML_HPP
#include "XhtmlHtml.hpp"
#endif

namespace afnix {

  /// The XhtmlRoot class is a xml root node used for the design of a xhtml
  /// document page. At construction, the root node is initialized with a 
  /// default xml processing instruction, and hmtl node with head and body
  /// The head and body nodes can be used to add more nodes in order to 
  /// build the document.
  /// @author amaury darsch

  class XhtmlRoot : public XmlRoot, public Mime {
  public:
    /// create a xhtml root node with a head title
    /// @param title the head node title
    XhtmlRoot (const String& title);

    /// @return the class name
    String repr (void) const;

    /// @return the xhtml html node
    XhtmlHtml* gethtml (void) const;

    /// @return the xhtml head node
    XhtmlHead* gethead (void) const;

    /// @return the xhtml body node
    XhtmlBody* getbody (void) const;

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;
    
    /// write a node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XhtmlRoot (const XhtmlRoot&);
    // make the assignment operator private
    XhtmlRoot& operator = (const XhtmlRoot&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

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
