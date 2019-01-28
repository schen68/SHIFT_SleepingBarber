#include <iostream>
#include <thread>
#include "barbershop.h"

using namespace std;

void creating_new_customer()
{
    customer c(0);
    for (int i = 1;; ++i)
    {
        std::thread t(&customer::arriving, std::ref(c), i);
        t.detach();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main(int argc, char** argv)
{
    int max_seat = 3;
    barber b;

    cout << "enter the max number of seats: " << endl;
    cout << "[MAX_SEATS]: ";
    cin >>max_seat;

    if (!cin) cout << "invalid input!" << endl << "the max_seats is 3, by default." << endl;
    else if (max_seat <= 0) cout << "invalid number!" << endl << "the max_seats is 3, by default." << endl;
    cout << "start in 3s ..." << endl;
    this_thread::sleep_for(std::chrono::seconds(3));

    thread barber_thread(&barber::execute, std::ref(b), std::ref(max_seat));
    thread creating_thread(&creating_new_customer);
    barber_thread.join();
    creating_thread.join();

    return 0;
}