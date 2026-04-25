module b

  integer, parameter :: val2 = 20

  interface
    module function add1(arg) result(u)
      integer, intent(in) :: arg
      integer :: u
    end function add1
  end interface

end module b
