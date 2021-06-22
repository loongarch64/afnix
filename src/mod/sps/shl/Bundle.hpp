// ---------------------------------------------------------------------------
// - Bundle.hpp                                                              -
// - afnix:sps module - literal bundle class definition                      -
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

#ifndef  AFNIX_BUNDLE_HPP
#define  AFNIX_BUNDLE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

#ifndef  AFNIX_ITERABLE_HPP
#include "Iterable.hpp"
#endif

namespace afnix {

  /// The Bundle class implements a literal bundle facility which is optimized
  /// for a set of standard literals, namely the boolean, integer and real
  /// types. This implementation is therefore faster and designed to store
  /// larger dataset as compared with a vector implementation. As a literal,
  /// the implementation is designed to operate with a resolved value pointed
  /// by a bundle index. The bundle is primarily used as a cell literal
  /// those current value is pointed by an internal literal index.
  /// @author amaury darsch

  class Bundle : public Iterable, public Literal {
  public:
    enum t_bndl {
      BNDL_BOOL, // boolean type
      BNDL_INTG, // integer type
      BNDL_REAL, // real type
      BNDL_OTHR  // other type
    };
  protected:
    /// the bundle size
    long   d_size;
    /// the bundle length
    long   d_slen;
    /// the literal index
    long   d_lidx;
    /// the bundle type
    t_bndl d_type;
    /// the allocated bundle
    union {
      bool*     p_bstk;
      long*     p_istk;
      t_real*   p_rstk;
      Literal** p_lstk;
    };

  public:
    /// create an empty bundle.
    Bundle (void);

    /// create a literal bundle by size
    /// @param size the requested size  
    Bundle (const long size);
  
    /// copy constructor for this bundle
    /// @param that the bundle to copy
    Bundle (const Bundle& that);

    /// destroy this literal bundle
    ~Bundle (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// clear this literal bundle
    void clear (void);

    /// @return a literal representation of this literal
    String toliteral (void) const;

    /// @return a string representation of this literal
    String tostring (void) const;

    /// @return a styled string of this literal
    String format (const Style& lstl) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this bundle to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a bundle from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// assign a bundle to this one
    /// @param that the bundle to assign
    Bundle& operator = (const Bundle& that);

    /// @return true if the bundle is empty
    virtual bool empty (void) const;

    /// @return the number of elements
    virtual long length (void) const;

    /// resize this literal bundle
    virtual void resize (const long size);

    /// set the literal index
    /// @param lidx the index to set
    virtual void setlidx (const long lidx);

    /// @return the literal index
    virtual long getlidx (void) const;

    /// add a boolean to this bundle
    /// @param bval the boolean to add
    virtual void add (const bool bval);

    /// add an integer to this bundle
    /// @param ival the integer to add
    virtual void add (const long ival);

    /// add a real to this bundle
    /// @param ival the integer to add
    virtual void add (const t_real rval);

    /// add a literal to this bundle
    /// @param lobj the literal object to add
    virtual void add (Literal* lobj);

    /// set a literal at a given position
    /// @param index the bundle index
    /// @param lobj  the literal object
    virtual void set (const long index, Literal* lobj);

    /// get a literal at a certain index
    /// @param index the bundle index
    virtual Literal* get (const long index) const;

    /// @return the first literal in this bundle
    virtual Literal* first (void) const;

    /// @return the last literal in this bundle
    virtual Literal* last (void) const;
    
    /// @return a new iterator for this bundle
    Iterator* makeit (void);

  private:
    // make the literal bundle iterator a friend
    friend class Bundleit;

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

  /// The Bundleit class is the iterator for the bundle class. Such 
  /// iterator is constructed with the "makeit" bundle method. The iterator
  /// is reset to the beginning of the bundle.
  /// @author amaury darsch

  class Bundleit : public Iterator {
  private:
    /// the bundle to iterate
    Bundle* p_sobj;
    /// the iterable bundle index
    long    d_sidx;

  public:
    /// create a new iterator from a literal bundle
    /// @param vobj the bundle to iterate
    Bundleit (Bundle* vobj);

    /// destroy this bundle iterator
    ~Bundleit (void);

    /// @return the class name
    String repr (void) const;

    /// reset the iterator to the begining
    void begin (void);

    /// reset the iterator to the end
    void end (void);

    /// move the bundle iterator to the next position
    void next (void);

    /// move the bundle iterator to the previous position
    void prev (void);

    /// @return the object at the current position
    Object* getobj (void) const;

    /// @return true if the iterator is at the end
    bool isend (void) const;

  private:
    // make the copy constructor private
    Bundleit (const Bundleit&);
    // make the assignment operator private
    Bundleit& operator = (const Bundleit&);
  };
}

#endif
