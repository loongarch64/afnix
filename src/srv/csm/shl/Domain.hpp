// ---------------------------------------------------------------------------
// - Domain.hpp                                                              -
// - afnix:csm service - blob domain class definition                        -
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

#ifndef  AFNIX_DOMAIN_HPP
#define  AFNIX_DOMAIN_HPP

#ifndef  AFNIX_BLOB_HPP
#include "Blob.hpp"
#endif

#ifndef  AFNIX_WHOIS_HPP
#include "Whois.hpp"
#endif

#ifndef  AFNIX_COLLECTION_HPP
#include "Collection.hpp"
#endif

namespace afnix {

  /// The Domain class is an extension of the part collection which operates
  /// with registered blob. The domain is configured by registration id which
  /// is used to validate a blob insertion. If the domain id is not set, the
  /// domain can store unregisterd blobs.
  /// @author amaury darsch

  class Domain : public Collection {
  protected:
    /// the registration id
    String d_rid;
    /// the creation time
    t_long d_ctim;
    /// the modification time
    t_long d_mtim;

  public:
    /// create an empty set
    Domain (void);
    
    /// create a domain by name
    /// @param name the domain name
    Domain (const String& name);

    /// create a domain by name and info
    /// @param name the domain name
    /// @param info the domain info
    Domain (const String& name, const String& info);

    /// create a domain by name, info and rid
    /// @param name the domain name
    /// @param info the domain info
    /// @param rid  the domain rid 
    Domain (const String& name, const String& info, const String& rid);

    /// copy construct this domain
    /// @param that the domain to copy 
    Domain (const Domain& that);
    
    /// copy move this domain
    /// @param that the object to copy
    Domain (Domain&& that) noexcept;
    
    /// assign a domain to this one
    /// @param that the domain to assign
    Domain& operator = (const Domain& that);

    /// move a domain to this one
    /// @param that the object to assign
    Domain& operator = (Domain&& that) noexcept;
    
    /// @return the class name
    String repr (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// serialize this domain
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this domain
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// add a blob in this domain
    /// @param part the blob to add
    Uuid addpart (Part* part) override;

    /// set a blob in this domain
    /// @param part the blob to set
    bool setpart (Part* part) override;

    /// @return the domain creation time
    virtual t_long getctim (void) const;

    /// @return the domain modification time
    virtual t_long getmtim (void) const;

    /// validate a domain registration id
    /// @param rid the rid to validate
    virtual bool isrid (const String& rid) const;

    /// set the registration id
    /// @param rid the rid to set
    virtual void setrid (const String& rid);

    /// @return the registration id
    virtual String getrid (void) const;

    /// @return the domain whois
    virtual Whois whois (void) const;
    
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
