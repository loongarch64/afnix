// ---------------------------------------------------------------------------
// - TlsProto.hpp                                                            -
// - afnix:tls service - tls protocol class definition                       -
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

#ifndef  AFNIX_TLSPROTO_HPP
#define  AFNIX_TLSPROTO_HPP

#ifndef  AFNIX_TLSSTATE_HPP
#include "TlsState.hpp"
#endif

#ifndef  AFNIX_TLSHBLOCK_HPP
#include "TlsHblock.hpp"
#endif

#ifndef  AFNIX_TLSCBLOCK_HPP
#include "TlsCblock.hpp"
#endif

#ifndef  AFNIX_TLSMESSAGE_HPP
#include "TlsMessage.hpp"
#endif

namespace afnix {

  /// The TlsProto class is the tls record/message protocol decoder/encoder.
  /// This class is designed to be derived for supporting extra protocols. By
  /// default, the protocol of the tls 3.1/3.2 is implemented for reference.
  /// Note that creating a protocol by state entails the instanciation of the
  /// right protocol.
  /// @author amaury darsch

  class TlsProto : public Object {
  public:
    /// create a tls protocol by version
    /// @param vmaj the tls major version
    /// @param vmin the tls minor version
    static TlsProto* create (const t_byte vmaj, const t_byte vmin);

    /// create a tls protocol by state
    /// @param sta the tls state
    static TlsProto* create (TlsState* sta);

    /// check for a handshake message
    /// @param iblk the nifo block
    /// @param type the message type to check
    static bool ishblk (TlsInfos* iblk, const t_byte type);
    
  public:
    /// create a default protocol
    TlsProto (void) =default;

    /// @return the class name
    String repr (void) const;

    /// compute a hash buffer by prf
    /// @param sbuf the secret buffer
    /// @param labl the prf label
    /// @param seed the prf seed
    /// @param rlen the prf result size
    virtual Buffer xprf (const Buffer& sbuf, const String& labl,
			 const Buffer& seed, const long    rlen) const;
    
    /// compute the server verify buffer
    /// @param mbuf the master secret
    /// @param vbuf the verify buffer 
    virtual Buffer sfin (const Buffer& mbuf, const Buffer& vbuf) const;
    
    /// compute the client verify buffer
    /// @param mbuf the master secret
    /// @param vbuf the verify buffer 
    virtual Buffer cfin (const Buffer& mbuf, const Buffer& vbuf) const;
    
    /// get a record by input stream and cipher
    /// @param is  the input strem to read
    /// @param sta the tls state
    virtual TlsRecord* getrcd (InputStream* is, TlsState* sta) const;

    /// get a message by record
    /// @param rcd the record to decode
    virtual TlsMessage* getmsg (TlsRecord* rcd) const;

    /// get a message by input stream and state
    /// @param is the input strem to read
    /// @param sta the tls state
    virtual TlsMessage* getmsg (InputStream* is, TlsState* sta) const;

    /// encode a tls message
    /// @param os  the output stream
    /// @param msg the message to encode
    virtual long encode (OutputStream* os, TlsMessage* msg) const;

    /// encode a tls message
    /// @param os  the output stream
    /// @param msg the message to encode
    /// @param sta the tls state
    virtual long encode (OutputStream* os,TlsMessage* msg,TlsState* sta) const;

    /// decode a handshake block
    /// @param hblk the block to decode
    virtual TlsInfos* decode (TlsHblock* hblk) const;

    /// receive a client hello by input stream
    /// @param is the input stream to read
    /// @param sta the tls state
    virtual TlsInfos* rcvchlo (InputStream* is, TlsState* sta) const;

    /// receive a server hello by input stream
    /// @param is the input stream to read
    /// @param sta the tls state
    virtual TlsInfos* rcvshlo (InputStream* is, TlsState* sta) const;

    /// receive a client handshake by input stream
    /// @param is the input stream to read
    /// @param sta the tls state
    virtual TlsHblock* rcvchsk (InputStream* is, TlsState* sta) const;

    /// receive a server hanshake by input stream
    /// @param is the input stream to read
    /// @param sta the tls state
    virtual TlsHblock* rcvshsk (InputStream* is, TlsState* sta) const;
    
    /// receive a client change cipher spec by input stream
    /// @param is the input stream to read
    /// @param sta the tls state
    virtual bool rcvcccs (InputStream* is, TlsState* sta) const;

    /// receive a server change cipher spec by input stream
    /// @param is the input stream to read
    /// @param sta the tls state
    virtual bool rcvsccs (InputStream* is, TlsState* sta) const;

    /// get a server hello chunk by state
    /// @param sta the tls state
    virtual TlsChunk toshlo (TlsState* sta) const;
    
    /// get a server hello message by state
    /// @param sta the tls state
    virtual TlsMessage* getshlo (TlsState* sta) const;

    /// send a server hello by state
    /// @param os  the output stream
    /// @param sta the tls state
    virtual void sndshlo (OutputStream* os, TlsState* sta) const;

    /// get a client hello chunk by state
    /// @param sta the tls state
    virtual TlsChunk tochlo (TlsState* sta) const;
    
    /// get a client hello message by state
    /// @param sta the tls state
    virtual TlsMessage* getchlo (TlsState* sta) const;

    /// send a client hello by state
    /// @param os  the output stream
    /// @param sta the tls state
    virtual void sndchlo (OutputStream* os, TlsState* sta) const;
    
    /// get a client key exchange chunk by state
    /// @param sta the tls state
    virtual TlsChunk tockxh (TlsState* sta) const;

    /// get a client key exhange message by state
    /// @param sta the tls state
    virtual TlsMessage* getckxh (TlsState* sta) const;

    /// send a client key exchange by state
    /// @param os  the output stream
    /// @param sta the tls state
    virtual void sndckxh (OutputStream* os, TlsState* sta) const;

    /// get a client change cipher spec message by state
    /// @param sta the tls state
    virtual TlsMessage* getcccs (TlsState* sta) const;

    /// send a client change cipher spec by state
    /// @param os  the output stream
    /// @param sta the tls state
    virtual bool sndcccs (OutputStream* os, TlsState* sta) const;
    
    /// get a server certificate message by state
    /// @param sta the tls state
    virtual TlsMessage* getscrt (TlsState* sta) const;

    /// send a certificate by state
    /// @param os  the output stream
    /// @param sta the tls state
    virtual void sndscrt (OutputStream* os, TlsState* sta) const;

    /// get a server done message by state
    /// @param sta the tls state
    virtual TlsMessage* getsdon (TlsState* sta) const;

    /// send a server done by state
    /// @param os  the output stream
    /// @param sta the tls state
    virtual void sndsdon (OutputStream* os, TlsState* sta) const;

    /// get a server change cipher spec by state
    /// @param sta the tls state
    virtual TlsMessage* getsccs (TlsState* sta) const;

    /// send a server change cipher spec by state
    /// @param os  the output stream
    /// @param sta the tls state
    virtual bool sndsccs (OutputStream* os, TlsState* sta) const;

    /// get a client/server finished message by state
    /// @param sta the tls state
    virtual TlsMessage* getxfin (TlsState* sta) const;

    /// send a client/server finished by state
    /// @param os  the output stream
    /// @param sta the tls state
    virtual void sndxfin (OutputStream* os, TlsState* sta) const;

    /// pop the next available buffer
    /// @param is  the output stream
    /// @param sta the tls state
    virtual Buffer popb (InputStream* is, TlsState* sta) const;

    /// push a buffer to the stream
    /// @param os  the output stream
    /// @param sta the tls state
    /// @param buf the buffer to push
    virtual long pushb (OutputStream* os, TlsState* sta,
			const Buffer& buf) const;
    
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
