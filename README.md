# shift-sleeping-barber-problem
## Problem discription
This problem is about simulating a barber shop with the following characteristics. The barber shop has a waiting room with M seats and a separate room with a single barber chair where the barber works. If there are no waiting customers, nor a customer having a haircut, the barber takes a break and sleeps. The moment a customer arrives, s/he wakes the barber up so that they can have their haircut. If another customer arrives while the barber is working, s/he will either sit in the waiting room (if there are available seats) or leave (if no seats are available).
## run it
download the source and open it from clion

If you use linux with g++, the compilation commander is
```
g++ -std=c++11 -pthread  barbershop.cpp main.cpp -o run
```
then use
```
./run
```
to run it
## Implement detial
1. route map<br/>
  create two threads. The first thread is to simulate the barber, which is rely on barber class; the other thread is for infinitely creating threads that are to simulate customers. 
  The barber, the class, has a mainly executive function to sleep, working, and show the status at every stage, and the status includes who are getting cutting, who are in the waiting room and the people who leave.
  The customer class also has a mainly excutive fucntion called arriving. When one thread of the customer is created, the thread checks whether the barber is asleep or not and the waiting seats to determine what to do. If the barber is asleep, wake it up through the function provided as public method of the barber class.
2. classes<br/>
  There are two classes, barber and customer.
## Test

P.S. I used a variable, "logging", to decide whether print out the logging information. It's false, by default.

_1.Test(work 1-5s randomly):_<br/>
Actually, if it is easy to see cus leaves, the code is totally wrong! The expectation of working time is 3. other word, the speed of generating cus is almost equal to the speed of working. To make some cus leave, I changed the code and made it wrong. the result of 1-5 working time. but now it is right.
```
Barber sleeping
Waiting room:

Barber cutting the hair of customer 1
Waiting room:

Barber sleeping
Waiting room:

Barber cutting the hair of customer 2
Waiting room:

Barber sleeping
Waiting room:

Barber cutting the hair of customer 3
Waiting room:

Barber sleeping
Waiting room:

Barber cutting the hair of customer 4
Waiting room: 5

Barber cutting the hair of customer 5
Waiting room: 6 7

Barber cutting the hair of customer 6
Waiting room: 7

Barber cutting the hair of customer 7
Waiting room: 8 9

Barber cutting the hair of customer 8
Waiting room: 9

Barber cutting the hair of customer 9
Waiting room: 10 11

Barber cutting the hair of customer 10
Waiting room: 11 12

Barber cutting the hair of customer 11
Waiting room: 12 13 14

Barber cutting the hair of customer 12
Waiting room: 13 14 15

Barber cutting the hair of customer 13
Waiting room: 14 15 16
customer 17 leaving

Barber cutting the hair of customer 14
Waiting room: 15 16 18
```

It should be:
```
Barber sleeping
Waiting room:

Barber cutting the hair of customer 1
Waiting room:

Barber sleeping
Waiting room:

Barber cutting the hair of customer 2
Waiting room:

Barber cutting the hair of customer 3
Waiting room:

Barber sleeping
Waiting room:

Barber cutting the hair of customer 4
Waiting room: 5

Barber cutting the hair of customer 5
Waiting room: 6

Barber cutting the hair of customer 6
Waiting room: 7

Barber cutting the hair of customer 7
Waiting room:

Barber sleeping
Waiting room:

Barber cutting the hair of customer 8
Waiting room:

Barber sleeping
Waiting room:

Barber cutting the hair of customer 9
Waiting room: 10

Barber cutting the hair of customer 10
Waiting room: 11
```
As it shown, the result is the same as the prediction.

_2. Test (work 1s):_
```
Barber sleeping
Waiting room:

Barber cutting the hair of customer 1
Waiting room:

Barber sleeping
Waiting room:

Barber cutting the hair of customer 2
Waiting room:

Barber sleeping
Waiting room:
```

_3. Testing result(work 10s):_
```
Barber sleeping
Waiting room:

Barber cutting the hair of customer 1
Waiting room: 2 3 4

Barber cutting the hair of customer 2
Waiting room: 3 4 5
customer 6 7 leaving

Barber cutting the hair of customer 3
Waiting room: 4 5 8
customer 9 10 leaving

Barber cutting the hair of customer 4
Waiting room: 5 8 11
customer 12 13 14 leaving
```
