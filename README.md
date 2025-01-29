# FT-Irc - A simple IRC server
A zero dependency IRC server written in C++. Can handle quite a few clients connected to it.
# Features
- Direct messages between users
- Channel operators
- Supports various channel modes: Invite, Password, Topic and Operator.
- Channel topics
- Invite only channels
- Password protected channels
- Message buffering per client (handles low bandwith connections)
# Building
## Requirements
Requires `make` to build
## Build
Simply run:
`make`
## Usage
`./ircserv <port> <password>`
