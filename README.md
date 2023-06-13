<div align="center">
	<h1>🗣️ minitalk</h1>
	<p>This project is a simple client-server communication system implemented using signals in C. The client sends a message to the server, and the server receives and displays the message character by character.</p>
   <a href="https://wakatime.com/badge/user/db0e5671-cec5-4e7b-9d41-19a881e67f7d/project/56f4877c-3ac5-4648-9b71-95c3b46426ff"><img src="https://wakatime.com/badge/user/db0e5671-cec5-4e7b-9d41-19a881e67f7d/project/56f4877c-3ac5-4648-9b71-95c3b46426ff.svg" alt="wakatime"></a>
   <br />
	<img src="https://img.shields.io/badge/norminette-passing-success"/>
	<a href="https://developer.apple.com/library/archive/documentation/Performance/Conceptual/ManagingMemory/Articles/FindingLeaks.html"><img src="https://img.shields.io/badge/leaks-none-success" /></a>
	<img src="https://img.shields.io/badge/bonus-included-success"/>
	<br />
	<img src="https://img.shields.io/badge/-125%2F100-success?logo=42&logoColor=fff" />
</div>

## 📖 Table of Contents
<!--ts-->
   * [Usage](#usage)
   * [How Does Minitalk Work?](#how-does-minitalk-work)
      * [Client](#client)
      * [Server](#client)
      * [Bonus Features](#bonus-features)
      * [Functions Explained](#functions-explained)
   * [Credits](#dependency)
<!--te-->

## Usage

1. Clone the repository:

```bash
git clone https://github.com/leogaudin/minitalk.git
```

2. Compile the source code using the provided Makefile:
```bash
make
```

> This will generate two executables: `client` and `server`.

3. Launch the server by executing the `server` binary:
```bash
./server
```

> The server will display its process ID (PID) on the console.

4. In a separate terminal window, launch the client by executing the client binary with the server's PID and the message you want to transmit:

```bash
./client <server_pid> <message>
```

> Replace `<server_pid>` with the PID displayed by the server, and `<message>` with the text you want to send.

Example:
```bash
./client 12345 Hello
```

> This will send the message "Hello" to the server with the PID 12345.

5. The server will receive the message and display it on the console:

```bash
Hello
```

> The server will display each character as it receives them.

6. You can repeat steps 4 and 5 to send multiple messages to the server.

## How Does Minitalk Work?

The communication between the client and server is achieved using signals.

The client converts each character of the message into 8 bits (1 byte) and sends them to the server one by one.

### Client
1. The client reads the server's PID and the message from the command-line arguments.
2. It iterates over each character in the message.
3. For each character, it calls the `send_signal` function, which sends 8 signals to the server.
4. The `send_signal` function converts the character into 8 bits and sends each bit as a signal to the server. It uses `SIGUSR1` to represent a 1 bit and `SIGUSR2` to represent a 0 bit.
5. After sending each bit, the client waits for a short duration (42 microseconds) using the `usleep` function. This delay allows the server to process the received signals.

### Server
1. The server starts by displaying its PID on the console.
2. It sets up signal handlers for `SIGUSR1` and `SIGUSR2` signals using the `sigaction` function.
3. The server enters an infinite loop and waits for signals using the `pause` function.
4. When a signal is received, the corresponding signal handler (`handle_signal`) is called.
5. The `handle_signal` function extracts the bit value (0 or 1) from the received signal and appends it to the current character being received.
6. If the character is fully received (8 bits), it is displayed on the console using `ft_printf`. The bit index and current character are reset to prepare for the next character.
7. After processing the received signal, the server sends a signal back to the client. If the received signal was SIGUSR1, it sends SIGUSR1 to acknowledge the received bit. If the received signal was SIGUSR2, it sends SIGUSR2.

### Bonus Features

The provided source code includes a bonus version (`client_bonus.c` and `server_bonus.c`) that adds additional functionality:

* Unicode characters like 🦁 are already supported in the mandatory part.
* Sending back a read receipt can be achieved by using the `sigaction` structure, that allows to access metadata about signal transmission.
   * The client displays a message on the console for each bit it sends. It prints "Received bit 1" for `SIGUSR1` and "Received bit 0" for `SIGUSR2`. This can help visualize the communication process.
   * The server sends an acknowledgment signal back to the client after receiving each bit. If the received signal was `SIGUSR1`, it sends `SIGUSR1` to the client. If the received signal was `SIGUSR2`, it sends `SIGUSR2`. This allows the client to know that the server has successfully received the bit.
These additional features enhance the interactivity and feedback during the communication process.

### Functions Explained
#### `send_signal` (client)

```C
void	send_signal(int pid, unsigned char character)
```
* Uses an intermediate `temp_char` buffer to read every bit of the character.
* At every iteration, `temp_char` is assigned the value of `character` shifted to the right `i` times.
   * Let's say we want to read **0**1100001 bit by bit, starting from the Most Significant Bit (left)

   * We first shift the value of `character` to the right 7 times, which gives us 0000000**0**.

   > **⚠️ Note: the Least Significant Bit (right) determines if a number is odd or even. If the LSB is 1, the number is odd. If the LSB is 0, the number is even.**

   * We then calculate `temp_char % 2`, which reveals if the number is odd (LSB = 1) or even (LSB = 0).

   * In this case, `temp_char % 2` is 0, so we send a `SIGUSR2` signal to the server, which will interpret it as a 0 bit.

   * We then shift the value of `character` to the right 6 times, which gives us 000000**01**.

   * In this case, `temp_char % 2` is 1, so we send a `SIGUSR1` signal to the server, which will interpret it as a 1 bit.

   * We repeat this process until we have sent all 8 bits.

#### `handle_signal` (server)
```C
void	handle_signal(int signal)
```
* Uses two static variables:
   ```C
   static unsigned char	current_char;
   static int				bit_index;
   ```
   * The use of static variables allows us to keep track of the information throughout the program's execution.

   * `current_char` stores the current character being received.

   * `bit_index` stores the index of the current bit being received.

* The following line extracts the bit value (0 or 1) from the received signal in a very condensed way:
   ```C
   current_char |= (signal == SIGUSR1);
   ```
   * The result of `(signal == SIGUSR1)` will either be 00000000 for false or 00000001 for true.

   * The `|=` operator is a bitwise OR assignment operator. It compares two bytes in their binary representation, and for every bit, assigns 0 if both bits are 0, and 1 otherwise.

   * Example:
      ```
      00101100   current_char
      00000001   result of (sigsent == SIGUSR1)
      --------
      00101101   result stored in message after the bitwise OR operation
      ```

   * Repeatingly doing this and shifting it to the left will allow us to store the entire character in `current_char`.

## Credits

*🙇🏻‍♂️ This project is largely based on the work and explanations of [ealgar-c](https://github.com/ealgar-c).*

Link to [ealgar-c/minitalk🗣️](https://github.com/ealgar-c/minitalk)
