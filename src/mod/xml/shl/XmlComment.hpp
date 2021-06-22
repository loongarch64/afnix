// ---------------------------------------------------------------------------
// - XmlComment.hpp                                                          -
// - afnix:xml module - xml comment node class definition                    -
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

#ifndef  AFNIX_XMLCOMMENT_HPP
#define  AFNIX_XMLCOMMENT_HPP

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlComment class is the comment node. A comment node is directly
  /// built by the xml reader and the content placed into string.
  /// @author amaury darsch

  class XmlComment : public XmlNode {
  protected:
    /// the comment value
    String d_xval;

  public:
    /// create a default comment node
    XmlComment (void);

    /// create a comment node with a string
    /// @param xval the commentstring value 
    XmlComment (const String& xval);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// set the node value
    /// @param xval the value to set
    virtual void setxval (const String& xval);

    /// @return the node value 
    virtual String getxval (void) const;

    /// write a comment node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a comment node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlComment (const XmlComment&);
    // make the assignment operator private
    XmlComment& operator = (const XmlComment&);

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
