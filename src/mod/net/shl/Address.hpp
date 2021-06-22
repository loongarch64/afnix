// ---------------------------------------------------------------------------
// - Address.hpp                                                             -
// - afnix:net module - address class definition                             -
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

#ifndef  AFNIX_ADDRESS_HPP
#define  AFNIX_ADDRESS_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Address class is the base class of the afnix networking environment
  /// which manipulates internet (ip) addresses. The address can be either
  /// a qualified name, or a family address. Currently the ip version 4 and 6
  /// are valid addresses. When constructed, the class manages a primary
  /// canonical name as well as a primary address. Eventually, aliases can be 
  /// added in the form of name/address pair value
  /// @author amaury darsch

  class Address : public Object {
  public:
    /// @return the loopback name
    static String loopname (void);

    /// get the tcp service port by name
    /// @param name the service name
    static t_word tservice (const String& name);

    /// get the udp service port by name
    /// @param name the service name
    static t_word uservice (const String& name);

    /// check for a numerical ip address
    /// @param name the address name
    /// @param sapg the string prototol family
    static bool isipn (const String& name, const String& sapf);
    
  private:
    /// the original name
    String  d_name;
    /// the canonical name
    String  d_cnam;
    /// the address bytes
    t_byte* p_addr;
    /// the alias structure
    struct s_alias {
      /// the alias name
      String  d_anam;
      /// the alias addresses
      t_byte* p_aadr;
      s_alias (void) {
	p_aadr = nullptr;
      }
      ~s_alias (void) {
	delete [] p_aadr;
      }
    };
    /// the alias size
    long     d_size;
    /// the alias array
    s_alias* p_aals;

  public:
    /// create a local address
    Address (void);

    /// create an address by name
    /// @param name the host name to use
    Address (const String& name);

    /// create an address by name and reverse lookup
    /// @param name the host name to use
    /// @param rflg the reverse lookup flag
    Address (const String& name, const bool rflg);

    /// create an address by name, reverse lookup and protocol flag
    /// @param name the host name to use
    /// @param rflg the reverse lookup flag
    /// @param sapf the string protocol family
    Address (const String& name, const bool rflg, const String& sapf);

    /// create an address by bytes
    /// @param addr the address bytes to use
    Address (const t_byte* addr);

    /// create an address by bytes and resolv flag
    /// @param addr the address bytes to use
    /// @param rflg the reverse lookup flag
    Address (const t_byte* addr, const bool rflg);

    /// copy construct an address
    /// @param that the address to copy
    Address (const Address& that);

    /// destroy this address class
    ~Address (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this address
    Object* clone (void) const;

    /// assign an address to this one
    /// @param that the address to assign
    Address& operator = (const Address& that);

    /// prefix add one position to the address
    Address& operator ++ (void);

    /// postfix add one position to the address
    Address operator ++ (int);

    /// compare two addresses
    /// @param addr the address to compare
    bool operator == (const Address& addr) const;

    /// compare two addresses
    /// @param addr the address to compare
    bool operator != (const Address& addr) const;

    /// compare two addresses
    /// @param addr the address to compare
    bool operator < (const Address& addr) const;

    /// compare two addresses
    /// @param addr the address to compare
    bool operator <= (const Address& addr) const;

    /// compare two addresses
    /// @param addr the address to compare
    bool operator > (const Address& addr) const;

    /// compare two addresses
    /// @param addr the address to compare
    bool operator >= (const Address& addr) const;

    /// update with the next address but do not resolve
    void next (void);

    /// resolve an address by flag
    /// @param rflg the reverse lookup flag
    void resolve (const bool rflg);

    /// resolve an address by name and resolve flag
    /// @param name the host name to use
    /// @param rflg the reverse lookup flag
    void resolve (const String& name, const bool rflg);

    /// resolve an address by name and resolve flag
    /// @param name the host name to use
    /// @param rflg the reverse lookup flag
    /// @param sapf the string protocol family
    void resolve (const String& name, const bool rflg, const String& sapf);

    /// resolve an address by bytes and resolve flag
    /// @param addr the address bytes to use
    /// @param rflg the reverse lookup flag
    void resolve (const t_byte* addr, const bool rflg);

    /// @return the address canonical name
    String getcanon (void) const;

    /// @return the address representation
    String getaddr (void) const;

    /// @return a integer vector representation
    Vector* getvmap (void) const;

    /// @return the alias size
    long getsize (void) const;

    /// @return the address canonical name by index
    String getcanon (const long index) const;

    /// @return the address representation by index
    String getaddr (const long index) const;

    /// @return a integer vector representation by index
    Vector* getvmap (const long index) const;
  
  private:
    // make the socket clas a friend
    friend class Socket;
    // make the udp socket class a friend
    friend class UdpSocket;
    // make the tcp socket class a friend
    friend class TcpSocket;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object);

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
