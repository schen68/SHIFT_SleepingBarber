# SHIFT_SleepingBarber# shift-sleeping-barber-problem
# Problem discription
This problem is about simulating a barber shop with the following characteristics. The barber shop has a waiting room with M seats and a separate room with a single barber chair where the barber works. If there are no waiting customers, nor a customer having a haircut, the barber takes a break and sleeps. The moment a customer arrives, s/he wakes the barber up so that they can have their haircut. If another customer arrives while the barber is working, s/he will either sit in the waiting room (if there are available seats) or leave (if no seats are available).
# Implement detial
1. route map
  create two threads. The first thread is to simulate the barber, which is rely on barber class; the other thread is for infinitely creating threads that are to simulate customers. 
  The barber, the class, has a mainly executive function to sleep, working, and show the status at every stage, and the status includes who are getting cutting, who are in the waiting room and the people who leave.
  The customer class also has a mainly excutive fucntion called arriving. When one thread of the customer is created, the thread checks whether the barber is asleep or not and the waiting seats to determine what to do. If the barber is asleep, wake it up through the function provided as public method of the barber class.
2. classes
  There are two classes, barber and customer.
# Test
  Create one customer thread every 3 second, and the cutting time is ranged on 1 to 5 seconds randomly. The speed of the two threads is the same, since the expection of the occurrence of all numbers is 3 second. (1*P1 +...+ 5*P5)/5 = 3 p1=p2=...=p5
Hence, if the number of seats is appropriate, most customers could get haircut.
  make the working time 10 seconds and the seat number 3. We can predict that, in the  first stage, 4 people would be shown after starting with sleeping barber. customer 1 is getting haircut; customer 2, 3, 4 are in the waiting room and no one leaves.
  
Testing result:
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

Barber cutting the hair of customer 5
Waiting room: 8 11 15 
customer 16 17 leaving

As it shown, the result is the same as the prediction.

