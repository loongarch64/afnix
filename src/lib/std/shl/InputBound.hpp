// ---------------------------------------------------------------------------
// - InputBound.hpp                                                          -
// - standard object library - input bounded stream base class definition    -
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

#ifndef  AFNIX_INPUTBOUND_HPP
#define  AFNIX_INPUTBOUND_HPP

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The InputBound class is a generic input stream that operates with
  /// input stream except that the input length is bounded by an integer 
  /// value. The class maintains a counter which indicates how many
  /// characters have been read. When the counter reaches the upper bound
  /// the end of stream is considered reached and no further characters
  /// cannot be read until a counter reset is done.
  /// @author amaury darsch

  class InputBound : public InputStream {
  protected:
    /// the reference stream
    InputStream* p_is;
    /// the stream upper bound
    long d_isub;
    /// the stream counter
    long d_scnt;

  public:
    /// create a default input bound
    InputBound (void);

    /// create an input bound by stream
    /// @param is the input stream to match
    InputBound (InputStream* is);

    /// create an input bound by stream and upper bound
    /// @param is the input stream to match
    /// @param isub the input stream upper bound
    InputBound (InputStream* is, const long isub);

    /// destroy this input bound stream
    ~InputBound (void);

    /// @return the class name
    String repr (void) const override;

    /// reset the input bound stream
    void reset (void);

    /// bind an input stream to this bounded stream
    /// @param is the input stream to bind
    void bind (InputStream* is);

    /// bind an input stream to this bounded stream
    /// @param is the input stream to bind
    /// @param isub the input stream upper bound
    void bind (InputStream* is, const long isub);

    /// @return the transcoding mode
    Encoding::t_tmod gettmod (void) const override;

    /// set the transcoding mode
    /// @param tmod the transcoding mode to set
    void settmod (const Encoding::t_tmod tmod) override;

    /// @return true if a character can be encoded
    bool valid (const char c) const override;

    /// @return true if a character can be decoded
    bool valid (const t_quad c) const override;

    /// transcode an input character into a unicode
    /// @param c the character to transcode
    t_quad encode (const char c) const override;

    /// transcode an input character buffer 
    /// @param s the character buffer to transcode
    t_quad* encode (const char* s) const override;

    /// transcode an input character buffer by size
    /// @param s    the character buffer to transcode
    /// @param size the size to transcode
    t_quad* encode (const char* s, const long size) const override;

    /// transcode an output unicode quad into a character
    /// @param c the character to transcode
    char decode (const t_quad c) const override;

    /// transcode an output unicode buffer into a character buffer
    /// @param s the character buffer to transcode
    char* decode (const t_quad* s) const override;

    /// transcode an output unicode buffer into a character buffer
    /// @param s    the character buffer to transcode
    /// @param size the size to transcode
    char* decode (const t_quad* s, const long size) const override;

    /// @return the stream descriptor
    int getsid (void) const override;

    /// @return the stream encoding mode
    Encoding::t_emod getemod (void) const override;

    /// set the stream encoding mode
    /// @param emod the stream encoding mode to set
    void setemod (const Encoding::t_emod emod) override;

    /// set the stream encoding mode by name
    /// @param mode the coding mode to set
    void setemod (const String& mode) override;

    /// @return the stream timeout
    long gettout (void) const override;

    /// set the stream timeout
    /// @param tout the stream time to set
    void settout (const long tout) override;

    /// flush the input stream buffer
    void flush (void) override;

    /// @return true if we are at the eos
    bool iseos (void) const override;

    /// check if we can read a character
    bool valid (void) const override;

    /// @return the next available character
    char read (void) override;

    /// pushback a character in the input buffer.
    /// @param value the character to push back
    long pushback (const char value) override;

    /// pushback a unicode character in the input buffer.
    /// @param value the character to push back
    long pushback (const t_quad value) override;

    /// pushback a c-string in the input buffer
    /// @param s the string to pushback
    long pushback (const char* s) override;

    /// pushback a c-string by size in the input buffer
    /// @param s    the string to pushback
    /// @param size the string size
    long pushback (const char* s, const long size) override;

    /// consume a stream by reading and pushing back
    long consume (void) override;

    /// @return the size of the input buffer
    long buflen (void) const override;

    /// @return a copy if the input buffer
    Buffer toibuf (void) const override;
    
    /// @return a string representation of the input buffer
    String tostring (void) const override;

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
