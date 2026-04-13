module b; &
  iNTeger, parameter :: b1 = 1; end module b !acomment
!anotherone
module c
  !anothercomment
end module c
module a
  use b
  use, intrinsic :: ISO_FORTRAN_ENV; &
  use, NON_intrinsic :: c
  integer, parameter :: dp = kind(1.0d0)
  interface
    module subroutine sub1()
    end subroutine
  end interface
end module a
submodule(a) as;&
iNTeger, parameter :: a1 = 1
end submodule
submodule(a: &
          as) &
  ass
integer, parameter :: as1 = 5
end submodule
&
