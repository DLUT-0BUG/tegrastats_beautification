#include "stdio.h"
#include "unistd.h"
#define BUF_SIZE 400

class TX2Status {
private:
    struct Status{    
    int RAM_used, RAM_all, lfb[2];          //RAM 1952/7852MB (lfb 196x4MB) 
    int SWAP_used, SWAP_all, cached;//SWAP 0/3926MB (cached 0MB) 
    int CPU_used[6];                //CPU [2%@345,off,off,2%@345,1%@346,0%@345]
    int CPU_freq[6]; 
    int EMC_bus, EMC_freq;          //EMC_FREQ 16%@102 		bus%@MHz
    int GPU_used, GPU_freq;         //GR3D_FREQ 0%@114 
    int APE;                        //APE 150 

    float PLL_temp;                   //PLL@33.5C 
    float MCPU_temp;                  //MCPU@33.5C 
    float PMICP_temp;                 //MIC@100C 
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
    
    Status tx2;
    FILE *stream;

public:
    TX2Status();
    ~TX2Status();
    int DisplayStatus();
    int CleanStatus();
    int StatusMatch();
}

TX2Status::TX2Status() {    
    stream = popen("tegrastats", "r");   
    if( stream == NULL) {
        printf("pipe failed\n");
        return -1;
    } 
    CleanStatus();
}
TX2Status::~TX2Status() {
    pclose(stream);
}


int TX2Status::CleanStatus () {
    tx2.RAM_used    = -1;
    tx2.RAM_all     = -1;
    tx2.lfb         = {-1};
    tx2.SWAP_used   = -1;
    tx2.SWAP_all    = -1;
    tx2.cached      = -1;
    tx2.CPU_used[6] = {-1}; 
    tx2.CPU_freq[6] = {-1}; 
    tx2.EMC_bus     = -1;
    tx2.EMC_freq    = -1;  
    tx2.GPU_used    = -1; 
    tx2.GPU_freq    = -1; 
    tx2.APE         = -1;   

    tx2.PLL_temp    = -1.0;  
    tx2.MCPU_temp   = -1.0;  
    tx2.PMICP_temp  = -1.0; 
    tx2.Tboard_temp = -1.0;  
    tx2.GPU_temp    = -1.0; 
    tx2.BCPU_temp   = -1.0;  
    tx2.thermal_temp= -1.0; 
    tx2.Tdiode_temp = -1.0; 

    tx2.VDD_SYS_GPU[2]  = {-1}; 
    tx2.VDD_SYS_SOC[2]  = {-1};  
    tx2.VDD_4V0_WIFI[2] = {-1}; 
    tx2.VDD_IN[2]       = {-1};  
    tx2.VDD_SYS_CPU[2]  = {-1}; 
    tx2.VDD_SYS_DDR[2]  = {-1}; 
    return 0;
}

int TX2Status::StatusMatch() {
    char *p = buffer;
    int i = 0 cnt = 0;
    int dot = 10;
    int buf = 0;
    float temp = 0.0;
    char buffer[BUF_SIZE]

    fgets(buffer, BUF_SIZE, stream);
    p = buffer;

    while( tx2.VDD_SYS_DDR[1] == 0) {
        switch( *p ) {
        case A: {
            buf = 0;
            p = p + 4;
            while( *p >= '0' && *p <= '9' ) {
                buf *= 10;
                buf += (*p - 0x30);
                p ++;
            }
            tx2.APE = buf;
            buf = 0;
            p ++;
            break;
        }
        case B: {
            temp = 0;
            p = p + 5;
            while( *p >= '0' && *p <= '9' ) {
                temp *= 10.0;
                temp += (float)(*p - 0x30);
                p ++;
            }
            p ++
            while( *p >= '0' && *p <= '9' ) {
                temp += ( (float)(*p - 0x30) / (float)dot );
                dot *= 10;
                p ++;
            }
            tx2.BCPU_temp = temp;
            temp = 0;
            p = p + 2;  
            break;          
        }
        case C: {
            int i = 0;
            buf = 0;
            p = p + 5;
            while( i < 6) {
                if ( *p == 'o' ) {
                    tx2.CPU_used[i] = -1;
                    tx2.CPU_freq[i] = -1;
                }
                else {
                    while( *p >= '0' && *p <= '9' ) {
                        buf *= 10;
                        buf += (*p - 0x30);
                        p ++;
                    }
                    tx2.CPU_used[i] = buf;
                    buf = 0;
                    p = p + 3;
                    while( *p >= '0' && *p <= '9' ) {
                        buf *= 10;
                        buf += (*p - 0x30);
                        p ++;
                    }
                    tx2.CPU_temp[i] = buf; 
                    buf = 0;
                    p = p + 2;                   
                }
                i ++;
            }
            p = p + 3;
            break;
        }
        case c: {
            buf = 0;
            p = p + 7;
            while( *p >= '0' && *p <= '9' ) {
                buf *= 10;
                buf += (*p - 0x30);
                p ++;
            }
            tx2.cached = buf;
            buf = 0;
            p = p + 5;            
        }
        case E: {
            
        }
        case G:
        case l:
        case M:
        case o:
        case P:
        case R:
        case S:
        case T:
        case t:
        case x:
        case V:
    }
    }
}

int main (void) {
    while(1) {
        TX2Status::StatusMatch();
        TX2Status::DisplayStatus();
        TX2Status::CleanStatus();
    }
    return 0;
}