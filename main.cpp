#include <iostream>
#include <thread>
#include "barbershop.h"

using namespace std;

void creating_new_customer(barber& b)//用一条线程来跑
{
    for (int i = 1;; ++i)
    {
        customer c(i);
        std::thread t(&customer::arriving, std::ref(c), std::ref(b));
        t.detach();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main(int argc, char** argv)
{
    barber b;
    thread barber_thread(&barber::execute, std::ref(b));
    thread creating_thread(&creating_new_customer, std::ref(b));
    barber_thread.join();
    creating_thread.join();

    return 0;
}