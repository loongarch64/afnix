// ---------------------------------------------------------------------------
// - Plist.hpp                                                               -
// - standard object library - property list class definition                -
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

#ifndef  AFNIX_PLIST_HPP
#define  AFNIX_PLIST_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_ITERABLE_HPP
#include "Iterable.hpp"
#endif

#ifndef  AFNIX_PROPERTY_HPP
#include "Property.hpp"
#endif

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif 

#ifndef  AFNIX_HASHTABLE_HPP
#include "HashTable.hpp"
#endif

#ifndef  AFNIX_PRINTTABLE_HPP
#include "PrintTable.hpp"
#endif

namespace afnix {

  /// The Plist class is a base container class used to manage property 
  /// object in an ordered way. The property list operates by maintaining a
  /// vector of property object along with a hash table that permits
  /// to find the object quickly.
  /// @author amaury darsch

  class Plist : public Taggable, public Iterable {
  private:
    /// the plist vector
    Vector    d_list;
    /// the plist hash
    HashTable d_hash;
    
  public:
    /// create an empty plist
    Plist (void);

    /// create a plist by case flag
    /// @param cifg the case flag
    Plist (const bool cifg);

    /// create a plist by name
    /// @param name the plist name
    Plist (const String& name);

    /// create a plist by name and info
    /// @param name the plist name
    /// @param info the plist info
    Plist (const String& name, const String& info);

    /// copy construct this property list
    /// @param that the plist to copy
    Plist (const Plist& that);

    /// destroy this plist
    ~Plist (void);

    /// assign a plist with another one
    /// @param that the plist to assign
    Plist& operator = (const Plist& that);
    
    /// add two plist by merging them
    /// @param plst the plist to merge
    Plist& operator += (const Plist& plst);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the plist did
    t_word getdid (void) const;

    /// @return the plist sid
    t_word getsid (void) const;

    /// serialize this plist to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a plist from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// reset this plist
    virtual void reset (void);

    /// @return true if the list is empty
    virtual bool empty (void) const;

    /// @return the plist length
    virtual long length (void) const;

    /// @return a property by index
    virtual Property* get (const long index) const;

    /// @return a property index by name
    virtual long locate (const String& name) const;
    
    /// @return a property by name
    virtual Property* find (const String& name) const;

    /// @return true if the property exists
    virtual bool exists (const String& name) const;

    /// @return true if the property exists with a certain value
    virtual bool exists (const String& name, const Literal& lval) const;

    /// add a property to this plist
    /// @param prop the property to add
    virtual void add (Property* prop);

    /// add a property by name and boolean value
    /// @param name the property name to add
    /// @param bval the property value to add
    virtual void add (const String& name, const bool bval);

    /// add a property by name and integer value
    /// @param name the property name to add
    /// @param ival the property value to add
    virtual void add (const String& name, const t_long ival);

    /// add a property by name and real value
    /// @param name the property name to add
    /// @param rval the property value to add
    virtual void add (const String& name, const t_real rval);

    /// add a property by name and value
    /// @param name the property name to add
    /// @param lval the property value to add
    virtual void add (const String& name, const Literal& lval);

    /// add a property by name, info and boolean value
    /// @param name the property name to add
    /// @param info the property info to add
    /// @param bval the property value to add
    virtual void add (const String& name, const String& info, 
		      const bool    bval);

    /// add a property by name, info and integer value
    /// @param name the property name to add
    /// @param info the property info to add
    /// @param ival the property value to add
    virtual void add (const String& name, const String& info, 
		      const t_long  ival);

    /// add a property by name, info and real value
    /// @param name the property name to add
    /// @param info the property info to add
    /// @param rval the property value to add
    virtual void add (const String& name, const String& info, 
		      const t_real  rval);

    /// add a property by name, info and value
    /// @param name the property name to add
    /// @param info the property info to add
    /// @param lval the property value to add
    virtual void add (const String&  name, const String& info,
		      const Literal& lval);

    /// @return a property by name or throw an exception
    virtual Property* lookup (const String& name) const;

    /// @return a property literal by name
    virtual Literal* toliteral (const String& name) const;
    
    /// @return a property value by name
    virtual String getpval (const String& name) const;

    /// @return the property as a boolean
    virtual bool tobool (const String& name) const;

    /// @return the property as an integer boolean
    virtual bool toboil (const String& name) const;

    /// @return the property as an integer
    virtual t_long tolong (const String& name) const;

    /// @return the property as a real
    virtual t_real toreal (const String& name) const;

    /// set a property to this plist
    /// @param prop the property to set
    virtual void set (Property* prop);

    /// set a property by name and boolean value
    /// @param name the property name to set
    /// @param bval the property value to set
    virtual void set (const String& name, const bool bval);

    /// set a property by name and integer value
    /// @param name the property name to set
    /// @param ival the property value to set
    virtual void set (const String& name, const t_long ival);

    /// set a property by name and real value
    /// @param name the property name to set
    /// @param rval the property value to set
    virtual void set (const String& name, const t_real rval);

    /// set a property by name and value
    /// @param name the property name to set
    /// @param lval the property value to set
    virtual void set (const String& name, const Literal& lval);

    /// set a property by name, info and value
    /// @param name the property name to set
    /// @param info the property info to set
    /// @param lval the property value to set
    virtual void set (const String&  name, const String& info,
		      const Literal& lval);

    /// merge a plist in this one
    /// @param plst the plist to merge
    virtual Plist merge (const Plist& plst) const;
    
    /// merge a plist in this one by index
    /// @param plst the plist to merge
    /// @param pidx the plist index
    virtual Plist merge (const Plist& plst, const long pidx) const;
    
    /// convert this property list into a print table
    /// @param iflg the info flag for the table
    virtual PrintTable* toptbl (const bool iflg) const;

    /// convert this property list into a print table
    /// @param iflg the info flag for the table
    /// @param lstl the the literal style
    virtual PrintTable* toptbl (const bool iflg, const Style& lstl) const;

    /// @return a new iterator for this plist
    Iterator* makeit (void);

  private:
    // make the plist iterator a friend
    friend class Plistit;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a plist of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
  
  /// The Plistit class is the iterator for the property list class. Such 
  /// iterator is constructed with the "makeit" plist method. The iterator is
  /// reset to the beginning of the list.
  /// @author amaury darsch
  
  class Plistit : public Iterator {
  private:
    /// the plist to iterate
    Plist* p_plst;
    /// the current index
    long   d_pidx;
    
  public:
    /// create a new iterator from a plist
    /// @param plst the plist to iterate
    Plistit (Plist* plst);

    /// destroy this plist iterator
    ~Plistit (void);

    /// @return the class name
    String repr (void) const;

    /// reset the iterator to the begining
    void begin (void);

    /// reset the iterator to the end
    void end (void);

    /// move the iterator to the next position
    void next (void);

    /// move the iterator to the previous position
    void prev (void);

    /// @return the object at the current position
    Object* getobj (void) const;

    /// @return true if the iterator is at the end
    bool isend (void) const;

  private:
    // make the copy constructor private
    Plistit (const Plistit&);
    // make the assignment operator private
    Plistit& operator = (const Plistit&);
  };
}

#endif
