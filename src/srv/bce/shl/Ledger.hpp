// ---------------------------------------------------------------------------
// - Ledger.hpp                                                              -
// - afnix:bce service - blockchain ledger class definition                  -
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

#ifndef  AFNIX_LEDGER_HPP
#define  AFNIX_LEDGER_HPP

#ifndef  AFNIX_KEY_HPP
#include "Key.hpp"
#endif

#ifndef  AFNIX_BLOCK_HPP
#include "Block.hpp"
#endif

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

namespace afnix {

  /// The Ledger class is the core class of the blockchain architecture as
  /// it contains the whole set of blocks. The ledger is a vector of blocks
  /// with signature carrier. For security reason, each block added to the
  /// chain is linked with the previous block by a signature mechanism based
  /// on a unique hash of the block content itself. Since each block is
  /// serializable, the ledger is also serializable.
  /// @author amaury darsch

  class Ledger : public virtual Serial {
  protected:
    /// the ledger creation time
    t_long d_ctim;
    /// the ledger modification time
    t_long d_mtim;
    /// the block ledger
    Vector* p_blks;

  public:
    /// create default ledger
    Ledger (void);

    /// copy construct this ledger
    /// @param that the object to copy
    Ledger (const Ledger& that);

    /// copy move this ledger
    /// @param that the ledger to move
    Ledger (Ledger&& that) noexcept;

    /// destroy this ledger
    ~Ledger (void);
    
    /// assign a ledger to this one
    /// @param that the object to assign
    Ledger& operator = (const Ledger& that);

    /// move a ledger to this one
    /// @param that the ledger to move
    Ledger& operator = (Ledger&& that) noexcept;

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// serialize this ledger
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this ledger
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// @return the ledger description
    virtual Plist getplst (void) const;
    
    /// @return the ledger creation time
    virtual t_long getctim (void) const;

    /// @return the ledger modification time
    virtual t_long getmtim (void) const;

    /// @return the ledger length
    virtual long length (void) const;

    /// get a block by index
    /// @param bidx the block index
    virtual Block* get (const long bidx) const;

    /// link a transaction with the ledger
    /// @param tran the transaction to link
    /// @param skey the signing key
    virtual bool link (Transaction* tran, const Key& skey);

    /// verify the ledger
    /// @param skey the signing key
    //virtual bool verify (const Key& skey) const;
    
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
