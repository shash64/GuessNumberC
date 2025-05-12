# Number Guessing Game in C

This project is a university project, the main goal is to implement a simple client-server application in C that allows a client to guess a number chosen by a server.

## Features

The application demonstrates fundamental network programming concepts using two different transport layer protocols:

1.  **TCP:**
    * A connection-oriented version where the client establishes a connection with the server before the game starts
    * The server tells the client if their guess is "too high" or "too low" until the correct number is guessed

2.  **UDP:**
    * A connectionless version where messages (guesses and responses) are sent as individual datagrams
    * The core game logic remains the same

## Goals
- Understand the basics of socket programming in C
- Learn about TCP and UDP communication.
- Implement client and server logic for a simple application
- Handle basic network operations like sending and receiving data

## How to Use


1.  **Compile the C files:**
    ```bash
    # TCP
    gcc -Wall serveur.c -o serveurTCP
    gcc -Wall client.c -o clientTCP

    # UDP
    gcc -Wall serveur.c -o serveurUDP
    gcc -Wall serveur.c -o clientUDP
    ```

2.  **Run the server:**
    * Open a terminal and start the server (`./serveurTCP` or `./serveurUDP`)

3.  **Run the client:**
    * Open another terminal and start the client (`./clientTCP` or `./clientUDP`).
    * The client will ask for the server IP address. If you are running on the same machine use `127.0.0.1`
