// ---------------------------------------------------------------------------
// - Block.hpp                                                               -
// - afnix:bce service - blockchain block class definition                   -
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

#ifndef  AFNIX_BLOCK_HPP
#define  AFNIX_BLOCK_HPP

#ifndef  AFNIX_SIGNABLE_HPP
#include "Signable.hpp"
#endif

#ifndef  AFNIX_SIGNATURE_HPP
#include "Signature.hpp"
#endif

#ifndef  AFNIX_TRANSACTION_HPP
#include "Transaction.hpp"
#endif

namespace afnix {

  /// The Block class is base constituent of the block chain. The block is
  /// designed to act a linked transaction. It contains the block time, the
  /// hash of the previous block, a transaction and the block signature.
  /// @author amaury darsch

  class Block : public virtual Serial, public Signable {
  protected:
    /// the block timestamp
    t_long d_stmp;
    /// the previous block hash
    Relatif d_hash;
    /// the block transaction
    Transaction* p_tran;
    /// the block signature
    Signature* p_sign;

  public:
    /// create default block
    Block (void);

    /// create a block by transaction
    /// @param tran the block transaction
    Block (Transaction* tran);

    /// copy construct this block
    /// @param that the object to copy
    Block (const Block& that);

    /// copy move this block
    /// @param that the block to move
    Block (Block&& that) noexcept;

    /// destroy this block
    ~Block (void);
    
    /// assign a block to this one
    /// @param that the object to assign
    Block& operator = (const Block& that);

    /// move a block to this one
    /// @param that the block to move
    Block& operator = (Block&& that) noexcept;

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// serialize this block
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this block
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// serialize the signable object
    /// @param os the output stream
    void wrsign (OutputStream& os) const;
    
    /// sign an object by key
    /// @param skey the signing key
    bool sign (const Key& skey);

    /// verify an object by key
    bool verify (void) const;

    /// @return the block description
    virtual Plist getplst (void) const;
    
    /// @return the block tiemstamp
    virtual t_long getstmp (void) const;

    /// @return the block transaction
    virtual Transaction* gettran (void) const;
    
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
