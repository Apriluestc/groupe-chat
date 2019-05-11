/*
 * ========================================================================
 * 
 *          fileName:           txproj_version.cpp
 *          description:        version.h 的测试文件
 *
 * ========================================================================
 * */

#include <unistd.h>

#include "../include/version.h"

int main()
{
    Version v;
    v.drawHeader();
    //v.drawFiles();
    v.drawOutput();
    v.drawOnline();
    v.drawInput();
    std::string title1  = "Multiplayer chat room";
    std::string title2  = "userInPut";
    std::string title3  = "olineList";
    std::string title4  = "userOutPut";
    
    size_t i = 1;
    int y = 0;
    int x = 0;
    int dir = 0;
    
    while(1) {
        v.drawHeader();
        getmaxyx(v.getHeader(), y, x);

        v.putStringOfVersion(v.getHeader(), y/2, 65, title1);
        v.putStringOfVersion(v.getInput(), 1, 55, title2);
        v.putStringOfVersion(v.getOnline(), 1, 15, title3);
        v.putStringOfVersion(v.getOutput(), 1, 55, title4);

        if(i >= x - title1.size() - 3) {
            dir = 1;
        }
        if( i <= 4) {
            dir = 0;
        }
        if(dir == 0) {
            i++;
        } else { 
            i--;
        }
        usleep(1000000);
    }
    sleep(10);
    
    return 0;
}
