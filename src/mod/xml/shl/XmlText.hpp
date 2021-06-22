// ---------------------------------------------------------------------------
// - XmlText.hpp                                                             -
// - afnix:xml module - xml text node class definition                       -
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

#ifndef  AFNIX_XMLTEXT_HPP
#define  AFNIX_XMLTEXT_HPP

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlText class is the text node. A text node is directly
  /// built by the xml reader and the content placed into a string.
  /// By definition, a text node is an empty node.
  /// @author amaury darsch

  class XmlText : public XmlNode {
  protected:
    /// the xml text value
    String d_xval;

  public:
    /// create a default text node
    XmlText (void);

    /// create a text node with a value
    /// @param xval the text string value 
    XmlText (const String& xval);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// set the node value
    /// @param xval the value to set
    virtual void setxval (const String& xval);

    /// @return the node value 
    virtual String getxval (void) const;

    /// @return the node text value
    String totext (void) const;

    /// @return the pre-normalized node text value
    String prenorm (void) const;

    /// @return the normalized node text value
    String tonorm (void) const;

    /// normalize this node
    void normalize (void);

    /// write a text node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a text node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlText (const XmlText&);
    // make the assignment operator private
    XmlText& operator = (const XmlText&);

  public:
    /// create an object in a generic way
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
