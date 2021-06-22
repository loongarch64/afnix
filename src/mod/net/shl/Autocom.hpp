// ---------------------------------------------------------------------------
// - Autocom.hpp                                                             -
// - afnix:net module - automatic connectable class definition               -
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

#ifndef  AFNIX_AUTOCOM_HPP
#define  AFNIX_AUTOCOM_HPP

#ifndef  AFNIX_TASKABLE_HPP
#include "Taskable.hpp"
#endif

#ifndef  AFNIX_CONNECTABLE_HPP
#include "Connectable.hpp"
#endif

namespace afnix {

  /// The Autocom class is an automatic connection class that operates with
  /// a connectable object. In theory, the class attemps to connect an object
  /// in a thread if needed.
  /// @author amaury darsch

  class Autocom : public Taskable {
  protected:
    /// the autocom structure
    struct s_acom* p_acom;
    /// the connectable object
    Connectable* p_cobj;
    
  public:
    /// create a default autocom
    Autocom (void);

    /// create an autocom by parameters
    /// @param prms the socket parameters
    Autocom (const SockParams& prms);

    /// create an autocom by parameters and connectable
    /// @param prms the socket parameters
    /// @param cobj the connectable object
    Autocom (const SockParams& prms, Connectable* cobj);
    
    /// create an autocom by host and port
    /// @param host the host name to connect
    /// @param port the port to listen
    Autocom (const String& host, const t_word port);

    /// create an autocom by address and port
    /// @param addr the ip address to connect
    /// @param port the port to listen
    Autocom (const Address& addr, const t_word port);

    /// create an autocom by host and port
    /// @param host the host name to connect
    /// @param port the port to listen
    /// @param cobj the connectable object
    Autocom (const String& host, const t_word port, Connectable* cobj);

    /// create an autocom by address and port
    /// @param addr the ip address to connect
    /// @param port the port to listen
    /// @param cobj the connectable object
    Autocom (const Address& addr, const t_word port, Connectable* cobj);

    /// destroy this autocom
    ~Autocom (void);

    /// @return the class name
    String repr (void) const override;

    /// run the autocom by a task
    void run (void) noexcept override;

    /// @return the autocom result
    Object* toresult (void) const override;
    
    /// bind the autocom by connectable
    /// @param cobj the connectable object
    virtual bool bind (Connectable* cobj);

    /// bind the autocom by parameters
    /// @param prms the socket parameters
    virtual bool bind (const SockParams& prms);
    
    /// bind the autocom by host and port
    /// @param host the host name to connect
    /// @param port the port to listen
    virtual bool bind (const String& host, const t_word port);

    /// bind the autocom by address and port
    /// @param addr the ip address to connect
    /// @param port the port to listen
    virtual bool bind (const Address& addr, const t_word port);

  private:
    // make the copy constructor private
    Autocom (const Autocom&) =delete;
    // make the assignement operator private
    Autocom& operator = (const Autocom&) =delete;
  };
}

#endif
