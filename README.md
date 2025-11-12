

---

 Q1: Process & File Handling

What it does:
- Creates a child process to run a command (e.g., ls)
- Parent writes to and reads from a temporary file
- Uses fork(), execvp(), wait(), and file I/O
- System calls are traced using strace

How to run:
./Q1 ls

---

 Q2: Buffered vs Unbuffered Logging

What it does:
- Writes 1,000,000 log entries to a file
- Buffered: uses fprintf()
- Unbuffered: uses write()
- strace and time used to compare system call count and execution time

How to run with strace:
strace -c ./unbuffered  
strace -c ./buffered

Results:
- Buffered: ~7,337 system calls, ~0.56 sec
- Unbuffered: ~1,000,035 system calls, ~110 sec
- Buffered logging is faster and more efficient

---

 Q3: Palindrome Counter with Threads

What it does:
- Uses 20 threads to count palindromic numbers between 0–100,000
- Threads update a shared global count using a mutex

How to run:
./Q3

---

 Q4: Multithreaded Log File Merger

What it does:
- Merges multiple log files into one output file using threads
- Each thread reads one input file and writes safely using lseek()
- Thread-safe with pthread_mutex

How to run:
./q4 merged_log.txt file1.txt file2.txt 2

---
