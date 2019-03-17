#include "HelloWorld.h"

int num_threads = 20;
BGeneralThreadPool hello_world_pool(num_threads, BAbstractThreadPool::BThreadControlMode::DynamicThreadCapacity);

int main(int argc, char** argv)
{   
    //1. Add task into thread pool.
    ChineseHelloWorldTask* p_chinese_hello = new ChineseHelloWorldTask();
    p_chinese_hello->setPriority(5);
    hello_world_pool.pushTask(static_cast<void*>(p_chinese_hello));
    
    EnglishHelloWorldTask* p_english_hello = new EnglishHelloWorldTask();
    p_english_hello->setPriority(5);
    hello_world_pool.pushTask(static_cast<void*>(p_english_hello));
    
    FrenchHelloWorldTask* p_french_hello = new FrenchHelloWorldTask();
    hello_world_pool.pushTask(static_cast<void*>(p_french_hello));
    
    SapnishHelloWorldTask* p_spanish_hello = new SapnishHelloWorldTask();
    hello_world_pool.pushTask(static_cast<void*>(p_spanish_hello));
    
    RussianHelloWorldTask* p_russian_hello = new RussianHelloWorldTask();
    hello_world_pool.pushTask(static_cast<void*>(p_russian_hello));
    
    EmojiHelloWorldTask* p_emoji_hello = new EmojiHelloWorldTask();
    p_emoji_hello->setPriority(4);
    hello_world_pool.pushTask(static_cast<void*>(p_emoji_hello));
    
    IndianHelloWorldTask* p_indian_hello = new IndianHelloWorldTask();
    p_indian_hello->setPriority(4);
    hello_world_pool.pushTask(static_cast<void*>(p_indian_hello));
    
    JapaneseHelloWorldTask* p_japanese_hello = new JapaneseHelloWorldTask();
    hello_world_pool.pushTask(static_cast<void*>(p_japanese_hello));
    
    KoreanHelloWorldTask* p_korean_hello = new KoreanHelloWorldTask();
    hello_world_pool.pushTask(static_cast<void*>(p_korean_hello));
    
    //A different way to print multi-language hello world.
    std::string hello_world_str;
    hello_world_str = "Hello World!";
    GeneralHelloWorldTask* p_general_hello = new GeneralHelloWorldTask(hello_world_str.c_str(), hello_world_str.size() + 1);
    p_general_hello->setPriority(1);
    hello_world_pool.pushTask(static_cast<void*>(p_general_hello));
    
    hello_world_str = "Bonjour le monde!";
    p_general_hello = new GeneralHelloWorldTask(hello_world_str.c_str(), hello_world_str.size() + 1);
    p_general_hello->setPriority(2);
    hello_world_pool.pushTask(static_cast<void*>(p_general_hello));
    
    hello_world_str = "Hola Mundo!";
    p_general_hello = new GeneralHelloWorldTask(hello_world_str.c_str(), hello_world_str.size() + 1);
    p_general_hello->setPriority(3);
    hello_world_pool.pushTask(static_cast<void*>(p_general_hello));
    
    hello_world_str = "Привет, мир!";
    p_general_hello = new GeneralHelloWorldTask(hello_world_str.c_str(), hello_world_str.size() + 1);
    p_general_hello->setPriority(4);
    hello_world_pool.pushTask(static_cast<void*>(p_general_hello));
    
    hello_world_str = "नमस्ते दुनिया!";
    p_general_hello = new GeneralHelloWorldTask(hello_world_str.c_str(), hello_world_str.size() + 1);
    p_general_hello->setPriority(5);
    hello_world_pool.pushTask(static_cast<void*>(p_general_hello));
    
    hello_world_str = "🙋  🌎";
    p_general_hello = new GeneralHelloWorldTask(hello_world_str.c_str(), hello_world_str.size() + 1);
    p_general_hello->setPriority(1);
    hello_world_pool.pushTask(static_cast<void*>(p_general_hello));
    
    hello_world_str = "こんにちは世界";
    p_general_hello = new GeneralHelloWorldTask(hello_world_str.c_str(), hello_world_str.size() + 1);
    p_general_hello->setPriority(2);
    hello_world_pool.pushTask(static_cast<void*>(p_general_hello));
    
    hello_world_str = "안녕하세요!";
    p_general_hello = new GeneralHelloWorldTask(hello_world_str.c_str(), hello_world_str.size() + 1);
    p_general_hello->setPriority(3);
    hello_world_pool.pushTask(static_cast<void*>(p_general_hello));
    
    hello_world_str = "你好，世界！";
    p_general_hello = new GeneralHelloWorldTask(hello_world_str.c_str(), hello_world_str.size() + 1);
    p_general_hello->setPriority(4);
    hello_world_pool.pushTask(static_cast<void*>(p_general_hello));
    
    hello_world_pool.startAllTasks();
    
    getchar();
    
    hello_world_pool.kill();
	
    return 0;
}
