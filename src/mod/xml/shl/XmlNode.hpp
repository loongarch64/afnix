// ---------------------------------------------------------------------------
// - XmlNode.hpp                                                             -
// - afnix:xml module - xml base node class definition                       -
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

#ifndef  AFNIX_XMLNODE_HPP
#define  AFNIX_XMLNODE_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef  AFNIX_COLLECTABLE_HPP
#include "Collectable.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The XmlNode class is the base class used to represent the xml tree.
  /// The tree is built as a double linked list of nodes. Each node owns
  /// as well its parent node. Walking in the tree is achieved by taking
  /// the child node and then moving to the child and/or next node. The
  /// node also manages an empty flags. It the empty flag is set, it is
  /// an error to add child nodes.
  /// @author amaury darsch

  class XmlNode : public Collectable {
  protected:
    /// the confine pointer
    void*   p_cptr;
    /// the empty flag
    bool     d_eflg;
    /// the eol flag
    bool     d_eolf;
    /// the line number
    long     d_lnum;
    /// the source name
    String   d_snam;
    /// the parent node
    XmlNode* p_pnod;
    /// the child nodes
    Vector   d_chld;

  public:
    /// create an empty node
    XmlNode (void);

    /// create a node with an empty flag
    /// @param eflg the node empty flag
    XmlNode (const bool eflg);

    /// destroy this node
    ~XmlNode (void);

    /// @return the class name
    String repr (void) const;

    /// release this object
    void release (void);

    /// @return the empty flag
    virtual bool geteflg (void) const;

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

    /// @return true if the node name is valid
    virtual bool isname (const String& name) const;

    /// @return true if the node index is valid
    virtual bool isnidx (const long nidx) const;

    /// @return true if the node attribute exists
    virtual bool isattr (const String& name) const;

    /// @return true if the node attribute is valid
    virtual bool isattr (const String& name, const String& pval) const;

    /// @return the parent node
    virtual XmlNode* getparent (void) const;

    /// set the parent node
    /// @param node the parent node to set
    virtual void setparent (XmlNode* node);

    /// @return a copy of the node tree
    virtual XmlNode* copy (void) const;

    /// @return true if there is no children
    virtual bool nilchild (void) const;

    /// @return true if a child exists by name
    virtual bool ischild (const String& name) const;

    /// @return true if a child exists by name with an attribute
    virtual bool ischild (const String& name, const String& anam) const;

    /// @return true if a child exists by name with an attribute name/value
    virtual bool ischild (const String& name, 
			  const String& anam, const String& pval) const;

    /// @return true if a child exists by attribute name
    virtual bool isachild (const String& anam) const;

    /// @return true if a child exists by attribute name
    virtual bool isachild (const String& anam, const String& pval) const;

    /// @return the number of children nodes
    virtual long lenchild (void) const;

    /// @return the number of children nodes by name
    virtual long lenchild (const String& name) const;

    /// add a child node to this node
    /// @param node the node to add
    virtual void addchild (XmlNode* node);

    /// add a child node to this node at a certain position
    /// @param node the node to add
    /// @param nidx the node index to use
    virtual void addchild (XmlNode* node, const long nidx);

    /// set a child node to this node at a certain position
    /// @param node the node to set
    /// @param nidx the node index to use
    virtual void setchild (XmlNode* node, const long nidx);

    /// @return a child node by index
    virtual XmlNode* getchild (const long index) const;

    /// @return a child node by name
    virtual XmlNode* getchild (const String& name) const;

    /// @return a child node by name or throw an exception
    virtual XmlNode* lookchild (const String& name) const;

    /// remove a child node by index
    /// @param nidx the node index to remove
    virtual void delchild (const long index);

    /// remove a child node by name
    /// @param name the child name to remove
    virtual void delchild (const String& name);

    /// remove a child node by name and attribute
    /// @param name the child name to remove
    /// @param anam the attribute name
    virtual void delchild (const String& name, const String& anam);

    /// remove a child node by name, attribute name and value
    /// @param name the child name to remove
    /// @param anam the attribute name
    /// @param pval the attribute value
    virtual void delchild (const String& name, const String& anam,
			   const String& pval);

    /// remove a child node by attribute name
    /// @param anam the attribute name
    virtual void delachild (const String& anam);

    /// remove a child node by attribute name and value
    /// @param anam the attribute name
    /// @param pval the attribute value
    virtual void delachild (const String& anam, const String& pval);

    /// clear the children node list
    virtual void clrchild (void);

    /// find a child index by node
    /// @param node the child node to find
    virtual long getnidx (const XmlNode* node) const;

    /// merge a node at a certain index
    /// @param snod the source node to merge
    /// @param nidx the node index to merge
    virtual void merge (const XmlNode* snod, const long nidx);

    /// parse a string and add the result as child nodes
    /// @param s the string to parse
    virtual void parse (const String& s);

    /// @return the node tree content as a text
    virtual String totext (void) const;

    /// normalize this node and its children
    virtual void normalize (void);

    /// replace a node with another one
    /// @param rnod the reference node to replace
    /// @param snos the source node to use
    virtual bool replace (XmlNode* rnod, XmlNode* snod);

    /// write a node into a buffer
    /// @param buf the buffer to write
    virtual void write (Buffer& buf) const =0;

    /// write a node into an output stream
    /// @param os the output stream to write
    virtual void write (OutputStream& os) const =0;

  private:
    // make the copy constructor private
    XmlNode (const XmlNode&) =delete;
    // make the assignment operator private
    XmlNode& operator = (const XmlNode&) =delete;

  public:
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
