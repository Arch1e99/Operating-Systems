# Operating-Systems

A collection of laboratory assignments for the Operating Systems course. This repository contains my coursework projects created to deepen my understanding of process management, multi-threading, and inter-process communication in both Linux and Windows environments.

## 📁 Completed Labs

* **Lab 08: Processes in Linux**
Recursive calculation of the Fibonacci sequence using a process tree.
* **Lab 09: Processes in Windows**
Fibonacci sequence calculation using the Windows API to spawn child processes.
* **Lab 10: Threads in Linux**
Multi-threaded calculation of standard deviation and variance from a collection of random floats.
* **Lab 12: Shared Memory in Linux**
Transferring file contents between two separate programs (`sender` and `receiver`) using inter-process communication.

## 🛠️ Technologies

* C
* Bash
* Linux API (`fork`, `wait`, `exec` family)
* Windows API (`CreateProcessA`, `WaitForSingleObject`)
* POSIX Threads (`pthreads`, `pthread_mutex`)
* Shared Memory API (`shmget`, `shmat`, `shmdt`)

## 💡 What I learned

* Creating and managing process trees in Linux and Windows operating systems.
* Implementing multi-threading to parallelize computations and dividing work among a thread pool.
* Managing critical sections using mutexes to prevent race conditions during global variable updates.
* Establishing inter-process communication (IPC) and mutual synchronization using shared memory and spinlocks.
* Measuring execution time using `clock_gettime()` and comparing the performance of single-threaded versus multi-threaded execution.
