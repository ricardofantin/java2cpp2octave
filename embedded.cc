#include <iostream>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>//feval
#include <octave/interpreter.h>
#include <string>

void matrix_string_m(double matrix[][3], char *string, double result[][3]);//could use pointers instead of matrix with dimension

//interpreter to be created only once by JNI
octave::interpreter interpreter;

void createInterpreter(){
	int status = interpreter.execute ();

	if (status != 0)
	{
		std::cerr << "creating embedded Octave interpreter failed!" << std::endl;
		//error not handled
		exit(0);
	}
}

int main(){
	std::cout << "To test if octaves libraries can be found.\n";
	createInterpreter();
	double M[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	char string[] = "not_square";//change to just square
	//call the function that will be called be as a java method
	double result[3][3];
	matrix_string_m(M, string, result);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++)
			std::cout << result[i][j] << " ";
		std::cout << std::endl;
	}
}

void matrix_string_m(double matrix[][3], char *string, double result[][3]){
	//converting data
	Matrix a_matrix = Matrix (3, 3);
	for (octave_idx_type i = 0; i < 3; i++)
		for (octave_idx_type j = 0; j < 3; j++)
			a_matrix(i,j) = matrix[i][j];
	std::cout << a_matrix << std::endl;//debug, comment out
	//function argument
	octave_value_list in;
	in(0) = a_matrix;
	std::string str = std::string(string);
	in(1) = str;
	
	//call the function in file matrix_string.m
	octave_value_list out = octave::feval ("matrix_string", in, 1);
	//methods to convert from octave objects to C types in https://stackoverflow.com/questions/25442432/octave-accessing-values-in-octave-value-list
	Matrix b_matrix = out(0).array_value ();//vector_value ();

	std::cout << b_matrix << std::endl;//debug, comment out
	
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			result[i][j] = b_matrix(i,j);

}
/*
 * example from https://octave.org/doc/interpreter/Standalone-Programs.html
int
main (void)
{
  // Create interpreter.

  octave::interpreter interpreter;

  try
    {
      // Inhibit reading history file by calling
      //
      //   interpreter.initialize_history (false);

      // Set custom load path here if you wish by calling
      //
      //   interpreter.initialize_load_path (false);

      // Perform final initialization of interpreter, including
      // executing commands from startup files by calling
      //
      //   int status interpreter.initialize ();
      //
      //   if (! interpreter.initialized ())
      //     {
      //       std::cerr << "Octave interpreter initialization failed!"
      //                 << std::endl;
      //       exit (status);
      //     }
      //
      // You may skip this step if you don't need to do anything
      // between reading the startup files and telling the interpreter
      // that you are ready to execute commands.

      // Tell the interpreter that we're ready to execute commands:

      int status = interpreter.execute ();

      if (status != 0)
        {
          std::cerr << "creating embedded Octave interpreter failed!"
                    << std::endl;
          return status;
        }

      octave_idx_type n = 2;
      octave_value_list in;

      for (octave_idx_type i = 0; i < n; i++)
        in(i) = octave_value (5 * (i + 2));

      octave_value_list out = octave::feval ("gcd", in, 1);

      if (out.length () > 0)
        std::cout << "GCD of ["
                  << in(0).int_value ()
                  << ", "
                  << in(1).int_value ()
                  << "] is " << out(0).int_value ()
                  << std::endl;
      else
        std::cout << "invalid\n";
    }
  catch (const octave::exit_exception& ex)
    {
      std::cerr << "Octave interpreter exited with status = "
                << ex.exit_status () << std::endl;
    }
  catch (const octave::execution_exception&)
    {
      std::cerr << "error encountered in Octave evaluator!" << std::endl;
    }

  return 0;
}*/
