// ---------------------------------------------------------------------------
// - Hyperlink.hpp                                                           -
// - afnix:nwg module - hyperlink class definition                           -
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

#ifndef  AFNIX_HYPERLINK_HPP
#define  AFNIX_HYPERLINK_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

namespace afnix {

  /// The Hyperlink class is a named uri augmented with a plist which can be
  /// used to store extra parameters. The hyperlink can also be used to store
  /// a single path. In this cas, the string uri does not have a scheme, and
  /// the uri predicate returns false. The gethost and getpath methods reacts
  /// therefore accordingly.
  /// @author amaury darsch

  class Hyperlink : public Plist {
  protected:
    /// the string uri
    String d_suri;
    
  public:
    /// create a nil hyperlink
    Hyperlink (void);

    /// create a hyperlink by string uri
    /// @paran suri the hyperlink uri
    Hyperlink (const String& suri);
    
    /// copy construct this hyperlink
    /// @param that the object to copy
    Hyperlink (const Hyperlink& that);

    /// assign a hyperlink to this one
    /// @param that the object to assign
    Hyperlink& operator = (const Hyperlink& hyperlink);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this hyperlink
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this hyperlink
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// reset this hyperlink
    virtual void reset (void);

    /// set the hyperlink uri
    /// @param suri the string uri
    virtual void setsuri (const String& suri);
    
    /// @return the hyperlink uri
    virtual String getsuri (void) const;

    /// @return the hyperlink host
    virtual String gethost (void) const;

    /// @return the hyperlink path
    virtual String getpath (void) const;

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
