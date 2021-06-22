// ---------------------------------------------------------------------------
// - Codec.hpp                                                               -
// - afnix:sec module - abstract codec class definition                      -
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

#ifndef  AFNIX_CODEC_HPP
#define  AFNIX_CODEC_HPP

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The Codec class is an abstract class that provides the method for
  /// encoding and decoding a buffer or a stream. The codec direction is
  /// controlled by the reverse flag.
  /// @author amaury darsch

  class Codec : public Nameable {
  protected:
    /// the codec name
    String  d_name;
    /// the reverse flag
    bool    d_rflg;
    /// the encode stats
    long    d_encs[2];
    /// the decode stats
    long    d_decs[2];

  public:
    /// create a default codec
    Codec (void);

    /// create a codec by name
    /// @param name the codec name
    Codec (const String& name);

    /// @return the codec name
    String getname (void) const override;

    /// reset this codec
    virtual void reset (void);

    /// clear this codec
    virtual void clear (void);
    
    /// set the codec reverse flag
    /// @param rflg the flag to set
    virtual void setrflg (const bool rflg);

    /// @return the reverse flag
    virtual bool getrflg (void) const;

    /// stream an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    virtual long stream (Buffer& ob, Buffer& ib);

    /// stream an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    virtual long stream (Buffer& ob, InputStream& is);

    /// stream an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    virtual long stream (OutputStream& os , InputStream& is);

    /// serialize an object into a buffer
    /// @param sobj the object to serialize
    virtual Buffer serialize (Object* sobj);

    /// serialize an object into a buffer
    /// @param sobj the object to serialize
    /// @param lbdr the line border flag
    virtual Buffer serialize (Object* sobj, const bool lbdr);
    
    /// unserialize a buffer into an object
    /// @param ibuf the input buffer to unserialize
    virtual Object* unserialize (Buffer& ibuf);
    
    /// unserialize a buffer into an object
    /// @param ibuf the infput buffer to unserialize
    /// @param lbdr the line border flag
    virtual Object* unserialize (Buffer& ibuf, const bool lbdr);
    
  protected:
    /// encode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    virtual long encode (Buffer& ob, Buffer& ib) =0;

    /// encode an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    virtual long encode (Buffer& ob, InputStream& is) =0;

    /// encode an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    virtual long encode (OutputStream& os, InputStream& is) =0;

    /// decode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    virtual long decode (Buffer& ob, Buffer& ib) =0;

    /// decode an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    virtual long decode (Buffer& ob, InputStream& is) =0;

    /// decode an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    virtual long decode (OutputStream& os, InputStream& is) =0;

    /// finish the stream processing
    virtual long preset (void ) =0;

    /// finish the stream processing
    virtual long finish (void ) =0;
    
  private:
    // make the copy constructor private
    Codec (const Codec&);
    // make the assignment operator private
    Codec& operator = (const Codec&);

  public:
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
