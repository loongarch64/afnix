// ---------------------------------------------------------------------------
// - Buffer.hpp                                                              -
// - standard object library - character buffer class definition             -
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

#ifndef  AFNIX_BUFFER_HPP
#define  AFNIX_BUFFER_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

#ifndef  AFNIX_ENCODING_HPP
#include "Encoding.hpp"
#endif

#ifndef  AFNIX_VIEWABLE_HPP
#include "Viewable.hpp"
#endif

namespace afnix {

  /// The Buffer class is a byte buffer which can be used to accumulate
  /// characters. By default, the class automatically resize itself when full.
  /// However, a flag controls whther or not this operation can proceed. If
  /// the buffer is full, an exception is raised. Standard methods to read 
  /// or write or extract data is provided. 
  /// @author amaury darsch

  class Buffer : public virtual Serial, public Viewable {
  public:
    /// generate a random buffer by size
    /// @param size the number of bytes
    static Buffer* random (const long size);

  protected:
    /// the buffer size
    long  d_size;
    /// the buffer length
    long  d_blen;
    /// the read index
    long  d_ridx;
    /// the resize flag
    bool  d_rflg;
    /// the protection flag
    bool  d_pflg;
    /// the buffer data
    char* p_data;
    /// the buffer encoding
    Encoding::t_emod d_emod;

  public:
    /// create a default buffer
    Buffer (void);

    /// create a new buffer with a default size.
    /// @param size the buffer size
    Buffer (const long size);

    /// create a new buffer with a mode
    /// @param emod the encoding mode
    Buffer (const Encoding::t_emod emod);

    /// create a new buffer by size with a mode
    /// @param size the buffer size
    /// @param emod the encoding mode
    Buffer (const long size, const Encoding::t_emod emod);
    
    /// create a new buffer and initialize it with a c string
    /// @param value the c string to initialize
    Buffer (const char* value);

    /// create a new buffer and initialize it with a string
    /// @param value the string to initialize
    Buffer (const String& value);

    /// create a new buffer by size and content, no copy, no resize
    /// @param size the buffer size
    /// @param blen the buffer length
    /// @param data the buffer data
    Buffer (const long size, const long blen, char* data);

    /// copy construct a buffer
    /// @param that the buffer to copy
    Buffer (const Buffer& that);

    /// copy move a buffer
    /// @param that the buffer to copy
    Buffer (Buffer&& that) noexcept;

    /// destroy this buffer
    ~Buffer (void);

    /// assign a buffer to this one
    /// @param that the buffer to assign
    Buffer& operator = (const Buffer& that);

    /// move a buffer to this one
    /// @param that the buffer to move
    Buffer& operator = (Buffer&& that) noexcept;

    /// compare a buffer with this one
    /// @param that the buffer to compare
    bool operator == (const Buffer& that) const;

    /// compare a buffer with this one
    /// @param that the buffer to compare
    bool operator != (const Buffer& that) const;

    /// xor this buffer with another one
    Buffer& operator ^= (const Buffer& that);

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the buffer did
    t_word getdid (void) const override;

    /// @return the buffer sid
    t_word getsid (void) const override;

    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// @return the viewable size
    long tosize (void) const override;
    
    /// @return the viewable data
    t_byte* tobyte (void) override;

    /// @return the viewable data
    const t_byte* tobyte (void) const override;
    
    /// reset this buffer 
    virtual void reset (void);
    
    /// @return the size of this buffer
    virtual long getsize (void) const;

    /// @return the length of this buffer
    virtual long length (void) const;

    /// set the buffer encoding mode
    /// param emod the encoding mode to set
    virtual void setemod (const Encoding::t_emod emod);

    /// @return the buffer encoding
    virtual Encoding::t_emod getemod (void) const;

    /// set the resize flag
    /// @param rflg the resize flag
    virtual void setrflg (const bool rflg);

    /// @return the resize flag
    virtual bool getrflg (void) const;

    /// @return true if the buffer is empty
    virtual bool empty (void) const;

    /// @return true if the buffer is full
    virtual bool full (void) const;

    /// add a character to this buffer
    /// @param value the character to add to this buffer
    virtual long add (const char value);

    /// add a unicode character to this buffer
    /// @param value the unicode character to add to this buffer
    virtual long add (const t_quad value);

    /// add a word to this buffer
    /// @param wval the word to add
    /// @param hflg the host flag
    virtual void add (const t_word wval, const bool hflg);

    /// add a quad to this buffer
    /// @param qval the quad to add
    /// @param hflg the host flag
    virtual void add (const t_quad qval, const bool hflg);

    /// add an octa to this buffer
    /// @param oval the octa to add
    /// @param hflg the host flag
    virtual void add (const t_octa oval, const bool hflg);

    /// add a c-string to this buffer
    /// @param s the c-string to add
    virtual long add (const char* s);

    /// add a character buffer in this buffer
    /// @param cbuf the character buffer to add
    /// @param size the character buffer size
    virtual long add (const char* cbuf, const long size);

    /// add a string to this buffer
    /// @param s the string to add to this buffer
    virtual long add (const String& s);

    /// add a buffer object to this buffer
    /// @param buf the buffer to add
    virtual long add (const Buffer& buf);
    
    /// @return the next available character
    virtual char read (void);

    /// @return the next available byte
    virtual t_byte readb (void);

    /// @return the next available  word 
    virtual t_word readw (const bool hflg);

    /// @return the next available quad 
    virtual t_quad readq (const bool hflg);

    /// @return the next available octa 
    virtual t_octa reado (const bool hflg);
    
    /// @return the next character but do not remove it
    virtual char get (void) const;

    /// @return a character by index
    virtual char get (const long index) const;

    /// set a character by index
    /// @param index the buffer index
    /// @param value the character to set
    virtual void set (const long index, const char value);

    /// @return the last character
    virtual char pop (void);

    /// move the buffer content into another one
    /// @param rbuf the reference buffer
    /// @param size the buffer size
    virtual long copy (char* rbuf, const long size);

    /// move the buffer content into another one
    /// @param size the buffer size
    virtual Buffer move (const long size);

    /// shift the buffer by a certain amount
    /// @param asl the amount of left shift
    virtual void shl (const long asl);

    /// trim the buffer to a certain size and lock
    /// @param size the buffer trim size
    /// @param rflg the resize flag
    virtual bool trim (const long size, const bool rflg);

    /// @return a line from this buffer
    virtual String readln (void);

    /// pushback a character in this buffer
    /// @param value the character to push back
    virtual long pushback (const char value);

    /// pushback a unicode character in this buffer
    /// @param value the character to push back
    virtual long pushback (const t_quad value);

    /// pushback a c-string in this buffer
    /// @param s the string to pushback
    virtual long pushback (const char* value);

    /// pushback a c-string in this buffer
    /// @param s    the string to pushback
    /// @param size the string size
    virtual long pushback (const char* s, const long size);

    /// pushback a string in this buffer
    /// @param value the string to push back
    virtual long pushback (const String& value);

    /// pushback a buffer in this buffer
    /// @param buffer the buffer to push back
    virtual long pushback (const Buffer& value);

    /// collect a buffer from this buffer
    /// @param size the buffer size
    virtual Buffer collect (const long size);
    
    /// extract a buffer from this buffer
    /// @param boff the buffer offset
    /// @param size the buffer size
    virtual Buffer extract (const long boff, const long size) const;
    
    /// @return the buffer content as character buffer
    virtual char* tochar (void) const;

    /// @return the buffer content as quad buffer
    virtual t_quad* toquad (void) const;

    /// @return the buffer contents as a string
    virtual String tostring (void) const;

    /// @return the buffer content as an octet string
    virtual String format (void) const;

    /// map this buffer to an anonymous data structure
    /// @param data the data structure to map
    /// @param size the data size to map
    virtual long tomap (void* data, const long size) const;

    /// copy a buffer upto a boudary string
    /// @param bnds the boundary string
    virtual Buffer* cpbnds (const String& bnds);

    /// remove the last cr/nl characters in the buffer
    virtual bool rmcrnl (void);

  protected:
    /// normalize the buffer
    virtual void normalize (void);
    
  public:
    /// create a new object in a generic way
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
