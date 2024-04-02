# Banker's Algorithm Implementation

## Overview

This repository contains an implementation of the Banker's algorithm, a resource allocation and deadlock avoidance algorithm used in operating systems. The algorithm is designed to prevent deadlocks by determining whether a particular resource allocation request from processes can be granted safely without causing deadlock.

## How It Works

1. **Reading Input**: The code reads the contents of the request, resource, and allocations files. The data from these files is stored in arrays as input parameters. Multidimensional arrays are used for requests and allocations.

2. **Resource Availability**: An available array is created to determine the available resources. This array is obtained by subtracting the allocated resources from the resource capacity.

3. **Checking Availability**: A flag variable is used for checking if there are enough available resources for the given process’ requests. The flag variable increments by one when the requested resource type is available or there is 0 request for that resource type. When the flag is 5, it means the process can work. A process array holds the order of the processes which can run.

4. **Output**: The allocation and request information of processes are printed. Additionally, the running order process array is printed. When a deadlock occurs, the process array will contain one or more instances with a value of 0. The processes causing the deadlock are identified by examining the finished array. When the finished array has a value of zero for a specific index, it indicates that the process denoted by P(index) did not run.

## Files

- `os.c`: Contains the implementation of the Banker's algorithm.
- `request.txt`: File containing resource requests from processes.
- `resource.txt`: File containing information about available resources.
- `allocations.txt`: File containing information about already allocated resources.
- `output.txt`: Output file containing the results of the algorithm execution.

## Usage

1. Clone this repository to your local machine.
2. Ensure you have a C compiler installed.
3. Compile the `os.c` file using your compiler.
4. Run the compiled executable.
5. Review the output in the `output.txt` file for the results of the algorithm execution.

## Contributors

- Doğa Güneş Karadağ

