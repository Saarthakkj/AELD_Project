/**
 * IEEE 802.11a Automatic Gain Control (AGC) Implementation
 * 
 * This file implements the AGC functionality for IEEE 802.11a receiver
 * as described in the AELD project.
 * 
 * Author: Saarthak
 * Based on MATLAB implementation by Jai Mangal, PhD22104
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

/**
 * Performs Automatic Gain Control on received payload data
 * Maps received constellation points to ideal QPSK constellation points
 * 
 * @param input_data Array of _Complex input data points
 * @param output_data Array to store the mapped output data points
 * @param length Length of the input/output arrays
 */
void perform_agc(double complex *input_data, double complex *output_data, int length) {
    const double SCALE_FACTOR = 0.707;
    
    for (int idx = 0; idx < length; idx++) {
        // Get the current point
        double _Complex current_point = input_data[idx];
        // Initialize mapped real and imaginary parts
        double mapped_real = 0.0;
        double mapped_imag = 0.0;
        
        // Decision logic for real part
        if (creal(current_point) > 0) {
            mapped_real = SCALE_FACTOR;  
        } else if (creal(current_point) < 0) {
            mapped_real = -SCALE_FACTOR; 
        }
        
        // Decision logic for imaginary part
        if (cimag(current_point) > 0) {
            mapped_imag = SCALE_FACTOR;  
        } else if (cimag(current_point) < 0) {
            mapped_imag = -SCALE_FACTOR; 
        }
        
        // Combine mapped real and imaginary parts
        output_data[idx] = mapped_real + mapped_imag * I;
    }
}

/**
 * Process both payload 1 and payload 2 through AGC
 * 
 * @param rx_payload_1 Array of _Complex input data points for payload 1
 * @param rx_payload_1_final Array to store the mapped output data points for payload 1
 * @param rx_payload_2 Array of _Complex input data points for payload 2
 * @param rx_payload_2_final Array to store the mapped output data points for payload 2
 * @param payload_length Length of each payload array
 */
void process_payloads_agc(double complex *rx_payload_1,double complex *rx_payload_1_final,
    double complex *rx_payload_2,double complex *rx_payload_2_final,
                          int payload_length) {
    // Process payload 1
    perform_agc(rx_payload_1, rx_payload_1_final, payload_length);
    
    // Process payload 2
    perform_agc(rx_payload_2, rx_payload_2_final, payload_length);
}

/**
 * Example usage function showing how to call the AGC processing
 * 
 * @param rx_payload_1_no_pilot Input data for payload 1 after pilot removal
 * @param rx_payload_2_no_pilot Input data for payload 2 after pilot removal
 * @param payload_length Length of each payload
 */
void example_agc_usage(double complex *rx_payload_1_no_pilot, double complex *rx_payload_2_no_pilot, int payload_length) {
    // Allocate memory for output arrays
    double complex * rx_payload_1_final = (double complex *)malloc(payload_length * sizeof(double complex));
    double complex * rx_payload_2_final = (double complex *)malloc(payload_length * sizeof(double complex));
    
    if (rx_payload_1_final == NULL || rx_payload_2_final == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    // Process both payloads through AGC
    process_payloads_agc(rx_payload_1_no_pilot, rx_payload_1_final,
                         rx_payload_2_no_pilot, rx_payload_2_final,
                         payload_length);
    
    // Free allocated memory
    free(rx_payload_1_final);
    free(rx_payload_2_final);
}