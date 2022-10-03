#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>

// A helper function that determines whether the input number is even
inline bool is_even(int num) {
    return (num % 2) == 0;
}

int main (int argc, char **argv){

    int num_elements; // number of elements to be sorted
    num_elements = atoi(argv[1]); // convert command line argument to num_elements

    int elements[num_elements]; // store elements
    int sorted_elements[num_elements]; // store sorted elements

    std::ifstream input(argv[2]);
    int element;
    int i = 0;
    while (input >> element) {
        elements[i] = element;
        i++;
    }
    std::cout << "actual number of elements:" << i << std::endl;

    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    std::chrono::duration<double> time_span;
    t1 = std::chrono::high_resolution_clock::now(); // record time

    /* TODO BEGIN */
   // The array is stored in `elements`
    for (int iter = 0; iter < num_elements; iter++) {  // in total need to perform n times to finish the sorting
        for (int i = 0; i < num_elements; i++) {    // inside each iteration
            // Determine whether the iteration number is odd or even
            // We assume iteration number starts from 0
            if (is_even(i)) {
                // In even iteration, do odd-even exchange

                // For each odd index item, compare with its preceding item and do sort among these 2
                for (int j = 1; j < num_elements; j += 2) {
                    if (elements[j] < elements[j-1]) {
                        // Do swap when the two are not in ascending order
                        int tmp = elements[j];
                        elements[j] = elements[j-1];
                        elements[j-1] = tmp;
                    }
                }
            } else {
                // In odd iteration, do even-odd exchange

                // For each even index item, compare with its preceding item and do sort among them
                for (int k = 2; k < num_elements; k += 2) {
                    if (elements[k] < elements[k-1]) {
                        // Do swap when the two are not in ascending order
                        int tmp = elements[k];
                        elements[k] = elements[k-1];
                        elements[k-1] = tmp;
                    }
                }
            }
        }
    }

    for (int i = 0; i < num_elements; i++) {
        sorted_elements[i] = elements[i];
    }
   // Print out my info
   printf("Name: Li Jiaqi\n");
   printf("Student ID: 120090727\n");
   printf("Assignment 1, Odd-Even Transposition Sort, Sequential Version.\n");
    /* TODO END */

    t2 = std::chrono::high_resolution_clock::now();  
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Run Time: " << time_span.count() << " seconds" << std::endl;
    std::cout << "Input Size: " << num_elements << std::endl;
    std::cout << "Process Number: " << 1 << std::endl;   
    
    std::ofstream output(argv[2]+std::string(".out"), std::ios_base::out);
    for (int i = 0; i < num_elements; i++) {
        output << sorted_elements[i] << std::endl;
    }
    
    return 0;
}


