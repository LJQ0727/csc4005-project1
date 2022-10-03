num_elems := 10000

default: sequential

sequential: ssort
	./ssort $(num_elems) ./test_data/$(num_elems)a.in

ssort: odd_even_sequential_sort.cpp
	g++ -g odd_even_sequential_sort.cpp -o ssort

generate:
	./gen $(num_elems) ./test_data/$(num_elems)a.in
	echo generated $(num_elems)
