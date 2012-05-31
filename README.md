SOFTENG 751 High Performance Computing Project

ImageFilter - A benchmarking tool for GCD and other threading approaches
=======

###[Github page](https://github.com/xlc/SE751Project)

------------

Author:
-------
 * Xiliang Chen - xche185
 * Alice Wang - ywan478

------------

Introduction
-------

Grand Central Dispatch (GCD) is a runtime library created by Apple that supports multithreaded programming. It works by managing threads at the operating system level rather than the user application level. Code to be run in parallel are defined in “blocks”, and placed on different queues that are provided. Tasks in the queue are then scheduled for execution by the operating system, thus abstracting away code required to multithread the application.Our project is to investigate GCD in terms of performance and programmer ease of use and compare results with other traditional multithreading approaches. We created an image processing application using the following multithreading approaches:

 * GCD
 * Single thread
 * Thread pool
 * Thread per task

The application is shown below:

![ImageFilter interface](https://github.com/xlc/SE751Project/raw/master/img/gui.png)

Breakdown of how to use the application GUI:
 * Settings:
	* Task Queue – the selected multithreading approach	* Granularity – number of pixels to be processed in a task
 * Filters: 	* Clicking a filter will apply the filter on the image	* Fine grain – Grey scale, Colourful, Invert, Sepia	* Course grain – Edge, Blur	* GPU Filter uses the GPU to process the image.----------

Results and Discussion
---------###Performance
The application was run on an iMac quad core 2.7GHz desktop.Firstly we ran the application with different task granularities on a large image, 2560 x 1600 pixels, using the blur filter (course-grained) and graphed the results below.From this we can see that GCD and Thread Pool approaches performed better than both sequential and single thread worker approaches when the number of tasks was smaller than 128,000. This is because as the number of tasks remains below 128,000 the tasks are computationally intensive enough to benefit from parallelism. The increases in time of the single thread approach shows the increase in computation overhead. As the number of tasks increase, the overhead of creating tasks also increase and eventually, the overhead is greater than the benefits of parallelizing the computation and sequential is faster. Note: as total size of the image is the same, as the number of task increases, computation of task decreases.![Performance between different task queues](https://github.com/xlc/SE751Project/raw/master/img/performance.png)

GCD and Thread Pool implementation results were very similar. This is because GCD is based on a thread pool implementation under the hood. We decided to investigate into this further by changing the number of threads created in the thread pool and by changing the nature of the tasks (computationally intensive vs interactive tasks).


###Ease of use

We measured the ease of use by the number of lines of code, excluding comments, which were required to code up each of the multithreading approaches. This is shown in the graph below:

![Lines of code to implement each task queues](https://github.com/xlc/SE751Project/raw/master/img/ease.png)


-----------

Conclusions
---------GCD is yet another way of developing multithreaded applications that runs on the Mac operating system. It works by dynamically balancing the workload by creating the appropriate amount of threads required. The performance is generally no worse than traditional multithreaded approaches when dealing with varying number of tasks, granularity of tasks, nature of tasks and performs only slightly worse than the best optimized multithreaded approach for each scenario. Despite the performance power and functionality of GCD, it is very easy to use and require the least amount of code to use. In terms of amount of effort vs performance, GCD is definitely the better approach for developers coding applications for the Mac.