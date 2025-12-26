#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

const std::string inputFilePath = "../files/input.in";

class Exception : public std::exception {
private:
    std::string errorMessage;
public:
    explicit Exception(const std::string& errorMessage) : errorMessage(errorMessage) {}

    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

void readVectorFromLine(std::vector<int>& v, const std::string& line) {
    v.clear();
    std::istringstream stream(line);
    int x;
    while (stream >> x)
        v.push_back(x);
}

void readInput(std::vector<int>& v1, std::vector<int>& v2) {
    std::ifstream fileIn(inputFilePath);
    if (!fileIn.is_open()) {
        throw Exception("Error: file path '" + inputFilePath + "' does not exist!");
    }

    std::string line;
    if (!std::getline(fileIn, line)) throw Exception("Error: missing first vector line!");
    readVectorFromLine(v1, line);

    if (!std::getline(fileIn, line)) throw Exception("Error: missing second vector line!");
    readVectorFromLine(v2, line);

    fileIn.close();

    if (v1.size() != v2.size()) {
        throw Exception("Error: vectors must have same length!");
    }
}

void producerWorker(const std::vector<int>& v1, const std::vector<int>& v2,
                    std::queue<int>& buffer, int maxQueueSize,
                    std::mutex& mtx, std::condition_variable& notFull, std::condition_variable& notEmpty,
                    bool& doneProducing) {
    for (int i = 0; i < v1.size(); ++i) {
        int product = v1[i] * v2[i];

        std::unique_lock<std::mutex> lock(mtx);
        notFull.wait(lock, [&]() { return buffer.size() < maxQueueSize; });

        buffer.push(product);
        std::cout << "Producer: computed " << product << " (index " << i << "), queue size = " << buffer.size() << "\n";

        lock.unlock();
        notEmpty.notify_one();
    }

    std::unique_lock<std::mutex> lock(mtx);
    doneProducing = true;
    lock.unlock();
    notEmpty.notify_all();
}

void consumerWorker(std::queue<int>& buffer,
                    std::mutex& mtx, std::condition_variable& notFull, std::condition_variable& notEmpty,
                    bool& doneProducing, int& sum) {
    sum = 0;

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        notEmpty.wait(lock, [&]() { return !buffer.empty() || doneProducing; });

        if (buffer.empty() && doneProducing) break;

        int value = buffer.front();
        buffer.pop();

        std::cout << "Consumer: added " << value << ", queue size = " << buffer.size() << "\n";

        lock.unlock();
        notFull.notify_one();

        sum += value;
    }
}

int main() {
    std::vector<int> v1, v2;
    try {
        readInput(v1, v2);
    } catch (const Exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    const size_t MAX_QUEUE_SIZE = 3;

    std::queue<int> buffer;
    std::mutex mtx;
    std::condition_variable notFull, notEmpty;
    bool doneProducing = false;
    int scalarSum = 0;

    auto start = std::chrono::high_resolution_clock::now();

    std::thread producer(producerWorker, std::ref(v1), std::ref(v2),
                         std::ref(buffer), MAX_QUEUE_SIZE,
                         std::ref(mtx), std::ref(notFull), std::ref(notEmpty),
                         std::ref(doneProducing));

    std::thread consumer(consumerWorker, std::ref(buffer),
                         std::ref(mtx), std::ref(notFull), std::ref(notEmpty),
                         std::ref(doneProducing), std::ref(scalarSum));

    producer.join();
    consumer.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\n---> The scalar product of the two vectors is " << scalarSum << ".\n";
    std::cout << "Execution time: " << duration << " ms\n";
    std::cout << "(Queue size = " << MAX_QUEUE_SIZE << ")\n";

    return 0;
}
