#include <iostream>
#include <map>
#include <thread>
#include <vector>

#include "bbq.h"

using namespace std;

const int kNumProducerThreads = 50;
const int kNumEntries = 15000;

map<int, unsigned int> results;

void producer(BoundedBlockingQueue<int>* bbq, const int n);
void consumer(BoundedBlockingQueue<int>* bbq, const int n);

int main(int argc, char* argv[]) {
	BoundedBlockingQueue<int> bbq;

	vector<thread> producerThreads;
	thread consumerThread(consumer, &bbq, kNumEntries * kNumProducerThreads);

	for (int i = 0; i < kNumProducerThreads; i++) {
		producerThreads.emplace_back(producer, &bbq, kNumEntries);
	}

	consumerThread.join();
	for (int i = 0; i < kNumProducerThreads; i++) {
		producerThreads[i].join();
	}

	int nerrors = 0;
	for (const auto& result : results) {
		// Each producer thread contributes one entry.
		if (result.second != kNumProducerThreads) {
			cout << "Error. Element " << result.first << " appears " 
				 << result.second << " times, expected " << kNumProducerThreads
				 << endl;
			nerrors++;
		}
	}

	cout << "test finished with " << nerrors << " error(s)." << endl;

	return 0;
}

void producer(BoundedBlockingQueue<int>* bbq, const int n) {
	for (int i = 0; i < n; ++i) {
		bbq->push(i);
	}
}

void consumer(BoundedBlockingQueue<int>* bbq, const int n) {
	for (int i = 0; i < n; ++i) {
		auto val = bbq->pop();
		if (results.find(val) == results.end()) {
			results[val] = 1;
		} else {
			results[val]++;
		}
	}
}