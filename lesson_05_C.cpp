#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <chrono>
int min(int a, int b) {
    return (a < b) ? a : b;
}
int main() {
    // Equivalent C code from the provided tutorial goes here
    // This includes all the loop structures mimicking the Halide schedules
    // ...
    auto start = std::chrono::high_resolution_clock::now();
    printf("1.) Equivalent C:\n");
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
        }
    }
    printf("\n");
  
    printf("2.) Equivalent C:\n");
    for (int y = 0; y < 4; y++) {
        for (int x_outer = 0; x_outer < 2; x_outer++) {
            for (int x_inner = 0; x_inner < 2; x_inner++) {
                int x = x_outer * 2 + x_inner;
                printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
            }
        }
    }
    printf("\n");
   
    printf("3.) Equivalent C:\n");
    for (int fused = 0; fused < 4 * 4; fused++) {
        int y = fused / 4;
        int x = fused % 4;
        printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
    }
    printf("\n");
    
    
    printf("Equivalent C:\n");
    for (int y_outer = 0; y_outer < 2; y_outer++) {
        for (int x_outer = 0; x_outer < 2; x_outer++) {
            for (int y_inner = 0; y_inner < 4; y_inner++) {
                for (int x_inner = 0; x_inner < 4; x_inner++) {
                    int x = x_outer * 4 + x_inner;
                    int y = y_outer * 4 + y_inner;
                    printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
                }
            }
        }
    }
    printf("\n");
    
    printf("Equivalent C:\n");
    for (int y = 0; y < 4; y++) {
        for (int x_outer = 0; x_outer < 2; x_outer++) {
            // The loop over x_inner has gone away, and has been
            // replaced by a vectorized version of the
            // expression. On x86 processors, Halide generates SSE
            // for all of this.
            int x_vec[] = {x_outer * 4 + 0,
                           x_outer * 4 + 1,
                           x_outer * 4 + 2,
                           x_outer * 4 + 3};
            int val[] = {x_vec[0] + y,
                         x_vec[1] + y,
                         x_vec[2] + y,
                         x_vec[3] + y};
            printf("Evaluating at <%d, %d, %d, %d>, <%d, %d, %d, %d>:"
                   " <%d, %d, %d, %d>\n",
                   x_vec[0], x_vec[1], x_vec[2], x_vec[3],
                   y, y, y, y,
                   val[0], val[1], val[2], val[3]);
        }
    }
    printf("\n");

    printf("Equivalent C:\n");
    for (int y = 0; y < 4; y++) {
        for (int x_outer = 0; x_outer < 2; x_outer++) {
            // The loop over x_inner has gone away, and has been
            // replaced by a vectorized version of the
            // expression. On x86 processors, Halide generates SSE
            // for all of this.
            int x_vec[] = {x_outer * 4 + 0,
                           x_outer * 4 + 1,
                           x_outer * 4 + 2,
                           x_outer * 4 + 3};
            int val[] = {x_vec[0] + y,
                         x_vec[1] + y,
                         x_vec[2] + y,
                         x_vec[3] + y};
            printf("Evaluating at <%d, %d, %d, %d>, <%d, %d, %d, %d>:"
                   " <%d, %d, %d, %d>\n",
                   x_vec[0], x_vec[1], x_vec[2], x_vec[3],
                   y, y, y, y,
                   val[0], val[1], val[2], val[3]);
        }
    }
    printf("\n");

    printf("Equivalent C:\n");
    for (int y = 0; y < 2; y++) {
        for (int x_outer = 0; x_outer < 3; x_outer++) {  // Now runs from 0 to 2
            for (int x_inner = 0; x_inner < 3; x_inner++) {
                int x = x_outer * 3;
                // Before we add x_inner, make sure we don't
                // evaluate points outside of the 7x2 box. We'll
                // clamp x to be at most 4 (7 minus the split
                // factor).
                if (x > 4) x = 4;
                x += x_inner;
                printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
            }
        }
    }
    printf("\n");
    
    printf("Equivalent (serial) C:\n");
    // This outermost loop should be a parallel for loop, but that's hard in C.
    for (int tile_index = 0; tile_index < 4; tile_index++) {
        int y_outer = tile_index / 2;
        int x_outer = tile_index % 2;
        for (int y_inner = 0; y_inner < 4; y_inner++) {
            for (int x_inner = 0; x_inner < 4; x_inner++) {
                int y = y_outer * 4 + y_inner;
                int x = x_outer * 4 + x_inner;
                printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
            }
        }
    }
    printf("\n");
    

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start;
    
    std::cout << "Time: " << duration.count() << " milliseconds.";

//    printf("Success!\n");
    return 0;
}
      
