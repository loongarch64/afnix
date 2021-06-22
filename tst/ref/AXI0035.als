# ---------------------------------------------------------------------------
# - AXI0035.als                                                              -
# - afnix engine test module                                                -
# ---------------------------------------------------------------------------
# - This program is free software;  you can redistribute it  and/or  modify -
# - it provided that this copyright notice is kept intact.                  -
# -                                                                         -
# - This program  is  distributed in  the hope  that it will be useful, but -
# - without  any  warranty;  without  even   the   implied    warranty   of -
# - merchantability or fitness for a particular purpose.  In no event shall -
# - the copyright holder be liable for any  direct, indirect, incidental or -
# - special damages arising in any way out of the use of this software.     -
# ---------------------------------------------------------------------------
# - copyright (c) 1999-2021 amaury darsch                                   -
# ---------------------------------------------------------------------------

# @info   exception test module
# @author amaury darsch

# basic test
assert 3       (try (+ 1 2))
assert nil     (try (throw))
assert nil     (try (throw "hello"))
assert nil     (try (throw "hello" "world"))
assert "folks" (try (throw "hello" "world" "folks"))


assert "hello" (try (throw "hello")              (eval what:eid))
assert "world" (try (throw "hello" "world")      (eval what:reason))
assert true    (try (throw "hello" "world" true) (eval what:object))

assert true (try (throw "hello" "world") {
    assert "hello" what:eid
    assert "world" what:reason
    eval true
})

# basic function exception
const fact (n) {
  if (not (integer-p n)) (throw "number-error" "invalid argument in fact")
  if (== n 0) 1 (* n (fact (- n 1)))
}

assert 120 (try (fact 5) 0)
assert 0   (try (fact "hello") 0)
