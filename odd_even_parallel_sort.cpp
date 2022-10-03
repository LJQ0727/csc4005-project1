#include <mpi.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>

// A helper function that determines whether the input number is even
inline bool is_even(int num) {
    return (num % 2) == 0;
}

int main (int argc, char **argv){

    MPI_Init(&argc, &argv); 

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int num_elements; // number of elements to be sorted
    
    num_elements = atoi(argv[1]); // convert command line argument to num_elements

    int elements[num_elements]; // store elements
    int sorted_elements[num_elements]; // store sorted elements

    if (rank == 0) { // read inputs from file (master process)
        std::ifstream input(argv[2]);
        int element;
        int i = 0;
        while (input >> element) {
            elements[i] = element;
            i++;
        }
        std::cout << "actual number of elements:" << i << std::endl;
    }

    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    std::chrono::duration<double> time_span;
    if (rank == 0){ 
        t1 = std::chrono::high_resolution_clock::now(); // record time
    }

    /* TODO BEGIN
        Implement parallel odd even transportation sort
        Code in this block is not a necessary. 
        Replace it with your own code.
        Useful MPI documentation: https://rookiehpc.github.io/mpi/docs
    */
   // within each process
    int num_my_element = num_elements / world_size; // number of elements allocated to each process
    // For the last node, there may be different elements than others
    if (rank == world_size-1) {
        num_my_element = num_elements - world_size * num_my_element;
    }
    int my_element[num_my_element]; // store elements of each process

    // here we change from scatter to scatterv, which allows variable size, 
    // so that non-evenly distribution is allowed. For example, 
    // we can now allocate 10 elements to 3 nodes, with 3, 3, 4 in each
    if (rank == 0) {
        int counts_send[world_size];
        for (int i = 0; i < world_size-1; i++) {
            counts_send[i] = num_my_element;
        }
        counts_send[world_size-1] = num_elements - world_size * num_my_element;

        int displacements[world_size];
        for (int i = 0; i < world_size; i++) {
            displacements[i] = i * num_my_element;
        }
        MPI_Scatterv(elements, counts_send, displacements, MPI_INT, my_element, num_my_element, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        // For non-root nodes, initialiize MPI_Scatterv
        MPI_Scatterv(NULL, NULL, NULL, MPI_INT, my_element, num_my_element, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // printf("I am process %d, I have %d elements\n", rank, num_my_element);
    // use `num_my_element` and `my_element`
    // BEGIN SORTING --------

    for (int iter = 0; iter < num_elements; iter++) {  // in total need to perform n times to finish the sorting
        for (int i = 0; i < num_my_element; i++) {    // inside each iteration
            // Determine whether the iteration number is odd or even
            // We assume iteration number starts from 0
            if (is_even(i)) {
                // In even iteration, do odd-even exchange

                // For each odd index item, compare with its preceding item and do sort among these 2
                for (int j = 1; j < num_my_element; j += 2) {
                    if (my_element[j] < my_element[j-1]) {
                        // Do swap when the two are not in ascending order
                        int tmp = my_element[j];
                        my_element[j] = my_element[j-1];
                        my_element[j-1] = tmp;
                    }
                }
            } else {
                // In odd iteration, do even-odd exchange

                // Send index 0 to last node for comparison, if rank != 0
                MPI_Request request;
                if (rank != 0) {
                    MPI_Isend(my_element, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
                }
                // For each even index item, compare with its preceding item and do sort among them
                for (int k = 2; k < num_my_element; k += 2) {
                    if (my_element[k] < my_element[k-1]) {
                        // Do swap when the two are not in ascending order
                        int tmp = my_element[k];
                        my_element[k] = my_element[k-1];
                        my_element[k-1] = tmp;
                    }
                }
                // For rank != 0, do wait after MPI_Isend
                if (rank != 0) {
                    MPI_Wait(&request, MPI_STATUS_IGNORE);
                }
            }
        }
    }

    // Here we change to gatherv
    if (rank == 0) {
        // For root node, MPI_Gatherv requires some more specifications
        int counts_recv[world_size];
        for (int i = 0; i < world_size-1; i++) {
            counts_recv[i] = num_my_element;
        }
        counts_recv[world_size-1] = num_elements - world_size * num_my_element;

        int displacements[world_size];
        for (int i = 0; i < world_size; i++) {
            displacements[i] = i * num_my_element;
        }

        MPI_Gatherv(my_element, num_my_element, MPI_INT, elements, counts_recv, displacements, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        // For non-root nodes
        MPI_Gatherv(my_element, num_my_element, MPI_INT, elements, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    /* TODO END */

    if (rank == 0){ // record time (only executed in master process)
        t2 = std::chrono::high_resolution_clock::now();  
        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << "Run Time: " << time_span.count() << " seconds" << std::endl;
        std::cout << "Input Size: " << num_elements << std::endl;
        std::cout << "Process Number: " << world_size << std::endl;   
    }

    if (rank == 0){ // write result to file (only executed in master process)
        std::ofstream output(argv[2]+std::string(".out"), std::ios_base::out);
        for (int i = 0; i < num_elements; i++) {
            output << sorted_elements[i] << std::endl;
        }
    }
    
    MPI_Finalize();
    
    return 0;
}


