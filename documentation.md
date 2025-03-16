# IEEE 802.11a Project Structure
## Overview
This document outlines the structure of the IEEE 802.11a wireless communication system implementation. The system includes both transmitter and receiver components with various signal processing stages.

## System Parameters
- FFT Size : 64
- Carrier Frequency : 5 GHz
- Sampling Frequency : 20 MHz
- Sampling Period : 50 ns
- Modulation : QPSK (4-QAM)
## Transmitter Components
### 1. Preamble Generation
- Short Preamble :
  
  - Generate frequency-domain short preamble sequence
  - Apply IFFT to convert to time domain
  - Create 10 repetitions of the 16-sample sequence
- Long Preamble :
  
  - Generate frequency-domain long preamble sequence
  - Apply IFFT to convert to time domain
  - Create cyclic prefix and repetitions
### 2. Payload Processing
- Data Generation :
  
  - Generate random binary data for two payload frames
  - Reshape data into symbol format (2 bits per symbol)
- QPSK Modulation :
  
  - Map bit pairs to QPSK constellation points
  - Normalize constellation points by 1/√2
- Frame Construction :
  
  - Insert pilot symbols at specified positions
  - Add virtual subcarriers at band edges
  - Insert DC null at center frequency
- OFDM Processing :
  
  - Apply IFFT to convert frequency domain to time domain
  - Add cyclic prefix (last 16 samples copied to beginning)
### 3. Frame Assembly
- Combine short preamble, long preamble, and payload frames
### 4. Signal Processing
- Oversampling :
  
  - Upsample by factor of 2 (insert zeros)
- Pulse Shaping :
  
  - Apply Root Raised Cosine (RRC) filter
  - Use roll-off factor of 0.5
  - Implement convolution operation
- Signal Repetition :
  
  - Repeat the signal for transmission reliability
## Channel Simulation
### 1. AWGN Channel
- Add white Gaussian noise based on specified SNR
- Test performance across SNR range (6-15 dB)
### 2. Packet Capture
- Simulate receiver capturing a portion of the transmitted signal
- Random starting point to simulate asynchronous reception
## Receiver Components
### 1. Signal Processing
- Pulse Shaping :
  - Apply matching RRC filter
  - Implement convolution operation
### 2. Packet Detection
- Correlation-based Detection :
  - Calculate auto-correlation with delayed version
  - Normalize by signal power
  - Apply threshold to detect packet start
### 3. Frequency Offset Correction
- Coarse CFO Estimation :
  
  - Use short preamble for initial correction
  - Calculate phase rotation between repeated sections
- Fine CFO Estimation :
  
  - Use long preamble for precise correction
  - Apply phase correction to received signal
### 4. Channel Estimation
- Use long preamble to estimate channel frequency response
- Average two long preamble sections for better estimation
### 5. Equalization
- Apply one-tap frequency domain equalizer
- Divide received signal by channel estimate
### 6. Demodulation
- Symbol Extraction :
  
  - Remove cyclic prefix
  - Apply FFT to convert to frequency domain
  - Extract data subcarriers (remove pilots and virtual carriers)
- Automatic Gain Control (AGC) :
  
  - Map received symbols to ideal constellation points
  - Make hard decisions based on symbol quadrants
- QPSK Demodulation :
  
  - Convert constellation points back to bit pairs
  - Reconstruct the original bit stream
### 7. Performance Evaluation
- Error Vector Magnitude (EVM) :
  
  - Calculate before and after AGC
  - Convert to dB scale
- Bit Error Rate (BER) :
  
  - Compare received bits with transmitted bits
  - Calculate error rate for different SNR values
## Visualization
- Plot EVM vs SNR performance
- Plot BER vs SNR performance on semi-logarithmic scale
## Implementation Notes for C Version
1. Replace MATLAB's built-in functions with C equivalents:
   
   - Implement FFT/IFFT using FFTW library
   - Create custom functions for complex number operations
   - Implement RRC filter design function
2. Memory management considerations:
   
   - Pre-allocate arrays for all signal processing stages
   - Ensure proper handling of complex data types
3. Modular design:
   
   - Create separate functions for each processing block
   - Implement clear interfaces between components
4. Performance optimization:
   
   - Consider using lookup tables for trigonometric functions
   - Optimize convolution operations
   - Use efficient memory access patterns