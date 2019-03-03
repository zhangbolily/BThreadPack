#include "HelloWorld.h"

int num_threads = 20;
BGeneralThreadPool hello_world_pool(num_threads, BAbstractThreadPool::BThreadControlMode::DynamicThreadCapacity);

int main(int argc, char** argv)
{   
    //1. Add task into thread pool.
    ChineseHelloWorldTask* p_chinese_hello = new ChineseHelloWorldTask();
    p_chinese_hello->setPriority(5);
    hello_world_pool.addTask(static_cast<void*>(p_chinese_hello));
    
    EnglishHelloWorldTask* p_english_hello = new EnglishHelloWorldTask();
    p_english_hello->setPriority(5);
    hello_world_pool.addTask(static_cast<void*>(p_english_hello));
    
    FrenchHelloWorldTask* p_french_hello = new FrenchHelloWorldTask();
    hello_world_pool.addTask(static_cast<void*>(p_french_hello));
    
    SapnishHelloWorldTask* p_spanish_hello = new SapnishHelloWorldTask();
    hello_world_pool.addTask(static_cast<void*>(p_spanish_hello));
    
    RussianHelloWorldTask* p_russian_hello = new RussianHelloWorldTask();
    hello_world_pool.addTask(static_cast<void*>(p_russian_hello));
    
    EmojiHelloWorldTask* p_emoji_hello = new EmojiHelloWorldTask();
    p_emoji_hello->setPriority(4);
    hello_world_pool.addTask(static_cast<void*>(p_emoji_hello));
    
    IndianHelloWorldTask* p_indian_hello = new IndianHelloWorldTask();
    p_indian_hello->setPriority(4);
    hello_world_pool.addTask(static_cast<void*>(p_indian_hello));
    
    JapaneseHelloWorldTask* p_japanese_hello = new JapaneseHelloWorldTask();
    hello_world_pool.addTask(static_cast<void*>(p_japanese_hello));
    
    KoreanHelloWorldTask* p_korean_hello = new KoreanHelloWorldTask();
    hello_world_pool.addTask(static_cast<void*>(p_korean_hello));
    
    hello_world_pool.startAllTasks();
    
    getchar();
    
    hello_world_pool.kill();
	
    return 0;
}
