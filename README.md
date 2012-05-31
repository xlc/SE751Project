SOFTENG 751 High Performance Computing Project

ImageFilter - A benchmarking tool for GCD and other threading approaches
=======

------------

Author:
-------
 * Xiliang Chen - xche185
 * Alice Wang - ywan478

------------

Introduction
-------

Grand Central Dispatch (GCD) is a runtime library created by Apple that supports multithreaded programming. It works by managing threads at the operating system level rather than the user application level. Code to be run in parallel are defined in “blocks”, and placed on different queues that are provided. Tasks in the queue are then scheduled for execution by the operating system, thus abstracting away code required to multithread the application.

 * GCD
 * Single thread
 * Thread pool
 * Thread per task

The application is shown below:

![ImageFilter interface](https://github.com/xlc/SE751Project/raw/master/img/gui.png)

Breakdown of how to use the application GUI:

	* Task Queue – the selected multithreading approach


Results and Discussion
---------


GCD and Thread Pool implementation results were very similar. This is because GCD is based on a thread pool implementation under the hood. We decided to investigate into this further by changing the number of threads created in the thread pool and by changing the nature of the tasks (computationally intensive vs interactive tasks).


###Ease of use

We measured the ease of use by the number of lines of code, excluding comments, which were required to code up each of the multithreading approaches. This is shown in the graph below:

![Lines of code to implement each task queues](https://github.com/xlc/SE751Project/raw/master/img/ease.png)


-----------

Conclusions
---------