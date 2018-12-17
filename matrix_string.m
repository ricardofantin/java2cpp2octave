function output = matrix_string(input_matrix, str = "square")
  if strcmp("square", str)
    output = input_matrix.^2;
  else
    output = input_matrix*2;
  endif
endfunction
