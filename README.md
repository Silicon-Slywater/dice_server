# dice_server
A simple server capable of chat and dice rolling functions, suitable for online DnD.
## Server Usage:
Forward port 4562 and run the server executable. Clients can connect through the server's public IP.
The maximum client count can be tweaked at start-up.

## Client Usage:
Run the client, and enter the IP address of the server. Once connected, enter a username.

*NOTE: Make sure the input buffer is cleared when receiving messages (CIN has been altered for async).*

### Dice Commands:
```
/d2   -   Roll a D2
/d4   -   Roll a D4
/d6   -   Roll a D6
/d8   -   Roll a D8
/d10  -   Roll a D10
/d12  -   Roll a D12
/d20  -   Roll a D20
/d100 -   Roll a D100

/g    -   Try your luck*

*(5% chance to critically fail, 45% chance to fail, 
45% chance to succeed, 5% chance to critically succeed)
```
### Multi-rolls & modifiers
```
A comma can be used to stack rolls:

e.g. /d20,d10,d4

Previous rolls can also be stacked:

e.g. /d6,,,	(This will roll a D6 four times.)


Addition and subtraction signs can be used to modify rolls:

e.g. /d4+6

This can also be stacked:

e.g. /d4+6,d6-2,d2+8

```
