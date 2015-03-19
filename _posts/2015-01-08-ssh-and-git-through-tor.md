---
layout: post
title: ssh and git through tor
categories: [proxy bypass]
tags: [tor, proxy, git, ssh]
published: True

---

As I mentioned before our college network has this squid proxy which makes us spend more time configuring our tools to bypass it than doing any actual work. My goal this time was to get ssh working behind the proxy. 


Most git repositories provide ssh links. So to get git working I needed to get ssh working first. A simple `nmap` showed that there are 4 ports open on the proxy server one of them being the ssh port: 22. So I guessed I only needed to tunnel the ssh request using http connect. But turns they have tunneling blocked for port 22. 


Solution, as always **TOR**. I configured ssh to use the tor socket and volla every thing was working including git. So here goes the few steps to get ssh working with TOR:  


### 1. Install connect-proxy: ###
{% highlight bash%}
sudo apt-get install connect-proxy
{% endhighlight %}


### 2. Add config file for ssh to work with connect: ###
Create a file `~/.ssh/config` and put the following inside it
{% highlight bash %}
ProxyCommand connect -5 -S localhost:9050 `tor-resolve %h` %p
{% endhighlight %}

Here `9050` is the default tor port, change it if yours is different. 
This should be enough to get ssh working through tor. If you are prompted for a socks5 password then follow the next step.


### 3. Set socks5 password for tor:
Edit the `/etc/torsocks.conf` file and add the following:
{% highlight bash %}
default_user = binayak      # put your username here
default_pass = hello        # set a desired password
{% endhighlight %}

And also add the `SOCKS5_PASSWORD` environment variable( in `~/.bashrc` file):
{% highlight bash %}
export SOCKS5_PASSWORD="hello"
{% endhighlight %}


### 4. Get the git:// protocol working with tor:
Create a file `~/.torgit` and put the following there
{% highlight bash %}
#!/bin/sh
exec connect -5 -S localhost:9050 "$@"
{% endhighlight %}

And add the `GIT_PROXY_COMMAND` environment variable ( in `~/.bashrc` file):
{% highlight bash %}
export GIT_PROXY_COMMAND=~/.torgit
{% endhighlight %}
[Source](https://oniondigest.wordpress.com/2010/04/16/using-git-with-tor/)

Yup thats all there is to it..