//
// Created by 陈帅帅 on 2019-01-23.
//

#include <random>
#include <thread>
#include <chrono>
#include <iostream>
//#include <time.h>
#include "barbershop.h"

std::mutex seat_mutex;
std::mutex batch_leaving_cus_mutex;
std::mutex current_customer_mutex;
std::mutex asleep_mutex;
std::condition_variable go_to_sleep_cv;

int current_customer_ID = 0;
bool asleep = true;
int MAX_SEATS = 3;
//std::queue<int> waiting_seats;
std::vector<int> waiting_seats;
std::vector<int> batch_leaving_cus;

int generate_random_num(int min, int max)
{
    unsigned time_seed = static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::mt19937 rand_num_generator(time_seed);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rand_num_generator);
}

void barber::working()//记得每次输出之后把全部初始化，特别是one_batch_leaving
{//std::cout<<"\n这里是: barber::working()\n";
    std::unique_lock<std::mutex> lock_for_current_customer(current_customer_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock_for_seats(seat_mutex, std::defer_lock);
    std::lock(lock_for_current_customer, lock_for_seats);
    if (!waiting_seats.empty())
    {
        current_customer_ID = waiting_seats.front();
        //waiting_seats.pop();
        waiting_seats.erase(waiting_seats.begin());
    }

    lock_for_seats.unlock();
    int waiting_time = generate_random_num(1,5);
    //std::cout<<"*工作时间: " + std::to_string(waiting_time) + "s 当前customer:" + std::to_string(current_customer_ID) +"\n";
    std::this_thread::sleep_for(std::chrono::seconds(6));

    std::lock_guard<std::mutex> lock_for_asleep(asleep_mutex);
    if (waiting_seats.empty())
        asleep = true;
}

void barber::sleeping()
{//std::cout<<"\n这里是:barber::sleeping()\n";
    std::unique_lock<std::mutex> lock_for_asleep(asleep_mutex);
    while (asleep && waiting_seats.empty())
    {
        std::cout << "\nBarber sleeping" << std::endl << "Waiting room:" << std::endl;
        go_to_sleep_cv.wait(lock_for_asleep);
    }
}

void barber::wakingup(int ID)
{//std::cout<<"\n这里是:barber::wakingup()\n";
    std::unique_lock<std::mutex> lock_for_waking_up(asleep_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock_for_current_customer(current_customer_mutex, std::defer_lock);
    std::lock(lock_for_current_customer, lock_for_waking_up);
    asleep = false;
    current_customer_ID = ID;
}

void barber::execute()
{//std::cout<<"\n这里是:barber::execute()\n";
    int counter = 0;
    while (true)
    {
        //std::cout<<std::endl<< ++counter<<std::endl;
        sleeping();
        working();

        std::lock_guard<std::mutex> lock_for_print(batch_leaving_cus_mutex);/////////////
        std::cout << "\nBarber cutting the hair of customer " << current_customer_ID << std::endl
                  << "Waiting room: ";
//        std::queue<int> queue_copy_for_print = waiting_seats;
//        while (!queue_copy_for_print.empty())
//        {
//            std::cout << queue_copy_for_print.front() << " ";
//            queue_copy_for_print.pop();
//        }
        for_each(waiting_seats.begin(), waiting_seats.end(), [](int i){std::cout << i << " ";});
        std::cout << std::endl;

        if (!batch_leaving_cus.empty())
        {
            std::cout << "customer ";
            for_each(batch_leaving_cus.begin(), batch_leaving_cus.end(), [](int i){std::cout << i << " ";});
            std::cout << "leaving" << std::endl;

            batch_leaving_cus.clear();
        }


    }
}

customer::customer(int _id)
{
    ID = _id;
}

void customer::leaving()
{
    std::string a = "\n这里是:customer::leaving() ID:" + std::to_string(ID) + '\n';
    //std::cout<<a;
    std::lock_guard<std::mutex> lock_for_leaving_notice(batch_leaving_cus_mutex);
    //std::cout << "Customer " << ID << " leaving" << std::endl;
    batch_leaving_cus.push_back(ID);
}

void customer::waiting()
{
    std::string a = "\n这里是:customer::waiting() ID:" + std::to_string(ID) + '\n';
    //std::cout<<a;
//    std::unique_lock<std::mutex> lock_for_asleep(asleep_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock_for_waiting_seats(seat_mutex);//, std::defer_lock);
//    std::lock(lock_for_asleep, lock_for_waiting_seats);
//
//    asleep = false;////////////////////要不要存疑
    waiting_seats.push_back(ID);
}

void customer::wakingup_barber(barber& b)
{
    std::string a = "\n这里是:customer::wakingup_barber() ID:" + std::to_string(ID) + '\n';
    //std::cout<<a;
    b.wakingup(ID);
}

void customer::arriving(barber& b)
{
    std::string a = "\n这里是:customer::arriving() ID:" + std::to_string(ID) + '\n';
    //std::cout<<a;
    if (waiting_seats.size() == MAX_SEATS)
    {
        std::string a = "\n他妈的这货跑了 ID:" + std::to_string(ID) + '\n';
        //std::cout<<a;
        leaving();

    }
    else if (waiting_seats.size() > MAX_SEATS)
    {
        std::cerr << "what?! the size of waiting_seats is larger than the MAX_SEATS?!" << std::endl;
    }
    else if (asleep && waiting_seats.empty())
    {
        std::string a = "\n唤醒了理发师 ID:" + std::to_string(ID) + '\n';
        //std::cout<<a;
        wakingup_barber(b);
        go_to_sleep_cv.notify_all();
    }
    else if (!asleep && waiting_seats.size() < MAX_SEATS)
    {
        std::string a = "\n进入了等待室 ID:" + std::to_string(ID) + '\n';
        //std::cout<<a;
        waiting();
        //go_to_sleep_cv.notify_all();
    }
    else
    {
        std::cerr<<"\n进入了坑爹领域: ID:" + std::to_string(ID) + "; 等待室size:" + std::to_string(waiting_seats.size()) + "; 是否在睡觉:" +std::to_string(asleep) + "\n";
    }
}