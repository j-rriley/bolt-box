Jaime Riley
CIS 3207
Networked Spell Checker

This program allows clients to connect to a server, input one word at a time the words they would like to get \n
spell-checked, and the server will check a dictionary and tell the client whether the word is spelled correctly or not. \n
The server will continue to expect words from a client until they enter the escape key and press return. The program \n
that enables the Networked Spell Checker is multithreaded. Therefore, multiple clients can access the server at once and \n
no data will be lost or compromised. Each time a word is enter, the word is returned followed by either "OK" or "MISPELLED" \n
based off whether the input was spelled correctly. \n


When the server first starts, this is what is seen on the server side: \n
![When the server first starts](ServerSide1.png)


When a client connects to the server, they will see the following. See what happens when words and the esc key are entered. \n
![When the client first connects to the server](ClientSide1.png)

From the server end, this is what is happening: \n
![When the server receives words and client disconnects](ServerSide2.png)

