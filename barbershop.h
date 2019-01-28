//
// Created by 陈帅帅 on 2019-01-22.
//
#include <queue>
#include <mutex>
#include <condition_variable>

#ifndef SHIFT_SLEEPINGBARBER_BARBERSHOP_H

#define SHIFT_SLEEPINGBARBER_BARBERSHOP_H



class barber
{
private:
    void working();
    void sleeping();

public:
    void execute(int _MAX_SEATS);
};

class customer
{
private:
    int ID;

    void leaving(int _ID);
    void waiting(int _ID);
    void wakingup_barber(int _ID);

public:
    explicit customer(int _id);
    void arriving(int _ID);
};

#endif //SHIFT_SLEEPINGBARBER_BARBERSHOP_H