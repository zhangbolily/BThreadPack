#include "multi_language_hello_world.h"

int num_threads = 20;
BGeneralThreadPool hello_world_pool(num_threads);

int main(int argc, char** argv)
{
    int task_num = 100;
    
    //1. Start threads
    if(hello_world_pool.initThreads(&hello_world_pool) == B_THREAD_POOL_IS_FULL)
        return B_THREAD_POOL_IS_FULL;
    
    //2. Add task into thread pool.
    ChineseHelloWorldTask* p_chinese_hello = new ChineseHelloWorldTask();
    hello_world_pool.addTask((void *)p_chinese_hello);
    
    EnglishHelloWorldTask* p_english_hello = new EnglishHelloWorldTask();
    hello_world_pool.addTask((void *)p_english_hello);
    
    FrenchHelloWorldTask* p_french_hello = new FrenchHelloWorldTask();
    hello_world_pool.addTask((void *)p_french_hello);
    
    SapnishHelloWorldTask* p_spanish_hello = new SapnishHelloWorldTask();
    hello_world_pool.addTask((void *)p_spanish_hello);
    
    JapaneseHelloWorldTask* p_japanese_hello = new JapaneseHelloWorldTask();
    hello_world_pool.addTask((void *)p_japanese_hello);
    
    KoreanHelloWorldTask* p_korean_hello = new KoreanHelloWorldTask();
    hello_world_pool.addTask((void *)p_korean_hello);
    
    hello_world_pool.startAllTask();
    
    sleep(1);
    
    cout<<"Press any key to exit......"<<endl;
    getchar();
    
    delete p_chinese_hello;
    delete p_english_hello;
    delete p_french_hello;
    delete p_spanish_hello;
	delete p_japanese_hello;
	delete p_korean_hello;
	
    exit(0);
}
