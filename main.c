#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>

#define N_FFT 64
#define fc_hz 5e9
#define fs_hz 20e6
#define ts_sec 50e-9
#define PI 3.1415926
#define NP_PACKETS_CAPTURE 3000
#define TX_SIGNAL_LENGTH 9800

void ifftshift(fftw_complex* data, int n) {
	int half_n = n / 2;
	for (int i = 0; i < half_n; i++) {
		double complex temp = data[i];
		data[i] = data[i+half_n];
		data[i+half_n] = temp;
    	}
}

void generate_random_bits(int* data, int size){
	for(int i = 0; i < size; i++){
		data[i] = rand() % 2;
	}
}

void qpsk_modulate(int (*data)[2], double complex* modulated, int size) {
    for (int i = 0; i < size; i++) {
        if (data[i][0] == 0 && data[i][1] == 0)
            modulated[i] = (1 + 1*I) / sqrt(2.0);
        else if (data[i][0] == 0 && data[i][1] == 1)
            modulated[i] = (-1 + 1*I) / sqrt(2.0);
        else if (data[i][0] == 1 && data[i][1] == 0)
            modulated[i] = (-1 - 1*I) / sqrt(2.0);
        else
            modulated[i] = (1 - 1*I) / sqrt(2.0);
    }
}

int main(){
	//Short Preamble
	double scale = sqrt(13.0/6.0);
	double complex S_k[53] = {0, 0, scale * (1 + I), 0, 0, 0, scale * (-1 - I), 0, 0, 0,
        	scale * (1 + I), 0, 0, 0, scale * (-1 - I), 0, 0, 0,
        	scale * (-1 - I), 0, 0, 0, scale * (1 + I), 0, 0, 0,
        	0, 0, 0, 0, scale * (-1 - I), 0, 0, 0, scale * (-1 - I), 0, 0, 0, scale * (1 + I), 0, 0, 0,
        	scale * (1 + I), 0, 0, 0, scale * (1 + I), 0, 0, 0, scale * (1 + I), 0, 0
	};
	double complex virtual_subcarrier[11] = {0};
	double complex Short_preamble_slot_Frequency[N_FFT];
	for (int i = 0; i < 6; i++){
		Short_preamble_slot_Frequency[i] = virtual_subcarrier[i];
	}
	for (int i = 0; i < 53; i++){
		Short_preamble_slot_Frequency[i+6] = S_k[i];
	}
	for (int i = 0; i < 5; i++){
		Short_preamble_slot_Frequency[i+59] = virtual_subcarrier[i+6];
	}
//	for (int i = 0; i < N_FFT; i++) {
//        	printf("Short_preamble_slot_Frequency[%d+1] = %.2f + %.2fi\n", i, creal(Short_preamble_slot_Frequency[i]), cimag(Short_preamble_slot_Frequency[i]));
//    	}
	double complex Short_preamble_slot_Time[N_FFT];
	for (int i = 0; i < N_FFT; i++){
		Short_preamble_slot_Time[i] = Short_preamble_slot_Frequency[i];
	}
	fftw_plan plan;
	ifftshift(Short_preamble_slot_Frequency, N_FFT);
	plan = fftw_plan_dft_1d(N_FFT, (fftw_complex*)Short_preamble_slot_Frequency, (fftw_complex*)Short_preamble_slot_Time,
				FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(plan);
	for (int i = 0; i < N_FFT; i++) {
        	Short_preamble_slot_Time[i] /= N_FFT;
      	}
	//ifft(Short_preamble_slot_Time, N_FFT);
	//for (int i = 0; i < N_FFT; i++) {
        //	printf("Short_preamble_slot_Time[%d+1] = %.2f + %.2fi\n", i, creal(Short_preamble_slot_Time[i]), cimag(Short_preamble_slot_Time[i]));
    	//}
	//printf("Short_preamble_slot_Time (Time-domain data):\n");
    	//for (int i = 0; i < N_FFT; i++) {
        //	printf("[%d] %.4f + %.4fi\n", i, creal(Short_preamble_slot_Time[i]), cimag(Short_preamble_slot_Time[i]));
    	//}

	double complex Short_preamble[160];
	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 16; j++){
			Short_preamble[j + 16 * i] = Short_preamble_slot_Time[j];
		}
	}
//	printf("Short_preamble:\n");
//	for (int i = 0; i < 160; i++) {
//        	printf("[%d] %.4f + %.4fi\n", i, creal(Short_preamble[i]), cimag(Short_preamble[i]));
//    	}

	//Long Preamble
	double complex L_k[53] = {
	    1.0 + 0.0*I, 1.0 + 0.0*I, -1.0 + 0.0*I, -1.0 + 0.0*I,
	    1.0 + 0.0*I, 1.0 + 0.0*I, -1.0 + 0.0*I, 1.0 + 0.0*I,
	    -1.0 + 0.0*I, 1.0 + 0.0*I, 1.0 + 0.0*I, 1.0 + 0.0*I,
	    1.0 + 0.0*I, 1.0 + 0.0*I, 1.0 + 0.0*I, -1.0 + 0.0*I,
	    -1.0 + 0.0*I, -1.0 + 0.0*I, 1.0 + 0.0*I, 1.0 + 0.0*I,
	    1.0 + 0.0*I, 1.0 + 0.0*I, 1.0 + 0.0*I, 1.0 + 0.0*I,
	    1.0 + 0.0*I, 1.0 + 0.0*I, 1.0 + 0.0*I, 0.0 + 0.0*I,
	    1.0 + 0.0*I, -1.0 + 0.0*I, -1.0 + 0.0*I, 1.0 + 0.0*I,
	    1.0 + 0.0*I, -1.0 + 0.0*I, 1.0 + 0.0*I, -1.0 + 0.0*I,
	    1.0 + 0.0*I, 1.0 + 0.0*I, 1.0 + 0.0*I, 1.0 + 0.0*I,
	    1.0 + 0.0*I, 1.0 + 0.0*I, -1.0 + 0.0*I, -1.0 + 0.0*I,
	    1.0 + 0.0*I, -1.0 + 0.0*I, 1.0 + 0.0*I, -1.0 + 0.0*I,
	    1.0 + 0.0*I, -1.0 + 0.0*I, 1.0 + 0.0*I
	};
	double complex Long_preamble_slot_Frequency[N_FFT];
	for (int i = 0; i < 6; i++){
		Long_preamble_slot_Frequency[i] = virtual_subcarrier[i];
	}
	for (int i = 0; i < 53; i++){
		Long_preamble_slot_Frequency[i+6] = L_k[i];
	}
	for (int i = 0; i < 5; i++){
		Long_preamble_slot_Frequency[i+59] = virtual_subcarrier[i+6];
	}
	double complex Long_preamble_slot_Time[N_FFT];
	for (int i = 0; i < N_FFT; i++){
		Long_preamble_slot_Time[i] = Long_preamble_slot_Frequency[i];
	}
	ifftshift(Long_preamble_slot_Frequency, N_FFT);
	plan = fftw_plan_dft_1d(N_FFT, (fftw_complex*)Long_preamble_slot_Frequency, (fftw_complex*)Long_preamble_slot_Time,
				FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(plan);
	for (int i = 0; i < N_FFT; i++) {
        	Long_preamble_slot_Time[i] /= N_FFT;
      	}
	double complex Long_preamble[160];
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 64; j++){
			Long_preamble[j + 64 * i + 32] = Long_preamble_slot_Time[j];
		}
	}
	for(int i = 0; i < 32; i++){
		Long_preamble[i] = Long_preamble_slot_Time[i + 32];
	}
//	printf("Long_preamble:\n");
//	for (int i = 0; i < 160; i++) {
//        	printf("[%d] %.4f + %.4fi\n", i, creal(Long_preamble[i]), cimag(Long_preamble[i]));
//    	}

	//Signal Field
	int Signal_Field_bits[24] = {1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int Signal_Field_bits_scrambled[24];
	int scrambler_state = 0;
	for (int i = 0; i < 24; i++) {
		int bit1 = (scrambler_state >> 6) & 1;
		int bit2 = (scrambler_state >> 3) & 1;
		int feedback = bit1 ^ bit2;
		scrambler_state = ((scrambler_state << 1) | feedback) & 0x7F;
		Signal_Field_bits_scrambled[i] = Signal_Field_bits[i] ^ feedback;
	}
	int Signal_Field_bits_encoded[48];
	for (int i = 0; i < 24; i++) {
		Signal_Field_bits_encoded[i] = Signal_Field_bits_scrambled[i];
		Signal_Field_bits_encoded[i+24] = Signal_Field_bits_scrambled[i];
	}
	int Signal_Field_bits_interleaved[48];
	for (int k = 0; k < 48; k++) {
		int i = (k / 16) + (k % 16) * 3;
		if (i >= 48) {
			i = i - 48 + 1;
		}
		Signal_Field_bits_interleaved[i] = Signal_Field_bits_encoded[k];
	}
	int Signal_Field_bits_interleaved_pairs[24][2];
	for (int i = 0; i < 24; i++) {
		Signal_Field_bits_interleaved_pairs[i][0] = Signal_Field_bits_interleaved[2*i];
		Signal_Field_bits_interleaved_pairs[i][1] = Signal_Field_bits_interleaved[2*i+1];
	}
	double complex Signal_Field_modulated[24];
	qpsk_modulate(Signal_Field_bits_interleaved_pairs, Signal_Field_modulated, 24);
	double complex Signal_Field_Frequency[N_FFT];
	for (int i = 0; i < 6; i++){
		Signal_Field_Frequency[i] = virtual_subcarrier[i];
	}
	for (int i = 0; i < 24; i++){
		Signal_Field_Frequency[i+6] = Signal_Field_modulated[i];
	}
	for (int i = 0; i < 5; i++){
		Signal_Field_Frequency[i+30] = virtual_subcarrier[i+6];
	}
	for (int i = 0; i < 24; i++){
		Signal_Field_Frequency[i+35] = 0;
	}
	for (int i = 0; i < 5; i++){
		Signal_Field_Frequency[i+59] = 0;
	}
	double complex Signal_Field_Time[N_FFT];
	for (int i = 0; i < N_FFT; i++){
		Signal_Field_Time[i] = Signal_Field_Frequency[i];
	}
	ifftshift(Signal_Field_Frequency, N_FFT);
	plan = fftw_plan_dft_1d(N_FFT, (fftw_complex*)Signal_Field_Frequency, (fftw_complex*)Signal_Field_Time,
				FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(plan);
	for (int i = 0; i < N_FFT; i++) {
        	Signal_Field_Time[i] /= N_FFT;
      	}
	double complex Signal_Field[80];
	for(int i = 0; i < 16; i++){
		Signal_Field[i] = Signal_Field_Time[i + 48];
	}
	for(int i = 0; i < 64; i++){
		Signal_Field[i + 16] = Signal_Field_Time[i];
	}

	//Payload
	srand(0); //Seed for reproducibility
	int n_bits = 96;
	int data_payload_1[] = {1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
				0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
				1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1,
				1, 0, 1, 0, 0, 1, 1, 1, 0};
	int data_payload_2[] = {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1,
				1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
				1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0,
				1, 0, 1, 0, 0, 1, 1, 0, 1, 1};
	int data_payload_1_reshape[n_bits/2][2], data_payload_2_reshape[n_bits/2][2];
	for(int i = 0; i < n_bits/2; i++){
		data_payload_1_reshape[i][0] = data_payload_1[2 * i];
		data_payload_1_reshape[i][1] = data_payload_1[2 * i + 1];
		data_payload_2_reshape[i][0] = data_payload_2[2 * i];
		data_payload_2_reshape[i][1] = data_payload_2[2 * i + 1];
	}
	double complex data_payload_1_mod[n_bits/2], data_payload_2_mod[n_bits/2];
	qpsk_modulate(data_payload_1_reshape, data_payload_1_mod, n_bits/2);
	qpsk_modulate(data_payload_2_reshape, data_payload_2_mod, n_bits/2);
	double complex pilot[4] = {1, 1, 1, -1};
	double complex data_frame_1[N_FFT], data_frame_2[N_FFT];

	for(int i = 0; i < 6; i++) data_frame_1[i] = virtual_subcarrier[i];
	for(int i = 0; i < 5; i++) data_frame_1[i + 6] = data_payload_1_mod[i];
	data_frame_1[11] = pilot[0];
	for(int i = 0; i < 13; i++) data_frame_1[i + 12] = data_payload_1_mod[i + 5];
	data_frame_1[25] = pilot[1];
	for(int i = 0; i < 6; i++) data_frame_1[i + 26] = data_payload_1_mod[i + 18];
	data_frame_1[32] = 0;
	for(int i = 0; i < 6; i++) data_frame_1[i + 33] = data_payload_1_mod[i + 24];
	data_frame_1[39] = pilot[2];
	for(int i = 0; i < 13; i++) data_frame_1[i + 40] = data_payload_1_mod[i + 30];
	data_frame_1[53] = pilot[3];
	for(int i = 0; i < 5; i++) data_frame_1[i + 54] = data_payload_1_mod[i + 43];
	for(int i = 0; i < 6; i++) data_frame_1[i + 59] = virtual_subcarrier[i + 6];

	for(int i = 0; i < 6; i++) data_frame_2[i] = virtual_subcarrier[i];
	for(int i = 0; i < 5; i++) data_frame_2[i + 6] = data_payload_2_mod[i];
	data_frame_2[11] = pilot[0];
	for(int i = 0; i < 13; i++) data_frame_2[i + 12] = data_payload_2_mod[i + 5];
	data_frame_2[25] = pilot[1];
	for(int i = 0; i < 6; i++) data_frame_2[i + 26] = data_payload_2_mod[i + 18];
	data_frame_2[32] = 0;
	for(int i = 0; i < 6; i++) data_frame_2[i + 33] = data_payload_2_mod[i + 24];
	data_frame_2[39] = pilot[2];
	for(int i = 0; i < 13; i++) data_frame_2[i + 40] = data_payload_2_mod[i + 30];
	data_frame_2[53] = pilot[3];
	for(int i = 0; i < 5; i++) data_frame_2[i + 54] = data_payload_2_mod[i + 43];
	for(int i = 0; i < 6; i++) data_frame_2[i + 59] = virtual_subcarrier[i + 6];

	double complex data_frame_1_ifft[N_FFT], data_frame_2_ifft[N_FFT];
	ifftshift(data_frame_1, N_FFT);
	plan = fftw_plan_dft_1d(N_FFT, (fftw_complex*)data_frame_1, (fftw_complex*)data_frame_1_ifft,
				FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(plan);
	for (int i = 0; i < N_FFT; i++) {
        	data_frame_1_ifft[i] /= N_FFT;
      	}

	ifftshift(data_frame_2, N_FFT);
	plan = fftw_plan_dft_1d(N_FFT, (fftw_complex*)data_frame_2, (fftw_complex*)data_frame_2_ifft,
				FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(plan);
	for (int i = 0; i < N_FFT; i++) {
        	data_frame_2_ifft[i] /= N_FFT;
      	}

	double complex data_1_TX_payload[80], data_2_TX_payload[80];
	for(int i = 0; i < 16; i++) data_1_TX_payload[i] = data_frame_1_ifft[48 + i];
	for(int i = 0; i < N_FFT; i++) data_1_TX_payload[i + 16] = data_frame_1_ifft[i];
	for(int i = 0; i < 16; i++) data_2_TX_payload[i] = data_frame_2_ifft[48 + i];
	for(int i = 0; i < N_FFT; i++) data_2_TX_payload[i + 16] = data_frame_2_ifft[i];

	//Frame Combination
	double complex Frame_Tx[480];
	for(int i = 0; i < 160; i++) Frame_Tx[i] = Short_preamble[i];
	for(int i = 0; i < 160; i++) Frame_Tx[i + 160] = Long_preamble[i];
	for(int i = 0; i < 80; i++) Frame_Tx[i + 320] = data_1_TX_payload[i];
	for(int i = 0; i < 80; i++) Frame_Tx[i + 400] = data_2_TX_payload[i];
	
	//Oversampling
	int oversampling_rate_Tx = 2;
	double complex Frame_Tx_Oversamp[960] = {0};
	for(int i = 0; i < 480; i++) Frame_Tx_Oversamp[oversampling_rate_Tx * i] = Frame_Tx[i];

	//Convolve Tx frame with the RRC filter 
	double rrc_filter_tx[] = {-0.0004547, 0.003537, -0.007146, 0.007579, 0.002144, -0.01061, 0.03001, -0.05305,
				 -0.07503, 0.4092, 0.8037, 0.4092, -0.07503, -0.05305, 0.03001, -0.01061, 0.002144,
				 0.007579, -0.007146, 0.003537, -0.0004547};
	int len_inp_sig = sizeof(Frame_Tx_Oversamp) / sizeof(Frame_Tx_Oversamp[0]);
	int len_coeff = sizeof(rrc_filter_tx) / sizeof(rrc_filter_tx[0]);
	int len_out_sig = len_inp_sig + len_coeff - 1;
	double complex Tx_signal[980] = {0}; // Output array
	double complex inp_sig_padded[1000] = {0};
	// Copy original signal into padded array
	for (int i = 0; i < len_inp_sig; i++) {
		inp_sig_padded[i + len_coeff - 1] = Frame_Tx_Oversamp[i];
	}

	// Perform convolution
	for (int n = 0; n < len_out_sig; n++) {
		for (int k = 0; k < len_coeff; k++) {
	    		Tx_signal[n] += rrc_filter_tx[k] * inp_sig_padded[n + k];
		}
	}
//	printf("Tx_signal:\n");
//	for (int i = 0; i < 980; i++) {
//        	printf("[%d] = %.7f + %.7fi\n", i, creal(Tx_signal[i]), cimag(Tx_signal[i]));
//    	}
	double complex Tx_signal_repeated[9800] = {0};
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < len_out_sig; j++) {
		    Tx_signal_repeated[i * len_out_sig + j] = Tx_signal[j];
		}
	}
	printf("Tx_signal_repeated:\n");
	for (int i = 0; i < 9800; i++) {
        	printf("[%d] = %.7f + %.7fi\n", i, creal(Tx_signal_repeated[i]), cimag(Tx_signal_repeated[i]));
    	}

	// Begin capture_packets.c code
	double complex Tx_OTA_signal[TX_SIGNAL_LENGTH];
	
	// Copy the transmitted signal to the OTA signal array
	for (int i = 0; i < TX_SIGNAL_LENGTH; i++) {
	    Tx_OTA_signal[i] = Tx_signal_repeated[i];
	}
	
	// Print the received signal
	printf("\nReceived signal (Tx_OTA_signal):\n");
	for (int i = 0; i < TX_SIGNAL_LENGTH; i++) {
	    printf("[%d] = %.7f + %.7fi\n", i, creal(Tx_OTA_signal[i]), cimag(Tx_OTA_signal[i]));
	}

	// Capture multiple packets
	FILE *fp;
	fp = fopen("captured_packets.txt", "w");
	if (fp == NULL) {
		printf("Error opening file for writing\n");
		return 1;
	}
	
	fprintf(fp, "Captured Packets:\n");
	for (int packet = 0; packet < NP_PACKETS_CAPTURE; packet++) {
		fprintf(fp, "Packet %d:\n", packet + 1);
		for (int i = 0; i < TX_SIGNAL_LENGTH; i++) {
			fprintf(fp, "[%d] = %.7f + %.7fi\n", i, creal(Tx_OTA_signal[i]), cimag(Tx_OTA_signal[i]));
		}
		fprintf(fp, "\n");
	}
	
	fclose(fp);
	printf("Successfully captured %d packets and saved to file\n", NP_PACKETS_CAPTURE);

	fftw_destroy_plan(plan);
    	fftw_cleanup();
	return 0;