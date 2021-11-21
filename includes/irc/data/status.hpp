# ifndef STATUS_HPP
# define STATUS_HPP

# define SUCCESS	0

/* Used to indicate the nickname parameter supplied to a
command is currently unused.
*/
# define ERR_NOSUCHNICK(A) "401 :" + A + " No such nick/channel"


/* Used to indicate the server name given currently
doesn't exist.
*/
# define ERR_NOSUCHSERVER(A) "402 :" + A + " No such server"


/* Used to indicate the given channel name is invalid.
*/
# define ERR_NOSUCHCHANNEL(A) "403 :" + A + " No such channel"


/* Sent to a user who is either (a) not on a channel
which is mode +n or (b) not a chanop (or mode +v) on
a channel which has mode +m set and is trying to send
a PRIVMSG message to that channel.
*/
# define ERR_CANNOTSENDTOCHAN(A) "404 " + A + " :Cannot send to channel"


/* Sent to a user when they have joined the maximum
number of allowed channels and they try to join
another channel.
*/
# define ERR_TOOMANYCHANNELS(A) "405 " + A + " :You have joined too many channels"


/* Returned by WHOWAS to indicate there is no history
information for that nickname.
*/
# define ERR_WASNOSUCHNICK(A) "406 :" + A + " There was no such nickname"


/* Returned to a client which is attempting to send a
PRIVMSG/NOTICE using the user@host destination format
and for a user@host which has several occurrences.
*/
# define ERR_TOOMANYTARGETS(A) "407 :" + A + " Duplicate recipients. No message delivered"


/* PING or PONG message missing the originator parameter
which is required since these commands must work
without valid prefixes.
*/
# define ERR_NOORIGIN() "409 :No origin specified"


/* 412 - 414 are returned by PRIVMSG to indicate that
the message wasn't delivered for some reason.
ERR_NOTOPLEVEL and ERR_WILDTOPLEVEL are errors that
are returned when an invalid use of "PRIVMSG $<server>" or "PRIVMSG #<host>" is attempted.
*/
# define ERR_NORECIPIENT(A) "411 :No recipient given (" + A + ")"

# define ERR_NOTEXTTOSEND() "412 :No text to send"

# define ERR_NOTOPLEVEL(A) "413 :" + A + " No toplevel domain specified"

# define ERR_WILDTOPLEVEL(A) "414 :" + A + " Wildcard in toplevel domain"


/* Returned to a registered client to indicate that the
command sent is unknown by the server.
*/
# define ERR_UNKNOWNCOMMAND(A) "421 " + A + " :Unknown command"


/* Server's MOTD file could not be opened by the server.
*/
# define ERR_NOMOTD() "422 :MOTD File is missing"


/* Returned by a server in response to an ADMIN message
when there is an error in finding the appropriate
information.
*/
# define ERR_NOADMININFO(A) "423 :" + A + " No administrative info available"


/* Generic error message used to report a failed file
operation during the processing of a message.
*/
# define ERR_FILEERROR(A, B) "424 :File error doing " + A + " on " + B


/* Returned when a nickname parameter expected for a
command and isn't found.
*/
# define ERR_NONICKNAMEGIVEN() "431 :No nickname given"


/* Returned after receiving a NICK message which contains
characters which do not fall in the defined set.  See
section x.x.x for details on valid nicknames.
*/
# define ERR_ERRONEUSNICKNAME(A, B) "432 " + A + " " + B + " :Erroneus nickname"


/* Returned when a NICK message is processed that results
in an attempt to change to a currently existing
nickname.
*/
# define ERR_NICKNAMEINUSE(A) "433 :" + A + " Nickname is already in use"


/* Returned by a server to a client when it detects a
nickname collision (registered of a NICK that
already exists by another server).
*/
# define ERR_NICKCOLLISION(A) "436 " + A + " :Nickname collision KILL"


/* Returned by the server to indicate that the target
user of the command is not on the given channel.
*/
# define ERR_USERNOTINCHANNEL(A, B) "441 " + A + " " + B + " :They aren't on that channel"


/* Returned by the server whenever a client tries to
perform a channel effecting command for which the
client isn't a member.
*/
# define ERR_NOTONCHANNEL(A) "442 :" + A + " You're not on that channel"


/* Returned when a client tries to invite a user to a
channel they are already on.
*/
# define ERR_USERONCHANNEL(A, B) "443 " + A + " " + B + " :is already on channel"


/* Returned by the summon after a SUMMON command for a
user was unable to be performed since they were not
logged in.
*/
# define ERR_NOLOGIN(A) "444 :" + A + " User not logged in"


/* Returned as a response to the SUMMON command.  Must be
returned by any server which does not implement it.
*/
# define ERR_SUMMONDISABLED() "445 :SUMMON has been disabled"


/* Returned as a response to the USERS command.  Must be
returned by any server which does not implement it.
*/
# define ERR_USERSDISABLED() "446 :USERS has been disabled"


/* Returned by the server to indicate that the client
must be registered before the server will allow it
to be parsed in detail.
*/
# define ERR_NOTREGISTERED() "451 :You have not registered"


/* Returned by the server by numerous commands to
indicate to the client that it didn't supply enough
parameters.
*/
# define ERR_NEEDMOREPARAMS(A) "461 :" + A + " Not enough parameters"


/* Returned by the server to any link which tries to
change part of the registered details (such as
password or user details from second USER message).
*/
# define ERR_ALREADYREGISTRED() "462 :Unauthorized command (already registered)"


/* Returned to a client which attempts to register with
a server which does not been setup to allow
connections from the host the attempted connection
is tried.
*/
# define ERR_NOPERMFORHOST() "463 :Your host isn't among the privileged"


/* Returned to indicate a failed attempt at registering
a connection for which a password was required and
was either not given or incorrect.
*/
# define ERR_PASSWDMISMATCH() "464 :Password incorrect"


/* Returned after an attempt to connect and register
yourself with a server which has been setup to
explicitly deny connections to you.
*/
# define ERR_YOUREBANNEDCREEP() "465 :You are banned from this server"


# define ERR_KEYSET(A) "467 :" + A + " Channel key already set"

# define ERR_CHANNELISFULL(A) "471 " + A + " :Cannot join channel (+l)"

# define ERR_UNKNOWNMODE(A) "472 " + A + " :is unknown mode char to me"

# define ERR_INVITEONLYCHAN(A) "473 " + A + " :Cannot join channel (+i)"

# define ERR_BANNEDFROMCHAN(A) "474 :" + A + " Cannot join channel (+b)"

# define ERR_BADCHANNELKEY(A) "475 :" + A + " Cannot join channel (+k)"

/* Any command requiring operator privileges to operate
must return this error to indicate the attempt was
unsuccessful.
*/
# define ERR_NOPRIVILEGES() "481 :Permission Denied You're not an IRC operator"

/* Any command requiring 'chanop' privileges (such as
MODE messages) must return this error if the client
making the attempt is not a chanop on the specified
channel.
*/
# define ERR_CHANOPRIVSNEEDED(A) "482 " + A + " :You're not channel operator"


/* Any attempts to use the KILL command on a server
are to be refused and this error returned directly
to the client.
*/
# define ERR_CANTKILLSERVER() "483 :You cant kill a server!"


/* If a client sends an OPER message and the server has
not been configured to allow connections from the
client's host as an operator, this error must be
returned.
*/
# define ERR_NOOPERHOST() "491 :No Olines for your host"


/* Returned by the server to indicate that a MODE
message was sent with a nickname parameter and that
the a mode flag sent was not recognized.
*/
# define ERR_UMODEUNKNOWNFLAG() "501 :Unknown MODE flag"


/* Error sent to any user trying to view or change the
user mode for a user other than themselves.
*/
# define ERR_USERSDONTMATCH() "502 Cant change mode for other users"


/*
Dummy reply number. Not used.
*/
# define RPL_NONE() 300


/* Reply format used by USERHOST to list replies to
the query list.  The reply string is composed as
follows:
<reply> ::= <nick>['*'] '=' <'+'|'-'><hostname>
The '*' indicates whether the client has registered
as an Operator.  The '-' or '+' characters represent
whether the client has set an AWAY message or not
respectively.
*/
# define RPL_USERHOST(A, B, C) "302 :[" + A + "{" + B + C + "}]"


/* Reply format used by ISON to list replies to the
query list.
*/
# define RPL_ISON(A, B, C) "303 :[" + A + " {" + B + C + "}]"


/* These replies are used with the AWAY command (if
allowed). RPL_AWAY is sent to any client sending a
PRIVMSG to a client which is away. RPL_AWAY is only
sent by the server to which the client is connected.
Replies RPL_UNAWAY and RPL_NOWAWAY are sent when the
client removes and sets an AWAY message.
*/
# define RPL_AWAY(A, B) "301 :" + A + " " + B

# define RPL_UNAWAY() "305 :You are no longer marked as being away"

# define RPL_NOWAWAY() "306 :You have been marked as being away"


/* Replies 311 - 313, 317 - 319 are all replies
generated in response to a WHOIS message.  Given that
there are enough parameters present, the answering
server must either formulate a reply out of the above
numerics (if the query nick is found) or return an
error reply.  The '*' in RPL_WHOISUSER is there as
the literal character and not as a wild card.  For
each reply set, only RPL_WHOISCHANNELS may appear
more than once (for long lists of channel names).
The '@' and '+' characters next to the channel name
indicate whether a client is a channel operator or
has been granted permission to speak on a moderated
channel.  The RPL_ENDOFWHOIS reply is used to mark
the end of processing a WHOIS message.
*/
# define RPL_WHOISUSER(A, B, C, D) "311 " + A + " " + B + " " + C + " * :" + D

# define RPL_WHOISSERVER(A, B, C) "312 " + A + " " + B + " :" + C

# define RPL_WHOISOPERATOR(A) "313 :" + A + " is an IRC operator"

# define RPL_WHOISIDLE(A, B) "317 " + A + " " + B + " :seconds idle"

# define RPL_ENDOFWHOIS(A) "318 :" + A + " End of /WHOIS list"

# define RPL_WHOISCHANNELS(A, B, C) "319 :" + A + " {[@|+]" + B + C + "}"


/* When replying to a WHOWAS message, a server must use
the replies RPL_WHOWASUSER, RPL_WHOISSERVER or
ERR_WASNOSUCHNICK for each nickname in the presented
list.  At the end of all reply batches, there must
be RPL_ENDOFWHOWAS (even if there was only one reply
and it was an error).
*/
# define RPL_WHOWASUSER(A, B, C, D) "314 " + A + " " + B + " " + C + " * :" + D

# define RPL_ENDOFWHOWAS(A) "369 :" + A + " End of WHOWAS"


/* Replies define RPL_LISTSTART, define RPL_LIST, define RPL_LISTEND mark
the start, actual replies with data and end of the
server's response to a LIST command.  If there are
no channels available to return, only the start
and end reply must be sent.
*/
# define RPL_LISTSTART() "321 Channel :Users Name"

# define RPL_LIST(A, B, C) "322 " + A + " " + B + " :" + C

# define RPL_LISTEND() "323 :End of /LIST"




# define RPL_CHANNELMODEIS(A, B, C) "324 " + A + " " + B + " " + C

/* When sending a TOPIC message to determine the
channel topic, one of two replies is sent.  If
the topic is set, RPL_TOPIC is sent back else
RPL_NOTOPIC.
*/
# define RPL_NOTOPIC(A) "331 :" + A + " No topic is set"

# define RPL_TOPIC(A, B) "332 :" + A + " " + B


/* Returned by the server to indicate that the
attempted INVITE message was successful and is
being passed onto the end client.
*/
# define RPL_INVITING(A, B) "341 " + A + " " + B


/* Returned by a server answering a SUMMON message to
indicate that it is summoning that user.
*/
# define RPL_SUMMONING(A) "342 :" + A + " Summoning user to IRC"


/* Reply by the server showing its version details.
The <version> is the version of the software being
used (including any patchlevel revisions) and the
<debuglevel> is used to indicate if the server is
running in "debug mode".
The "comments" field may contain any comments about
the version or further version details.
*/
# define RPL_VERSION(A, B, C, D) "351 " + A + "." + B + " " + C + " :" + D


/* The RPL_WHOREPLY and RPL_ENDOFWHO pair are used
to answer a WHO message.  The RPL_WHOREPLY is only
sent if there is an appropriate match to the WHO
query.  If there is a list of parameters supplied
with a WHO message, a define RPL_ENDOFWHO must be sent
after processing each list item with <name> being
the item.
*/
# define RPL_WHOREPLY(A, B, C, D, E, F, G, H) "352 " + A + " " + B + " " + C + " " + D + " " + E + " " + F + "[*][@|+] :" + G + " " + H

# define RPL_ENDOFWHO(A) "315 :" + A + " End of /WHO list"


/* To reply to a NAMES message, a reply pair consisting
of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
server back to the client.  If there is no channel
found as in the query, then only RPL_ENDOFNAMES is
returned.  The exception to this is when a NAMES
message is sent with no parameters and all visible
channels and contents are sent back in a series of
RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
the end.
*/
# define RPL_NAMREPLY(A, B, C) "353 :" + A + " [[@|+]" + B + " [[@|+]" + C + " [...]]]"

# define RPL_ENDOFNAMES(A) "366 :" + A + " End of /NAMES list"


/* In replying to the LINKS message, a server must send
replies back using the RPL_LINKS numeric and mark the
end of the list using an  RPL_ENDOFLINKS reply.
*/
# define RPL_LINKS(A, B, C, D) "364 " + A + " " + B + " :" + C + " " + D

# define RPL_ENDOFLINKS(A) "365 :" + A + " End of /LINKS list"

/* When listing the active 'bans' for a given channel,
a server is required to send the list back using the
RPL_BANLIST and RPL_ENDOFBANLIST messages.  A separate
 RPL_BANLIST is sent for each active banid.  After the
banids have been listed (or if none present) a
*/
# define RPL_BANLIST(A, B) "367 " + A + " " + B

# define RPL_ENDOFBANLIST(A) "368 :" + A + " End of channel ban list"

/* A server responding to an INFO message is required to
send all its 'info' in a series of RPL_INFO messages
with a RPL_ENDOFINFO reply to indicate the end of the
replies.
*/
# define RPL_INFO(A) "371 :" + A

# define RPL_ENDOFINFO() "374 :End of /INFO list"

/* When responding to the MOTD message and the MOTD file
is found, the file is displayed line by line, with
each line no longer than 80 characters, using
#RPL_MOTD format replies.  These should be surrounded
by a RPL_MOTDSTART (before the RPL_MOTDs) and an
RPL_ENDOFMOTD (after).
*/
# define RPL_MOTDSTART(A) "375 :- " + A + " Message of the day - "

# define RPL_MOTD(A) "372 :- " + A 

# define RPL_ENDOFMOTD() "376 :End of /MOTD command"

/* RPL_YOUREOPER is sent back to a client which has
just successfully issued an OPER message and gained
operator status.
*/
# define RPL_YOUREOPER() "381 :You are now an IRC operator"


/* If the REHASH option is used and an operator sends
a REHASH message, an RPL_REHASHING is sent back to
the operator.
*/
# define RPL_REHASHING(A) "382 :" + A + " Rehashing"

/* When replying to the TIME message, a server must send
the reply using the define RPL_TIME format above.  The string
showing the time need only contain the correct day and
time there.  There is no further requirement for the
time string.
*/
# define RPL_TIME(A, B) "391 :" + A + " " + B

/* If the USERS message is handled by a server, the
replies RPL_USERSTART, RPL_USERS, RPL_ENDOFUSERS and
 RPL_NOUSERS are used.  RPL_USERSSTART must be sent
first, following by either a sequence of RPL_USERS
or a single define RPL_NOUSER.  Following this is
RPL_ENDOFUSERS.
*/
# define RPL_USERSSTART() "392 :UserID Terminal Host"

# define RPL_USERS() "393 :%-8s %-9s %-8s"

# define RPL_ENDOFUSERS() "394 :End of users"

# define RPL_NOUSERS() "395 :Nobody logged in"

/* The RPL_TRACE* are all returned by the server in
response to the TRACE message.  How many are
returned is dependent on the the TRACE message and
whether it was sent by an operator or not.  There
is no predefined order for which occurs first.
Replies RPL_TRACEUNKNOWN, RPL_TRACECONNECTING and
RPL_TRACEHANDSHAKE are all used for connections
which have not been fully established and are either
unknown, still attempting to connect or in the
process of completing the 'server handshake'.
RPL_TRACELINK is sent by any server which handles
a TRACE message and has to pass it on to another
server.  The list of RPL_TRACELINKs sent in
response to a TRACE command traversing the IRC
network should reflect the actual connectivity of
the servers themselves along that path.
RPL_TRACENEWTYPE is to be used for any connection
which does not fit in the other categories but is
being displayed anyway.
*/
# define RPL_TRACELINK(A, B, C) "200 Link " + A + " " + B + " " + C

# define RPL_TRACECONNECTING(A, B) "201 Try. " + A + " " + B

# define RPL_TRACEHANDSHAKE(A, B) "202 H.S. " + A + " " + B

# define RPL_TRACEUNKNOWN(A, B) "203 conf srcs " + A + " [" + B + "]"

# define RPL_TRACEOPERATOR(A, B) "204 Oper " + A + " " + B

# define RPL_TRACEUSER(A, B) "205 User " + A + " " + B

# define RPL_TRACESERVER(A, B, C, D, E, F) "206 Serv " + A + " " + B + "S " + C + "C " + D + " " + E + "@" + F

# define RPL_TRACENEWTYPE(A, B) "208 " + A + " 0 " + B

# define RPL_TRACELOG(A, B) "261 File " + A + " " + B


# define RPL_STATSLINKINFO(A, B, C, D, E, F, G) "211 " + A + " " + B + " " + C + " " + D + " " + E + " " + F + " " + G

# define RPL_STATSCOMMANDS(A, B) "212 " + A + " " + B

# define RPL_STATSCLINE(A, B, C, D) "213 C " + A + " * " + B + " " + C + " " + D

# define RPL_STATSNLINE(A, B, C, D) "214 N " + A + " * " + B + " " + C + " " + D

# define RPL_STATSILINE(A, B, C, D) "215 I " + A + " * " + B + " " + C + " " + D

# define RPL_STATSKLINE(A, B, C, D) "216 K " + A + " * " + B + " " + C + " " + D

# define RPL_STATSYLINE(A, B, C, D) "218 Y " + A + " " + B + " " + C + " " + D 

# define RPL_ENDOFSTATS(A) "219 :" + A + " End of /STATS report"

# define RPL_STATSLLINE(A, B, C) "241 L " + A + " " + B + " " + C

# define RPL_STATSUPTIME() "242 :Server Up %d days %d:%02d:%02d"

# define RPL_STATSOLINE(A, B) "243 O " + A + " " + B

# define RPL_STATSHLINE(A, B) "244 H " + A + " " + B

/* To answer a query about a client's own mode,
RPL_UMODEIS is sent back.
*/
# define RPL_UMODEIS(A) "221 " + A

/* In processing an LUSERS message, the server
sends a set of replies from RPL_LUSERCLIENT,
RPL_LUSEROP, RPL_USERUNKNOWN,
RPL_LUSERCHANNELS and RPL_LUSERME.  When
replying, a server must send back
RPL_LUSERCLIENT and RPL_LUSERME.  The other
replies are only sent back if a non-zero count
is found for them.
*/
# define RPL_LUSERCLIENT(A, B, C) "251 :There are " + A + " users and " + B + " invisible on " + C + " servers"

# define RPL_LUSEROP(A) "252 :" + A + " operator(s) online"

# define RPL_LUSERUNKNOWN(A) "253 :" + A + " unknown connection(s)"

# define RPL_LUSERCHANNELS(A) "254 :" + A + " channels formed"

# define RPL_LUSERME(A, B) "255 :I have " + A + " clients and " + B + " servers"

/* When replying to an ADMIN message, a server
is expected to use replies RLP_ADMINME
through to RPL_ADMINEMAIL and provide a text
message with each.  For RPL_ADMINLOC1 a
description of what city, state and country
the server is in is expected, followed by
details of the university and department
(RPL_ADMINLOC2) and finally the administrative
contact for the server (an email address here
is required) in RPL_ADMINEMAIL.
*/
# define RPL_ADMINME(A) "256 :" + A + " Administrative info"

# define RPL_ADMINLOC1(A) "257 :" + A

# define RPL_ADMINLOC2(A) "258 :" + A

# define RPL_ADMINEMAIL(A) "259 :" + A

/*
A = recipient
B = <nick>!<user>@<host>
*/
# define RPL_WELCOME(A, B) "001 " + A + " :Welcome to the Internet Relay Network " + B

/* A = servername B = vers */
# define RPL_YOURHOST(A, B, C) "002 " + A + " :Your host is " + B + ", running version " + C

/* A = <date> */
# define RPL_CREATED(A, B) "003 " + A + " :This server was created " + B

/*
A = <servername>
B = <version>
C = <available user modes>
D = <available channelsmode*/
# define RPL_MYINFO() "004 " + A + " " + B + " " + C + " " + D
#endif // STATUS_HPP