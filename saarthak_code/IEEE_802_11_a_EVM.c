/**
 * IEEE 802.11a Error Vector Magnitude (EVM) Calculation
 * 
 * This file implements the EVM calculation functionality for IEEE 802.11a receiver
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
 * Calculates EVM for received signals both before and after AGC
 * 
 * @param rx_payload_1_no_pilot Received payload 1 before AGC
 * @param rx_payload_2_no_pilot Received payload 2 before AGC
 * @param rx_payload_1_final Received payload 1 after AGC
 * @param rx_payload_2_final Received payload 2 after AGC
 * @param tx_payload_1_mod Original transmitted payload 1
 * @param tx_payload_2_mod Original transmitted payload 2
 * @param payload_length Length of each payload
 * @param evm_before_agc Pointer to store EVM before AGC (in dB)
 * @param evm_after_agc Pointer to store EVM after AGC (in dB)
 */
void calculate_evm(double complex *rx_payload_1_no_pilot, 
                  double complex *rx_payload_2_no_pilot,
                  double complex *rx_payload_1_final, 
                  double complex *rx_payload_2_final,
                  double complex *tx_payload_1_mod, 
                  double complex *tx_payload_2_mod,
                  int payload_length,
                  double *evm_before_agc,
                  double *evm_after_agc) {
    
    // Temporary arrays for error vectors
    double complex *error_vector = (double complex *)malloc(2 * payload_length * sizeof(double complex));
    double complex *error_vector_agc = (double complex *)malloc(2 * payload_length * sizeof(double complex));
    
    if (error_vector == NULL || error_vector_agc == NULL) {
        fprintf(stderr, "Memory allocation failed in EVM calculation\n");
        return;
    }

    // Calculate error vectors
    for (int i = 0; i < payload_length; i++) {
        // Before AGC
        error_vector[i] = rx_payload_1_no_pilot[i] - tx_payload_1_mod[i];
        error_vector[i + payload_length] = rx_payload_2_no_pilot[i] - tx_payload_2_mod[i];
        
        // After AGC
        error_vector_agc[i] = rx_payload_1_final[i] - tx_payload_1_mod[i];
        error_vector_agc[i + payload_length] = rx_payload_2_final[i] - tx_payload_2_mod[i];
    }

    // Calculate mean square error and mean square of transmitted symbols
    double error_power = 0.0;
    double error_power_agc = 0.0;
    double tx_power = 0.0;

    for (int i = 0; i < 2 * payload_length; i++) {
        error_power += cabs(error_vector[i]) * cabs(error_vector[i]);
        error_power_agc += cabs(error_vector_agc[i]) * cabs(error_vector_agc[i]);
        
        // For tx power, use either payload 1 or 2 based on index
        double complex tx_symbol = (i < payload_length) ? tx_payload_1_mod[i] : tx_payload_2_mod[i - payload_length];
        tx_power += cabs(tx_symbol) * cabs(tx_symbol);
    }

    // Calculate RMS values
    double rms_error = sqrt(error_power / (2 * payload_length));
    double rms_error_agc = sqrt(error_power_agc / (2 * payload_length));
    double rms_tx = sqrt(tx_power / (2 * payload_length));

    // Calculate EVM in dB
    *evm_before_agc = 20 * log10(rms_error / rms_tx);
    *evm_after_agc = 20 * log10(rms_error_agc / rms_tx);

    // Free allocated memory
    free(error_vector);
    free(error_vector_agc);
}


// Example usage : 


// double evm_db_before, evm_db_after;
// calculate_evm(rx_payload_1_no_pilot, rx_payload_2_no_pilot,
//              rx_payload_1_final, rx_payload_2_final,
//              data_payload_1_mod, data_payload_2_mod,
//              payload_length,
//              &evm_db_before, &evm_db_after);