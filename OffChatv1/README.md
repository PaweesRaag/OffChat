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
