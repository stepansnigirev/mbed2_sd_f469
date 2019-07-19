#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "sdcard.h"
#include "stm32469i_discovery_sd.h"
#include "fatfs/ff_gen_drv.h"
#include "fatfs/sd_diskio.h"

FATFS SDFatFs;
FIL MyFile;

extern SD_HandleTypeDef uSdHandle;
DMA2D_HandleTypeDef hdma2d_disco;

char SDPath[4];
static bool is_mounted = false;

bool SDCard::detected() const{
    return BSP_SD_IsDetected();
}

bool SDCard::mounted() const{
    return is_mounted;
}

int SDCard::mount(){
    if(!detected()){
        return SD_NO_CARD;
    }
    FATFS_LinkDriver(&SD_Driver, SDPath);
    FRESULT res;

    res = f_mount(&SDFatFs, (TCHAR const*)SDPath, 0);
    if(res != FR_OK){
        return SD_MOUNT_FAIL;
    }

    is_mounted = true;
    return SD_SUCCESS;
}

int SDCard::unmount(){
    f_close(&MyFile);
    FATFS_UnLinkDriver(SDPath);
    is_mounted = false;
    return SD_SUCCESS;
}

int SDCard::save(const char * filename, const char * content){
    int res = mount();
    if(res != SD_SUCCESS){
        return res;
    }

    res = f_open(&MyFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if(res != FR_OK){
        return SD_OPEN_FAIL;
    }
 
    unsigned byteswritten;
    res = f_write(&MyFile, content, strlen(content), &byteswritten);
    if(res != FR_OK){
        return SD_WRITE_FAIL;
    }

    unmount();
    return SD_SUCCESS;
}

int SDCard::save(const string filename, const string content){
    return save(filename.c_str(), content.c_str());
}

int SDCard::read(const char * filename, char * buffer, unsigned size){
    int res = mount();
    if(res != SD_SUCCESS){
        return res;
    }

    res = f_open(&MyFile, filename, FA_READ);
    if(res != FR_OK){
        return SD_OPEN_FAIL;
    }
 
    unsigned bytesread;
    res = f_read(&MyFile, buffer, size, &bytesread);
    if(res != FR_OK){
        return SD_WRITE_FAIL;
    }

    unmount();
    return bytesread;
}

string SDCard::read(const string filename){
    int res = mount();
    if(res != SD_SUCCESS){
        return "";
    }

    res = f_open(&MyFile, filename.c_str(), FA_READ);
    if(res != FR_OK){
        return "";
    }

    unsigned size = f_size(&MyFile);
    char * buffer = (char *)calloc(size+1,sizeof(char));
    unsigned bytesread;
    res = f_read(&MyFile, buffer, size, &bytesread);
    string s = buffer;
    free(buffer);

    if(res != FR_OK){
        return "";
    }

    unmount();
    return s;
}

SDCard SD;