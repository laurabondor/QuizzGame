# Quizz Game - "Computer Networks" course

QuizzGame Server is a multithreaded application designed to facilitate an engaging quiz game experience for multiple clients. The server efficiently manages a dynamic number of clients, coordinating their participation in a sequential question-and-answer format. Each client receives questions in the order of their registration, with a set time limit to respond.

## Features
+ **Multithreading**: The server is capable of handling numerous clients concurrently, ensuring a smooth and responsive quiz experience for all participants.

+ **Question Management**: The server loads questions from either XML files or an SQLite database, presenting them to clients in a systematic order. 

+ **Client Registration**: Clients are seamlessly registered with the server, allowing them to join the quiz session. The server keeps track of the order of registration to determine the order in which questions are presented.

+ **Time-Limited Responses**: Each client is allocated a specific time duration to respond to a given question. The server enforces this time constraint, creating a dynamic and fast-paced quiz atmosphere.

+ **Scoring System**: The server evaluates client responses, assigning points for correct answers. The scoring mechanism ensures fair and competitive gameplay.

+ **Client Synchronization**: The server synchronizes all clients, providing them with the same amount of time to answer each question. This ensures a level playing field for all participants.

+ **Graceful Client Exit**: The server gracefully handles situations where a client decides to leave the game. Exiting clients are removed from subsequent rounds, allowing the quiz to continue seamlessly.

+ **Game Conclusion**: The quiz concludes when all questions have been presented, and the server announces the winner to all remaining participants. The end of the game is managed systematically, providing closure to the quiz session.

## Communication
The communication between the server and clients is established using sockets, enabling efficient and reliable data exchange.

## Question Data Storage
Questions, along with their multiple-choice options, are stored an SQLite database, offering flexibility in managing and updating the quiz content.

## How to Run

1. Compile the Server and Client:

+ Before running the server and clients, compile the source code files.
```bash
gcc -o quizzgame_server server.c -pthread -lsqlite3
gcc -o quizzgame_client client.c
```
+ Ensure that the necessary dependencies, such as the SQLite library, are installed.

2. Start the Server:

+ Open a terminal and navigate to the server directory.
+ Run the compiled server using the following command:
```bash
./quizzgame_server
```
+ The server will start and await incoming client connections.
  
2. Connect Clients:

+ Clients can connect to the server using the IP address 127.0.0.1 and port 2908.
+ Clients should run the client application, providing the server's IP address and port during the connection process.
Example for connecting a client:
```bash
./quizzgame_client 127.0.0.1 2908
```
