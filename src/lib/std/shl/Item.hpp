// ---------------------------------------------------------------------------
// - Item.hpp                                                                -
// - standard object library - item class definition                         -
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

#ifndef  AFNIX_ITEM_HPP
#define  AFNIX_ITEM_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Item class is a enumeration item. Such item is bound to the 
  /// enumeration object and the associated quark. An enumeration item
  /// cannot be created by itself. Only an enumeration evaluation can
  /// create it. An eneumretaion item can be either static or dynamic.
  /// The static version is created with a tid and a quark. A dynamic
  /// item is created with an object and a quark. The static version
  /// is generally used to bound item to a native object. The dynamic
  /// version is used by the enumeration object, to bound all items around
  /// a single one.
  /// @author amaury darsch

  class Item : public Literal {
  public:
    /// the item enumeration type
    enum t_item {
      ITEM_STIC, // static item
      ITEM_DYNC  // dynamic item
    };

  private:
    /// the enumeration type
    t_item d_type;
    union {
      /// the static id
      long    d_tid;
      /// The supporting object
      Object* p_obj;
    };
    /// the enumeration quark
    long d_quark;

  public:
    /// create an item by id and quark
    /// @param tid the enumeration id
    /// @param quark the item quark
    Item (const long tid, const long quark);

    /// create an item by object and quark
    /// @param obj the enumeration object
    /// @param quark the item quark
    Item (Object* obj, const long quark);

    /// copy constructor for this enumeration item
    /// @param that the item to copy
    Item (const Item& that);

    /// destroy this enumeration item
    ~Item (void);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// clear this item
    void clear (void) override;

    /// @return a literal representation of this item
    String toliteral (void) const override;

    /// @return a string representation of this item
    String tostring (void) const override;

    /// compare this item with another one
    /// @param item the item to compare
    bool operator == (const Item& item) const;
 
    /// compare this item with another one
    /// @param item the item to compare
    bool operator != (const Item& item) const;

    /// @return the item quark
    long getquark (void) const;

    /// @return true if the item is static with that id
    bool scheck (const long tid) const;

    /// @return the static tid
    long gettid (void) const;

  private:
    // make the assignment operator private
    Item& operator = (const Item&);

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object) override;

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
