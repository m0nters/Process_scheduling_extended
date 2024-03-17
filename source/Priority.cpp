#include "Header.h"

// these 2 are implemented starvation prevention methods

void Priority_nonpreemtive(vector<Process>& processes, const int& upgrade_time, ostream& os) {
	int time = 0;

	// gantt chart for the result
	vector<string> CPU_chart;
	vector<string> R_chart;

	Comparator priroityComparator(PRIORITY_COMPARISON);
	priority_queue<Process*, vector<Process*>, Comparator> CPU_queue(priroityComparator);
	queue<Process*> R_queue;

	Process* current_CPU_used_process = NULL;
	Process* current_R_used_process = NULL;

	int finished_processes_num = 0; // the number of the processes that have finished
	unordered_map<Process*, int> upgrade_priroty_time_map; // store the time when the prirority of the process staying in the ready queue will be upgraded if it doesn't get out

	int processes_added_to_CPU_queue_count = 0; // optimization: avoid abundant check to add into ready queue, reduce O(n)
	while (finished_processes_num != processes.size()) {
		// maximum time complexity: O(n)
		for (auto& process : upgrade_priroty_time_map) // checking whether there is any process staying in the ready queue for too long that needs to be upgraded priroty RIGHT NOW
			if (process.second == time)
				++process.first->priority_attributes.priority_level;

		for (int i = processes_added_to_CPU_queue_count; i < processes.size(); i++) {
			if (processes[i].arrival_time == time) {
				processes[i].priority_attributes.last_time_push_in_CPU_queue = time;
				upgrade_priroty_time_map[&processes[i]] = time + upgrade_time;
				CPU_queue.push(&processes[i]);
				++processes_added_to_CPU_queue_count;
			}
			else
				break;
		}

		// take the next process in ready queue -> CPU                  
		if (!current_CPU_used_process && !CPU_queue.empty()) { // if there isn't any process running in the CPU right now
			current_CPU_used_process = CPU_queue.top();
			upgrade_priroty_time_map.erase(current_CPU_used_process); // when the process is taken out of the ready queue, it's no longer needed to be upgraded
			CPU_queue.pop();
			current_CPU_used_process->waiting_time += time - current_CPU_used_process->priority_attributes.last_time_push_in_CPU_queue;
		}

		// take the next process in R_queue to work with
		if (!current_R_used_process && !R_queue.empty()) { // if there isn't any process running in the R right now
			current_R_used_process = R_queue.front(); // pick the next one waiting in the queue (if the queue is not empty)
			R_queue.pop();
		}

		// draw the CPU gantt chart
		if (current_CPU_used_process) { // if there's a process running in CPU
			int index = current_CPU_used_process->id - 1;
			CPU_chart.push_back(to_string(current_CPU_used_process->id));

			if (!--current_CPU_used_process->CPU_burst_time.front()) {
				current_CPU_used_process->CPU_burst_time.pop();
				current_CPU_used_process->priority_attributes.last_time_get_out_CPU = time + 1;
				if (!current_CPU_used_process->resource_usage_time.empty()) // check if there's next R, if have, put into R_ready_queue
					R_queue.push(current_CPU_used_process);
				else { // otherwise, the process is done! now we can calculate the turn around time
					++finished_processes_num;
					current_CPU_used_process->turn_around_time = time - current_CPU_used_process->arrival_time + 1;
				}

				current_CPU_used_process = NULL;
			}
		}
		else
			CPU_chart.push_back("_");

		// draw the R gantt chart
		// FCFS ALWAYS!
		if (current_R_used_process) { // if there's a process running in R
			R_chart.push_back(to_string(current_R_used_process->id));

			if (!--current_R_used_process->resource_usage_time.front()) {
				current_R_used_process->resource_usage_time.pop();
				current_R_used_process->priority_attributes.last_time_push_in_CPU_queue = time + 1; // for example time is 7 but IN FACT the time the process get out of the R_queue is 8! since we are considering time as BLOCKS!
				if (!current_R_used_process->CPU_burst_time.empty()) {
					upgrade_priroty_time_map[current_R_used_process] = time + 1 + upgrade_time;
					CPU_queue.push(current_R_used_process);
				}
				else { // otherwise, the process is done! now we can calculate the turn around time
					++finished_processes_num;
					current_R_used_process->turn_around_time = time - current_R_used_process->arrival_time + 1;
				}

				current_R_used_process = NULL;
			}
		}
		else
			R_chart.push_back("_");
		// ==========================================================
		++time;
	}
	writeFile(os, processes, CPU_chart, R_chart);
}

void Priority_preemtive(vector<Process>& processes, const int& upgrade_time, ostream& os) {
	int time = 0;

	// gantt chart for the result
	vector<string> CPU_chart;
	vector<string> R_chart;

	Comparator priroityComparator(PRIORITY_COMPARISON);
	priority_queue<Process*, vector<Process*>, Comparator> CPU_queue(priroityComparator);
	queue<Process*> R_queue;

	Process* current_CPU_used_process = NULL;
	Process* current_R_used_process = NULL;

	int finished_processes_num = 0; // the number of the processes that have finished
	unordered_map<Process*, int> upgrade_priroty_time_map;

	int processes_added_to_CPU_queue_count = 0; // optimization: avoid abundant check to add into ready queue, reduce O(n)
	while (finished_processes_num != processes.size()) {
		for (auto& process : upgrade_priroty_time_map) // checking whether there is any process staying in the ready queue for too long that needs to be upgraded priroty RIGHT NOW
			if (process.second == time)
				++process.first->priority_attributes.priority_level;

		for (int i = processes_added_to_CPU_queue_count; i < processes.size(); i++) {
			if (processes[i].arrival_time == time) {
				processes[i].priority_attributes.last_time_push_in_CPU_queue = time;
				upgrade_priroty_time_map[&processes[i]] = time + upgrade_time;
				CPU_queue.push(&processes[i]);
				++processes_added_to_CPU_queue_count;
			}
			else
				break;
		}

		// if there's a (newer) process with smaller or equal `priority_level` than the current process at the moment
		if (current_CPU_used_process && !CPU_queue.empty() && CPU_queue.top()->priority_attributes.priority_level <= current_CPU_used_process->priority_attributes.priority_level) {

			// IMMEDIATELY terminate the current process by pushing it into queue
			current_CPU_used_process->priority_attributes.last_time_get_out_CPU = current_CPU_used_process->priority_attributes.last_time_push_in_CPU_queue = time;
			upgrade_priroty_time_map[current_CPU_used_process] = time + upgrade_time;
			CPU_queue.push(current_CPU_used_process);
			current_CPU_used_process = NULL;

		}

		// take the next process in ready queue -> CPU 
		if (!current_CPU_used_process && !CPU_queue.empty()) {
			current_CPU_used_process = CPU_queue.top();
			upgrade_priroty_time_map.erase(current_CPU_used_process);
			CPU_queue.pop();
			current_CPU_used_process->waiting_time += time - current_CPU_used_process->priority_attributes.last_time_push_in_CPU_queue;
		}

		// take the next process in R_queue to work with
		if (!current_R_used_process && !R_queue.empty()) { // if there isn't any process running in the R right now
			current_R_used_process = R_queue.front(); // pick the next one waiting in the queue (if the queue is not empty)
			R_queue.pop();
		}

		// draw the CPU gantt chart
		if (current_CPU_used_process) { // if there's a process running in CPU
			int index = current_CPU_used_process->id - 1;
			CPU_chart.push_back(to_string(current_CPU_used_process->id));

			if (!--current_CPU_used_process->CPU_burst_time.front()) {
				current_CPU_used_process->CPU_burst_time.pop();
				current_CPU_used_process->priority_attributes.last_time_get_out_CPU = time + 1;
				if (!current_CPU_used_process->resource_usage_time.empty()) // check if there's next R, if have, put into R_ready_queue
					R_queue.push(current_CPU_used_process);
				else { // otherwise, the process is done! now we can calculate the turn around time
					++finished_processes_num;
					current_CPU_used_process->turn_around_time = time - current_CPU_used_process->arrival_time + 1;
				}

				current_CPU_used_process = NULL;
			}
		}
		else
			CPU_chart.push_back("_");

		// draw the R gantt chart
		// FCFS ALWAYS!
		if (current_R_used_process) { // if there's a process running in R
			R_chart.push_back(to_string(current_R_used_process->id));

			if (!--current_R_used_process->resource_usage_time.front()) {
				current_R_used_process->resource_usage_time.pop();
				current_R_used_process->priority_attributes.last_time_push_in_CPU_queue = time + 1; // for example time is 7 but IN FACT the time the process get out of the R_queue is 8! since we are considering time as BLOCKS!
				if (!current_R_used_process->CPU_burst_time.empty()) {
					upgrade_priroty_time_map[current_R_used_process] = time + 1 + upgrade_time;
					CPU_queue.push(current_R_used_process);
				}
				else { // otherwise, the process is done! now we can calculate the turn around time
					++finished_processes_num;
					current_R_used_process->turn_around_time = time - current_R_used_process->arrival_time + 1;
				}

				current_R_used_process = NULL;
			}
		}
		else
			R_chart.push_back("_");
		// ==========================================================
		++time;
	}
	writeFile(os, processes, CPU_chart, R_chart);
}
