#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "Header.h"

int main(int argc, char* argv[]) {
	/*if (argc != 3) {
		cout << "Usage: " << argv[0] << " <INPUT_FILE> <OUTPUT_FILE>" << endl;
		return 1;
	}
	ifstream input_file(argv[1]);
	ofstream output_file(argv[2]);*/

	ifstream input_file("input.txt");
	ofstream output_file("output.txt");

	if (!input_file.is_open() || !output_file.is_open()) {
		cout << "Error opening files." << endl;
		return 1;
	}

	int algorithm_type = 0, num_of_process = 0, quantum = 0, upgrade_time = 0;
	vector<Process> processes;

	// Read input file
	readFile(input_file, algorithm_type, num_of_process, processes, quantum, upgrade_time);

	auto start = chrono::high_resolution_clock::now();
	// Perform scheduling based on algorithm type
	switch (algorithm_type) {
	case FIRST_COME_FIRST_SERVED:
		FCFS(processes, output_file);
		break;
	case ROUND_ROBIN:
		RR(processes, quantum, output_file);
		break;
	case SHORTEST_JOB_FIRST:
		SJF(processes, output_file);
		break;
	case SHORTEST_REMAINING_TIME_NEXT:
		SRTN(processes, output_file);
		break;
	case PRIROITY_NONPREEMPTIVE:
		Priority_NP(processes, upgrade_time, output_file);
		break;
	case PRIORITY_PREEMPTIVE:
		Priority_P(processes, upgrade_time, output_file);
		break;
	default:
		cout << "Invalid algorithm type." << endl;
		break;
	}
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> duration = end - start;
	cout << "Execution time: " << duration.count() << " seconds" << endl;

	input_file.close();
	output_file.close();
}