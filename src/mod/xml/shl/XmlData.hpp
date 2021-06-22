// ---------------------------------------------------------------------------
// - XmlData.hpp                                                             -
// - afnix:xml module - xml cdata node class definition                      -
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

#ifndef  AFNIX_XMLDATA_HPP
#define  AFNIX_XMLDATA_HPP

#ifndef  AFNIX_XMLNODE_HPP
#include "XmlNode.hpp"
#endif

namespace afnix {

  /// The XmlData class is the cdata node. The cdata node distinguishes 
  /// itself by accepting characters that can acts as markup delimiters
  /// @author amaury darsch

  class XmlData : public XmlNode {
  protected:
    /// the xml data value
    String d_xval;

  public:
    /// create a default cdata node
    XmlData (void);

    /// create a cdata node with a value
    /// @param xval the cdata string value 
    XmlData (const String& xval);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this node
    Object* clone (void) const;

    /// set the node value
    /// @param xval the value to set
    virtual void setxval (const String& xval);

    /// @return the node value 
    virtual String getxval (void) const;

    /// parse a string and add the result to the node
    /// @param s the string to parse
    void parse (const String& s);

    /// @return the node tree content as a text
    String totext (void) const;

    /// write a cdata node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;

    /// write a cdata node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    XmlData (const XmlData&);
    // make the assignment operator private
    XmlData& operator = (const XmlData&);

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
