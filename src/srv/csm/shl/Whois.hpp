// ---------------------------------------------------------------------------
// - Whois.hpp                                                               -
// - afnix:csm service - domain information class definition                 -
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

#ifndef  AFNIX_WHOIS_HPP
#define  AFNIX_WHOIS_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Whois class is an informative class designed to report information
  /// about a domain since a domain can be quite large. The domain information
  /// is particularly useful with serialized domain as it avoid the
  /// serialization of the domain content.
  /// @author amaury darsch

  class Whois : public Taggable {
  protected:
    /// the domain rid
    String d_rid;
    /// the creation time
    t_long d_ctim;
    /// the modification time
    t_long d_mtim;
    /// the domain length
    long d_dlen;

  public:
    /// create a default domain whois
    Whois (void);
    
    /// copy construct this domain whois 
    /// @param that the object to copy 
    Whois (const Whois& that);
    
    /// assign a domain whois to this one
    /// @param that the object to assign
    Whois& operator = (const Whois& that);

    /// @return the class name
    String repr (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// serialize this domain whois
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this domain whois
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// @return true if the object is valid
    virtual bool valid (void) const;
    
    /// @return the domain rid
    virtual String getrid (void) const;
    
    /// @return the creation time
    virtual t_long getctim (void) const;

    /// @return the modification time
    virtual t_long getmtim (void) const;
    
    /// @return the domain length
    virtual long length (void) const;

    /// merge a domain whois
    /// @param wois the whois to merge
    virtual void merge (const Whois& wois);

    /// @return the whois plist
    virtual Plist getplst (void) const;
    
  protected:
    // make the domain class a frien
    friend class Domain;
    
  public:
    /// create a object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

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
