// ---------------------------------------------------------------------------
// - Transaction.hpp                                                         -
// - afnix:bce service - blockchain transaction class definition             -
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

#ifndef  AFNIX_TRANSACTION_HPP
#define  AFNIX_TRANSACTION_HPP

#ifndef  AFNIX_UUID_HPP
#include "Uuid.hpp"
#endif

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Transaction class is a base class that acts as the base ingredient
  /// of the blockchain. Normally a transaction is recorded in a block and
  /// linked with the previous block to form the blockchain itself. One of
  /// the requirement for a transaction is to be serializable. An optional
  /// signature can be attached to the transaction, given a signing key.
  /// @author amaury darsch

  class Transaction : public Taggable {
  protected:
    /// the transaction uuid
    Uuid d_uuid;
    
  public:
    /// create default transaction
    Transaction (void);

    /// create a transaction by name
    /// @param name the registration name
    Transaction (const String& name);

    /// create a transaction by name and info
    /// @param name the registration name
    /// @param info the registration info
    Transaction (const String& name, const String& info);

    /// copy construct this transaction
    /// @param that the object to copy
    Transaction (const Transaction& that);

    /// copy move this transaction
    /// @param that the transaction to move
    Transaction (Transaction&& that) noexcept;

    /// assign a transaction to this one
    /// @param that the object to assign
    Transaction& operator = (const Transaction& that);

    /// move a transaction to this one
    /// @param that the transaction to move
    Transaction& operator = (Transaction&& that) noexcept;
    
    /// serialize this transaction
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this transaction
    /// @param is the input stream
    void rdstream (InputStream& os);
    
    /// @return the transaction plist
    virtual Plist getplst (void) const;

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
