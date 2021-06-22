// ---------------------------------------------------------------------------
// - Heap.hpp                                                                -
// - standard object library - heap class definition                         -
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

#ifndef  AFNIX_HEAP_HPP
#define  AFNIX_HEAP_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Heap class is an object based heap class that organises object with
  /// respect to a key. The heap is organised as a binary tree those root
  /// element is either the object with the highest or the lowest key. A flag
  /// controls whether the heap is operating in ascending or descending mode.
  /// By default, the heap operates in ascending mode, which means that the
  /// root node is the lowest one. The heap is self-resizable.
  /// @author amaury darsch

  class Heap : public virtual Object {
  private:
    /// the heap size
    long   d_size;
    /// the heap length
    long   d_hlen;
    /// the heap mode
    bool   d_mode;
    /// the min key flag
    bool   d_minf;
    /// the min key value
    t_long d_mink;
    /// the max key flag
    bool   d_maxf;
    /// the max key value
    t_long d_maxk;
    /// the heap array
    struct s_heap* p_heap;

  public:
    /// create a default heap
    Heap (void);
  
    /// create a heap by size
    /// @param size the heap size
    Heap (const long size);

    /// create a heap by mode
    /// @param mode the heap mode
    Heap (const bool mode);

    /// create a heap by size and mode
    /// @param size the heap size
    /// @param mode the heap mode
    Heap (const long size, const bool mode);

    /// destroy this heap
    ~Heap (void);

    /// return the class name
    String repr (void) const override;

    /// reset this heap
    void reset (void);

    /// set the heap mode 
    /// @para mode the mode to set
    void setmode (const bool mode);

    /// @return the heap mode
    bool getmode (void) const;

    /// @return true if the min flag is set
    bool ismink (void) const;

    /// @return true if the max flag is set
    bool ismaxk (void) const;

    /// reset the min key flag
    void rstmink (void);

    /// reset the max key flag
    void rstmaxk (void);

    /// set the min key
    /// @param mink the minimum key to set
    void setmink (const t_long mink);

    /// @return the min key value
    t_long getmink (void) const;

    /// set the max key
    /// @param maxk the maxium key to set
    void setmaxk (const t_long maxk);

    /// @return the max key value
    t_long getmaxk (void) const;

    /// @return the heap length
    long length (void) const;

    /// @return true if the heap is empty
    bool empty (void) const;

    /// add an object by key
    /// @param key the object key
    /// @param obj the object to add
    void add (const t_long key, Object* obj);

    /// @return the root object
    Object* pop (void);

    // @return the root key
    t_long getkey (void) const;

    /// @return the key by index
    t_long getkey (const long index) const;

    /// @return hthe object by index
    Object* getobj (const long index) const;

    /// resize this heap
    /// @param size the new heap size
    void resize (const long size);

  private:
    // make the copy constructor private
    Heap (const Heap&);
    // make the assignment operator private
    Heap& operator = (const Heap&);    
    // balance the help
    void balance (void);

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
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
