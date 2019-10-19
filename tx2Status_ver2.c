#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pwd.h"
#include "string.h"
#include "time.h"
#define BUF_SIZE 400

struct Status{
    int RAM_used, RAM_all, lfb[2];          //RAM 1952/7852MB (lfb 196x4MB) 
    int SWAP_used, SWAP_all, cached;//SWAP 0/3926MB (cached 0MB) 
    int CPU_used[6];                //CPU [2%@345,off,off,2%@345,1%@346,0%@345]
    int CPU_freq[6]; 
    int EMC_used, EMC_freq;          //EMC_FREQ 16%@102 		bus%@MHz
    int GPU_used, GPU_freq;         //GR3D_FREQ 0%@114 
    int APE;                        //APE 150 
    float PLL_temp;                   //PLL@33.5C 
    float MCPU_temp;                  //MCPU@33.5C 
    float PMIC_temp;                 //MIC@100C 
    float Tboard_temp;                //Tboard@32C 
    float GPU_temp;                   //GPU@33C 
    float BCPU_temp;                  //BCPU@33.5C 
    float thermal_temp;               //thermal@33.6C 
    float Tdiode_temp;                //Tdiode@32C 
    int VDD_SYS_GPU[2];                //VDD_SYS_GPU 153/153 
    int VDD_SYS_SOC[2];                //VDD_SYS_SOC 384/384 
    int VDD_4V0_WIFI[2];               //VDD_4V0_WIFI 0/0 
    int VDD_IN[2];                     //VDD_IN 1423/1423 
    int VDD_SYS_CPU[2];                //VDD_SYS_CPU 153/153 
    int VDD_SYS_DDR[2];                //VDD_SYS_DDR 230/230
};

int CleanStatus( struct Status (*tx2) ) {
    (*tx2).RAM_used    = -1;
    (*tx2).RAM_all     = -1;
    (*tx2).lfb[0]      = -1;
    (*tx2).lfb[1]      = -1;
    (*tx2).SWAP_used   = -1;
    (*tx2).SWAP_all    = -1;
    (*tx2).cached      = -1;
    (*tx2).CPU_used[0] = -1;
    (*tx2).CPU_used[1] = -1; 
    (*tx2).CPU_used[2] = -1; 
    (*tx2).CPU_used[3] = -1; 
    (*tx2).CPU_used[4] = -1; 
    (*tx2).CPU_used[5] = -1;  
    (*tx2).CPU_freq[0] = -1;
    (*tx2).CPU_freq[1] = -1;
    (*tx2).CPU_freq[2] = -1;
    (*tx2).CPU_freq[3] = -1;
    (*tx2).CPU_freq[4] = -1;
    (*tx2).CPU_freq[5] = -1; 
    (*tx2).EMC_used     = -1;
    (*tx2).EMC_freq    = -1;  
    (*tx2).GPU_used    = -1; 
    (*tx2).GPU_freq    = -1; 
    (*tx2).APE         = -1;   

    (*tx2).PLL_temp    = -1.0;  
    (*tx2).MCPU_temp   = -1.0;  
    (*tx2).PMIC_temp  = -1.0; 
    (*tx2).Tboard_temp = -1.0;  
    (*tx2).GPU_temp    = -1.0; 
    (*tx2).BCPU_temp   = -1.0;  
    (*tx2).thermal_temp= -1.0; 
    (*tx2).Tdiode_temp = -1.0; 

    (*tx2).VDD_SYS_GPU[0]  = -1; 
    (*tx2).VDD_SYS_GPU[1]  = -1;
    (*tx2).VDD_SYS_SOC[0]  = -1; 
    (*tx2).VDD_SYS_SOC[1]  = -1; 
    (*tx2).VDD_4V0_WIFI[0] = -1; 
    (*tx2).VDD_4V0_WIFI[1] = -1;
    (*tx2).VDD_IN[0]       = -1;  
    (*tx2).VDD_IN[1]       = -1; 
    (*tx2).VDD_SYS_CPU[0]  = -1; 
    (*tx2).VDD_SYS_CPU[1]  = -1; 
    (*tx2).VDD_SYS_DDR[0]  = -1; 
    (*tx2).VDD_SYS_DDR[1]  = -1;
    return 0;
}

int DayTimeDisplay() {
    char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    time_t timep;
    struct tm *p;
   
    time(&timep); 
    p = gmtime(&timep); 
    printf("\|  %d/%d/%d   ", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
    printf("%s   ", wday[p->tm_wday]);
    if( p->tm_hour < 10 )
        printf("0%d:",  p->tm_hour);
    else
        printf("%d:",   p->tm_hour);
    
    if( p->tm_min < 10 )
        printf("0%d:",  p->tm_min );
    else
        printf("%d:",   p->tm_min );

    if( p->tm_sec < 10 )
        printf("0%d\t\t\t\t\t\|\n", p->tm_sec);
    else
        printf("%d\t\t\t\t\t\|\n",  p->tm_sec);
    return 0;
}

int main (void){
    int i = 0, cnt = 0;
    int dot = 10;

    int buf = 0;
    float temp = 0.0;

    char *p;
    char buffer[BUF_SIZE];
    FILE *stream;
    struct Status tx2;

    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if( strcmp(pw->pw_name, "root") != 0 ) {
        printf("Permission denied!\n\033[31mYou must add \"sudo\" before your command.\n\033[0m");
        return -1;
    }


    stream = popen("tegrastats", "r");
    if( stream == NULL) {
        printf("pipe failed\n");
        return -1;
    }

        while(1) {  
            fgets(buffer, BUF_SIZE, stream);          
            CleanStatus(&tx2);
            i = 0;
            cnt = 0;
            buf = 0;
            p = buffer;

                p = p + 4;
                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.RAM_used = buf;
                buf = 0;
                p ++;

                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.RAM_all = buf;
                buf = 0;
                p = p + 9;

                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.lfb[0] = buf;
                buf = 0;
                p ++;

                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.lfb[1] = buf;
                buf = 0;
                p = p + 9;

                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.SWAP_used = buf;
                buf = 0;
                p ++;

                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.SWAP_all = buf;
                buf = 0;
                p = p + 11;

                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.cached = buf;
                buf = 0;
                p = p + 9;

                for(cnt = 0; cnt < 6; cnt ++) {
                    if( *p == 'o' ) {
                        tx2.CPU_used[cnt] = -1;
                        tx2.CPU_freq[cnt] = -1;
                        p = p + 4;
                    }
                    else {
                        while( *p >= '0' && *p <= '9' ) {
                            buf *= 10;
                            buf += (*p - 0x30);
                            p ++;
                        }
                        tx2.CPU_used[cnt] = buf;
                        buf = 0;
                        p = p + 2;

                        while( *p >= '0' && *p <= '9' ) {
                            buf *= 10;
                            buf += (*p - 0x30);
                            p ++;
                        }
                        tx2.CPU_freq[cnt] = buf;
                        buf = 0;
                        p = p + 1;
                    }
                }
                p = p + 10;
//EMC
                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.EMC_used = buf;
                buf = 0;
                p = p + 2;

                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.EMC_freq = buf;
                buf = 0;
                p = p + 11;
//GPU
                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }

                tx2.GPU_used = buf;
                buf = 0;
                p = p + 2;

                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.GPU_freq = buf;
                buf = 0;
                p ++;
//NVDEC
                
                while( *p != 'A' )
                    p ++;
                p = p + 4;                        
//APE
                while( *p >= '0' && *p <= '9' ) {
                    buf *= 10;
                    buf += (*p - 0x30);
                    p ++;
                }
                tx2.APE = buf;
                buf = 0;
                p ++;
//other stuff
                while( *p != 'P' )
                    p ++;
                p = p + 4;
//PLL temperature
                while( *p >= '0' && *p <= '9' ) {
                    temp *= 10.0;
                    temp += (*p - 0x30);
                    p ++;
                }
                if(*p == '.') {
                    p ++;
                    while( *p >= '0' && *p <= '9' ) {
                        temp += ( (float)(*p - 0x30) / (float)dot );
                        dot *= 10;
                        p ++;
                    }
                    dot = 10;
                }
                tx2.PLL_temp = temp;      
                temp = 0.0;   
                p = p + 7;
//MCPU temperature
                while( *p >= '0' && *p <= '9' ) {
                    temp *= 10.0;
                    temp += (*p - 0x30);
                    p ++;
                }
                if(*p == '.') {
                    p ++;
                    while( *p >= '0' && *p <= '9' ) {
                        temp += ( (float)(*p - 0x30) / (float)dot );
                        dot *= 10;
                        p ++;
                    }
                    dot = 10;
                }
                tx2.MCPU_temp = temp;
                temp = 0.0;
                p = p + 7;
//PMIC temperature
                while( *p >= '0' && *p <= '9' ) {
                    temp *= 10.0;
                    temp += (*p - 0x30);
                    p ++;
                }
                if(*p == '.') {
                    p ++;
                    while( *p >= '0' && *p <= '9' ) {
                        temp += ( (float)(*p - 0x30) / (float)dot );
                        dot *= 10;
                        p ++;
                    }
                    dot = 10;
                }
                tx2.PMIC_temp = temp;
                temp = 0.0;
                p = p + 9;
//Board temperature
                while( *p >= '0' && *p <= '9' ) {
                    temp *= 10.0;
                    temp += (*p - 0x30);
                    p ++;
                }
                if(*p == '.') {
                    p ++;
                    while( *p >= '0' && *p <= '9' ) {
                        temp += ( (float)(*p - 0x30) / (float)dot );
                        dot *= 10;
                        p ++;
                    }
                    dot = 10;
                }
                tx2.Tboard_temp = temp;
                temp = 0.0;
                p = p + 6;
//GPU temperature
                while( *p >= '0' && *p <= '9' ) {
                    temp *= 10.0;
                    temp += (*p - 0x30);
                    p ++;
                }
                if(*p == '.') {
                    p ++;
                    while( *p >= '0' && *p <= '9' ) {
                        temp += ( (float)(*p - 0x30) / (float)dot );
                        dot *= 10;
                        p ++;
                    }
                    dot = 10;
                }
                tx2.GPU_temp = temp;
                temp = 0.0;
                p = p + 7;
//BCPU temperature
//printf("%s\n",p);
                while( *p >= '0' && *p <= '9' ) {
                    temp *= 10.0;
                    temp += (*p - 0x30);
                    p ++;
                }
//printf("%f\n",temp);
                if(*p == '.') {
                    p ++;
                    while( *p >= '0' && *p <= '9' ) {
                        temp += ( (float)(*p - 0x30) / (float)dot );
                        dot *= 10;
                        p ++;
                    }
                    dot = 10;
                }
                tx2.BCPU_temp = temp;
                temp = 0.0;
                p = p + 10;
//thermal temperature
                while( *p >= '0' && *p <= '9' ) {
                    temp *= 10.0;
                    temp += (*p - 0x30);
                    p ++;
                }
                if(*p == '.') {
                    p ++;
                    while( *p >= '0' && *p <= '9' ) {
                        temp += ( (float)(*p - 0x30) / (float)dot );
                        dot *= 10;
                        p ++;
                    }
                    dot = 10;
                }
                tx2.thermal_temp = temp;
                temp = 0.0;
                p = p + 9;
//diode temperature
                while( *p >= '0' && *p <= '9' ) {
                    temp *= 10.0;
                    temp += (*p - 0x30);
                    p ++;
                }
                if(*p == '.') {
                    p ++;
                    while( *p >= '0' && *p <= '9' ) {
                        temp += ( (float)(*p - 0x30) / (float)dot );
                        dot *= 10;
                        p ++;
                    }
                    dot = 10;
                }
                tx2.Tdiode_temp = temp;
                temp = 0.0;
                p = p + 14;

//        system("clear");
//        system("date");

        printf("\e[1;1H\e[2J"); //clear the screen
        printf("=================================================================\n");
        DayTimeDisplay();

        printf("=================================================================\n");

        if(tx2.CPU_used[0] == -1) {
            printf("\|  \033[40;35mCPU%d\tOFF\033[0m\t\t\t\|", 0);
        }
        else {
            printf("\|  \033[40;36mCPU%d\t\t%dMHz\t%d\%\033[0m\t\|", 0, tx2.CPU_freq[0], tx2.CPU_used[0]);
        }
        printf("  \033[40;36mGPU\t\t%d\%\t%dMHz\033[0m\t\|\n", tx2.GPU_used, tx2.GPU_freq);
        if(tx2.CPU_used[1] == -1) {
            printf("\|  \033[40;35mCPU%d\tOFF\033[0m\t\t\t\|", 1);
        }
        else {
            printf("\|  \033[40;36mCPU%d\t\t%dMHz\t%d\%\033[0m\t\|", 1, tx2.CPU_freq[1], tx2.CPU_used[1]);
        }
        printf("  \033[40;32mEMC\t\t%d\%\t%dMHz\033[0m\t\|\n", tx2.EMC_used, tx2.EMC_freq);
        if(tx2.CPU_used[2] == -1) {
            printf("\|  \033[40;35mCPU%d\tOFF\033[0m\t\t\t\|", 2);
        }
        else {
            printf("\|  \033[40;36mCPU%d\t\t%dMHz\t%d\%\033[0m\t\|", 2, tx2.CPU_freq[2], tx2.CPU_used[2]);
        }
        printf("  \033[40;32mAPE\t\t%d\033[0m\t\t\|\n", tx2.APE);
        if(tx2.CPU_used[3] == -1) {
            printf("\|  \033[40;35mCPU%d\tOFF\033[0m\t\t\t\|", 3);
        }
        else {
            printf("\|  \033[40;36mCPU%d\t\t%dMHz\t%d\%\033[0m\t\|", 3, tx2.CPU_freq[3], tx2.CPU_used[4]);
        }
        printf("  \033[40;34mRAM\t%d\t%d\t%d\%\033[0m\t\|\n", tx2.RAM_used, tx2.RAM_all, tx2.RAM_used * 100 / tx2.RAM_all);
        if(tx2.CPU_used[4] == -1) {
            printf("\|  \033[40;35mCPU%d\tOFF\033[0m\t\t\t\|", 4);
        }
        else {
            printf("\|  \033[40;36mCPU%d\t\t%dMHz\t%d\%\033[0m\t\|", 4, tx2.CPU_freq[4], tx2.CPU_used[4]);
        }
        printf("  \033[40;34mSWAP\t%d\t%d\t%d\%\033[0m\t\|\n", tx2.SWAP_used, tx2.SWAP_all, tx2.SWAP_used * 100 / tx2.SWAP_all);

        printf("=================================================================\n");
        if(tx2.MCPU_temp >= 40.0) 
            if(tx2.MCPU_temp >= 60.0)
                printf("\|  \033[40;31mMPCU\t%.2fC\033[0m", tx2.MCPU_temp);
            else
                printf("\|  \033[40;33mMPCU\t%.2fC\033[0m", tx2.MCPU_temp);
        else
            printf("\|  \033[40;34mMPCU\t%.2fC\033[0m", tx2.MCPU_temp);
        
        printf("\t  \|  \033[40;32mPLL\t%.2fC\'\033[0m\t\|   \033[40;32mBoard   %.2fC\'\033[0m\t\|\n", tx2.PLL_temp,  tx2.Tboard_temp);
        
        if(tx2.BCPU_temp >= 40.0) 
            if(tx2.BCPU_temp >= 60.0)
                printf("\|  \033[40;31mBPCU\t%.2fC\033[0m", tx2.BCPU_temp);
            else
                printf("\|  \033[40;33mBPCU\t%.2fC\033[0m", tx2.BCPU_temp);
        else
            printf("\|  \033[40;34mBPCU\t%.2fC\033[0m", tx2.BCPU_temp);
        
        printf("\t  \|  \033[40;32mPMIC\t%dC\'\033[0m\t\|   \033[40;32mDiode   %.2fC\'\033[0m\t\|\n", (int)tx2.PMIC_temp, tx2.Tdiode_temp);
        printf("\|\t\t  \|\t\t\t\|   \033[40;32mthermal %.2fC\'\033[0m\t\|\n", tx2.thermal_temp);

        printf("=================================================================\n");
        
            }     
    pclose(stream);
    return 0;
    }
