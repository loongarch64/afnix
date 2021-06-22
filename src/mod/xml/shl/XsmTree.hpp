// ---------------------------------------------------------------------------
// - XsmTree.hpp                                                             -
// - afnix:xml module - xsm tree class definition                            -
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

#ifndef  AFNIX_XSMTREE_HPP
#define  AFNIX_XSMTREE_HPP

#ifndef  AFNIX_XSOINFO_HPP
#include "XsoInfo.hpp"
#endif

#ifndef  AFNIX_XSMNODE_HPP
#include "XsmNode.hpp"
#endif

namespace afnix {

  /// The XsmTree class is a base class that maintains a list of nodes.
  /// Generally the class is constructed from a derived class and the nodes
  /// are stored in a vector which can be access by index.
  /// @author amaury darsch

  class XsmTree : public Nameable {
  protected:
    /// the document name
    String   d_name;
    /// the node tree
    Vector*  p_tree;

  public:
    /// create a default xsm tree
    XsmTree (void);

    /// destroy this xsm tree
    ~XsmTree (void);

    /// @return the class name
    String repr (void) const;

    /// @return the xsm tree name
    String getname (void) const;

    /// set the xsm tree name
    /// @param name the name to set
    virtual void setname (const String& name);

    /// @return the number of nodes
    virtual long length (void) const;

    /// @return a node by index
    virtual XsmNode* getnode (const long index) const;

    /// @return an info object by index
    virtual XsoInfo* getinfo (const long index) const;

    /// @return an info object by index and case flag
    virtual XsoInfo* getinfo (const long index, const bool lwcf) const;

    /// @return an info vector by name
    virtual Vector* getivec (const String& name) const;

    /// @return an info vector by name and case flag
    virtual Vector* getivec (const String& name, const bool lwcf) const;

    /// @return a vector of words
    virtual Vector* getwords (void) const;

  private:
    // make the copy constructor private
    XsmTree (const XsmTree&);
    // make the assignment operator private
    XsmTree& operator = (const XsmTree&);

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
