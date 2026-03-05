# Operating-Systems

A collection of laboratory assignments for the Operating Systems course. This repository contains my coursework projects created to deepen my understanding of process management, multi-threading, and inter-process communication in both Linux and Windows environments.

## 📁 Completed Labs

* **Lab 08: Processes in Linux**
  [cite_start]Recursive calculation of the Fibonacci sequence using a process tree[cite: 122, 128].
* **Lab 09: Processes in Windows**
  [cite_start]Fibonacci sequence calculation using the Windows API to spawn child processes[cite: 194, 201].
* **Lab 10: Threads in Linux**
  [cite_start]Multi-threaded calculation of standard deviation and variance from a collection of random floats[cite: 8, 25].
* **Lab 12: Shared Memory in Linux**
  [cite_start]Transferring file contents between two separate programs (`sender` and `receiver`) using inter-process communication[cite: 58, 63].

## 🛠️ Technologies

* C
* Bash
* Linux API (`fork`, `wait`, `exec` family)
* Windows API (`CreateProcessA`, `WaitForSingleObject`)
* POSIX Threads (`pthreads`, `pthread_mutex`)
* Shared Memory API (`shmget`, `shmat`, `shmdt`)

## 💡 What I learned

* [cite_start]Creating and managing process trees in Linux and Windows operating systems[cite: 113, 185].
* [cite_start]Implementing multi-threading to parallelize computations and dividing work among a thread pool[cite: 2, 18].
* [cite_start]Managing critical sections using mutexes to prevent race conditions during global variable updates[cite: 4, 21].
* [cite_start]Establishing inter-process communication (IPC) and mutual synchronization using shared memory and spinlocks[cite: 53, 66].
* [cite_start]Measuring execution time using `clock_gettime()` and comparing the performance of single-threaded versus multi-threaded execution[cite: 5, 25, 26].
