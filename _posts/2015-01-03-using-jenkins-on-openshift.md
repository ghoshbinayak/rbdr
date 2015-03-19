---
layout: post
title: using jenkins on openshift
categories: [web-development, openshift]
tags: [openshift, jenkins, automation]
published: True

---

So I have got my django app running on Openshift. ([how to run django app on Openshift]({% post_url 2014-12-29-django-on-openshift %}))
Now I wanted to automate the deployment of the app from a Github repo. The tool people use to do this is called Jenkins. So here's the rundown of what I have been able to achieve. 


First of all I havent been able to automatically build the app for deployment. What I've been able to do is setup a jenkins server, create a new job which receives an event from Github notifying a modification, after receiving which it simply pulls the changes from Github and pushes it to the Openshift server running the django app. On receiving the push the django app is rebuilt and deployed. Therefore I don't have to push the app to both Github and django. But it would be better to have continous integration as the Openshift server stops the django app during rebuild so it becomes unreachable. So next step would be to implement CI.


#Steps to get Jenkins up and running

###1. Create a jenkins app
{% highlight bash %}
    rhc app create jenkins jenkins-1
{% endhighlight %}

###2. Login to the jenkins using ssh

###3. Create a file `git-ssh.sh` in the data directory
Create a file named `git-ssh.sh` in the `$OPENSHIFT_DATA_DIR` and put the following code inside it.

{% highlight bash %}
    #!/bin/bash
 
    ID_RSA="${OPENSHIFT_DATA_DIR}id_rsa"
    KNOWN_HOSTS="${OPENSHIFT_DATA_DIR}known_hosts"
 
    ssh -o UserKnownHostsFile=$KNOWN_HOSTS -i $ID_RSA $1 $2 
{% endhighlight %}

###4. Generate ssh keys
We need to generate the ssh keys needed for git pull/push to work.
{% highlight bash %}
    ssh-keygen -t rsa
{% endhighlight %}
Enter the path to the id_rsa file pointed to by git-ssh.sh ie `${OPENSHIFT_DATA_DIR}id_rsa`
Keep the passphrase field blank.

###5. Put the public key on Github and Openshift
Copy the public key just created and add it to the list of SSH keys in [Github](https://github.com/settings/ssh) and [Openshift](https://openshift.redhat.com/app/console/settings).
The public key is located at `${OPENSHIFT_DATA_DIR}id_rsa.pub`

###6. Now open your jenkins dashboard and create a new job
Open the jenkins server that you just created on Openshift. Once you get to the dashboard, create a new job with the following properties:

+ `Item name: my-bldr` you can give any name you wish
+ `Build a free-style software project` select this option and click ok
+ `Application UUID: <UUID of the gear you want to push to>`
+ `Builder Size: small `
+ `Builder Timeout: 300000`
+ `Builder Type: redhat-python-2.7`
+ `Platfom: Linux`
+ `Restrict where this project can be run` check this option
+ `Label Expression: master`
+ `Source code management: Git`
+ Then enter the details of git repo you want to pull from
+ We'll setup the trigger a little later
+ Now add a Build script of type `Execute shell` and enter the following code there and save the job: 
{% highlight bash%}
#/bin/bash

echo "Trying to push.. "

git checkout master

git pull

git push -f dest master

{% endhighlight %}

###7. Add to the list of khown hosts
Login in to the jenkins server using ssh and then run the following commands:
{% highlight bash%}
export GIT_SSH=${OPENSHIFT_DATA_DIR}git-ssh.sh

#replace <Github repo url> with ssh url of your repo 
git -c core.askpass=true ls-remote -h <Github repo url> HEAD

#replace <Openshift repo url> with ssh url of git repo of your openshift app
git -c core.askpass=true ls-remote -h <Openshift repo url> 
{% endhighlight %}

###8. Configure jenkins to add required environment variable
Go to `Manage Jenkins > Configure System`

+ In `Global properties` check `Environment variables`
+ Add the following: `name: GIT_SSH`, `value: ${OPENSHIFT_DATA_DIR}git-ssh.sh`
+ save 


###9. Thats it now open my-bldr and click Build Now.
If I haven't missed anything then build should be successful.


Next step would be to add the [Github plugin](https://wiki.jenkins-ci.org/display/JENKINS/Github+Plugin) and follow their instructions to add a trigger so that a push to your git repo triggers a build.

