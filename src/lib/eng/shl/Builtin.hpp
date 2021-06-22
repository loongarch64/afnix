// ---------------------------------------------------------------------------
// - Builtin.hpp                                                             -
// - afnix engine - builtin functions definitions                            -
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

#ifndef  AFNIX_BUILTIN_HPP
#define  AFNIX_BUILTIN_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif

namespace afnix {
  /// The Builtin contains the definitions of all special forms used by the
  /// interpreter. These forms are bound by the interpreter and used during
  /// the read-eval loop.
  /// @author amaury darsch

  class Builtin {
  public:
    // reserved keywords
    static Object* sfif      (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfdo      (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sffor     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sftry     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfeval    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfsync    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfloop    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfenum    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfconst   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sftrans   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfunref   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfclass   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfblock   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfwhile   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfgamma   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfthrow   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfforce   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfdelay   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sffuture  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflaunch  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflambda  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfswitch  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfreturn  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfprotect (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfnameset (Evaluable* zobj, Nameset* nset, Cons* args);

    // standard operators
    static Object* sfadd     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfsub     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfmul     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfdiv     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfeql     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfneq     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfgeq     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfgth     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfleq     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflth     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfassert  (Evaluable* zobj, Nameset* nset, Cons* args);

    // logical operator
    static Object* sfor      (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfnot     (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfand     (Evaluable* zobj, Nameset* nset, Cons* args);

    // standard predicates
    static Object* sfnilp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfobjp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfevlp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfsrlp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfnblp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfcblp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflexp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfsymp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfsetp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfclop    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflitp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfcstp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfnump    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfintp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfrltp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfstrp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfbufp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfstvp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfvecp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfcmbp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfnstp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfashp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfclsp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfprmp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfthrp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflogp    (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfbbufp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfhashp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sffifop   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfheapp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sftreep   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflbrnp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfqualp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflistp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfinstp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfbytep   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfrealp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfnumrp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfboolp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfcharp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfconsp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfformp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfenump   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfitemp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfbitsp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfcondp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfatblp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfptblp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfmesgp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfpropp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfcntrp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfthrsp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfloadp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfrslvp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfviewp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfstrcp   (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflocalp  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfregexp  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfqueuep  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfplistp  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflexerp  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfstylep  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfformrp  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sfreadrp  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sflockfp  (Evaluable* zobj, Nameset* nset, Cons* args);
    static Object* sffuturp  (Evaluable* zobj, Nameset* nset, Cons* args);
  };
}

#endif
