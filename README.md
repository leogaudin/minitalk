# Minitalk Project

## Introduction
Minitalk is a client-server application that allows the transmission of messages between two processes using signals. The client sends a message character by character to the server, which receives and prints the message. This project is part of the curriculum at 42.

## Getting Started

### Prerequisites
- This project requires a Unix-like operating system.
- The GNU C Library (glibc) must be installed.

### Compilation
To compile the project, use the provided Makefile by running the following command:

```bash
make
```

This will generate the `client` and `server` executables.

## Usage

### Server
1. Start the server by running the `server` executable. It will display the server's process ID (PID) on the console.

2. Keep the server running in the background.

### Client
1. To transmit a message, execute the `client` executable with the following arguments:
   ```bash
   ./client <server_pid> <message>
   ```
   - `<server_pid>`: The process ID of the server (obtained from the server's console output).
   - `<message>`: The message you want to transmit to the server.

2. The client will convert the message into individual characters and send them bit by bit to the server.

3. The server will receive the signals and reconstruct the transmitted message.

4. The server will print the received message character by character on the console.

5. Repeat the steps to send additional messages.

## Implementation Details

### Client (`client.c`)
The client is responsible for transmitting the message to the server.

1. The `send_signal` function takes the server's PID and a character as input. It sends the character bit by bit to the server using signals.
   - For each bit of the character, it sends `SIGUSR1` if the bit is 1 or `SIGUSR2` if it is 0.
   - It uses the `kill` function to send the signals and `usleep` to introduce a small delay between each signal.

2. The `main` function in the client takes the server's PID and message as command-line arguments.
   - It iterates through each character in the message and calls the `send_signal` function to transmit it to the server.

### Server (`server.c`)
The server is responsible for receiving and printing the transmitted message.

    1. The `handle_signal` function is the signal handler for `SIGUSR1` and `SIGUSR2`.
       - It maintains a static `current_char` variable to store the bits of the received character.
       - It also keeps track of the current bit index.

    2. When a signal is received, the `handle_signal` function checks if it is `SIGUSR1`.
       - If it is, it assigns 1 to the least significant bit (LSB) of the `current_char`.
       - If it is `SIGUSR2`, the `current_char` remains unchanged (0).

    3. The `handle_signal` function increments the bit index.

    4. If the bit index reaches 8, it means that a full character has been received.
       - The function prints the character using `ft_printf` and resets the bit index and `current_char`.
       - Otherwise, it shifts `current_char` to the left by 1 to make room for the next bit.

    5. The `main` function starts the server by registering the `handle_signal` function as the handler for `SIGUSR1` and `SIGUSR2`.

    6. The server then enters an infinite loop using pause, waiting for signals to arrive.

    7.	Whenever a signal is received, the corresponding signal handler is called to process it.

### Example

Here’s an example of how to use the Minitalk project:

    1.	Start the server by running ./server in one terminal. Note down the server’s PID displayed on the console.
	2.	In another terminal, execute ./client <server_pid> "Hello, World!" to send the message “Hello, World!” to the server. Replace <server_pid> with the actual PID obtained in step 1.
	3.	The server will receive the signals, reconstruct the message, and print it as follows:

Hello, World!


	4.	You can send more messages by repeating step 2 with different messages.
	5.	To stop the server, you can use Ctrl+C in the terminal where the server is running.

This README provides an overview of the Minitalk project, how to compile and use it, and explains the implementation details of both the client and server components. Make sure to adjust any formatting or specific details according to your needs.