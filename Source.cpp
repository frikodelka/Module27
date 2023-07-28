#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <mutex>
#include <shared_mutex>
#include <thread>

class Logger 
{

private:
    std::fstream log;
    std::shared_mutex mutex;

public:
    Logger()
    {
        log.open("log.txt", std::fstream::in | std::fstream::out | std::fstream::app);
        if (!log.is_open())
            std::cout << "Error of open()\n";
        else std::cout << "File 'log.txt' open() is OK\n";
    }

        ~Logger()
        {
            log.close();
        }
    

    void writeLog(const std::string& sender, const std::string&  receiver, const std::string&  text) 
    {
        std::unique_lock<std::shared_mutex> lock(mutex, std::try_to_lock);
        if (lock.owns_lock()) 
        {
            log << "Sender: " << sender << " Receiver: " << receiver << " Text: " << text << '\n';
        }
    }

    void showLog(const std::string& str)
    {
        std::cout << "Thread ID: " << std::this_thread::get_id() << "  " << str << std::endl;
    }

    std::string readLog(int numstr) {
        std::shared_lock<std::shared_mutex> lock(mutex);
        std::string line;
        std::ifstream log("log.txt");
        log.clear();
        log.seekg(0, std::ios::beg);
        
        for (int i = 0; i < numstr; i++)
        {
            std::getline(log, line);
        }
     
        showLog("String N " + std::to_string(numstr) + " : " + line + '\n');
        return line;
    }
};

int main()
{
    Logger test;
    std::thread t1([&]() {test.writeLog("A", "A", "AAA"); });
    std::thread t2([&]() {test.writeLog("B", "B", "BBB"); });
    std::thread t3([&]() {test.writeLog("C", "C", "CCC"); });
    std::thread t4([&]() {test.writeLog("D", "D", "DDD"); });

    std::thread t5([&]() {test.readLog(1); });
    std::thread t6([&]() {test.readLog(2); });
    std::thread t7([&]() {test.readLog(3); });
    std::thread t8([&]() {test.readLog(4); });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    t5.join();
    t6.join();
    t7.join();
    t8.join();

    return 0;
}