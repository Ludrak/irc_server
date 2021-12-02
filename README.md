# irc_server
42 school ft_irc project ment to make an Internet Relay Chat. Made by lrobino and nlecaill


## Architecture

        <= NetworkEntity <= 
AEntity 
        <= RelayedEntity
                      <= RelayedClient


        <= ClientInfo
CommonInfo
        <= ServerInfo

## Futures évolution

- Attribute `O` channel flag only to `safe-channel` wich should be uniq
- `Away` status (RPL_AWAY) (easy to implement)