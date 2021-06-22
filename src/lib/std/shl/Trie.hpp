// ---------------------------------------------------------------------------
// - Trie.hpp                                                                -
// - standard object library - trie object class definition                  -
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

#ifndef  AFNIX_TRIE_HPP
#define  AFNIX_TRIE_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif
 
namespace afnix {
  
  /// The Trie class is a lexicon tree class designed to store efficently
  /// a collection of names with a set of parameters. The class operates
  /// with unicode characters and is designed to ease the search with
  /// or without diacritics. Each terminal node can store an object that can
  /// be obtained after a succesfull match.
  /// @author amaury darsch

  class Trie : public virtual Object {
  private:
    /// the trie length
    long d_tlen;
    /// the trie tree
    struct s_trie* p_tree;

  public:
    /// create a empty trie
    Trie (void);

    /// destroy this trie
    ~Trie (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this trie
    void reset (void);
    
    /// @return the number of elements
    long length (void) const;

    /// @return the trie total count
    long count (void) const;

    /// @return the trie count by name
    long count (const String& name) const;

    /// @return the trie reference index by name
    long index (const String& name) const;

    /// @retrun true if an entry exists
    bool exists (const String& name) const;

    /// mark a binding in the trie
    /// @param name the name to bind
    void mark (const String& name);

    /// set or create  new binding in the trie
    /// @param name the name to bind
    /// @param wobj the name  object
    void add (const String& name, Object* wobj);

    /// get a trie element by name
    /// @param name the name to search 
    Object* get (const String& name) const;

    /// get a trie element by name or throw an exception
    /// @param name the name to search 
    Object* lookup (const String& name) const;

    /// @return the trie names in a vector
    Strvec* tonames (void) const;

  private:
    // make the copy constructor private
    Trie (const Trie&);
    // make the assignement operator private
    Trie& operator = (const Trie&); 

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
