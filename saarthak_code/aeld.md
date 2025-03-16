# Design & Development of IEEE 802.11a Architecture on Zedboard  
**Jai Mangal**  
PhD Scholar, ECE, IIIT Delhi  
AELD Project 2025  

---

## Evolution of OFDM (Page 2)  
- **Transmission Medium Components**:  
  - BAMPAPASS FILTER  
  - LOWPASS FILTER  
  - ADAPTIVE CORRELATOR  
- Mathematical expression:  
  \[ \cos[2\pi(1-t)f_5+t_0^2], \quad t=3 \]  
- Reference:  
  Cooley & Tukey (1965). *An algorithm for the machine calculation of complex Fourier series.*  

---

## OFDM Block Design (Page 3)  
**Modulation**:  
- S/P → IFFT → Insert CP & Preamble → P/S → DAC → Channel & Noise → ADC → Packet Detection  

**Demodulation**:  
- P/S → FFT → Remove CP → S/P → Channel Estimation → Fine CFO Estimation → Coarse CFO Estimation  

---

## Signal Parameters (Page 4)  
| Parameter          | Value      |  
|---------------------|------------|  
| Centre Frequency    | 5 GHz      |  
| Bandwidth           | 20 MHz     |  
| Sample Time         | 50 ns      |  
| FFT Size            | 64         |  

---

## Short Preamble Structure (Page 5)  
- **Components**:  
  - Virtual Subcarrier [1x6]  
  - S_k [1x53]  
  - Virtual Subcarrier [1x5]  
- **Frequency Domain**:  
  - `Short_preamble_slot_Frequency [1x64]`  
- **Time Domain**:  
  - `Short_preamble_slot_Time [1x64]`  
- **Slot Repetition**:  
  - 16 slots × 10 repetitions = 160 samples  

---

## Long Preamble Structure (Page 6)  
- **Components**:  
  - Virtual Subcarrier [1x6] | L_k [1x53] | Virtual Subcarrier [1x5]  
- **Frequency Domain**:  
  - `Long_preamble_slot_Frequency [1x64]`  
- **Time Domain**:  
  - CP [1x32] + Slot [1x64] + Slot [1x64] = **160 samples**  

---

## Payload Structure (Page 7)  
- **Frequency Domain**:  
  | VS [1x6] | Data [1x6] | Pilot [1x1] | Data [1x13] | Pilot [1x1] | Data [1x6] | 0 [1x1] | ... | VS [1x6] |  
- **Time Domain**:  
  - CP [1x16] + Payload Time [1x64] = **80 samples**  

---

## QPSK Modulation (Page 8)  
| Bits | Symbol | Constellation Point        |  
|------|--------|----------------------------|  
| 00   | 0      | \( 0.707 + j0.707 \)       |  
| 01   | 1      | \( -0.707 + j0.707 \)      |  
| 10   | 2      | \( -0.707 - j0.707 \)      |  
| 11   | 3      | \( 0.707 - j0.707 \)       |  

---

## Frame Design (Page 9)  
- **Components**:  
  - Short Preamble [1x160]  
  - Long Preamble [1x160]  
  - Payload 1 [1x80] + Payload 2 [1x80]  
- **Total Samples**:  
  - 160 + 160 + 80 + 80 = **480 samples**  
- **Oversampled (×2)**:  
  - 320 + 320 + 160 + 160 = **960 samples**  
- **After RRC Filter**:  
  - 10 + 320 + 320 + 160 + 160 + 10 = **980 samples**  

---

## Channel Model (Page 10)  
- Transmit Signal: \( 980 \times 10 = 9800 \) samples  
- Received Signal: Captures **3000 samples**  
- AWGN noise added based on SNR.  

---

## Received Signal Processing (Page 11)  
- Apply RRC Filter:  
  - Input: `Catch Rx Samples [1x3000]`  
  - Output: \( 10 + 3000 + 10 = 3020 \) samples  

---

## Packet Detection (Page 12)  
1. Perform Correlation  
2. Find Power  
3. Normalize  
- **Correlation Magnitude vs. Samples**:  
  - Peaks at intervals (0.8, 0.6, ...) over 0–3000 samples.  

---

## CFO Estimation (Page 13)  
**Coarse CFO**:  
1. Compute complex conjugate  
2. Estimate angle  
3. Apply correction  

**Fine CFO**:  
1. Compute complex conjugate  
2. Estimate angle  
3. Apply correction  

---

## Channel Estimation & Equalization (Page 14)  
1. Extract fine CFO sections  
2. Perform FFT  
3. Average FFT of long preambles  
4. IFFT for time-domain channel estimate  
5. Equalize by dividing received frame with estimates  

---

## Automatic Gain Control (Page 15)  
Mapping based on received signal quadrant:  
1. \( \text{Re} > 0, \text{Im} > 0 \rightarrow 0.707 + j0.707 \)  
2. \( \text{Re} < 0, \text{Im} > 0 \rightarrow -0.707 + j0.707 \)  
3. \( \text{Re} < 0, \text{Im} < 0 \rightarrow -0.707 - j0.707 \)  
4. \( \text{Re} > 0, \text{Im} < 0 \rightarrow 0.707 - j0.707 \)  

---

## Error Vector Magnitude (Page 16)  
1. \( \text{Error} = \text{Rx Symbols} - \text{Tx Symbols} \)  
2. \( \text{EVM} = 20 \log_{10} \left( \frac{\text{RMS(Error)}}{\text{RMS(Tx Symbols)}} \right) \)  
- **EVM vs. SNR**: Ranges from -5 dB to -40 dB.  

---

## Bit Error Rate (Page 17)  
1. \( \text{BER} = \frac{\text{Bit Errors}}{\text{Total Bits}} \)  
- **BER vs. SNR**: From \( 10^0 \) to \( 10^{-6} \) at SNR 6–15 dB.  

---

## Deliverables (Pages 18–19)  
**Midsem**:  
1. Implement 802.11a transceiver  
2. Calculate EVM & BER vs. SNR  
3. Measure PS execution time  

**Endsem**:  
1. Design IPs for QPSK, packet detection, CFO estimation, channel equalization, etc.  
2. Compare PS & PL execution times  

---

**Indraprastha Institute of Information Technology Delhi**  