# IRC Server Testing Checklist (Using HexChat)

## 1. Basic Connection and Registration
- [ ] **Connect to the server**:Here’s the updated Markdown version with a checklist format for tracking progress:

  - Use HexChat to connect to your server's IP and port.
  - Check if the server accepts connections and returns the correct welcome messages (`RPL_WELCOME`).
- [ ] **Handle invalid nicknames**:
  - Try setting nicknames with invalid characters or too long (e.g., `nick!name` or `verylongnickname`).
  - Verify if the server correctly rejects them with `ERR_ERRONEUSNICKNAME`.
- [ ] **Detect duplicate nicknames**:
  - Attempt to connect two clients with the same nickname.
  - Ensure the server returns `ERR_NICKNAMEINUSE`.

---

## 2. User Commands
- [ ] **Set and change nicknames**:
  - Use `/nick` to change nicknames.
  - Confirm updates propagate to other users in the same channel.
- [ ] **Set user information**:
  - Test `/user` to set user information (username, hostname, realname).
  - Ensure the information is displayed correctly with a `WHOIS` query.

---

## 3. Channels Management
- [ ] **Create and join channels**:
  - Test `/join #channelname` with valid and invalid channel names.
  - Confirm users can successfully join and the server broadcasts their presence to the channel.
- [ ] **Leave a channel**:
  - Use `/part #channelname` to leave a channel.
  - Ensure the server notifies other members in the channel.
- [ ] **Channel modes**:
  - Test setting channel modes (e.g., `/mode #channel +t` for topic restriction).
  - Verify the server enforces the modes.
- [ ] **Set and retrieve topic**:Here’s the updated Markdown version with a checklist format for tracking progress:

  - Use `/topic #channel NewTopic`.
  - Ensure only authorized users (if `+t` mode is enabled) can set topics.

---

## 4. Messaging
- [ ] **Private messages**:
  - Send a `/msg nickname message`.
  - Verify delivery and format.
- [ ] **Channel messages**:
  - Send messages to a channel (`/msg #channel message`).
  - Confirm all members of the channel receive the message.
- [ ] **Notice messages**:
  - Test `/notice nickname message`.
  - Check if it bypasses channel and personal messaging filters (if implemented).

---

## 5. Error Handling
- [ ] **Disconnect handling**:
  - Close a client's connection unexpectedly and verify the server cleans up properly (e.g., removing the user from channels).
- [ ] **Invalid commands**:
  - Enter unrecognized or malformed commands.
  - Ensure the server responds with `ERR_UNKNOWNCOMMAND`.

---

## 6. Operator Commands
- [ ] **Grant operator privileges**:
  - Use `/mode #channel +o nickname` to assign operator rights.
  - Ensure only existing operators can perform this action.
- [ ] **Kick users**:
  - Use `/kick #channel nickname reason`.
  - Verify the user is removed and a message is broadcast.
- [ ] **Ban users**:
  - Test `/mode #channel +b nickname`.
  - Confirm the banned user cannot rejoin the channel.

---

## 7. Multiple Client Tests
- [ ] **Simultaneous connections**:
  - Connect multiple HexChat clients to test concurrency.
  - Verify message delivery and synchronization between users.
- [ ] **Stress test**:
  - Attempt many rapid connections and disconnections to check server stability.

---

## 8. Advanced Features
- [ ] **WHO and WHOIS**:
  - Test `/who` and `/whois` commands to list channel members and retrieve user information.
- [ ] **Names list**:
  - Use `/names #channel` to list all users in a channel.
  - Ensure the list is accurate.
- [ ] **Channel limits**:
  - Set a channel user limit (e.g., `/mode #channel +l 2`) and attempt to exceed it.
  - Ensure the server enforces the limit.

---

## 9. Compatibility with Other Clients
- [ ] Test your server with other IRC clients (e.g., mIRC or Weechat) to ensure broad compatibility.

---

## 10. Logging and Debugging
- [ ] Ensure your server logs all activities (connections, disconnections, errors, commands) for easier debugging during tests.

---

By systematically going through these tests, you can uncover edge cases and fix any bugs or missing features. Let me know if you need help with any specific commands or error codes!
