
//this was previously named as "aeld.c"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>


void fftshift(double complex data[], int n) {
    int half = n / 2;
    for (int i = 0; i < half; i++) {
        double complex temp = data[i];
        data[i] = data[i + half];
        data[i + half] = temp;
    }
}

void ifftshift(double complex data[], int n) {
    int half = n / 2;
    for (int i = 0; i < half; i++) {
        double complex temp = data[i];
        data[i] = data[i + half];
        data[i + half] = temp;
    }
}
// Complex division function
double complex complex_div(double complex a, double complex b) {
	double real_a = creal(a);
	double imag_a = cimag(a);
	double real_b = creal(b);
	double imag_b = cimag(b);

	double denominator = real_b * real_b + imag_b * imag_b;
//	printf("%.4f\n", denominator);
	return (real_a * real_b + imag_a * imag_b) / denominator +
	   I * (imag_a * real_b - real_a * imag_b) / denominator;
}

void extract_data(double complex *input, double complex *output) {
    // Copy relevant elements while adjusting for 0-based indexing
    int index = 0;

    // Copy ranges corresponding to MATLAB's 1-based indices
    for (int i = 6; i <= 10; i++) output[index++] = input[i];    
    for (int i = 12; i <= 24; i++) output[index++] = input[i];   
    for (int i = 26; i <= 31; i++) output[index++] = input[i];   
    for (int i = 33; i <= 38; i++) output[index++] = input[i];   
    for (int i = 40; i <= 52; i++) output[index++] = input[i];   
    for (int i = 54; i <= 58; i++) output[index++] = input[i];   
}

int main(){
	//Coarse CFO Estimation
	double complex rx_frame[] = {
		0.03984 + 0.04603i, -0.1527 + 0.002116i, -0.03708 + -0.07849i, 0.1221 + -0.01239i, 0.07516 + -0.0004202i, 0.1501 + -0.0126i, -0.0338 + -0.0782i, -0.1491 + 0.00213i, 0.05185 + 0.04637i, -0.002928 + -0.1324i, -0.07445 + -0.01385i, -0.03015 + 0.1428i, -0.01586 + 0.09211i, -0.01717 + 0.1428i, -0.08325 + -0.01385i, -0.0003888 + -0.1324i, 0.01432 + 0.04637i, -0.1515 + 0.002117i, -0.01661 + -0.07819i, 0.1563 + -0.01247i, 0.1147 + -0.0007608i, 0.1657 + -0.01247i, -0.0225 + -0.07819i, -0.1301 + 0.002117i, 0.05059 + 0.04637i, 0.02522 + -0.1324i, -0.08002 + -0.01385i, 0.007545 + 0.1428i, -0.009557 + 0.09211i, -0.02069 + 0.1428i, -0.07176 + -0.01385i, 0.004305 + -0.1324i, 0.04629 + 0.04637i, -0.143 + 0.002117i, -0.04192 + -0.07819i, 0.1208 + -0.01247i, 0.1052 + -0.0007608i, 0.1391 + -0.01247i, -0.01848 + -0.07819i, -0.136 + 0.002117i, 0.0676 + 0.04637i, -0.0008467 + -0.1324i, -0.07987 + -0.01385i, -0.001947 + 0.1428i, 0.01402 + 0.09211i, -0.0006933 + 0.1428i, -0.06018 + -0.01385i, -0.02192 + -0.1324i, 0.05466 + 0.04637i, -0.1389 + 0.002117i, -0.0005252 + -0.07819i, 0.1321 + -0.01247i, 0.06694 + -0.0007608i, 0.1287 + -0.01247i, -0.02704 + -0.07819i, -0.118 + 0.002117i, 0.04584 + 0.04637i, 0.02552 + -0.1324i, -0.0608 + -0.01385i, -0.02011 + 0.1428i, 0.02868 + 0.09211i, -0.02361 + 0.1428i, -0.07873 + -0.01385i, 0.007258 + -0.1324i, 0.03164 + 0.04637i, -0.133 + 0.002117i, -0.01835 + -0.07819i, 0.1533 + -0.01247i, 0.07957 + -0.0007608i, 0.1343 + -0.01247i, -0.005375 + -0.07819i, -0.1432 + 0.002117i, 0.05152 + 0.04637i, 0.02138 + -0.1324i, -0.0903 + -0.01385i, 0.002152 + 0.1428i, -0.003179 + 0.09211i, 0.005183 + 0.1428i, -0.0695 + -0.01385i, 0.004696 + -0.1324i, 0.04583 + 0.04637i, -0.1516 + 0.002117i, -0.0225 + -0.07819i, 0.1546 + -0.01247i, 0.1254 + -0.0007608i, 0.1584 + -0.01247i, -0.01567 + -0.07819i, -0.153 + 0.002117i, 0.05329 + 0.04637i, -0.01172 + -0.1324i, -0.08554 + -0.01385i, -0.002727 + 0.1428i, 0.01506 + 0.09211i, -0.03904 + 0.1428i, -0.08315 + -0.01385i, 0.01435 + -0.1324i, 0.03298 + 0.04637i, -0.1247 + 0.002117i, -0.007179 + -0.07819i, 0.1462 + -0.01247i, 0.077 + -0.0007608i, 0.1608 + -0.01247i, -0.03677 + -0.07819i, -0.1271 + 0.002117i, 0.04333 + 0.04637i, -0.02258 + -0.1324i, -0.06177 + -0.01385i, -0.001084 + 0.1428i, -0.004595 + 0.09211i, -0.02207 + 0.1428i, -0.09754 + -0.01385i, 0.01134 + -0.1324i, 0.06091 + 0.04637i, -0.1199 + 0.002117i, -0.001839 + -0.07819i, 0.1426 + -0.01247i, 0.0767 + -0.0007608i, 0.1531 + -0.01247i, -0.009305 + -0.07819i, -0.1297 + 0.002117i, 0.06503 + 0.04637i, -0.0006143 + -0.1324i, -0.09383 + -0.01385i, -0.03023 + 0.1428i, -0.006453 + 0.09211i, -0.009023 + 0.1428i, -0.04683 + -0.01385i, 0.01814 + -0.1324i, 0.04429 + 0.04637i, -0.1427 + 0.002117i, -0.003807 + -0.07819i, 0.1231 + -0.01247i, 0.09861 + -0.0007608i, 0.1582 + -0.01247i, -0.03051 + -0.07819i, -0.1371 + 0.002117i, 0.01935 + 0.04637i, 0.01139 + -0.1324i, -0.08919 + -0.01385i, -0.02965 + 0.1428i, 0.01593 + 0.09211i, -0.0114 + 0.1428i, -0.05604 + -0.01385i, 0.01419 + -0.1324i, 0.05684 + 0.04637i, -0.148 + 0.002117i, -0.009539 + -0.07819i, 0.1577 + -0.01247i, 0.0789 + -0.0007608i, 0.1277 + -0.01247i, -0.009423 + -0.07819i, -0.1394 + 0.002116i, 0.03924 + 0.04636i, 0.01438 + -0.1324i, -0.0781 + -0.0138i, -0.003105 + 0.1427i, -0.01315 + 0.0919i, -0.03581 + 0.1429i, -0.09315 + -0.01405i, 0.005645 + -0.1326i, -0.1399 + 0.0005569i, 0.02158 + -0.09763i, 0.1273 + -0.1057i, -0.1062 + -0.1148i, 0.01356 + -0.0541i, 0.08395 + 0.074i, -0.1317 + 0.02043i, -0.1029 + 0.01627i, -0.03839 + 0.1506i, -0.04263 + 0.02174i, -0.05938 + -0.08136i, 0.05712 + -0.01393i, 0.101 + -0.09233i, -0.1339 + -0.06531i, -0.07886 + -0.03923i, 0.01133 + -0.09848i, 0.04838 + 0.06257i, 0.09771 + 0.00419i, -0.03768 + -0.1608i, 0.05813 + 0.01493i, 0.0318 + 0.05846i, -0.1137 + 0.04764i, 0.009179 + 0.1153i, 0.08056 + -0.004567i, 0.09879 + 0.02574i, -0.0595 + 0.1065i, -0.1271 + 0.05543i, 0.03001 + 0.08802i, 0.03427 + -0.02823i, 0.1029 + -0.08307i, 0.05645 + 0.1117i, 0.02918 + 0.1204i, 0.1651 + 3.72e-18i, -0.00504 + -0.1204i, 0.0426 + -0.1117i, 0.0978 + 0.08307i, 0.02142 + 0.02823i, 0.04247 + -0.08802i, -0.1364 + -0.05543i, -0.04519 + -0.1065i, 0.1026 + -0.02574i, 0.05591 + 0.004567i, 0.00969 + -0.1153i, -0.1429 + -0.04764i, 0.04548 + -0.05846i, 0.06178 + -0.01493i, -0.02892 + 0.1608i, 0.09838 + -0.00419i, 0.0373 + -0.06257i, 0.03149 + 0.09848i, -0.05402 + 0.03923i, -0.1221 + 0.06531i, 0.08695 + 0.09233i, 0.05835 + 0.01393i, -0.07321 + 0.08136i, -0.07559 + -0.02174i, -0.04232 + -0.1506i, -0.125 + -0.01629i, -0.1318 + -0.02043i, 0.06857 + -0.07378i, -0.02648 + 0.05359i, -0.1045 + 0.1147i, 0.08168 + 0.1058i, 0.02522 + 0.09749i, -0.1462 + -5.557e-19i, -0.01649 + -0.09749i, 0.07371 + -0.1058i, -0.09097 + -0.1147i, -0.01236 + -0.05359i, 0.06522 + 0.07378i, -0.1444 + 0.02043i, -0.1147 + 0.01629i, -0.03365 + 0.1506i, -0.03314 + 0.02174i, -0.07929 + -0.08136i, 0.06347 + -0.01393i, 0.06452 + -0.09233i, -0.1498 + -0.06531i, -0.02606 + -0.03923i, 0.04302 + -0.09848i, 0.06126 + 0.06257i, 0.1049 + 0.00419i, -0.01474 + -0.1608i, 0.05954 + 0.01493i, 0.05105 + 0.05846i, -0.1299 + 0.04764i, -0.01201 + 0.1153i, 0.05165 + -0.004567i, 0.1255 + 0.02574i, -0.02923 + 0.1065i, -0.1502 + 0.05543i, 0.06437 + 0.08802i, 0.004256 + -0.02823i, 0.0989 + -0.08307i, 0.04409 + 0.1117i, -0.01354 + 0.1204i, 0.1729 + 3.72e-18i, 0.0005724 + -0.1204i, 0.05399 + -0.1117i, 0.1056 + 0.08307i, 0.03552 + 0.02823i, 0.06637 + -0.08802i, -0.1233 + -0.05543i, -0.05716 + -0.1065i, 0.1025 + -0.02574i, 0.04898 + 0.004567i, 0.009503 + -0.1153i, -0.1528 + -0.04764i, 0.01932 + -0.05846i, 0.06514 + -0.01493i, -0.02278 + 0.1608i, 0.1358 + -0.00419i, 0.06334 + -0.06257i, 0.04964 + 0.09848i, -0.02223 + 0.03923i, -0.1206 + 0.06531i, 0.1013 + 0.09233i, 0.05652 + 0.01393i, -0.06902 + 0.08136i, -0.06059 + -0.02174i, -0.02177 + -0.1506i, -0.1217 + -0.01629i, -0.126 + -0.02043i, 0.06494 + -0.07378i, -0.01331 + 0.05359i, -0.08474 + 0.1147i, 0.07015 + 0.1058i, -0.008744 + 0.09749i, -0.1419 + -5.557e-19i, 0.002149 + -0.09749i, 0.08161 + -0.1058i, -0.08168 + -0.1147i, 0.004884 + -0.05359i, 0.0816 + 0.07378i, -0.1074 + 0.02043i, -0.1128 + 0.01629i, -0.03991 + 0.1506i, -0.07404 + 0.02174i, -0.04283 + -0.08136i, 0.08322 + -0.01393i, 0.09949 + -0.09233i, -0.1591 + -0.06531i, -0.05572 + -0.03923i, 0.06049 + -0.09848i, 0.08291 + 0.06257i, 0.113 + 0.00419i, -0.01093 + -0.1608i, 0.07761 + 0.01493i, 0.01498 + 0.05846i, -0.1178 + 0.04764i, 0.006381 + 0.1153i, 0.03782 + -0.004565i, 0.1002 + 0.02576i, -0.03975 + 0.1065i, -0.09911 + 0.0553i, 0.04721 + 0.08818i, 0.02935 + -0.02785i, 0.1018 + -0.08311i, 0.04427 + 0.1114i, 0.01283 + 0.1201i, -0.07818 + 0.1198i, 0.05721 + 0.03874i, -0.02414 + -0.1407i, -0.03124 + -0.05664i, 0.06061 + -0.08435i, -0.09987 + -0.0006223i, -0.1631 + 0.07278i, 0.01502 + -0.03668i, -0.02294 + -0.01286i, -0.02525 + 0.06214i, 0.08646 + 0.07597i, 0.06439 + 0.05669i, -0.04366 + 0.04357i, -0.07562 + 0.06315i, 0.0541 + 0.1033i, -0.01471 + -0.02772i, -0.07613 + -0.1769i, -0.002867 + -0.03106i, 0.06898 + 0.08132i, 0.0556 + 0.07919i, 0.005216 + -0.01728i, 0.003229 + -0.09321i, -0.0386 + -0.02029i, -0.02109 + -0.01641i, 0.04986 + 0.1249i, 0.08892 + 0.008085i, 0.05363 + -0.1936i, -0.04434 + 0.05946i, -0.03689 + -0.03339i, -0.02904 + -0.06443i, -0.01084 + 0.1176i, 0.06021 + 0.007419i, 0.108 + 0.1019i, 0.02953 + 0.07946i, 0.03615 + 0.02873i, 0.03496 + 0.04979i, 0.1912 + -0.0849i, 0.0715 + 0.06138i, -0.247 + 0.05342i, -0.007751 + -0.07321i, 0.07794 + -0.03109i, -0.1675 + -0.08749i, 0.02361 + -0.03292i, 0.1286 + 0.07552i, 0.06198 + 0.1255i, 0.03358 + 0.02352i, -0.05308 + -0.02524i, 0.04741 + 0.07138i, 0.0393 + 0.04403i, -0.1477 + 0.1392i, -0.03619 + 0.06668i, 0.08192 + -0.009137i, 0.1097 + 0.09828i, 0.06867 + -0.09834i, -0.09484 + -0.0168i, -0.06297 + 0.06383i, -0.1043 + -0.1692i, -0.1464 + -0.08345i, -0.05879 + -0.06348i, 0.04838 + -0.1287i, 0.07293 + -0.04697i, -0.02717 + -0.01732i, 0.02301 + -0.107i, -0.00314 + -0.1142i, -0.06218 + 0.1193i, 0.02813 + 0.0388i, -0.002959 + -0.1404i, -0.06107 + -0.05716i, 0.06477 + -0.08479i, -0.09582 + -0.0003903i, -0.1354 + 0.07278i, 0.009687 + -0.03671i, -0.03606 + -0.01284i, 0.002212 + 0.06213i, 0.07483 + 0.07578i, 0.0353 + 0.05717i, -0.0341 + 0.04361i, -0.04356 + 0.06328i, 0.04005 + 0.1031i, 0.004424 + -0.02804i, -0.217 + 0.03156i, -0.1537 + 0.03773i, -0.04784 + 0.1051i, 0.03586 + -0.02654i, 0.03929 + -0.1145i, 0.007916 + 0.05341i, 0.07243 + -0.01228i, -0.01472 + -0.006301i, -0.02298 + 0.02458i, 0.07344 + -0.0003919i, -0.06259 + -0.02245i, 0.006985 + -0.06477i, 0.06037 + 0.00664i, -0.008225 + -0.06143i, 0.1396 + -0.07407i, 0.03428 + 0.08647i, 0.04249 + 0.1105i, 0.2929 + 0.08445i, -0.02191 + 0.07063i, -0.1065 + -0.01426i, 0.0536 + -0.07076i, -0.1183 + 0.06821i, -0.1299 + 0.07035i, -0.03828 + 0.001371i, -0.1401 + 0.1443i, -0.09065 + 0.04874i, 0.03711 + -0.0364i, 0.05916 + 0.09639i, 0.03999 + 0.08341i, -0.02211 + -0.03296i, -0.01941 + -0.1176i, 0.04572 + 0.0301i, 0.02733 + 0.1016i, 0.01517 + 0.002271i, 0.03235 + -0.006255i, -0.129 + -0.08552i, -0.02038 + -0.04957i, 0.1248 + 0.03021i, -0.07928 + 0.08163i, -0.1345 + 0.1322i, -0.07555 + -0.02513i, -0.02374 + -0.05409i, 0.0696 + 0.02991i, 0.02752 + 0.008645i, 0.0929 + -0.08164i, 0.1852 + -0.05158i, 0.07136 + 0.1101i, -0.1047 + 0.01852i, -0.108 + 0.02189i, 0.09777 + -0.008601i, 0.102 + -0.1698i, -0.05806 + 0.01123i, 0.03841 + -0.03064i, -0.01951 + -0.1035i, -0.08993 + -0.01427i, 0.03238 + -0.07977i, -0.0422 + -0.05545i, -0.02888 + -0.1106i, 0.03531 + 0.02894i, -0.04634 + 0.07608i, -0.1015 + -0.09675i, 0.001328 + 0.09845i, 0.1341 + -0.04387i, 0.01055 + -0.1836i, -0.2419 + 0.03172i, -0.1534 + 0.03729i, -0.01995 + 0.1054i, 0.05761 + -0.02685i, 0.02829 + -0.1148i, 0.01496 + 0.05356i, 0.1238 + -0.01229i, -0.009529 + -0.006322i, -0.02015 + 0.02457i, 0.07112 + -0.0003936i, -0.06467 + -0.02235i, 0.03852 + -0.06498i, 0.08082 + 0.006552i, -0.02599 + -0.06159i, 0.1242 + -0.07406i, 0.01014 + 0.08671i
	};
	int Short_preamble_slot_length = 16;
//	Calculate the complex conjugate product between the two relevant sections
	double complex prod_consq_frame_coarse = 0.0;
	for (int i = 0; i < Short_preamble_slot_length; i++) {
        	prod_consq_frame_coarse += 
			rx_frame[Short_preamble_slot_length * 5 + i] * 
			conj(rx_frame[Short_preamble_slot_length * 6 + i]);
    	}
//	printf("prod_consq_frame_coarse:\n");
//	printf("%.4f + %.4fi", creal(prod_consq_frame_coarse), 
//				cimag(prod_consq_frame_coarse));
	// Estimate the coarse frequency offset
	double ts_sec = 50e-9;
    	double freq_coarse_est = (-1.0 / (2.0 * M_PI * Short_preamble_slot_length * ts_sec)) *
                             atan2(cimag(prod_consq_frame_coarse), creal(prod_consq_frame_coarse));
//	printf("freq_coarse_est:\n");
//	printf("%.4f", freq_coarse_est);

   	 // Apply the coarse frequency offset
	double complex rx_frame_after_coarse[480];
	for (int i = 0; i < 480; i++) {
        	rx_frame_after_coarse[i] = rx_frame[i] * cexp(-I * 2.0 *
				 M_PI * freq_coarse_est * ts_sec * i);
    	}
//	printf("rx_frame_after_coarse:\n");
//	for(int i = 0; i < 480; i++){
//		printf("[%d] %.4f + %.4fi\n",i, creal(rx_frame_after_coarse[i]), 
//				cimag(rx_frame_after_coarse[i]));
//	}
	//Fine CFO Estimation
	double complex prod_consq_frame_fine = 0.0;
	for (int i = 0; i < 64; i++) {
        	prod_consq_frame_fine += 
			rx_frame_after_coarse[Short_preamble_slot_length * 12 + i] * 
			conj(rx_frame_after_coarse[Short_preamble_slot_length * 16 + i]);
    	}
//	printf("prod_consq_frame_fine:\n");
//	printf("%.4f + %.4fi\n", creal(prod_consq_frame_fine), 
//				cimag(prod_consq_frame_fine));
	// Estimate the fine frequency offset
    	double freq_fine_est = (-1.0 / (2.0 * M_PI * 64 * ts_sec)) *
                           atan2(cimag(prod_consq_frame_fine), creal(prod_consq_frame_fine));
//	printf("freq_fine_est:\n");
//	printf("%.4f\n", freq_fine_est);

    	// Apply the fine frequency offset
    	double complex rx_frame_after_fine[480];
	for (int i = 0; i < 480; i++) {
        	rx_frame_after_fine[i] = rx_frame_after_coarse[i] *
                        cexp(-I * 2.0 * M_PI * freq_fine_est * ts_sec * i);
    	}
//	printf("rx_frame_after_fine:\n");
//	for(int i = 0; i < 480; i++){
//		printf("[%d]%.4f + %.4fi\n",i,creal(rx_frame_after_fine[i]), 
//				cimag(rx_frame_after_fine[i]));
//	}

	//Channel Estimation
	int preamble_length = 64;
	double complex Long_preamble_1[64];
	double complex Long_preamble_2[64];
	double complex Long_preamble_1_After_FFT[64];
	double complex Long_preamble_2_After_FFT[64];
	// Extract sections of the frame
	for (int i = 0; i < preamble_length; i++) {
	    	Long_preamble_1[i] = rx_frame_after_fine[Short_preamble_slot_length * 12 + i];
        	Long_preamble_2[i] = rx_frame_after_fine[Short_preamble_slot_length* 16 + i];
    	}
	//Perform FFT
	fftw_plan fft1 = fftw_plan_dft_1d(preamble_length, 
                              (fftw_complex*)Long_preamble_1, 
	                      (fftw_complex*)Long_preamble_1_After_FFT, 
                              FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_plan fft2 = fftw_plan_dft_1d(preamble_length, 
                              (fftw_complex*)Long_preamble_2, 
                              (fftw_complex*)Long_preamble_2_After_FFT, 
                              FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(fft1);
	fftw_execute(fft2);
	// Apply FFT shift (swap halves)
	for (int i = 0; i < preamble_length / 2; i++) {
		double complex temp1 = Long_preamble_1_After_FFT[i];
		Long_preamble_1_After_FFT[i] = Long_preamble_1_After_FFT[i + preamble_length / 2];
		Long_preamble_1_After_FFT[i + preamble_length / 2] = temp1;
		double complex temp2 = Long_preamble_2_After_FFT[i];
		Long_preamble_2_After_FFT[i] = Long_preamble_2_After_FFT[i + preamble_length / 2];
		Long_preamble_2_After_FFT[i + preamble_length / 2] = temp2;
	}
//	printf("Long preamble 1 after fft: \n");
//	for(int i = 0; i < preamble_length; i++){
//		printf("[%d] %.4f + %.4fi\n", i,
//			creal(Long_preamble_1_After_FFT[i]), 
//			cimag(Long_preamble_1_After_FFT[i]));
//	}
//	printf("Long preamble 2 after fft: \n");
//	for(int i = 0; i < preamble_length; i++){
//		printf("[%d] %.4f + %.4fi\n", i,
//			creal(Long_preamble_2_After_FFT[i]), 
//			cimag(Long_preamble_2_After_FFT[i]));
//	}
	// Estimate the channel (average of both FFT results, then conjugate product)
    	double complex H_est[64];
	double Long_preamble_slot_Frequency[] = {0, 0, 0, 0, 0, 0, 1, 1, 
		-1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1, -1, 1, 1,
		-1, 1, -1, 1, 1, 1, 1, 0, 1, -1, -1, 1, 1, -1, 1, -1, 1,
		-1, -1, -1, -1, -1, 1, 1, -1, -1, 1, -1, 1, -1, 1, 1, 1, 
		1, 0, 0, 0, 0, 0};
	for (int i = 0; i < preamble_length; i++) {
        	H_est[i] = 0.5 * (Long_preamble_1_After_FFT[i] + 
					Long_preamble_2_After_FFT[i]) *
                   		conj(Long_preamble_slot_Frequency[i]);
    	}
//	printf("H_est: \n");
//	for(int i = 0; i < preamble_length; i++){
//		printf("[%d] %.10f + %.10fi\n",i,creal(H_est[i]),cimag(H_est[i]));
//	}
	// IFFT shift before performing IFFT
	double complex H_est_time[64];
	double complex H_est_copied[64];
	for(int i = 0; i < 64; i++){
		H_est_copied[i] = H_est[i];
	}
	ifftshift(H_est_copied, preamble_length);
//	printf("H_est_copied: \n");
//	for(int i = 0; i < preamble_length; i++){
//		printf("[%d] %.10f + %.10fi\n",i,creal(H_est_copied[i]),cimag(H_est_copied[i]));
//	}
	// Perform IFFT to return to time domain
	fftw_plan ifft = fftw_plan_dft_1d(preamble_length, 
	                              (fftw_complex*)H_est_copied, 
	                              (fftw_complex*)H_est_time, 
	                              FFTW_BACKWARD, FFTW_ESTIMATE);

	fftw_execute(ifft);

	// Normalize the IFFT result
	for (int i = 0; i < preamble_length; i++) {
		H_est_time[i] /= preamble_length;
	}
	
//	printf("H_est_time: \n");
//	for(int i = 0; i < preamble_length; i++){
//		printf("[%d] %.4f + %.4fi\n",i,creal(H_est_time[i]),cimag(H_est_time[i]));
//	}
	fftw_destroy_plan(ifft);
	

	//One Tap Equalizer
	double complex RX_Payload_1_time[80];
	double complex RX_Payload_2_time[80];
	double complex RX_Payload_1_no_CP[64];
	double complex RX_Payload_2_no_CP[64];
	double complex RX_Payload_1_Frequency[64];
	double complex RX_Payload_2_Frequency[64];
	double complex RX_Payload_1_Frequency_Equalizer[64];
	double complex RX_Payload_2_Frequency_Equalizer[64];
	
	for(int i = 0; i < 80; i++){
		RX_Payload_1_time[i] = rx_frame_after_fine[320+i];
		RX_Payload_2_time[i] = rx_frame_after_fine[400+i];
	}
	for(int i = 0; i < 64; i++){
		RX_Payload_1_no_CP[i] = RX_Payload_1_time[16+i];
		RX_Payload_2_no_CP[i] = RX_Payload_2_time[16+i];
	}
//	printf("RX_Payload_1_no_CP: \n");
//	for(int i = 0; i < 64; i++){
//		printf("[%d] %.4f + %.4fi\n",i,creal(RX_Payload_1_no_CP[i]),cimag(RX_Payload_1_no_CP[i]));
//	}
//	printf("RX_Payload_2_no_CP: \n");
//	for(int i = 0; i < 64; i++){
//		printf("[%d] %.4f + %.4fi\n",i,creal(RX_Payload_2_no_CP[i]),cimag(RX_Payload_2_no_CP[i]));
//	}
	// FFT
	fft1 = fftw_plan_dft_1d(64, 
	                              (fftw_complex*)RX_Payload_1_no_CP, 
	                              (fftw_complex*)RX_Payload_1_Frequency, 
	                              FFTW_FORWARD, FFTW_ESTIMATE);
	fft2 = fftw_plan_dft_1d(64, 
	                              (fftw_complex*)RX_Payload_2_no_CP, 
	                              (fftw_complex*)RX_Payload_2_Frequency, 
	                              FFTW_FORWARD, FFTW_ESTIMATE);

	fftw_execute(fft1);
	fftw_execute(fft2);

	// FFT shift for both FFT outputs
	fftshift(RX_Payload_1_Frequency, 64);
	fftshift(RX_Payload_2_Frequency, 64);
//	printf("RX_Payload_1_Frequency: \n");
//	for(int i = 0; i < 64; i++){
//		printf("[%d] %.4f + %.4fi\n",i,creal(RX_Payload_1_Frequency[i]),cimag(RX_Payload_1_Frequency[i]));
//	}
//	printf("RX_Payload_2_Frequency: \n");
//	for(int i = 0; i < 64; i++){
//		printf("[%d] %.4f + %.4fi\n",i,creal(RX_Payload_2_Frequency[i]),cimag(RX_Payload_2_Frequency[i]));
//	}
	// Equalization (complex division)
	for (int i = 0; i < 64; i++) {
	       	RX_Payload_1_Frequency_Equalizer[i] = complex_div(RX_Payload_1_Frequency[i], H_est[i]);
		RX_Payload_2_Frequency_Equalizer[i] = complex_div(RX_Payload_2_Frequency[i], H_est[i]);
	}

//	printf("RX_Payload_1_Frequency_Equalizer: \n");
//	for(int i = 0; i < 64; i++){
//		printf("[%d] %.4f + %.4fi\n",i,creal(RX_Payload_1_Frequency_Equalizer[i]),cimag(RX_Payload_1_Frequency_Equalizer[i]));
//	}
//	printf("RX_Payload_2_Frequency_Equalizer: \n");
//	for(int i = 0; i < 64; i++){
//		printf("[%d] %.4f + %.4fi\n",i,creal(RX_Payload_2_Frequency_Equalizer[i]),cimag(RX_Payload_2_Frequency_Equalizer[i]));
	//}
	double complex RX_Payload_1_no_Equalizer[48];
	double complex RX_Payload_1_no_pilot[48];
	double complex RX_Payload_2_no_Equalizer[48];
	double complex RX_Payload_2_no_pilot[48];

	// Perform extraction
	extract_data(RX_Payload_1_Frequency, RX_Payload_1_no_Equalizer);
	extract_data(RX_Payload_1_Frequency_Equalizer, RX_Payload_1_no_pilot);
	extract_data(RX_Payload_2_Frequency, RX_Payload_2_no_Equalizer);
	extract_data(RX_Payload_2_Frequency_Equalizer, RX_Payload_2_no_pilot);

//	printf("RX_Payload_1_no_Equalizer:\n");
//	for (int i = 0; i < 48; i++) {
//		printf("[%d] %.4f + %.4fi\n",i, creal(RX_Payload_1_no_Equalizer[i]), cimag(RX_Payload_1_no_Equalizer[i]));
//	}
//	printf("RX_Payload_1_no_pilot:\n");
//	for (int i = 0; i < 48; i++) {
//		printf("[%d] %.4f + %.4fi\n",i, creal(RX_Payload_1_no_pilot[i]), cimag(RX_Payload_1_no_pilot[i]));
//	}
//	printf("RX_Payload_2_no_Equalizer:\n");
//	for (int i = 0; i < 48; i++) {
//		printf("[%d] %.4f + %.4fi\n",i, creal(RX_Payload_2_no_Equalizer[i]), cimag(RX_Payload_2_no_Equalizer[i]));
//	}
//	printf("RX_Payload_2_no_pilot:\n");
//	for (int i = 0; i < 48; i++) {
//		printf("[%d] %.4f + %.4fi\n",i, creal(RX_Payload_2_no_pilot[i]), cimag(RX_Payload_2_no_pilot[i]));
//	}
	fftw_destroy_plan(fft1);
	fftw_destroy_plan(fft2);
	return 0;
}
