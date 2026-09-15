*This project has been created as part of the 42 curriculum by ckruk*

# Codexion

## Description

**Codexion** is a problem solving program, which is about multiple threads (in this case coders) are competing for limited resources (in this case dongles) that are shared between other coders. The number of dongles is equal to number of coders, however to compile each coder needs 2 dongles (one on his right side and one on his left side). Each coders has to compile defined number of times, which is given by the user when running the program. During the coder life time he is compiling (this stage requires 2 dongles), debugging and refactoring. Time for each stage and time till burnout (within this time coder has to compile) is also defined by the user at the beginning. Program should never burnout if given times are reasonable.

## Instructions

### Compilation

```bash
make
```
### Execution

```bash
./codexion a b c d e f g scheduler
```
### Arguments

Letters in the command stands for:
- a - number of coders
- b - time to burnout (in milliseconds)
- c -  time to compile (in milliseconds)
- d - time to debug (in milliseconds)
- e - time to refactor (in milliseconds)
- f - number of compiles required
- g - dongle cooldown (in milliseconds)
scheduler - fifo (First In, First Out) / edf (Earliest Deadline First)

### Examples

```bash
./codexion 3 150 100 100 100 3 10 fifo
```
```bash
./codexion 2 100 100 100 100 3 10 edf
```

### Available Make Commands

```bash
make all           # Compiles all C source files into object files (.o) and links them to build the main codexion executable
make clean         # Removes the compiled object files (.o), leaving source files and the final executable intact
make fclean        # Performs a "full clean" by removing both the object files and the codexion binary
make re            # Performs a full rebuild from scratch by running fclean followed by all
```

## Blocking cases handled

### Threadsynchronization mechanisms

To prevent race condition, each shared resource is protected by general mutex. The general mutex is used when a thread checks or changes shared state. This prevents multiple threads from accessing and modifying the same state at the same time. A condition variable is also used to allow coders waiting for dongles to sleep until the state of the resources changes. When dongles are released, waiting coders are notified and can check again whether they are allowed to continue.

### Deadlock prevention and Coffman’s conditions

Deadlock is prevented by controlling access to both dongles while holding the general mutex. A coder does not take one dongle and then wait for the second one. Instead, the program checks whether both required dongles are available before assigning them to the coder. The waiting queue and scheduler determine which coder can acquire the dongles when they become available.

### Starvation prevention

Coders waiting for dongles are placed in a queue.
With the FIFO scheduler, the coder at the front of the queue has priority. With the EDF scheduler, the coder with the smallest remaining time before burnout has priority. This allows the program to make a scheduling decision between waiting coders instead of relying only on thread execution order.

### Cooldown handling

The cooldown time is provided as a program argument and is stored as part of the game configuration. Dongles are released after the compilation stage so that they can become available to other waiting coders.

### Burnout detection

Burnout is detected by the monitor function, which works as an additional thread during the lifetime of the program. The monitor checks the elapsed time since each coder's last compilation started and compares it with the configured burnout time. If the elapsed time exceeds the burnout limit, the monitor prints information about the coder and changes the shared burnout state. The condition variable is then broadcast so that waiting coders are notified that the program has ended. If all coders complete the required number of compilations before burnout, the program prints: "All coders finished compiling. No burnout"

### Log serialization

State changes are printed through the state_info() function.
The program prints messages when a coder:
- takes a dongle,
- starts compiling,
- starts debugging,
- starts refactoring,
- burns out.
Shared state is synchronized using the general mutex to prevent race conditions while the program is processing coder and dongle states.

### Scheduling
FIFO - The FIFO scheduler uses the waiting queue and gives priority to the coder that has been waiting first.

EDF - The EDF scheduler compares the remaining time before burnout for coders waiting in the queue. The remaining time is calculated using the coder's configured burnout time and the elapsed time since their last compilation started. The coder with the earliest deadline is selected first.

## Resources

### References

- **Understading the concept**: https://dev.to/yel-bakk/codexion-4fk8
- **Threads explanation**: https://dev.to/yel-bakk/thread-in-c-codexion-42-1ao
- **Multithreading explanation**: https://dev.to/yel-bakk/thread-in-c-codexion-42-1ao
- **Thread funtions**: https://www.geeksforgeeks.org/c/thread-functions-in-c-c/

### AI Usage

**AI was used for:**
- **Interpretation**: Help to interpretate instructions from the subject
- **Kick-off**: Help with understanding what is required in the project and what is it about to be able to start doing it
- **Explanation**: Guidance with new concepts without giving direct solutions
- **Testing**: Giving examples of cases to test the program

## Project Structure

```
├── README.md
├── assigner.c
├── codexion.c
├── codexion.h
├── life_cycle.c
├── makefile
├── queue.c
├── states.c
├── utils.c
└── utils2.c
```
