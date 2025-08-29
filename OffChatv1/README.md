#  OffChat v1 – Pseudo Off-Grid Chat System  

### System Overview  

										 SERVER (Raspberry Pi)
									|------------------------------|
								    | Create socket + bind + listen|
									|------------------------------|
										    	    |
										  +---------+---------+
										  |                   |
									  Client A             Client B
									  (Alice)              (Bob)

									   1. Connect            1. Connect
									   2. Send "Alice"       2. Send "Bob"
									   3. Read input         3. Read input
									  	  "Hi"                  "Hey"
									   4. Server receives    4. Server receives
										  "Alice: Hi"           "Bob: Hey"
									   5. Broadcast to all   5. Broadcast to all
										  (Bob sees it)         (Alice sees it)
## Usage :
1. First of change the line
   
   ```c
   addr.sin_addr.s_addr = INADDR_ANY;   // instead of inet_addr("127.0.0.1")
   ```
   > (Or use the Pi’s Wi-Fi IP explicitly; INADDR_ANY is easiest.)
2. Clone the repo
3. Install build tools
   
   ``c
   sudo apt update
   sudo apt install -y build-essential
   ``
4. Build
   ``c
   gcc -O2 -Wall -o server server.c
   gcc -O2 -Wall -o client client.c
   ``
5. Find the Pi’s Wi-Fi IP
   `hostname -I`
6. Run the server
   `./OffChatv1`
7. Connect clients on the same Wi-Fi
   >On other devices (another terminal on the Pi, your laptop, etc.)
   './client'

### Feature
1. Pseudo Off-Grid – Uses local Wi-Fi but runs independently of the internet by hosting the server on a Raspberry Pi (or any local device).
2. First Step Toward Off-Grid Comms – Currently broadcasts all messages to all connected clients (basic group chat).
3. Encryption Coming Soon – Right now, messages are in plain text for simplicity. Future versions will implement end-to-end encryption.
4. Inspired by LoRa & Mesh Networks – OffChat follows the philosophy of resilient, decentralized communication without relying on central infrastructure.

## This project is a foundation for fully off-grid communication systems. While basic, it demonstrates the principles of mesh/static networking and decentralized messaging.
