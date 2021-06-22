// ---------------------------------------------------------------------------
// - Cons.hpp                                                                -
// - standard object library - cons cell class definition                    -
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

#ifndef  AFNIX_CONS_HPP
#define  AFNIX_CONS_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

#ifndef  AFNIX_MONITOR_HPP
#include "Monitor.hpp"
#endif

#ifndef  AFNIX_ITERABLE_HPP
#include "Iterable.hpp"
#endif

#ifndef  AFNIX_COLLECTABLE_HPP
#include "Collectable.hpp"
#endif

namespace afnix {

  /// The Cons class is the class used to create generic forms. A cons
  /// cell consists of a car and a cdr. The car holds an object while the cdr
  /// is the link to the next cell. When a cons cell is destroyed, the car and
  /// the cdr are destroyed as well. A cons cell is either a normal cell or
  /// a block cell. In the case of block cell, the car hold the cons cell 
  /// which is a list of forms.
  /// @author amaury darsch

  class Cons : public virtual Serial, public Iterable, public Collectable {
  public:
    /// the cons cell type
    enum t_cctp : t_byte
      {
       CCTP_NORM = 0x00U, // normal form
       CCTP_BLOK = 0x01U  // block  form
      };
    
  protected:
    /// the cons cell type
    t_cctp  d_cctp;
    /// the car holds the object
    Object* p_car;
    /// the cdr is always a cons
    Cons*   p_cdr;
    /// the synchronize monitor
    Monitor* p_mon;

  public:
    /// create a new cons cell with a null car and cdr
    Cons (void);

    /// create a new cons cell with a type
    /// @param cctp the cons cell type
    Cons (const t_cctp cctp);

    /// create a cons cell with a car
    /// @param car the car of this cons cell
    Cons (Object* car);

    /// create a cons cell with a type and a car
    /// @param cctp the cons cell type
    /// @param car the car of this cons cell
    Cons (const t_cctp cctp, Object* car);

    /// copy construct this cons cell
    /// @param that the cons to copy
    Cons (const Cons& that);

    /// copy move this cons cell
    /// @param that the cons to copy
    Cons (Cons&& that) noexcept;
    
    /// destroy this cons cell
    ~Cons (void);

    /// assign a cons cell to this one
    /// @param that the cons cell to assign
    Cons& operator = (const Cons& that);

    /// move a cons cell to this one
    /// @param that the cons cell to omve
    Cons& operator = (Cons&& that) noexcept;

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the cons cell did
    t_word getdid (void) const override;

    /// @return the cons cell sid
    t_word getsid (void) const override;

    /// serialize this cons cell to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a cons cell from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// release this cons cell
    void release (void) override;

    /// add an object to the last cdr of this cons with a new cons cell
    /// @param object the object to add
    virtual void add (Object* object);

    /// set the car of this cons cell
    /// @param car the car to set
    virtual void setcar (Object* object);

    /// set the cdr of this cons cell
    /// @param cdr the cdr to set
    virtual void setcdr (Cons* cdr);
  
    /// @return the car of this cons cell or nil.
    virtual Object* getcar (void) const;

    /// @return the cdr of this cons cell or nil
    virtual Cons* getcdr (void) const;

    /// @return the car of the cdr or nil
    virtual Object* getcadr (void) const;

    /// @return the car of the cdr of the cdr or nil
    virtual Object* getcaddr (void) const;

    /// @return the car of the cdr of the cdr of the cdr or nil
    virtual Object* getcadddr (void) const;

    /// @return the car of the cdr of the cdr of the cdr of the cdr or nil
    virtual Object* getcaddddr (void) const;

    /// @return the length of this cons cell
    virtual long length (void) const;

    /// @return true if the cdr of this cons cell is nil
    virtual bool isnil (void) const;

    /// @return true if the cons cell is of type block
    virtual bool isblock (void) const;

    /// @return an object by index
    virtual Object* get (const long index) const;

    /// install the form synchronizer
    virtual void mksync (void);

  public:
    /// @return a new iterator for this cons
    Iterator* makeit (void) override;

  private:
    // make the cons iterator a friend
    friend class Consit;

  public:
    /// evaluate a cons cell. Each car is evaluated and placed in the 
    /// resulting cell with the whole form returned
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    /// @param cons the cons cell to evaluate    
    static Cons* eval (Evaluable* zobj, Nameset* nset, Cons* cons);

    /// evaluate a cons cell and return a new cons cell or the object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to evaluate
    static Object* mkobj (Evaluable* zobj, Nameset* nset, Object* object);

    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// set an object to the car of this cons cell
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object) override;

    /// evaluate this object within the calling nameset
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset 
    Object* eval (Evaluable* zobj, Nameset* nset) override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj   zobj the current evaluable
    /// @param nset   the current nameset    
    /// @param quark  the quark to apply these arguments
    /// @param argv   the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
  
  /// The Consit class is the iterator for the cons class. Such iterator
  /// is constructed with the "makeit" cons method. The iterator is reset to
  /// the beginning of the cons cell.
  /// @author amaury darsch

  class Consit : public Iterator {
  private:
    /// the cons cell to iterate
    Cons* p_cons;
    /// the current cons cell
    Cons* p_cell;

  public:
    /// create a new iterator from a cons
    /// @param cons the list to iterate
    Consit (Cons* cons);

    /// destroy this cons iterator
    ~Consit (void);

    /// @return the class name
    String repr (void) const override;

    /// reset the iterator to the begining
    void begin (void) override;

    /// reset the iterator to the end
    void end (void) override;

    /// move the iterator to the next position
    void next (void) override;
 
    /// move the iterator to the previous position
    void prev (void) override;

    /// @return the object at the current position
    Object* getobj (void) const override;

    /// @return true if the iterator is at the end
    bool isend (void) const override;

  private:
    // make the copy constructor private
    Consit (const Consit&) =delete;
    // make the assignment operator private
    Consit& operator = (const Consit&) =delete;
  };
}

#endif
