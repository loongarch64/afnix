// ---------------------------------------------------------------------------
// - Intercom.hpp                                                            -
// - afnix:sio module - interpreter communication class definition           -
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

#ifndef  AFNIX_INTERCOM_HPP
#define  AFNIX_INTERCOM_HPP

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

#ifndef  AFNIX_OUTPUTBUFFER_HPP
#include "OutputBuffer.hpp"
#endif

namespace afnix {

  /// The Intercom class is the interpreter commmunication class. The class
  /// operates with two streams. One output stream is used to send serialized
  /// data while the input stream is used to deserialize data. The "send"
  /// method can be used to send the data, while the "recv" can be used to
  /// receive them. 
  /// @author amaury darsch

  class Intercom : public virtual Object {
  protected:
    /// the input stream
    InputStream*  p_is;
    /// the output stream
    OutputStream* p_os;
    /// the output buffer
    OutputBuffer  d_ob;

  public:
    /// create a default intercom
    Intercom (void);

    /// create an intercom by input stream
    /// @param is the input stream to bind
    Intercom (InputStream* is);

    /// create an intercom by output stream
    /// @param os the output stream to bind
    Intercom (OutputStream* os);

    /// create an intercom by input and output stream
    /// @param is the input  stream to bind
    /// @param os the output stream to bind
    Intercom (InputStream* is, OutputStream* os);

    /// destroy this object
    ~Intercom (void);

    /// @return the class name
    String repr (void) const;

    /// set the intercom input stream
    /// @param is the input stream to set
    void setis (InputStream* is);

    /// @return the intercom input stream
    InputStream* getis (void) const;

    /// set the intercom output stream
    /// @param os the output stream to set
    void setos (OutputStream* os);

    /// @return the intercom output stream
    OutputStream* getos (void) const;

    /// send an object by serialization
    /// @param obj the object to send
    void send (Object* obj);

    /// @receive an object by serialization
    Object* recv (void);

    /// perform a send-receive request
    /// @param obj the object to send
    Object* rqst (Object* obj);

  private:
    // make the copy constructor private
    Intercom (const Intercom&);
    // make the assignment operator private
    Intercom& operator = (const Intercom&);

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
