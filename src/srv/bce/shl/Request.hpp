// ---------------------------------------------------------------------------
// - Request.hpp                                                             -
// - afnix:bce service - transaction request class definition                -
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

#ifndef  AFNIX_REQUEST_HPP
#define  AFNIX_REQUEST_HPP

#ifndef  AFNIX_TRANSACTION_HPP
#include "Transaction.hpp"
#endif

namespace afnix {

  /// The Request class is a class used to map blockchain transaction request.
  /// When a transaction needs to be placed in the blockchain, a transaction
  /// request is emitted and placed in the blockchain request stack. Such
  /// request are later processed. It is up to the blockchain implementation
  /// to decide what is to be done when processing a request. The request code
  /// is designed to guide the request processing and separate transaction
  /// request with administrative request. Request code 0 is the default code.
  /// @author amaury darsch

  class Request : public virtual Serial {
  public:
    /// the link request code
    static const long TR_CODE_LINK = 0L;
    
  protected:
    /// the request code
    long d_code;
    /// the request timestamp
    t_long d_stmp;
    /// the transaction object
    Transaction* p_tran;
    
  public:
    /// create a default request
    Request (void);

    /// create a request by transaction
    /// @param tran the transaction
    Request (Transaction* tran);

    /// create a request by code and transaction
    /// @param code the request code
    /// @param tran the transaction
    Request (const long code, Transaction* tran);

    /// copy construct this request
    /// @param that the object to copy
    Request (const Request& that);

    /// copy move this request
    /// @param that the request to move
    Request (Request&& that) noexcept;

    /// destroy this request
    ~Request (void);
    
    /// assign a request to this one
    /// @param that the object to assign
    Request& operator = (const Request& that);

    /// move a request to this one
    /// @param that the request to move
    Request& operator = (Request&& that) noexcept;

    /// @return the class name
    String repr (void) const;
    
    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// serialize this request
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this request
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// @return the request description
    virtual Plist getplst (void) const;
    
    /// set the request code
    /// @param code the request code to set
    virtual void setcode (const long code);
    
    /// @return the request code
    virtual long getcode (void) const;

    /// @return the request timestamp
    virtual t_long getstmp (void) const;
    
    /// set the transaction
    /// @param tran the transaction to set
    virtual void settran (Transaction* tran);
    
    /// @return the transaction object
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
