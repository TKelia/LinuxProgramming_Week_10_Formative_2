


 Q1: Process & File Handling

This program demonstrates process creation and inter-process communication using a child and parent process. The child executes a command (e.g., ls) while the parent process writes to and reads from a temporary file.

Implementation Highlights:
Utilizes fork(), execvp(), and wait() system calls.
Performs file creation, writing, and reading operations.
System calls are traced with strace, and the results are analyzed to show understanding of each call's role.

How to run:
./Q1 ls

Outcome:
Fully functional program correctly demonstrates process creation and execution.
File operations complete successfully.
Strace output is filtered and analyzed, showing comprehension of system call usage.


 Q2: Buffered vs Unbuffered Logging

This exercise compares the performance of buffered versus unbuffered logging when writing a large number of entries to a file.

Implementation Highlights:
Both versions write 1,000,000 timestamped log entries.
Buffered version uses fprintf(), unbuffered version uses write().
Strace and time are used to count system calls and measure execution time.

How to run with strace:
strace -c ./unbuffered  
strace -c ./buffered

Results:
- Both implementations work correctly and efficiently.
- Strace accurately measures system calls and execution times.
- Report provides a detailed performance analysis showing buffered logging is significantly faster and more efficient.


 Q3: Palindrome Counter with Threads

What it does:
- Uses 20 threads to count palindromic numbers between 0–100,000
- Threads update a shared global count using a mutex

How to run:
./Q3

Outcomes:
- Program runs correctly with 20 threads.
- Palindrome detection is accurate.
- Global count synchronization is implemented properly.

 Q4: Multithreaded Log File Merger

Merges multiple log files into a single output file using threads.

Implementation Highlights:
Each thread reads one input file and writes safely to a shared output file using lseek() and fwrite().
Synchronization mechanisms (mutexes) are applied where necessary.
Performance is evaluated using 2, average, and maximum number of threads.

How to run:
./q4 merged_log.txt file1.txt file2.txt 2

Outcome:
Fully functional multithreaded file merger.
Each thread handles its file safely and synchronizes access correctly.
Output and execution format meet the assignment requirements.

