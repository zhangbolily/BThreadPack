#ifndef _MULTI_LANGUAGE_HELLO_WORLD_
#define _MULTI_LANGUAGE_HELLO_WORLD_

#include <iostream>
#include <sstream>
#include <unistd.h>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BGeneralTask.h"
#include "BThreadPack/BGeneralThreadPool.h"

using namespace std;
using namespace BThreadPack;

class EnglishHelloWorldTask: public BGeneralTask{
public:
    EnglishHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~EnglishHelloWorldTask()
    {
    }
    
    virtual int process()
    {
    	std::ostringstream _os;
    	_os<<"\033[32m"<<"Hello World!"<<"\033[0m\n";
        cout<<_os.str();
    }
};

class FrenchHelloWorldTask: public BGeneralTask{
public:
    FrenchHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~FrenchHelloWorldTask()
    {
    }
    
    virtual int process()
    {
    	std::ostringstream _os;
    	_os<<"\033[32m"<<"Bonjour le monde!"<<"\033[0m\n";
        cout<<_os.str();
    }
};

class SapnishHelloWorldTask: public BGeneralTask{
public:
    SapnishHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~SapnishHelloWorldTask()
    {
    }
    
    virtual int process()
    {
    	std::ostringstream _os;
    	_os<<"\033[32m"<<"Hola Mundo!"<<"\033[0m\n";
        cout<<_os.str();
    }
};

class RussianHelloWorldTask: public BGeneralTask{
public:
    RussianHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~RussianHelloWorldTask()
    {
    }
    
    virtual int process()
    {
    	std::ostringstream _os;
    	_os<<"\033[32m"<<"Привет, мир!"<<"\033[0m\n";
        cout<<_os.str();
    }
};

class IndianHelloWorldTask: public BGeneralTask{
public:
    IndianHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~IndianHelloWorldTask()
    {
    }
    
    virtual int process()
    {
    	std::ostringstream _os;
    	_os<<"\033[32m"<<"नमस्ते दुनिया!"<<"\033[0m\n";
        cout<<_os.str();
    }
};

class EmojiHelloWorldTask: public BGeneralTask{
public:
    EmojiHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~EmojiHelloWorldTask()
    {
    }
    
    virtual int process()
    {
    	std::ostringstream _os;
    	
    	_os<<"\033[32m"<<"🙋  🌎"<<"\033[0m\n";
        cout<<_os.str();
    }
};

class JapaneseHelloWorldTask: public BGeneralTask{
public:
    JapaneseHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~JapaneseHelloWorldTask()
    {
    }
    
    virtual int process()
    {
    	std::ostringstream _os;
    	_os<<"\033[32m"<<"こんにちは世界"<<"\033[0m\n";
        cout<<_os.str();
    }
};

class KoreanHelloWorldTask: public BGeneralTask{
public:
    KoreanHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~KoreanHelloWorldTask()
    {
    }
    
    virtual int process()
    {
    	std::ostringstream _os;
    	_os<<"\033[32m"<<"안녕하세요!"<<"\033[0m\n";
        cout<<_os.str();
    }
};

class ChineseHelloWorldTask: public BGeneralTask{
public:
    ChineseHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~ChineseHelloWorldTask()
    {
    }
    
    virtual int process()
    {
    	std::ostringstream _os;
    	_os<<"\033[32m"<<"你好，世界！"<<"\033[0m\n";
        cout<<_os.str();
    }
};

#endif
