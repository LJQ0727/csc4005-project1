num_elems := 50000
num_procs := 1

default: parallel

# ------ Parallel -------
parallel: psort generate
	mpirun -np $(num_procs) ./psort $(num_elems) ./test_data/$(num_elems)a.in

psort: odd_even_parallel_sort.cpp
	mpic++ odd_even_parallel_sort.cpp -o psort

# ------ Sequential --------
sequential: ssort generate
	./ssort $(num_elems) ./test_data/$(num_elems)a.in

ssort: odd_even_sequential_sort.cpp
	g++ -O1 odd_even_sequential_sort.cpp -o ssort

generate:
	./gen $(num_elems) ./test_data/$(num_elems)a.in
	echo generated $(num_elems)

test: 
	./check $(num_elems) ./test_data/$(num_elems)a.in.out
