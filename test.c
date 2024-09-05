#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "mms_test_api.h"

void main()
{
    int *handle = NULL;
    MmsServerInit((void*)&handle, "/mnt/hgfs/ubuntuShare/model.cfg", "192.168.5.197", 555);
    // while(1);
    float value = 0.0;
    float value2 = 0.0;
    int valueInt = 0;
    int valueInt2 = 0;
    while(1) {
#if 1
        MmsServerSetValue(handle, "PL1101LD0/LLN0.Mod.ctlModel", &valueInt, 4);
        printf("set value : %d\n", valueInt);
        sleep(1);
        valueInt += 1;
        if(valueInt == 5) {
            valueInt = 0;
        }  
        MmsServerGetValue(handle, "PL1101LD0/LLN0.Mod.ctlModel", &valueInt2, 4);   
        printf("get value : %d\n", valueInt2); 
        sleep(1);    
#else
        sleep(10);
        // MmsServerSetValue(handle, "PL1101LD0/STMP1.Tmp.mag.f", &value, 4);
        // printf("set value : %f\n", value);
        // value += 1.1;
        // if(value == 5.0) {
        //     value = 0;
        // } 
        // usleep(3000);
        // MmsServerGetValue(handle, "PL1101LD0/STMP1.Tmp.mag.f", &value2, 4);
        // printf("get value : %f\n", value2);
        // usleep(3000);
#endif
    }
}
