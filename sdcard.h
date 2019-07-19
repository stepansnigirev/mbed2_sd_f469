#ifndef SD_F469_H
#define SD_F469_H

#include <string>
using std::string;

#define SD_SUCCESS     0
#define SD_NO_CARD    -1
#define SD_MOUNT_FAIL -2
#define SD_OPEN_FAIL  -3
#define SD_WRITE_FAIL -4
#define SD_READ_FAIL  -5

class SDCard{
public:
    bool detected() const;
    bool mounted() const;
    int mount();
    int unmount();
    int save(const char * filename, const char * content);
    int save(const string filename, const string content);
    int read(const char * filename, char * buffer, unsigned int size);
    string read(string filename);
};
extern SDCard SD;

#endif