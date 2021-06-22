// ---------------------------------------------------------------------------
// - List.hpp                                                                -
// - standard object library - doubly linked list class definition           -
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

#ifndef  AFNIX_LIST_HPP
#define  AFNIX_LIST_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

#ifndef  AFNIX_ITERABLE_HPP
#include "Iterable.hpp"
#endif

namespace afnix {

  /// The List class implements the behavior of a doubly linked list. Each
  /// object can be added at the beginning or at the end of the list. The
  /// list can be either accessed by iteration or by index. Needless to say
  /// that index access is very slow.
  /// @author amaury darsch

  class List : public virtual Serial, public Iterable {
  private:
    /// the root list 
    struct s_list* p_root;
    /// the last object in the list
    struct s_list* p_last;

  public:
    /// create an empty ist
    List (void);

    /// copy constructor for this list
    /// @param that the list to copy 
    List (const List& that);

    /// destroy this list
    ~List (void);

    /// assign a list to this one
    /// @param that the list to assign
    List& operator = (const List& that);

    /// @return the class name
    String repr (void) const override;

    /// @return the list did
    t_word getdid (void) const override;

    /// @return the list sid
    t_word getsid (void) const override;

    /// serialize this cons cell to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const override;

    /// deserialize a cons cell from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is) override;

    /// @return the number of objects in this list
    long length (void) const;

    /// @return an object by index
    Object* get (const long index) const;

    /// add an object to the end of the list
    /// @param object the object to add
    void add (Object* object);

    /// insert an object at the beginning of the list
    /// @param object the object to insert
    void insert (Object* object);

    /// @return a new iterator for this list
    Iterator* makeit (void) override;

    /// create an iterator at the begining
    class Listit begin (void);

    /// create an iterator at the end
    class Listit end (void);

  private:
    // make the list iterator a friend
    friend class Listit;

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
    /// @param args  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };

  /// The Listit class is the iterator for the list class. Such iterator
  /// is constructed with the "makeit" list method. The iterator is reset to
  /// the beginning of the list
  /// @author amaury darsch

  class Listit : public Iterator {
  private:
    /// the list to iterate
    List* p_list;
    /// the iterator node
    struct s_list* p_node;

  public:
    /// create a new iterator from a list
    /// @param lst the list to iterate
    Listit (List* lst);

    /// copy construct this iterator
    /// @param that the object to copy
    Listit (const Listit& that);
    
    /// destroy this list iterator
    ~Listit (void);

    /// assign an iterator to this one
    /// @param that the object to assign
    Listit& operator = (const Listit& that);

    /// compare two iterators
    /// @param it the iteraror to compare
    bool operator == (const Listit& it) const;

    /// compare two iterators
    /// @param it the iteraror to compare
    bool operator != (const Listit& it) const;

    /// move the iterator to the next position
    Listit& operator ++ (void);
    
    /// move the iterator to the previous position
    Listit& operator -- (void);
    
    /// @return the iterator object
    Object* operator * (void) const;
    
    /// @return the class name
    String repr (void) const override;

    /// reset the iterator to the begining
    void begin (void) override;

    /// reset the iterator to the end
    void end (void) override;

    /// move the list iterator to the next position
    void next (void) override;

    /// move the list iterator to the previous position
    void prev (void) override;

    /// @return the object at the current position
    Object* getobj (void) const override;

    /// @return true if the iterator is at the end
    bool isend (void) const override;
  };
}

#endif
