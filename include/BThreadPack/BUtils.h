/*
 * @Author      : Ball Chang
 * @File        : BUtils.h
 * @Date        : 2019-3-20
*/

#ifndef _BUTILS_H_
#define _BUTILS_H_

#include <string>
#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "BCore/BCore.h"

namespace BUtils{

std::string generateUUID4();

bool isUUID4(const std::string &_uuid);

}

#endif
