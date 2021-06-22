// ---------------------------------------------------------------------------
// - Ivector.hpp                                                             -
// - afnix:mth module - integer vector definitions                           -
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

#ifndef  AFNIX_IVECTOR_HPP
#define  AFNIX_IVECTOR_HPP

#ifndef  AFNIX_IVI_HPP
#include "Ivi.hpp"
#endif
 
#ifndef  AFNIX_VIEWABLE_HPP
#include "Viewable.hpp"
#endif
 
namespace afnix {

  /// This Ivector class is the default implementation of the integer vector
  /// interface. Internally, the vector is represented as an array of long.
  /// @author amaury darsch

  class Ivector : public Ivi, public Viewable {
  public:
    /// generate a random vector by size
    /// @param size the vector size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static Ivector* random (const t_long size,
			    const long   rmin, const long rmax);

    /// add a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar argument
    friend Ivector operator + (const Ivector& x, const long s);

    /// add a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    friend Ivector operator + (const Ivector& x, const Ivector& y);

    /// substract a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar argument
    friend Ivector operator - (const Ivector& x, const long s);

    /// substract a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    friend Ivector operator - (const Ivector& x, const Ivector& y);

    /// multiply a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar argument
    friend Ivector operator * (const Ivector& x, const long s);

  protected:
    /// the vector elements
    long* p_vtab;

  public:
    /// create a null vector
    Ivector (void);

    /// create a vector by size
    /// @param size the vector size
    Ivector (const t_long size);

    /// copy construct this vector
    /// @param that the object to copy
    Ivector (const Ivector& that);

    /// destroy this vector
    ~Ivector (void);

    /// assign a vector to this one
    /// @param that the object to assign
    Ivector& operator = (const Ivector& that);

    /// compare two vectors
    /// @param  x the vector argument
    /// @return true if they are equals
    bool operator == (const Ivi& x) const;
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// resize this vector
    /// @param size the new vector size
    void resize (const t_long size);

    /// reset this vector
    void reset (void);

    /// clear this vector
    void clear (void);

    /// preset this vector
    void preset (void);

    /// copy a vector into this one
    /// @param x the vector to copy
    Ivi& cpy (const Ivi& x);

    /// add a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar factor
    Ivi& add (const Ivi& x, const long s);

    /// add a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    Ivi& add (const Ivi& x, const Ivi& y);

    /// add a vector with another scaled one
    /// @param x the vector argument
    /// @param y the vector argument
    /// @param s the scalar factor
    Ivi& add (const Ivi& x, const Ivi& y, const long s);

    /// substract a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar factor
    Ivi& sub (const Ivi& x, const long s);

    /// substract a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    Ivi& sub (const Ivi& x, const Ivi& y);

    /// multiply a vector with a scaled vector
    /// @param x the vector to multiply
    /// @param s the scaling factor
    Ivi& mul (const Ivi& x, const long s);

    /// multiply a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    Ivi& mul (const Ivi& x, const Ivi& y);

    /// add equal with a vector
    /// @param x the vector to add
    Ivi& aeq (const Ivi& x);

    /// add equal with a scaled vector
    /// @param x the vector to add
    /// @param s the scaling factor
    Ivi& aeq (const Ivi& x, const long s);

    /// resize equal with a scaled vector
    /// @param x the vector to add
    /// @param s the scaling factor
    Ivi& req (const Ivi& x, const long s);

    /// permutate this vector
    /// @param p the permutation object
    Ivi* permutate (const Cpi& p) const;
    
    /// reverse this vector permutation
    /// @param p the permutation object
    Ivi* reverse (const Cpi& p) const;

    /// @return the viewable size
    long tosize (void) const;

    /// @return the viewable data
    t_byte* tobyte (void);
    
    /// @return the viewable data
    const t_byte* tobyte (void) const;
    
  public:
    /// no lock - set a vector by position
    /// @param pos the vector position
    /// @param val the value to set
    void nlset (const t_long pos, const long val);

    /// no lock - get a vector value by position
    /// @param pos the vector position
    long nlget (const t_long pos) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object);
  };
}

#endif
