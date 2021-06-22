// ---------------------------------------------------------------------------
// - Blob.hpp                                                                -
// - afnix:csm service - registered blob class definition                    -
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

#ifndef  AFNIX_BLOB_HPP
#define  AFNIX_BLOB_HPP

#ifndef  AFNIX_PART_HPP
#include "Part.hpp"
#endif

#ifndef  AFNIX_WHATIS_HPP
#include "Whatis.hpp"
#endif

namespace afnix {

  /// The Blob class is a base class that models the behavior of a registered
  /// blob through the use of a registration id. The blob is registered as soon
  /// as its registration id is set. If the registration id is unset, the
  /// object is unregistered or anonymous. The registration id can be anything
  /// as long as as it is understood by the implementation that such
  /// registration is to be interpreted somewhere else. The blob is also a part
  /// which means that it has a name, info and unique key.
  /// @author amaury darsch

  class Blob : public Part {
  protected:
    /// the registration id
    String  d_rid;
     /// the creation time
    t_long d_ctim;
    /// the modification time
    t_long d_mtim;
    /// the transient flag
    bool d_tflg;
    /// the performance counters
    struct s_perf* p_perf;
    
  public:
    /// create nil blob
    Blob (void);

    /// create a blob by name
    /// @param name the registration name
    Blob (const String& name);

    /// create a blob by name and info
    /// @param name the registration name
    /// @param info the registration info
    Blob (const String& name, const String& info);

    /// create a blob by rid, name and info
    /// @param rid  the registration id
    /// @param name the registration name
    /// @param info the registration info
    Blob (const String& rid, const String& name, const String& info);

    /// copy construct this registered object
    /// @param that the object to copy
    Blob (const Blob& that);

    /// destroy this blob
    ~Blob (void);
    
    /// assign an blob to this one
    /// @param that the object to assign
    Blob& operator = (const Blob& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;
    
    /// serialize this blob
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this blob
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// @return the blob plist
    Plist getplst (void) const override;

    /// @return true if this blob is swappable
    bool isswap (Part* part) const override;

    /// @return the blob creation time
    virtual t_long getctim (void) const;

    /// @return the blob modification time
    virtual t_long getmtim (void) const;
    
    /// set the blob transient flag
    /// @param tflg the transient flag
    virtual bool settflg (const bool tflg);

    /// @return the blob transient flag
    virtual bool gettflg (void) const;
    
    /// validate a blob registration id
    virtual bool isrid (void) const;

    /// validate a blob registration id
    /// @param rid the rid to validate
    virtual bool isrid (const String& rid) const;

    /// set the registration id
    /// @param rid the rid to set
    virtual void setrid (const String& rid);

    /// @return the registration id
    virtual String getrid (void) const;

    /// @return the blob whatis
    virtual Whatis whatis (void) const;
    
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
