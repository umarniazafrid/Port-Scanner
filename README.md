# 🔍 C++ Port Scanner with Banner Grabbing

This is a lightweight, educational port scanner written in C++.  
It demonstrates how attackers and defenders can enumerate services 
running on a host.

⚠️ Disclaimer: For educational purposes only. Use it only on machines you own or have explicit permission to test.

## Features
- TCP connect scanning
- Multi-threading
- Banner grabbing (basic service detection)
- Logs results to file

## Usage
```
g++ src/portscanner.cpp -o portscanner -pthread
./portscanner <target_ip> <start_port> <end_port>
# Port-Scanner
