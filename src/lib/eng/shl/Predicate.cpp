// ---------------------------------------------------------------------------
// - Predicate.cpp                                                           -
// - afnix engine - predicate builtin functions implementation               -
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

#include "Set.hpp"
#include "Byte.hpp"
#include "Real.hpp"
#include "Form.hpp"
#include "List.hpp"
#include "Enum.hpp"
#include "Item.hpp"
#include "Fifo.hpp"
#include "Heap.hpp"
#include "Tree.hpp"
#include "Queue.hpp"
#include "Regex.hpp"
#include "Plist.hpp"
#include "Lockf.hpp"
#include "Buffer.hpp"
#include "Combo.hpp"
#include "Loader.hpp"
#include "Thrset.hpp"
#include "Reader.hpp"
#include "Strvec.hpp"
#include "Vector.hpp"
#include "Bitset.hpp"
#include "Locale.hpp"
#include "Logger.hpp"
#include "Symbol.hpp"
#include "Future.hpp"
#include "Numeral.hpp"
#include "Message.hpp"
#include "Options.hpp"
#include "Lexical.hpp"
#include "Condvar.hpp"
#include "Relatif.hpp"
#include "Builtin.hpp"
#include "Boolean.hpp"
#include "Promise.hpp"
#include "Closure.hpp"
#include "Counter.hpp"
#include "Constant.hpp"
#include "Resolver.hpp"
#include "Instance.hpp"
#include "Structure.hpp"
#include "Exception.hpp"
#include "Character.hpp"
#include "HashTable.hpp"
#include "Qualified.hpp"
#include "Librarian.hpp"
#include "AliasTable.hpp"
#include "PrintTable.hpp"
#include "BlockBuffer.hpp"

namespace afnix {

  // this procedure checks that we have one argument only and returns
  // the evaluated object
  static inline Object* get_obj (Evaluable* zobj, Nameset* nset, Cons* args,
				 const String& pname) {
    if ((args == nullptr) || (args->length () != 1))
      throw Exception ("argument-error", "illegal arguments with predicate",
		       pname);
    Object* car = args->getcar ();
    Object* obj = (car == nullptr) ? nullptr : car->eval (zobj,nset);
    return obj;
  }

  // nilp: nilp predicate

  Object* Builtin::sfnilp (Evaluable* zobj, Nameset* nset, Cons* args) {
    if (args == nullptr) return new Boolean (true);
    Object* obj = get_obj (zobj, nset, args, "nil-p");
    bool result = (obj == nullptr) ? true : false;
    Object::cref (obj);
    return new Boolean (result);
  }

  // objp: object predicate

  Object* Builtin::sfobjp (Evaluable* zobj, Nameset* nset, Cons* args) {
    if (args == nullptr) return new Boolean (false);
    Object* obj = get_obj (zobj, nset, args, "object-p");
    bool result = (obj == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // evlp: eval predicate

  Object* Builtin::sfevlp (Evaluable* zobj, Nameset* nset, Cons* args) {
    try {
      Object* obj = get_obj (zobj, nset, args, "eval-p");
      Object::cref (obj);
      return new Boolean (true);
    } catch (Exception&) {
      return new Boolean (false);
    }
  }
  
  // cblp: collectable predicate

  Object* Builtin::sfcblp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "collectable-p");
    bool result = (dynamic_cast <Collectable*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // srlp: serial predicate

  Object* Builtin::sfsrlp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "serial-p");
    bool result = (dynamic_cast <Serial*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // nblp: nameable predicate

  Object* Builtin::sfnblp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "nameable-p");
    bool result = (dynamic_cast <Nameable*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // symp: symbol predicate

  Object* Builtin::sfsymp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "symbol-p");
    bool result = (dynamic_cast <Symbol*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lexp: lexical predicate

  Object* Builtin::sflexp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "lexical-p");
    bool result = (dynamic_cast <Lexical*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // setp: set predicate

  Object* Builtin::sfsetp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "set-p");
    bool result = (dynamic_cast <Set*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // clop: closure predicate

  Object* Builtin::sfclop (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "closure-p");
    bool result = (dynamic_cast <Closure*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // quap: qualified predicate

  Object* Builtin::sfqualp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "qualified-p");
    bool result = (dynamic_cast <Qualified*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // nump: number predicate

  Object* Builtin::sfnump (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "number-p");
    bool result = (dynamic_cast <Number*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);

  }

  // intp: integer predicate

  Object* Builtin::sfintp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "integer-p");
    bool result = (dynamic_cast <Integer*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rltp: relatif predicate

  Object* Builtin::sfrltp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "relatif-p");
    bool result = (dynamic_cast <Relatif*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // bytep: byte predicate

  Object* Builtin::sfbytep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "byte-p");
    bool result = (dynamic_cast <Byte*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // realp: real predicate

  Object* Builtin::sfrealp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "real-p");
    bool result = (dynamic_cast <Real*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // numrp: numeral predicate

  Object* Builtin::sfnumrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "numeral-p");
    bool result = (dynamic_cast <Numeral*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // boolp: boolean predicate

  Object* Builtin::sfboolp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "boolean-p");
    bool result = (dynamic_cast <Boolean*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // charp: character predicate

  Object* Builtin::sfcharp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "character-p");
    bool result = (dynamic_cast <Character*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // strp: string predicate

  Object* Builtin::sfstrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "string-p");
    bool result = (dynamic_cast <String*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // litp: literal predicate

  Object* Builtin::sflitp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "literal-p");
    bool result = (dynamic_cast <Literal*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // cstp: constant predicate

  Object* Builtin::sfcstp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "constant-p");
    bool result = (dynamic_cast <Constant*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // bufp: buffer predicate

  Object* Builtin::sfbufp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "buffer-p");
    bool result = (dynamic_cast <Buffer*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
   
  // stvp: string vector predicate

  Object* Builtin::sfstvp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "strvec-p");
    bool result = (dynamic_cast <Strvec*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // vecp: vector predicate

  Object* Builtin::sfvecp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "vector-p");
    bool result = (dynamic_cast <Vector*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // consp: cons predicate

  Object* Builtin::sfconsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "cons-p");
    bool result = (dynamic_cast <Cons*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // formp: form predicate

  Object* Builtin::sfformp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "form-p");
    bool result = (dynamic_cast <Form*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // listp: list predicate

  Object* Builtin::sflistp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "list-p");
    bool result = (dynamic_cast <List*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // cmbp: combo predicate

  Object* Builtin::sfcmbp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "combo-p");
    bool result = (dynamic_cast <Combo*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }


  // nstp: nameset predicate

  Object* Builtin::sfnstp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "nameset-p");
    bool result = (dynamic_cast <Nameset*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // prmp: promise predicate

  Object* Builtin::sfprmp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "promise-p");
    bool result = (dynamic_cast <Promise*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lbrnp: librarian predicate

  Object* Builtin::sflbrnp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "librarian-p");
    bool result = (dynamic_cast <Librarian*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // clsp: class predicate

  Object* Builtin::sfclsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "class-p");
    bool result = (dynamic_cast <Class*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // instp: instance predicate

  Object* Builtin::sfinstp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "instance-p");
    bool result = (dynamic_cast <Instance*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ashp: hash table predicate

  Object* Builtin::sfashp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "hashtable-p");
    bool result = (dynamic_cast <HashTable*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // thrp: thread predicate

  Object* Builtin::sfthrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "thread-p");
    bool result = (dynamic_cast <Thread*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // logp: logger predicate

  Object* Builtin::sflogp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "logger-p");
    bool result = (dynamic_cast <Logger*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // bbufp: block buffer predicate

  Object* Builtin::sfbbufp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "block-buffer-p");
    bool result = (dynamic_cast <BlockBuffer*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hashp: hash table predicate

  Object* Builtin::sfhashp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "hash-p");
    bool result = (dynamic_cast <HashTable*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // fifop: fifo predicate

  Object* Builtin::sffifop (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "fifo-p");
    bool result = (dynamic_cast <Fifo*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // heapp: heap predicate

  Object* Builtin::sfheapp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "heap-p");
    bool result = (dynamic_cast <Heap*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // treep: tree predicate

  Object* Builtin::sftreep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "tree-p");
    bool result = (dynamic_cast <Tree*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // enump: enum predicate

  Object* Builtin::sfenump (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "enum-p");
    bool result = (dynamic_cast <Enum*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // itemp: item predicate

  Object* Builtin::sfitemp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "enum-p");
    bool result = (dynamic_cast <Item*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // queuep: queue predicate

  Object* Builtin::sfqueuep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "queue-p");
    bool result = (dynamic_cast <Queue*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // bitset predicate

  Object* Builtin::sfbitsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "bitset-p");
    bool result = (dynamic_cast <Bitset*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // condvar predicate

  Object* Builtin::sfcondp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "condvar-p");
    bool result = (dynamic_cast <Condvar*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // loader predicate

  Object* Builtin::sfloadp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "loader-p");
    bool result = (dynamic_cast <Loader*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // resolver predicate

  Object* Builtin::sfrslvp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "resolver-p");
    bool result = (dynamic_cast <Resolver*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // viewable predicate

  Object* Builtin::sfviewp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "viewable-p");
    bool result = (dynamic_cast <Viewable*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // structure predicate

  Object* Builtin::sfstrcp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "structure-p");
    bool result = (dynamic_cast <Structure*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // locale predicate
  
  Object* Builtin::sflocalp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "locale-p");
    bool result = (dynamic_cast <Locale*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // regex predicate

  Object* Builtin::sfregexp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "regex-p");
    bool result = (dynamic_cast <Regex*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // alias-table predicate

  Object* Builtin::sfatblp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "alias-table-p");
    bool result = (dynamic_cast <AliasTable*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // print-table predicate

  Object* Builtin::sfptblp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "print-table-p");
    bool result = (dynamic_cast <PrintTable*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // message predicate

  Object* Builtin::sfmesgp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "message-p");
    bool result = (dynamic_cast <Message*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // property predicate

  Object* Builtin::sfpropp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "property-p");
    bool result = (dynamic_cast <Property*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // plist predicate

  Object* Builtin::sfplistp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "plist-p");
    bool result = (dynamic_cast <Plist*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // counter predicate

  Object* Builtin::sfcntrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "counter-p");
    bool result = (dynamic_cast <Counter*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // thread set predicate

  Object* Builtin::sfthrsp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "thrset-p");
    bool result = (dynamic_cast <Thrset*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lexer predicate

  Object* Builtin::sflexerp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "lexer-p");
    bool result = (dynamic_cast <Lexer*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // style predicate

  Object* Builtin::sfstylep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "style-p");
    bool result = (dynamic_cast <Style*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // former predicate

  Object* Builtin::sfformrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "former-p");
    bool result = (dynamic_cast <Former*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // reader predicate
  
  Object* Builtin::sfreadrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "reader-p");
    bool result = (dynamic_cast <Reader*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lock file predicate
  
  Object* Builtin::sflockfp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "lockf-p");
    bool result = (dynamic_cast <Lockf*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // future predicate
  
  Object* Builtin::sffuturp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, " future-p");
    bool result = (dynamic_cast <Future*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
