/* MIT License
* 
* Copyright (c) 2018 Ball Chang
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
 * @Author      : Ball Chang
 * @File        : BUtils.cpp
 * @Date        : 2019-3-20
*/

#include "BUtils/BUtils.h"

namespace BUtils {

const char UUID_CHARS[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

std::string generateUUID4() {
    std::random_device true_rand;
    std::string uuid = std::string(36, ' ');
    int random_value = 0;

    uuid[8] = '-';
    uuid[13] = '-';
    uuid[18] = '-';
    uuid[23] = '-';

    uuid[14] = '4';

    for (int i = 0; i < 36; i++) {
        if (i != 8 && i != 13 && i != 18 && i != 14 && i != 23) {
            random_value = true_rand();
            uuid[i] = UUID_CHARS[(i == 19) ?
                     ((random_value & 0xf) & 0x3) | 0x8 : random_value & 0xf];
        }
    }

    return uuid;
}

bool isUUID4(const std::string &_uuid) {
    if (_uuid.size() != 36)
        return false;

    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if (_uuid[i] != '-')
                return false;
        } else if (i == 14) {
            if (_uuid[i] != '4')
                return false;
        } else if (_uuid[i] != '0' &&
                    (strtol(_uuid.substr(i, 1).c_str(), nullptr, 16) > 15)) {
            return false;
        }
    }

    return true;
}

}  // namespace BUtils

