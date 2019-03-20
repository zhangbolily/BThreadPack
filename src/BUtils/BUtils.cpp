/*
 * @Author      : Ball Chang
 * @File        : BUtils.cpp
 * @Date        : 2019-3-20
*/

#include "BUtils/BUtils.h"

namespace BUtils{

const std::string UUID_CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

std::string generateUUID4()
{
    std::random_device true_rand;
    std::string uuid = std::string(36,' ');
    int random_value = 0;

    uuid[8] = '-';
    uuid[13] = '-';
    uuid[18] = '-';
    uuid[23] = '-';

    uuid[14] = '4';

    for(int i=0;i < 36;i++){
        if (i != 8 && i != 13 && i != 18 && i != 14 && i != 23) {
            random_value = true_rand();
            uuid[i] = UUID_CHARS[(i == 19) ? ((random_value & 0xf) & 0x3) | 0x8 : random_value & 0xf];
        }
    }
    
    return uuid;
}

bool isUUID4(const std::string &_uuid)
{
    if(_uuid.size() != 36)
        return false;

    for(int i = 0;i < 36;i++){
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if(_uuid[i] != '-')
                return false;
        } else if(i == 14) {
            if(_uuid[i] != '4')
                return false;
        } else if(_uuid[i] != '0' && (strtol(_uuid.substr(i, 1).c_str(), nullptr, 16) > 15))
                return false;
    }
    
    return true;
}

}

