// ---------------------------------------------------------------------------
// - Basexx.hpp                                                              -
// - afnix:uri service - base 16/32/64 codec class definition                -
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

#ifndef  AFNIX_BASEXX_HPP
#define  AFNIX_BASEXX_HPP

#ifndef  AFNIX_CODEC_HPP
#include "Codec.hpp"
#endif

namespace afnix {

  /// The Basexx class is a codec that implements the rfc 4648 for
  /// base 64/32/16 encoding. The operation of the base codec are governed
  /// by the type of encoding selected which is the standard base 64 by
  /// default.
  /// @author amaury darsch

  class Basexx : public Codec {
  public:
    enum t_base
      {
       BASE_SC64, // standard base 64
       BASE_UC64, // url encoding base 64
       BASE_SC32, // standard base 32
       BASE_EC32, // extended encoding base 32
       BASE_SC16  // standard base 16
      };
    
  private:
    /// the base encoding
    t_base d_base;
    /// the split line mode
    bool d_slmd;
    /// the private codec
    struct s_codc* p_codc;
    
  public:
    /// create a default base codec
    Basexx (void);

    /// create a base codec by type
    /// @param  base the base type
    Basexx (const t_base base);

    /// destroy this base codec
    ~Basexx (void);
    
    /// @return the class name
    String repr (void) const override;

    /// reset the base codec
    void reset (void) override;

    /// clear the base codec
    void clear (void) override;

    /// set the split line mode
    /// @param slmd the split mode
    virtual void setslmd (const bool slmd);

    /// @return the split line mode
    virtual bool getslmd (void) const;
    
  protected:
    /// encode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    long encode (Buffer& ob, Buffer& ib) override;

    /// encode an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    long encode (Buffer& ob, InputStream& is) override;

    /// encode an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    long encode (OutputStream& os, InputStream& is) override;

    /// decode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    long decode (Buffer& ob, Buffer& ib) override;

    /// decode an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    long decode (Buffer& ob, InputStream& is) override;

    /// decode an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    long decode (OutputStream& os, InputStream& is) override;

    /// preset the stream processing
    long preset (void) override;

    /// finish the stream processing
    long finish (void) override;
    
  private:
    // make the copy constructor private
    Basexx (const Basexx&) =delete;
    // make the assignement operator private
    Basexx& operator = (const Basexx&) =delete;

  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
