#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "Header.h"

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <INPUT_FILE> <OUTPUT_FILE>" << endl;
		return 1;
	}

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
	case 1:
		FCFS(processes, output_file);
		break;
	case 2:
		RR(processes, quantum, output_file);
		break;
	case 3:
		SJF(processes, output_file);
		break;
	case 4:
		SRTN(processes, output_file);
		break;
	case 5:
		Priority_nonpreemtive(processes, upgrade_time, output_file);
		break;
	case 6:
		Priority_preemtive(processes, upgrade_time, output_file);
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