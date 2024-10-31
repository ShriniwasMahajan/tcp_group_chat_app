# Command-Line Chat Application

A multi-threaded TCP socket-based chat server and client implementation using POSIX threads and socket APIs in C, supporting concurrent users with real-time message broadcasting through a centralized server in a command-line interface.

## Features

- Multi-client messaging support (up to 10 concurrent connections)
- Real-time message broadcasting through TCP sockets
- Thread-per-client architecture
- Named user identification
- Clean disconnection handling
- IPv4 network communication

## Technical Implementation

- POSIX TCP/IPv4 sockets for reliable network communication
- pthread library for handling concurrent connections
- Server-client architecture with centralized message broadcasting
- 1024 byte message buffer size
- Local network communication (default port: 2000)
- Dynamic memory allocation for client handling
- Thread detachment for automatic resource cleanup

## Requirements

- GCC compiler
- POSIX-compliant system (Linux/Unix)
- pthread library

## Building the Application

1. Ensure all required files are present:
   - `socket_server.c`
   - `socket_client.c`
   - `socket_util.c`
   - `socket_util.h`
   - `Makefile`

2. Compile:
   ```bash
   make
   ```

3. To remove the executables and object files after use:
   ```bash
   make clean
   ```

## Usage

### Start the Server
```bash
./socket_server.exe
```

### Connect as a Client
```bash
./sockt_client.exe
```

1. Enter your name when prompted
2. Type messages and press Enter to send
3. View incoming messages from other users automatically
4. Type 'exit' to disconnect

![Snapshot of the app in use](https://github.com/user-attachments/assets/8c99ceb2-ad17-4b87-a265-0eea825aa6fb)

## Notes

- Server must be running before clients can connect
- Messages follow format: "username: message"
- Server displays all chat activity
