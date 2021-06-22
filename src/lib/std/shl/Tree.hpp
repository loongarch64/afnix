// ---------------------------------------------------------------------------
// - Tree.hpp                                                                -
// - standard object library - binary object class definition                -
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

#ifndef  AFNIX_TREE_HPP
#define  AFNIX_TREE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif
 
namespace afnix {
  
  /// The Tree class is an unbalanced binary tree which operates with a key
  /// and an object. The key is used to order and serach the tree.
  /// @author amaury darsch

  class Tree : public virtual Object {
  protected:
    /// the root node
    struct s_node* p_root;

  public:
    /// create a empty tree
    Tree (void);

    /// destroy this tree
    ~Tree (void);

    /// @return the class name
    String repr (void) const;

    /// reset this tree
    virtual void reset (void);
    
    /// @return the number of elements
    virtual long length (void) const;

    /// @retrun true if a node index exists
    virtual bool exists (const long nidx) const;

    /// add an object by node index
    /// @param nidx the node index
    /// @param nobj the node object
    virtual void add (const long nidx, Object* nobj);

    /// find an object by node index
    /// @param nidx the node index    
    virtual Object* find (const long nidx) const;
    
  private:
    // make the copy constructor private
    Tree (const Tree&) =delete;
    // make the assignement operator private
    Tree& operator = (const Tree&) =delete; 

  public:
    /// create a new object in a generic way
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
