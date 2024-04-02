# Online Shopping Routine with Multiprocessing and Multithreading

## Overview

This project aims to design an online shopping routine utilizing multiprocessing and multithreading concepts in operating systems. In an online shopping routine, multiple customers can place orders simultaneously, potentially leading to race conditions. Race conditions occur when multiple processes or threads attempt to access and modify shared resources concurrently, resulting in unexpected or inconsistent outputs. To prevent such issues, mutual exclusion methods such as mutexes are employed.

### What is a Race Condition?

A race condition occurs in a concurrent system when the outcome of operations depends on the timing of uncontrollable events. In the context of online shopping, race conditions can arise when multiple customers attempt to purchase the same product simultaneously. Without proper synchronization mechanisms, such as mutexes, race conditions can lead to inconsistencies in product quantities and other unexpected behavior.

### Why Use Mutexes?

Mutexes, or mutual exclusion methods, are synchronization primitives used to prevent multiple threads or processes from simultaneously accessing shared resources. By acquiring and releasing mutex locks, threads or processes can ensure exclusive access to critical sections of code, thereby preventing race conditions and maintaining data integrity.

## Implementation Details

Both multiprocessing and multithreading approaches are used to implement the online shopping routine.

### Multithreading Implementation

- Each customer is represented by a separate thread.
- Dynamic memory allocation is used to create customer instances.
- Mutexes are employed within the `order_products` function to prevent race conditions.
- Customers enter the `order_products` function, where they wait until the necessary conditions are met before proceeding to purchase the product.

### Multiprocessing Implementation

- Similar to the multithreading implementation, multiprocessing is used to handle customer requests concurrently.
- The `fork` system call is used to create child processes, each representing a customer.
- Shared memory mechanisms (`shmget` and `shmat`) are employed to facilitate communication between processes.

## Files

- `threads.c`: Implementation of the online shopping routine using multithreading.
- `process.c`: Implementation of the online shopping routine using multiprocessing.

## Usage

1. Compile the code using a C compiler.
2. Run the compiled executable.
3. Review the output to ensure proper synchronization and prevention of race conditions.

## Contributors

- Doğa Güneş Karadağ