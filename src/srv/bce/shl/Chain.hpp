// ---------------------------------------------------------------------------
// - Chain.hpp                                                               -
// - afnix:bce service - blockchain class definition                         -
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

#ifndef  AFNIX_CHAIN_HPP
#define  AFNIX_CHAIN_HPP

#ifndef  AFNIX_LOGGER_HPP
#include "Logger.hpp"
#endif

#ifndef  AFNIX_LEDGER_HPP
#include "Ledger.hpp"
#endif

#ifndef  AFNIX_REQUEST_HPP
#include "Request.hpp"
#endif

namespace afnix {

  /// The Chain class is the top level class for the blockchain architecture.
  /// The blockchain is the assocation of a ledger, a request stack and a
  /// public signing key used for the block chain integrity verification.
  /// The chain object is designed to process the request stack and inserts
  /// the block in the chain.
  /// @author amaury darsch

  class Chain : public virtual Serial {
  protected:
    /// the chain creation time
    t_long d_ctim;
    /// the chain modification time
    t_long d_mtim;
    /// the request stack
    Vector* p_rstk;
    /// the ledger
    Ledger* p_lgdr;

  public:
    /// create default chain
    Chain (void);

    /// copy construct this chain
    /// @param that the object to copy
    Chain (const Chain& that);

    /// copy move this chain
    /// @param that the chain to move
    Chain (Chain&& that) noexcept;

    /// destroy this chain
    ~Chain (void);
    
    /// assign a chain to this one
    /// @param that the object to assign
    Chain& operator = (const Chain& that);

    /// move a chain to this one
    /// @param that the chain to move
    Chain& operator = (Chain&& that) noexcept;

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// serialize this chain
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this chain
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// @return the chain description
    virtual Plist getplst (void) const;
    
    /// @return the chain creation time
    virtual t_long getctim (void) const;

    /// @return the chain modification time
    virtual t_long getmtim (void) const;

    /// @return the block chain ledger
    virtual Ledger* getlgdr (void);

    /// @return the block chain ledger
    virtual const Ledger* getlgdr (void) const;

    /// push a request the stack
    /// @param rqst the request to add
    virtual void push (Request* rqst);

    /// process the request stack by signing key
    /// @param skey the signing key
    /// @param the logger object
    virtual bool process (const Key& skey, Logger* logr);
    
    /// verify the blockchain
    //virtual bool verify (void) const;

  protected:
    /// validate a request before processing it
    /// @param rqst the request to validate
    /// @param logr the logger object
    virtual bool validate (const Request& rqst, Logger* logr) const;
    
    /// process a request by signing key
    /// @param rqst the request to process
    /// @param skey the signing key
    /// @param logr the logger object
    virtual bool process (const Request& rqst, const Key& skey, Logger* logr);
    
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
