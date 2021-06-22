// ---------------------------------------------------------------------------
// - XsmNode.hpp                                                             -
// - afnix:xml module - xsm node class definition                            -
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

#ifndef  AFNIX_XSMNODE_HPP
#define  AFNIX_XSMNODE_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

namespace afnix {

  /// The XsmNode is a base class which is part of the xml simple model (xsm).
  /// In this model, a xml (or sgml, or html) text is represented by a node
  /// which can be either a tag, a text or a reference node. There is no
  /// concept of tree. The node content is stored in the form of a text
  /// string. This simple model is designed to parse weak data representation
  /// such like html text and later process it at the user discretion. The
  /// default representation is an empty text node.
  /// @author amaury darsch

  class XsmNode : public virtual Literal {
  public:
    /// the node type
    enum t_xsmt {
      TXT_NODE, // text node
      TAG_NODE, // tag node
      REF_NODE, // reference node
      END_NODE  // end node
    };

    /// the tag subtype
    enum t_xsub {
      TAG_TEXT, // normal tag
      TAG_RESV  // reserved tag
    };

  protected:
    /// the node type
    t_xsmt d_type;
    /// the node subtype
    t_xsub d_tsub;
    /// the node value
    String d_xval;
    /// the line number
    long   d_lnum;
    /// the source name
    String d_snam;

  public:
    /// create an empty node
    XsmNode (void);

    /// create a text node by value
    /// @param xval the node value
    XsmNode (const String& xval);

    /// create a node by type and value
    /// @param type the node type
    /// @param xval the node value
    XsmNode (const t_xsmt type, const String& xval);

    /// copy construct this xsm node
    /// @param that the node to copy
    XsmNode (const XsmNode& that);

    /// @return the class name
    String repr (void) const;
    
    /// @return a clone of this node
    Object* clone (void) const;

    /// clear this node
    void clear (void);

    /// @return the node string value
    String tostring (void) const;

    /// @return the node literal value
    String toliteral (void) const;

    /// assign a node to this one
    /// @param that the node to assign
    XsmNode& operator = (const XsmNode& that);

    /// @return the node type
    virtual t_xsmt gettype (void) const;

    /// @return the node subtype
    virtual t_xsub gettsub (void) const;

    /// set the source line snum
    /// @param lnum the line number to set
    virtual void setlnum (const long lnum);

    /// @return the source line number
    virtual long getlnum (void) const;

    /// set the node source name
    /// @param snam the source name to set
    virtual void setsnam (const String& snam);

    /// @return the source name
    virtual String getsnam (void) const;

    /// @return true if the node is a text node
    virtual bool istext (void) const;

    /// @return true if the node is a tag
    virtual bool istag (void) const;

    /// @return true if the node is a normal tag
    virtual bool isntag (void) const;

    /// @return true if the node is reserved
    virtual bool isresv (void) const;

    /// @return true if the node is text value node
    virtual bool isxval (void) const;

    /// @return true if the node is a reference node
    virtual bool isref (void) const;

    /// @return true if the node is a end node
    virtual bool isend (void) const;

    /// @return the tag node name
    virtual String getname (void) const;

    /// get the node name by case flag
    /// @param lwcf the lower case flag
    virtual String getname (const bool lwcf) const;

    /// get an attribute by name
    /// @param name the attribute name
    virtual Property* getattr (const String& name) const;

    /// get an attribute by name and flag
    /// @param name the attribute name
    /// @param lwcf he lower case flag
    virtual Property* getattr (const String& name, const bool lwcf) const;

    /// @return the tag attribute list
    virtual Plist getattr (void) const;

    /// get the tag attribute list aby case flag
    /// @param lwcf the lower case flag
    virtual Plist getattr (const bool lwcf) const;

    /// @return a vector of words from a text node
    virtual Strvec getwords (void) const;

  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
