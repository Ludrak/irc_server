# IRC HELPER INFORMATIONS
## IRC login routine
    PASS <serverpass>
    NICK <nickname>
    USER <loginname> 0 * <realname>

## IRC command format
    <command> <params ...> \r\n

## IRC commands according to RFC standards
    https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands
    https://fr.wikipedia.org/wiki/Aide:IRC/commandes
    https://en.wikipedia.org/wiki/IRCd#K-line
* [IRC connection return values](https://gist.github.com/proxypoke/2264878)
* [IRC hostmask](https://fr.wikipedia.org/wiki/Hostmask)
    Channels names are strings (beginning with a '&' or '#' character) of
   length up to 200 characters.  Apart from the the requirement that the
   first character being either '&' or '#'; the only restriction on a
   channel name is that it may not contain any spaces (' '), a control G
   (^G or ASCII 7), or a comma (',' which is used as a list item
   separator by the protocol).

   There are two types of channels allowed by this protocol.  One is a
   distributed channel which is known to all the servers that are
##  TLS encryption
* [sha256 with c++](https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c/10632725)
* [TLS wikipedia](https://fr.wikipedia.org/wiki/Transport_Layer_Security)

