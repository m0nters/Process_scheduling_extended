#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

enum ComparisonMode { TIME_COMPARISON, ORDER_COMPARISON, PRIORITY_COMPARISON }; // in case there's other priority rules in the future, add in here

struct Process
{
	int id = 0;
	int arrival_time = 0;
	queue<int> CPU_burst_time;
	queue<int> resource_usage_time;
	int turn_around_time = 0;
	int waiting_time = 0;

	struct {
		int last_time_push_in_CPU_queue = 0; // first in first out (FIFO)
		int last_time_get_out_CPU = 0; // in case both got pushed into CPU_queue at the same time, the one that got out CPU before first will have higher priority
		int priority_level = 0; // for priroity scheduling algorithm
		
		// Additional priority-related members can be added here in the future
	} priority_attributes;
};

struct Comparator {
	ComparisonMode mode;

	Comparator(ComparisonMode mode) : mode(mode) {}

	bool operator()(Process* a, Process* b) {
		switch (mode) {
			case TIME_COMPARISON:
				if (a->CPU_burst_time.front() != b->CPU_burst_time.front())
					return a->CPU_burst_time.front() > b->CPU_burst_time.front(); // FIRST PRIORITY: one has smaller CPU burst time
				break;
			case PRIORITY_COMPARISON:
				if (a->priority_attributes.priority_level != b->priority_attributes.priority_level)
					return a->priority_attributes.priority_level > b->priority_attributes.priority_level; // FIRST PRIORITY: the one with lower 'priority_level' will have higher priority
				break;
			default:
				break;
		}

		// these rules shall be the same for all algorithms
		if (a->priority_attributes.last_time_push_in_CPU_queue != b->priority_attributes.last_time_push_in_CPU_queue)
			return a->priority_attributes.last_time_push_in_CPU_queue > b->priority_attributes.last_time_push_in_CPU_queue; // FIFO rule (at this point, if there're no above conflicts exist, this is just like normal queue)

		return a->priority_attributes.last_time_get_out_CPU > b->priority_attributes.last_time_get_out_CPU; // if both got pushed into CPU_queue at the same time, the one that got out CPU first before will have higher priority
	}
};

void readFile(istream& is, int& algorithm_type, int& num_of_process, vector <Process>& processes, int& quantum, int& upgrade_time);
void writeFile(ostream& os, vector<Process>& processes, vector<string>& CPU_chart, vector<string>& R_chart);

void FCFS(vector<Process>& processes, ostream& os);
void RR(vector<Process>& processes, const int& quantum, ostream& os);
void SJF(vector<Process>& processes, ostream& os);
void SRTN(vector<Process>& processes, ostream& os);
void Priority_nonpreemtive(vector<Process>& processes, const int& upgrade_time, ostream& os);
void Priority_preemtive(vector<Process>& processes, const int& upgrade_time, ostream& os);