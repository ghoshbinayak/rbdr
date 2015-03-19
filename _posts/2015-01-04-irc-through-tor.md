---
layout: post
title: IRC through tor
categories: [proxy, fucking-iiest]
tags: [Tor, IRC, Proxy]
published: True

---

In my college they use squid proxy with only a few open ports. This doesn't allow us to use the irc clients. The solution is to use some application like [XChat](http://xchat.org/) through [Tor](https://www.torproject.org/). But most irc servers block tor exit nodes. Freenode is also not an exception. But it does allow access through tor. To get connect to freenode through tor follow the instructions [here](https://www.freenode.net/irc_servers.shtml#tor) and also [here](https://cortman.wordpress.com/2013/03/12/how-to-set-up-xchat-with-tor-sasl/). 

###Note: Don't use the sasl script in python for XChat. Use the perl script.