#ifndef _MULTI_LANGUAGE_HELLO_WORLD_
#define _MULTI_LANGUAGE_HELLO_WORLD_

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "BThreadPack/BThreadPack.h"
#include "BThreadPack/BGeneralTask.h"
#include "BThreadPack/BGroupTask.h"
#include "BThreadPack/BThreadPool.h"

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
    
    virtual int execute()
    {
    	std::ostringstream _os;
#ifdef WIN32
        _os << "Hello World!\n";
#else
    	_os << "\033[32m" << "Hello World!" << "\033[0m\n";
#endif
        cout << _os.str();
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
    
    virtual int execute()
    {
    	std::ostringstream _os;
#ifdef WIN32
        _os << "Bonjour le monde!\n";
#else
    	_os << "\033[32m"<<"Bonjour le monde!" << "\033[0m\n";
#endif
        cout << _os.str();
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
    
    virtual int execute()
    {
    	std::ostringstream _os;
#ifdef WIN32
        _os << "Hola Mundo!\n";
#else
    	_os << "\033[32m"<<"Hola Mundo!" << "\033[0m\n";
#endif
        std::cout << _os.str();
    }
};

#ifndef WIN32
class RussianHelloWorldTask: public BGeneralTask{
public:
    RussianHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    ~RussianHelloWorldTask()
    {
    }
    
    virtual int execute()
    {
    	std::ostringstream _os;
    	_os << "\033[32m" << "Привет, мир!" << "\033[0m\n";
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
    
    virtual int execute()
    {
    	std::ostringstream _os;
    	_os << "\033[32m" << "नमस्ते दुनिया!" << "\033[0m\n";
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
    
    virtual int execute()
    {
    	std::ostringstream _os;
    	_os << "\033[32m" << "🙋  🌎" << "\033[0m\n";
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
    
    virtual int execute()
    {
    	std::ostringstream _os;
    	_os << "\033[32m" << "こんにちは世界" << "\033[0m\n";
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
    
    virtual int execute()
    {
    	std::ostringstream _os;
    	_os << "\033[32m" << "안녕하세요!" << "\033[0m\n";
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
    
    virtual int execute()
    {
    	std::ostringstream _os;
    	_os << "\033[32m" << "你好，世界！" << "\033[0m\n";
        cout<<_os.str();
    }
};
#endif

class GeneralHelloWorldTask: public BGeneralTask{
public:
    GeneralHelloWorldTask()
        :BGeneralTask()
    {
    }
    
    GeneralHelloWorldTask(const void* _buffer, size_t _size)
        :BGeneralTask(_buffer, _size)
    {
    }
    
    ~GeneralHelloWorldTask()
    {
    }
    
    virtual int execute()
    {
        void* hello_world_str;
        size_t length = 0;
        inputBuffer(&hello_world_str, length);
        static_cast<char*>(hello_world_str)[length - 1] = '\0';
    	std::ostringstream _os;
#ifdef WIN32
        _os << static_cast<char*>(hello_world_str) << "\n";
#else
    	_os << "\033[32m" << static_cast<char*>(hello_world_str) << "\033[0m\n";
#endif
        cout<<_os.str();
    }
};

#endif
