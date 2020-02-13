RUN #1:

----------------------------------------------STATS-----------------------------------------
Stats for 1 Queue: 
Average size of 1- 0.000000
Max size of 1- 3.000000
Utilization of 1- 0.000000
Average response time of 1- 12.000000
Max response time of 1- 30.000000
Throughput of 1- 0.000000
Stats for 2 Queue: 
Average size of 2- 0.000000
Max size of 2- 0.000000
Utilization of 2- 0.000000
Average response time of 2- 0.000000
Max response time of 2- 0.000000
Throughput of 2- 0.000000
Stats for 3 Queue: 
Average size of 3- 0.000000
Max size of 3- 0.000000
Utilization of 3- 0.000000
Average response time of 3- 32.000000
Max response time of 3- 70.000000
Throughput of 3- 0.000000
Stats for 4 Queue: 
Average size of 4- 0.000000
Max size of 4- 0.000000
Utilization of 4- 0.000000
Average response time of 4- 7.000000
Max response time of 4- 20.000000
Throughput of 4- 0.000000

---------------------------------------SCREENSHOT OF LOG------------------------------------
![run1]("RUN1.png)
---------------------------------------CONFIG----------------------------------------------
#define SEED 2
#define INIT_TIME 0 
#define FIN_TIME 20000
#define ARRIVE_MIN 20
#define ARRIVE_MAX 50
#define QUIT_PROB .10
#define NETWORK_PROB .12
#define CPU_MIN 20
#define CPU_MAX 30
#define DISK1_MIN 50
#define DISK1_MAX 67
#define DISK2_MIN 60
#define DISK2_MAX 75
#define NETWORK_MIN 10
#define NETWORK_MAX 20
----------------------------------------TAKEAWAYS-------------------------------------------
In cases with high event arrival rates, if the CPU matches that, it flows just as it would with the opposite (both being low times). As seen by my statists (the ones that ran properly), The max size of the CPU at a time was 3, and it remained low at other times.










RUN #2:

----------------------------------------------STATS-----------------------------------------
Stats for 1 Queue: 
Average size of 1- 436.000000
Max size of 1- 887.000000
Utilization of 1- 0.000000
Average response time of 1- 10.000000
Max response time of 1- 30.000000
Throughput of 1- 0.000000
Stats for 2 Queue: 
Average size of 2- 0.000000
Max size of 2- 0.000000
Utilization of 2- 0.000000
Average response time of 2- 0.000000
Max response time of 2- 0.000000
Throughput of 2- 0.000000
Stats for 3 Queue: 
Average size of 3- 0.000000
Max size of 3- 0.000000
Utilization of 3- 0.000000
Average response time of 3- 11.000000
Max response time of 3- 43.000000
Throughput of 3- 0.000000
Stats for 4 Queue: 
Average size of 4- 0.000000
Max size of 4- 0.000000
Utilization of 4- 0.000000
Average response time of 4- 6.000000
Max response time of 4- 15.000000
Throughput of 4- 0.000000


---------------------------------------SCREENSHOT OF LOG------------------------------------
![run2]("RUN2.png)
----------------------------------------CONFIG----------------------------------------------
#define SEED 2
#define INIT_TIME 0 
#define FIN_TIME 10000
#define ARRIVE_MIN 2
#define ARRIVE_MAX 15
#define QUIT_PROB .4
#define NETWORK_PROB .5
#define CPU_MIN 13
#define CPU_MAX 30
#define DISK1_MIN 24
#define DISK1_MAX 45
#define DISK2_MIN 12
#define DISK2_MAX 43
#define NETWORK_MIN 11
#define NETWORK_MAX 15
----------------------------------------TAKEAWAYS-------------------------------------------
When you have a small arrive time and a much higher CPU time, it is easy for the CPU queue to build up and have long waiting rates. 
















RUN #3:

----------------------------------------------STATS-----------------------------------------
Stats for 1 Queue: 
Average size of 1- 0.000000
Max size of 1- 5.000000
Utilization of 1- 0.000000
Average response time of 1- 26.000000
Max response time of 1- 55.000000
Throughput of 1- 0.000000
Stats for 2 Queue: 
Average size of 2- 0.000000
Max size of 2- 0.000000
Utilization of 2- 0.000000
Average response time of 2- 0.000000
Max response time of 2- 0.000000
Throughput of 2- 0.000000
Stats for 3 Queue: 
Average size of 3- 0.000000
Max size of 3- 0.000000
Utilization of 3- 0.000000
Average response time of 3- 14.000000
Max response time of 3- 42.000000
Throughput of 3- 0.000000
Stats for 4 Queue: 
Average size of 4- 0.000000
Max size of 4- 0.000000
Utilization of 4- 0.000000
Average response time of 4- 20.000000
Max response time of 4- 50.000000
Throughput of 4- 0.000000


---------------------------------------SCREENSHOT OF LOG------------------------------------
![run3]("RUN3.png)
---------------------------------------CONFIG----------------------------------------------
#define SEED 2
#define INIT_TIME 0 
#define FIN_TIME 50000
#define ARRIVE_MIN 60
#define ARRIVE_MAX 65
#define QUIT_PROB .1
#define NETWORK_PROB .1
#define CPU_MIN 50
#define CPU_MAX 55
#define DISK1_MIN 31
#define DISK1_MAX 41
#define DISK2_MIN 10
#define DISK2_MAX 43
#define NETWORK_MIN 30
#define NETWORK_MAX 50
----------------------------------------TAKEAWAYS-------------------------------------------
By this, we see that any event with a high time of arrival and bounds for the random generator will have a much longer time executing. 