//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <deque>
//#include <chrono>
//#include <random>
//#include <vector>
//#include <algorithm>
//#include <set>
//#include <vector>
//using namespace std;
//
//int maxArea(vector<int>& height) {
//    int max_area = 0;
//    for (auto ite1 = height.begin(); ite1 != height.end(); ++ite1)
//    {
//        int temp_area = 0;
//        for (auto ite2 = ite1 + 1; ite2 != height.end(); ++ite2)
//        {
//            temp_area = max(temp_area, (min(*ite1, *ite2) * distance(ite1, ite2)));
//        }
//        max_area = max(max_area, temp_area);
//    }
//    return max_area;
//
//}
//
//
//#define NUM_OF_SEATS 5
//
//deque<int> consumer_id;
//mutex M;
//condition_variable condition;
//
//
//int generate_random_num(int min, int max)
//{
//    unsigned time_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
//    std::mt19937 rand_num_generator(time_seed);
//    std::uniform_int_distribution<int> dist(min, max);
//    return dist(rand_num_generator);
//}
//
//void create_consumers()
//{
//    for (int i = 1;; ++i)
//    {
//        unique_lock<mutex> locker(M);
//        if (consumer_id.size() < NUM_OF_SEATS + 1)
//            consumer_id.push_front(i);
//        else
//            cout << "Consumer " << i << " is leaving" << endl;
//        locker.unlock();
//        condition.notify_one();
//        this_thread::sleep_for(chrono::seconds(3));
//    }
//}
//
//void barber_shop()
//{
//    int consumer_getting_haircut = -1;
//    for (int i = 1;; ++i)
//    {
//        if (consumer_id.size() == 0)
//        {
//            cout << "The barber is sleeping" << endl;
//
//        }
//        unique_lock<mutex> locker(M);
//        condition.wait(locker);
//        consumer_getting_haircut = consumer_id.back();
//        consumer_id.pop_back();
//        locker.unlock();
//        cout << "Barber got the consumer " << consumer_getting_haircut << endl;
//        cout << "Waiting Room: ";
//        auto print = [](int i){ cout << i << " "; };
//        for_each(consumer_id.begin(), consumer_id.end(), print);
//        cout << endl << endl;
//        //this_thread::sleep_for(chrono::seconds(1));
//        //this_thread::sleep_for(chrono::seconds(generate_random_num(1,2)));
//
//
//    }
//}
//
//void press_any_key_to_end()
//{
//    cout << "Press any key to end!" << endl;
//    char anykey;
//    cin >> anykey;
//    return;
//}
//
//int main()
//{
//    set<vector<int>[3]> a;
//
//    vector<int> b;
//    for (int i = 0; i < 4; ++i)
//    {
//        b.push_back(i);
//    }
//    a.;
//    cout<<a;
////    thread T1(create_consumers);
////    thread T2(barber_shop);
////    T1.join();
////    T2.join();
////    press_any_key_to_end();
//    return 0;
//}

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