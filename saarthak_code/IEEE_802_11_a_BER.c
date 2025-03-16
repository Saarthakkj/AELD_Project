/**
 * IEEE 802.11a Bit Error Rate (BER) Calculation
 * 
 * This file implements the BER calculation functionality for IEEE 802.11a receiver
 * as described in the AELD project.
 * 
 * Author: Saarthak
 * Based on MATLAB implementation by Jai Mangal, PhD22104
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Calculates Bit Error Rate for received signals
 * 
 * @param data_payload_1 Original transmitted payload 1 bits
 * @param data_payload_2 Original transmitted payload 2 bits
 * @param rx_payload_1_demod Demodulated received payload 1 bits
 * @param rx_payload_2_demod Demodulated received payload 2 bits
 * @param payload_length Length of each payload in bits
 * @return Calculated BER value
 */
double calculate_ber(const int *data_payload_1, 
                    const int *data_payload_2,
                    const int *rx_payload_1_demod, 
                    const int *rx_payload_2_demod,
                    int payload_length) {
    int error_bits = 0;
    
    // Count bit errors in payload 1
    for (int i = 0; i < payload_length; i++) {
        if (data_payload_1[i] != rx_payload_1_demod[i]) {
            error_bits++;
        }
    }
    
    // Count bit errors in payload 2
    for (int i = 0; i < payload_length; i++) {
        if (data_payload_2[i] != rx_payload_2_demod[i]) {
            error_bits++;
        }
    }
    
    // Calculate BER
    double ber = (double)error_bits / (2.0 * payload_length);
    
    return ber;
}

/**
 * Structure to store results for each SNR value
 * This structure contains all metrics that will be plotted in the results section
 */
struct Results {
    double evm;           // EVM before AGC (dB)
    double evm_AGC;       // EVM after AGC (dB)
    double ber;           // Bit Error Rate
    double snr_dB;        // SNR value in dB
};

/**
 * Updates all result metrics in the results structure
 * 
 * @param results Pointer to results structure
 * @param snr_index Index for current SNR value
 * @param evm_dB EVM before AGC in dB
 * @param evm_AGC_dB EVM after AGC in dB
 * @param ber Calculated BER value
 * @param snr_dB SNR value in dB
 */
void update_results(struct Results *results, int snr_index, 
                   double evm_dB, double evm_AGC_dB, 
                   double ber, double snr_dB) {
    results[snr_index].evm = evm_dB;
    results[snr_index].evm_AGC = evm_AGC_dB;
    results[snr_index].ber = ber;
    results[snr_index].snr_dB = snr_dB;
}

/**
 * Post-processes results to handle special cases (like -inf and 0 values)
 * 
 * @param results Pointer to results structure
 * @param num_snr_values Number of SNR values
 */
void process_results(struct Results *results, int num_snr_values) {
    for (int i = 0; i < num_snr_values; i++) {
        // Replace -inf values (taking -1000.0 as -inf) with -40 dB 
        if (results[i].evm_AGC < -1000.0) { 
            results[i].evm_AGC = -40.0;
        }
        
        // Replace 0 BER values with 1e-6 
        if (results[i].ber == 0.0) {
            results[i].ber = 1e-6;
        }
    }
}

/**
 * Writes results to a CSV file for plotting
 * 
 * @param results Pointer to results structure
 * @param num_snr_values Number of SNR values
 * @param filename Output CSV filename
 * @return 0 on success, -1 on failure
 */
int write_results_to_file(struct Results *results, int num_snr_values, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return -1;
    }
    
    // Write header
    fprintf(fp, "SNR_dB,EVM_dB,EVM_AGC_dB,BER\n");
    
    // Write data
    for (int i = 0; i < num_snr_values; i++) {
        fprintf(fp, "%.2f,%.6f,%.6f,%.10e\n", 
                results[i].snr_dB, 
                results[i].evm, 
                results[i].evm_AGC, 
                results[i].ber);
    }
    
    fclose(fp);
    return 0;
}

/**
 * Updates results structure with new BER value
 * 
 * @param results Pointer to results structure
 * @param snr_index Index for current SNR value
 * @param ber Calculated BER value
 */
void update_ber_results(struct Results *results, int snr_index, double ber) {
    results[snr_index].ber = ber;
}


//example usage : 
// struct Results results[10];  // For 10 SNR values
// double ber = calculate_ber(data_payload_1, data_payload_2,
//                           rx_payload_1_demod, rx_payload_2_demod,
//                           96);  // 96 bits per payload
// update_ber_results(results, snr_index, ber);