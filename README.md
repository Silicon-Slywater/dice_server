# dice_server
A simple server capable of chat and dice rolling functions, suitable for online DnD.
## Server Usage:
Forward port 4562 and run the server executable. Clients should be able to connect via the server's IP.

## Client Usage:
Run the client, and enter the IP address of the server. Once connected, enter a username.

### Dice Commands:
```
/d2   -   Roll a D2
/d4   -   Roll a D4
/d2   -   Roll a D6
/d2   -   Roll a D8
/d2   -   Roll a D10
/d2   -   Roll a D12
/d2   -   Roll a D20
/d2   -   Roll a D100

/g    -   Try your luck*

*(5% chance to critically fail, 45% chance to fail, 
45% chance to succeed, 5% chance to critically succeed)
```
