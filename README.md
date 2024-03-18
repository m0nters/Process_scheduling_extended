# Process-scheduling-extended
Inspired by my school project, I aim to expand this to include other scheduling algorithms, building upon the existing code inherited from my school project.

This work may be updated periodically.
## Work
Almost everything is the same like instructions shown in the related project [here][ref1],
but here are some new algorithm implementations:
- Priroity Scheduling (non-preemtive and preemtive version) (the smaller the `priority_level`, the higher the priority of the process) with starvation prevent method (reduce `priroity_level` over time if the process is still in `CPU_queue`)

	- The first line: an integer representing the scheduling algorithm — Their algorithm numbers are 5 and 6 respectively.
	- The second line is a positive integer representing the waiting time for processes in the ready queue that have not yet been scheduled to increase their `priority_level`.
	- The next line: a positive integer N representing the number of processes to be scheduled, up to a maximum of 4 processes. 
	- The following N lines: each line describes the scheduling information of a process with the syntax:
  
        `<priroity_level>` `<Arrival time in Ready Queue>` [`<CPU Burst Time>` `<Resource Usage Time>`]
   
        (the smaller the `priority_level`, the higher the priority of the process)
      (the smaller the `priority_level`, the higher the priority of the process)
  	-  (From here onwards, it follows the same format and condition as the [previous project][ref1] for both input and output)...
	
	 	For example:

  	   -  Input:
          ```
          6
          10
          3
          3 0 24
          2 1 5
          1 2 3 
          ```
          
          means
          | #   | Arrival Time | CPU Burst | Priority |
          | --- | ------------ | --------- | -------- |
          | 1   | 0            | 24        | 3        |
          | 2   | 1            | 5         | 2        |
          | 3   | 2            | 3         | 1        |
          
          (Remember: the smaller  the `priority_level`, the higher the priority of the process, in this example P1 < P2 < P3)

       -  Output:
            ```
            1 2 3 3 3 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 

            32 8 3 
            8 3 0 
            ```

          since:

          | second | 0 - 1 | 1 - 2 | 2 - 3 | 3 - 4 | 4 - 5 | 5 - 6 | 6 - 7 | 7 - 8 | 8 - 9 | 9 - 10 | 10 - 11 | 11 - 12 | 12 - 13 | 13 - 14 | 14 - 15 | 15 - 16 | 16 - 17 | 17 - 18 | 18 - 19 | 19 - 20 | 20 - 21 | 21 - 22 | 22 - 23 | 23 - 24 | 24 - 25 | 25 - 26 | 26 - 27 | 27 - 28 | 28 - 29 | 29 - 30 | 30 - 31 | 31 - 32 |
          | ------ | ------ | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- | ------ | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- |
          | CPU    | P1     | P2    | P3    | P3    | P3    | P2    | P2    | P2    | P2    | P1     | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      | P1      |

          |    | Turn-around Time | Waiting Time |
          | -- | ---------------- | ------------ |
          | P1 | 32               | 8            |
          | P2 | 8                | 3            |
          | P3 | 3                | 0            |


[ref1]: https://github.com/m0nters/-OS-Process-scheduling-HCMUS 
