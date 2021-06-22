// ---------------------------------------------------------------------------
// - Rvector.hpp                                                             -
// - afnix:mth module - real vector definitions                              -
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

#ifndef  AFNIX_RVECTOR_HPP
#define  AFNIX_RVECTOR_HPP

#ifndef  AFNIX_RVI_HPP
#include "Rvi.hpp"
#endif
 
#ifndef  AFNIX_VIEWABLE_HPP
#include "Viewable.hpp"
#endif
 
namespace afnix {

  /// This Rvector class is the default implementation of the real vector
  /// interface. Internally, the vector is represented as an array of t_real.
  /// @author amaury darsch

  class Rvector : public Rvi, public Viewable {
  public:
    /// generate a random vector by size
    /// @param size the vector size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static Rvector* random (const t_long size, 
			    const t_real rmin, const t_real rmax);

    /// add a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar argument
    friend Rvector operator + (const Rvector& x, const t_real s);

    /// add a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    friend Rvector operator + (const Rvector& x, const Rvector& y);

    /// substract a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar argument
    friend Rvector operator - (const Rvector& x, const t_real s);

    /// substract a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    friend Rvector operator - (const Rvector& x, const Rvector& y);

    /// multiply a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar argument
    friend Rvector operator * (const Rvector& x, const t_real s);

    /// divide a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar argument
    friend Rvector operator / (const Rvector& x, const t_real s);

  protected:
    /// the vector elements
    t_real* p_vtab;

  public:
    /// create a null vector
    Rvector (void);

    /// create a vector by size
    /// @param size the vector size
    Rvector (const t_long size);

    /// copy construct this vector
    /// @param that the object to copy
    Rvector (const Rvector& that);

    /// destroy this vector
    ~Rvector (void);

    /// assign a vector to this one
    /// @param that the object to assign
    Rvector& operator = (const Rvector& that);

    /// compare two vectors
    /// @param  x the vector argument
    /// @return true if they are equals
    bool operator == (const Rvi& x) const;

    /// compute the vector dot product
    /// @param x the vector argument
    t_real operator ^ (const Rvi& x) const;
    
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

    /// @return the vector norm
    t_real norm (void) const;

    /// copy a vector into this one
    /// @param x the vector to copy
    Rvi& cpy (const Rvi& x);

    /// add a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar factor
    Rvi& add (const Rvi& x, const t_real s);

    /// add a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    Rvi& add (const Rvi& x, const Rvi& y);

    /// add a vector with another scaled one
    /// @param x the vector argument
    /// @param y the vector argument
    /// @param s the scalar factor
    Rvi& add (const Rvi& x, const Rvi& y, const t_real s);

    /// substract a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar factor
    Rvi& sub (const Rvi& x, const t_real s);

    /// substract a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    Rvi& sub (const Rvi& x, const Rvi& y);

    /// multiply a vector with a scaled vector
    /// @param x the vector to multiply
    /// @param s the scaling factor
    Rvi& mul (const Rvi& x, const t_real s);

    /// multiply a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    Rvi& mul (const Rvi& x, const Rvi& y);

    /// divide a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    Rvi& div (const Rvi& x, const Rvi& y);

    /// add equal with a vector
    /// @param x the vector to add
    Rvi& aeq (const Rvi& x);

    /// add equal with a scaled vector
    /// @param x the vector to add
    /// @param s the scaling factor
    Rvi& aeq (const Rvi& x, const t_real s);

    /// resize equal with a scaled vector
    /// @param x the vector to add
    /// @param s the scaling factor
    Rvi& req (const Rvi& x, const t_real s);

    /// permutate this vector
    /// @param p the permutation object
    Rvi* permutate (const Cpi& p) const;
    
    /// reverse this vector permutation
    /// @param p the permutation object
    Rvi* reverse (const Cpi& p) const;

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
    void nlset (const t_long pos, const t_real val);

    /// no lock - get a vector value by position
    /// @param pos the vector position
    t_real nlget (const t_long pos) const;

  private:
    // make the matrix class a friend
    friend class Rmatrix;

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
