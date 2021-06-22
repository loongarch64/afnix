// ---------------------------------------------------------------------------
// - Tracker.hpp                                                             -
// - afnix:csm service - tracker blob class definition                      -
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

#ifndef  AFNIX_TRACKER_HPP
#define  AFNIX_TRACKER_HPP

#ifndef  AFNIX_BLOB_HPP
#include "Blob.hpp"
#endif

namespace afnix {

  /// The Tracker class is an blob used for registering a server with the
  /// connection parameters. The tracker is identified by its name, host, port
  /// and supported protocol. Since the tracker is an blob, it can be
  /// associated with a registration id or remains aninymous.
  /// @author amaury darsch

  class Tracker : public Blob {
  protected:
    /// the tracker host
    String d_host;
    /// the tracker port
    long d_port;
    /// the tracker protocol
    String d_prto;
    
  public:
    /// create a default tracker
    Tracker (void);

    /// create a tracker by name
    /// @param name the tracker name
    Tracker (const String& name);

    /// create a tracker by name and info
    /// @param name the tracker name
    /// @param info the tracker info
    Tracker (const String& name, const String& info);

    /// copy construct this tracker
    /// @param that the object to copy
    Tracker (const Tracker& that);

    /// assign a tracker to this one
    /// @param that the object to assign
    Tracker& operator = (const Tracker& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// @return the blob plist
    Plist getplst (void) const;
    
    /// set the tracker host
    /// @param host the host name
    virtual void sethost (const String& host);

    /// @return the tracker host name
    virtual String gethost (void) const;

    /// set the tracker port
    /// @param port the host port number
    virtual void setport (const long port);

    /// @return the tracker port number
    virtual long getport (void) const;

    /// set the tracker protocol
    /// @param prto the tracker protocol
    virtual void setprto (const String& prto);

    /// @return the tracker protocol
    virtual String getprto (void) const;

    /// check of a tracker as the same authority
    /// @param tckr the tracker to check
    virtual bool isauth (const Tracker& tckr) const;

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
