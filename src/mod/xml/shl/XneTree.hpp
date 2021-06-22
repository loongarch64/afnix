// ---------------------------------------------------------------------------
// - XneTree.hpp                                                             -
// - afnix:xml module - xne tree functions class definition                  -
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

#ifndef  AFNIX_XNETREE_HPP
#define  AFNIX_XNETREE_HPP

#ifndef  AFNIX_XNECOND_HPP
#include "XneCond.hpp"
#endif

namespace afnix {

  /// The XneTree class is the xne tree manipulation class. The class operates
  /// with a node and provides numerous methods to manipulate the tree as well
  /// as changing it. Before a tree is manipulated, it is recommended to make
  /// a copy of such tree with the help of the node "copy" method.
  /// @author amaury darsch

  class XneTree : public Object {
  private:
    /// the node tree
    XmlNode* p_node;

  public:
    /// create a default tree
    XneTree (void);

    /// create a tree by node
    /// @param node the tree node
    XneTree (XmlNode* node);

    /// destroy this tree
    ~XneTree (void) ;

    /// @return the class name
    String repr (void) const;

    /// attach a node to the tree
    /// @param node the node to attach
    void setnode (XmlNode* node);

    /// @return the node attached to the tree
    XmlNode* getnode (void) const;

    /// @return the depth of the tree
    long depth (void) const;

    /// @return the size of the tree
    long size (void) const;

    /// generate a unique id for all node in the tree
    void genid (void);

    /// set all tree node with an attribute
    /// @param name the attribute name
    /// @param lval the attribute value
    void setattr (const String& name, const Literal& lval);

    /// set all tree node with an attribute by tag name
    /// @param name the attribute name
    /// @param lval the attribute value
    /// @param tnam the tag name to match
    void setattr (const String& name, const Literal& lval, const String& tnam);

    /// clear the attributes of the nodes tree
    void clrattr (void);

    /// clear the attributes of the nodes tree by name
    /// @param name the node name to select
    void clrattr (const String& name);

    /// set all tree node prefix
    /// @param pnam the prefix to set
    void setpfix (const String& pnam);

    /// clear all tree node prefix
    void clrpfix (void);

    /// normalize all tree nodes
    void normalize (void);

    /// select the nodes that match a condition
    /// @param cond the condition to check
    /// @param hflh the hierarchy flag
    Vector* select (const XneCond& cond, const bool hflg) const;

  private:
    // make the copy constructor private
    XneTree (const XneTree&);
    // make the assignment operator private
    XneTree& operator = (const XneTree&);

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
