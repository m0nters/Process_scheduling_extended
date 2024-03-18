#include "Header.h"
#include <numeric>
#include <iomanip>

void readFile(istream& is, int& algorithm_type, int& num_of_process, vector <Process>& processes, int& quantum, int& upgrade_time) {
	is >> algorithm_type;

	switch (algorithm_type)
	{
	case ROUND_ROBIN:
		is >> quantum;
		break;
	case PRIROITY_NONPREEMPTIVE:
	case PRIORITY_PREEMPTIVE:
		is >> upgrade_time; // starvation prevention
		break;
	default:
		break;
	}

	is >> num_of_process;
	if (num_of_process > 4) {
		cout << "maximum 4 processes only!";
		return;
	}
	is.ignore(numeric_limits<streamsize>::max(), '\n');
	for (int i = 0; i < num_of_process; i++) {
		Process p;
		p.id = i + 1;
		string line;
		getline(is, line);
		stringstream ss(line);
		if (algorithm_type == PRIROITY_NONPREEMPTIVE || algorithm_type == PRIORITY_PREEMPTIVE) // Priority non-preemptive or preemptive (priority scheduling algorithm)
			ss >> p.priority_attributes.priority_level;
		ss >> p.arrival_time;
		int cnt = 0, num;
		while (cnt < 6 && ss >> num) { // cnt < 6 due to restraint "maximum of 3 times for each (CPU/R)"
			if (cnt++ % 2 == 0)
				p.CPU_burst_time.push(num);
			else p.resource_usage_time.push(num);
		}
		processes.push_back(p);
	}
	sort(processes.begin(), processes.end(), [](Process a, Process b) { // for every algorithm, time needs to be sorted ascending
		return a.arrival_time < b.arrival_time;
		});
}

void writeFile(ostream& os, vector<Process>& processes, vector<string>& CPU_chart, vector<string>& R_chart) {
	// Overall, if the vector resizing is not frequent, the time complexity of the the code below is O(k), 
	// where k is the number of "_" characters at the end of the CPU_chart vector. However, if resizing occurs frequently, 
	// the time complexity could be higher, approaching O(n) in the worst case, where n is the size of the vector.
	while (!CPU_chart.empty() && CPU_chart[CPU_chart.size() - 1] == "_") // dam bao nhung time slot cuoi khong co _
		CPU_chart.erase(CPU_chart.begin() + CPU_chart.size() - 1);
	while (!R_chart.empty() && R_chart[R_chart.size() - 1] == "_")  // dam bao nhung time slot cuoi khong co _
		R_chart.erase(R_chart.begin() + R_chart.size() - 1);


	for (auto& it : CPU_chart)
		os << it << " ";
	os << "\n";
	for (auto& it : R_chart)
		os << it << " ";
	os << "\n";

	sort(processes.begin(), processes.end(), [](Process a, Process b) {
		return a.id < b.id;
		});

	float AVG_TT = 0, AVG_WT = 0;
	for (auto& it : processes) {
		os << it.turn_around_time << " ";
		AVG_TT += it.turn_around_time;
	}
	os << "\n";
	for (auto& it : processes) {
		os << it.waiting_time << " ";
		AVG_WT += it.waiting_time;
	}
	os << "\n";

	AVG_TT /= processes.size();
	AVG_WT /= processes.size();
	os << "AVG TT = " << setprecision(2) << fixed << AVG_TT << "\n";
	os << "AVG WT = " << setprecision(2) << fixed << AVG_WT << "\n";
}