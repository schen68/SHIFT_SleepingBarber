//
// Created by 陈帅帅 on 2019-01-23.
//

#include <random>
#include <thread>
#include <chrono>
#include <iostream>
#include "barbershop.h"

int current_customer_ID = 0;
bool asleep = true;
int MAX_SEATS = 3;

std::mutex seat_mutex;
std::mutex batch_leaving_cus_mutex;
std::mutex current_customer_mutex;
std::mutex asleep_mutex;
std::condition_variable wake_up_cv;

std::vector<int> waiting_seats;
std::vector<int> batch_leaving_cus;

bool logging = false; //whether print out the calling log info or not, it is just for debugging. By default, it's false

void barber::working()
{
    if (logging) std::cout << "\n[logging]: barber::working()\n"; // logging info

    std::unique_lock<std::mutex> lock_for_current_customer(current_customer_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock_for_seats(seat_mutex, std::defer_lock);
    std::lock(lock_for_current_customer, lock_for_seats);
    if (!waiting_seats.empty())
    {
        current_customer_ID = waiting_seats.front();
        waiting_seats.erase(waiting_seats.begin());
    }

    lock_for_seats.unlock();

    int working_time = std::rand() % 5000 + 1000; //To create the same random number, do not use srand or another way to create random numbers
    std::this_thread::sleep_for(std::chrono::milliseconds(working_time));

    if (logging) std::cout << "\n*[logging]: working time: " + std::to_string(working_time) + "ms | current customer:" + std::to_string(current_customer_ID) +"\n";

    std::lock_guard<std::mutex> lock_for_asleep(asleep_mutex);
    if (waiting_seats.empty())
        asleep = true;
}

/*
 * if the barber finds that there is no cus in the waiting room, it goes to sleep.
 * It means that gos into the "while" loop, and runs the loop one time, then locks and wait for the signal of wake_up_cv
 * at this time, a new cus arrives, cus calls the customer::wakingup_barber(), which turns asleep to false, and gives the signal
 * the barber goes forward, and the asleep is false, so it goes out and calls barber::working()
 */
void barber::sleeping()
{
    if (logging) std::cout << "\n[logging]: barber::sleeping(): " + std::string (asleep ? "YES" : "NO") + "\n";// logging info

    std::unique_lock<std::mutex> lock_for_asleep(asleep_mutex);
    while (asleep && waiting_seats.empty())
    {
        std::cout << "\n" + std::string(logging ? "----->" : "") + "Barber sleeping" << std::endl << std::string(logging ? "----->" : "") + "Waiting room:" << std::endl;
        wake_up_cv.wait(lock_for_asleep);
    }
}

void barber::execute(int _MAX_SEATS)
{
    MAX_SEATS = _MAX_SEATS;
    if (logging) std::cout << "\n[logging]: barber::execute()\n";// logging info
    while (true)
    {
        sleeping();
        working();
        /*
         * if print out logging information,
         * use "----->" to high line the cutting, waiting and leaving information
         */
        std::lock_guard<std::mutex> lock_for_print(batch_leaving_cus_mutex);
        std::cout << "\n" + std::string(logging ? "----->" : "") + "Barber cutting the hair of customer " << current_customer_ID << std::endl
                  << std::string(logging ? "----->" : "") + "Waiting room: ";
        for_each(waiting_seats.begin(), waiting_seats.end(), [](int i){std::cout << i << " ";});
        std::cout << std::endl;

        if (!batch_leaving_cus.empty())
        {
            std::cout << std::string(logging ? "----->" : "") + "customer ";
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

void customer::leaving(int _ID)
{
    if (logging) std::cout << "\n[logging]: customer::leaving() ID:" + std::to_string(ID) + '\n';// logging info

    std::lock_guard<std::mutex> lock_for_leaving_notice(batch_leaving_cus_mutex);
    batch_leaving_cus.push_back(_ID);
}

void customer::waiting(int _ID)
{
    if (logging) std::cout << "\n[logging]: customer::waiting() ID:" + std::to_string(ID) + '\n';// logging info
    std::unique_lock<std::mutex> lock_for_waiting_seats(seat_mutex);
    waiting_seats.push_back(_ID);
}
/*
 * actually, I don't want to create this function, but you said waking up the barber is a task, so I create wakingup_barber()
 *
 * in this way, if a customer arrives, no one in th waiting room and the barber is sleeping,
 * the customer wakes up the barber and gets cutting directly. the cus will not go into the waiting room.
 *
 * Actually, the cus could go into the waiting room and wait for barber to take it to the current_customer_ID.
 *
 * both two ways work
 */
void customer::wakingup_barber(int _ID)//
{
    if (logging) std::cout << "\n[logging]: customer::wakingup_barber() ID:" + std::to_string(_ID) + '\n';// logging info
    std::unique_lock<std::mutex> lock_for_waking_up(asleep_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock_for_current_customer(current_customer_mutex, std::defer_lock);
    std::lock(lock_for_current_customer, lock_for_waking_up);
    asleep = false;
    current_customer_ID = _ID;

    wake_up_cv.notify_all();
}

void customer::arriving(int _ID)
{
    ID = _ID;

    if (logging) std::cout << "\n[logging]: customer::arriving() ID:" + std::to_string(ID) + '\n';// logging info

    if (waiting_seats.size() == MAX_SEATS)
    {
        if (logging) std::cout << "\n[logging]: leaving ID:" + std::to_string(ID) + '\n';// logging info
        leaving(ID);
    }
    else if (asleep && waiting_seats.empty())
    {
        if (logging) std::cout << "\n[logging]: wake up the barber! ID:" + std::to_string(ID) + '\n';// logging info
        wakingup_barber(ID);
    }
    else if (!asleep && waiting_seats.size() < MAX_SEATS)
    {
        if (logging) std::cout << "\n[logging]: go into waiting room! ID:" + std::to_string(ID) + '\n';// logging info
        waiting(ID);
    }
    else if (waiting_seats.size() > MAX_SEATS) //from here to the end, the code is not meaningful, it just helps me to catch errors, and no error occurs
    {
        std::cerr << "what?! the size of waiting_seats is larger than the MAX_SEATS?!" << std::endl;
    }
    else
    {
        std::cerr<<"\nserious error: ID:" + std::to_string(ID) + "; waiting room size:" + std::to_string(waiting_seats.size()) + "; asleep:" +std::to_string(asleep) + "\n";
    }
}