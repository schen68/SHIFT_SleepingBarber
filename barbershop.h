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
    //int customer_getting_haircut_ID;
    //bool status_of_sleeping = false;

    void working();
    void sleeping();

public:
    //~barber();
    void wakingup(int ID);
    //bool issleeping();
    void execute();
};

class customer
{
private:
    int ID;

    void leaving();
    void waiting();
    void wakingup_barber(barber& b);

public:
    explicit customer(int _id);
    //~customer();
    void arriving(barber& b);
};

#endif //SHIFT_SLEEPINGBARBER_BARBERSHOP_H