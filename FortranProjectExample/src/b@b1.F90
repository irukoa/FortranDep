submodule(b) b1

contains

function add1(arg) result(u)
  integer, intent(in) :: arg
  integer :: u
  u = arg + 1
end function add1

end submodule b1
