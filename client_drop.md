To detect when a client disconnects unexpectedly from an IRC server, a few different techniques may be used:

### 1. **Detecting a Closed Socket**
   - When a client disconnects without explicitly sending a `LEAVE` command, the server’s `recv()` call on the client’s socket will typically return `0`, indicating an EOF (End of File). This means the client closed the connection, and you should then close the server’s side of the socket.
   - Here’s how it generally works in pseudo-code:
     ```cpp
     int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
     if (bytes_received == 0) {
         // Connection closed by client
         close(client_socket);
         removeClient(client_socket); // Custom function to handle cleanup
     } else if (bytes_received < 0) {
         // Error handling
     }
     ```

### 2. **Polling with `select()` or `poll()` for Non-Blocking I/O**
   - Using `select()` or `poll()` allows the server to monitor multiple sockets for activity, including errors. If a socket reports an error state, it can indicate that the client has dropped unexpectedly.
   - In the `poll()` structure, for example, if the `POLLHUP` flag is set on a socket, it usually means the client has disconnected.
   - Here’s an example using `poll()`:
     ```cpp
     struct pollfd pfds[NUM_CLIENTS];
     // setup pfds with client sockets and poll them
     if (poll(pfds, NUM_CLIENTS, timeout) > 0) {
         for (int i = 0; i < NUM_CLIENTS; i++) {
             if (pfds[i].revents & POLLHUP) {
                 // Client disconnected
                 close(pfds[i].fd);
                 removeClient(pfds[i].fd);
             }
         }
     }
     ```

### 3. **Implementing a Keep-Alive / Ping-Pong Mechanism**
   - Many IRC servers use a “ping-pong” protocol to check client connectivity. Periodically, the server sends a `PING` message to each connected client. If a client doesn’t respond with `PONG` within a certain time frame, the server can assume the client has disconnected.
   - For example:
     ```cpp
     send(client_socket, "PING :server", strlen("PING :server"), 0);
     // Set a timeout for receiving a PONG response
     if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0) {
         // No response from client; assume disconnected
         close(client_socket);
         removeClient(client_socket);
     }
     ```

### 4. **Handling Socket Errors in `recv()` or `send()`**
   - When an attempt to `send()` or `recv()` on a disconnected socket, it might return an error code (e.g., `ECONNRESET` or `EPIPE`), indicating the client is no longer available. In such cases, the client resources should be cleaned up

Using a combination of these approaches can help create a robust server that accurately detects unexpected client disconnections.