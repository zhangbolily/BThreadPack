/*
 * @Author      : Ball Chang
 * @File        : BAbstractClass.h
 * @Date        : 2019-3-3
*/

#ifndef _BABSTRACT_CLASS_H_
#define _BABSTRACT_CLASS_H_

namespace BThreadPack {

class NoneCopy{
protected:
    NoneCopy(){}
    ~NoneCopy(){}
    
private:
    NoneCopy(const NoneCopy&);
    NoneCopy& operator=(const NoneCopy&);
};
};

#endif
