#include "stdio.h"
#include "unistd.h"
#define BUF_SIZE 400

int main (void){
    int i = 0, cnt = 0;
    int dot = 10;
    char *p;
    char buffer[BUF_SIZE];
    float tx2[50] = {0};
    FILE *stream;
    stream = popen("tegrastats", "r");
    if( stream == NULL) {
        printf("pipe failed\n");
        return -1;
    }
        while(1) { 
            for(i = 0; i < 50; i ++) {
                tx2[i] = 0;
            }
            fgets(buffer, BUF_SIZE, stream);

            i = 0;
            cnt = 0; //count the '@' to judge
            p = buffer;

            while( i < 50 ){
                //Find the number
                while( *p >= '0' && *p <= '9' ) {
                    tx2[i] *= 10.0;
                    tx2[i] += (float)(*p - 0x30);
                    p ++;
                }
                //Spot the dot
                if(*p == '.') {
                    dot = 10;
                    p ++;
                    while( *p >= '0' && *p <= '9' ) {
                        tx2[i] += ( (float)(*p - 0x30) / dot );
                        dot *= 10;
                        p ++;
                        }
                }
                i ++;
                //Pass other char
                while( *p < '0' || *p > '9' ) {
                    if( *p == '@' ){
                        cnt ++;
                    }
                    p ++;
                }
            }

            switch(cnt){
                case 14: 
            }
            for( i = 0; i < 50; i ++ ) {
                printf("%d\t%.1f\n", i, tx2[i]);
            }
        }       
    pclose(stream);
    return 0;
    }
/*
RAM 2007/7852MB (lfb 180x4MB) SWAP 0/3926MB (cached 0MB) 
CPU [1%@345,off,off,0%@345,2%@346,0%@345] 
EMC_FREQ 16%@102 GR3D_FREQ 0%@114 APE 150 
PLL@35.5C MCPU@35.5C PMIC@100C Tboard@33C 
GPU@34.5C BCPU@35.5C thermal@35.1C Tdiode@33.5C 
VDD_SYS_GPU 153/153 VDD_SYS_SOC 384/384 
VDD_4V0_WIFI 0/0 VDD_IN 1423/1423 
VDD_SYS_CPU 153/153 VDD_SYS_DDR 230/230
*/
