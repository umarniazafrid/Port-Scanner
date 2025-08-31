#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>
#include <string>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../include/utils.h"

std::mutex output_mutex;

void scanPort(const std::string& ip, int port, std::ofstream& logfile) {
    int sock;
    struct sockaddr_in target;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return;

    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &target.sin_addr);

    if (connect(sock, (struct sockaddr*)&target, sizeof(target)) == 0) {
        std::string banner = grabBanner(sock);
        std::lock_guard<std::mutex> guard(output_mutex);
        std::cout << "[+] Port " << port << " open " << banner << std::endl;
        logfile << "[+] Port " << port << " open " << banner << std::endl;
        logfile.flush();
    } else {
        std::lock_guard<std::mutex> guard(output_mutex);
        logfile << "[-] Port " << port << " closed" << std::endl;
        logfile.flush();
    }
    close(sock);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <target_ip> <start_port> <end_port>\n";
        return 1;
    }

    std::string ip = argv[1];
    int start_port = std::stoi(argv[2]);
    int end_port = std::stoi(argv[3]);

    std::ofstream logfile("scan_results.txt");

    std::vector<std::thread> threads;
    for (int port = start_port; port <= end_port; ++port) {
        threads.emplace_back(scanPort, ip, port, std::ref(logfile));
        if (threads.size() >= 100) { // Limit concurrency
            for (auto& t : threads) t.join();
            threads.clear();
        }
    }

    for (auto& t : threads) t.join();
    logfile.close();

    std::cout << "[*] Scan complete. Results saved to scan_results.txt\n";
    return 0;
}
