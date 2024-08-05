ex1:
	@gcc -o build/compiler_warnings ex1_compiler_warnings.c

ex2:
	@gcc -g -o build/pwd_checker test_pwd_checker.c pwd_checker.c
	@gdb build/pwd_checker

ex5:
	@gcc -g -o build/ex5_segfault ex5_segfault.c
	@gdb build/ex5_segfault

ex7:
	@gcc -g -o build/ex7_vector ex7_vector.c ex7_test_vector.c
	@gdb build/ex7_vector

ex8:
	@gcc -g -o build/ex8_double_pointers ex8_double_pointers.c
	@gdb build/ex8_double_pointers

git:
	git add .
	git commit -m "Finish lab02 aparting from something about valgrind"
	git push