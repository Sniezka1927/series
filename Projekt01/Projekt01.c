#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#pragma warning(disable:4996)

double fx(double x) {
    // DOMAIN (-1,1)
    // (1-x)^(-5/2)
    // 1 / sqrt(1-x)^5

    return 1 / sqrt(pow(1 - x, 5));
};

double series_sum(double x, int max_steps, double accuracy, int* code, int* counter)
{
    double sum = 1.;
    double nominator = 5.0;
    double denominator = 2.0;

    for (int i = 1; i < max_steps; i++) {
        
        double quotient = nominator / denominator;      
        double single_step = quotient * pow(x, i);

        if (fabs(single_step) < accuracy) {
            *code = 1;
            break;
        }

        sum += single_step;
        *counter += 1;

        nominator *= (2. * i + 5.);
        denominator *= (2. * i + 2.);
    }
   
    return sum;
}

int main()
{
    double a, b;
    int total_divisions;

    double max_domain = 1.;
    // It is the perfect accuracy to optimalize overflowing and provide the most accurate values between [-.902,.902]
    double accuracy = 0.0003;
    
    // max steps to get real value is 150
    // if max_steps is over 150 it overflows in intermediate operations and returns infinity / -infinity | -nan depending on x value in step
    int max_steps = 150;
   
    printf("Program that calculates series and function value in [a,b] slice, while domain is |x| < 1 \n");
 
    printf("Provide the starting value:");
    if (scanf("%lf", &a) != 1) {
        printf("Provided incorrect value");
        return 1;
    }

    if (fabs(a) >= max_domain) {
        printf("Provided value does not fit into domain |x| < 1.");
        return 1;
    }

    printf("Provide the ending value:");
    if (scanf("%lf", &b) != 1) {
        printf("Provided incorrect value");
        return 1;
    }  

    if (fabs(b) >= max_domain || b <= a) {
        printf("Providing value does not fit int domiain |x| < 1 or b is not higher than a");
        return 1;
    }

    printf("Provide total divions (integer): ");
    if (scanf("%d", &total_divisions) != 1) {
        printf("Provided incorrect value");
        return 1;
    }

    if (total_divisions <= 0) {
        printf("You have to split the range for atleast 1 piece");
        return 1;
    }
    

    double spacing = (b - a) / total_divisions;

    FILE *file;
    file = fopen("./result.txt", "w+");
    if (file == NULL) {
          printf("We could not find the results file or it could not be opened. Please try again.\n");
          return 1;
    }

    fprintf(file, "%-20s %-20s %-20s %-20s %-20s\n", "x", "series", "fx", "series counter", "exit reason");
    //printf("%-20s %-20s %-20s %-20s %-20s\n", "x",  "series","fx", "counter", "exit reason");

    for (int i = 0; i <= total_divisions; i++) {
        double current = a + (spacing * i);
        int code = 0;
        int counter = 1;
        double fx_v = fx(current);
        double estimated = series_sum(current, max_steps, accuracy, &code, &counter);
        if (code == 0) {
            //printf("%-20.8lf %-20.8lf %-20.8lf %-8d steps limit reached\n", current, estimated, fx_v, counter);
            fprintf(file, "%-20.8lf %-20.8lf %-20.8lf %-20d %-15s\n", current, estimated, fx_v, counter, "steps limit reached");
        }
        else {
            //printf("%-20.8lf %-20.8lf %-20.8lf %-8d accuracy too small\n", current, estimated, fx_v,  counter);
            fprintf(file, "%-20.8lf %-20.8lf %-20.8lf %-20d %-15s\n", current, estimated, fx_v,  counter, "accuracy too small");
            
        }
        // Flush the buffer to write immediately
        // fflush(file); 
    }
    fclose(file);
    printf("Results are saved to results.txt file");
    return 0;
}

