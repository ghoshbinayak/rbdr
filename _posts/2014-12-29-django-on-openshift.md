---
layout: post
title: django on Openshift
categories: [web-development, ]
tags: [django, openshift]
published: True

---

[Openshift](https://www.openshift.com/) is a new app hosting service from RedHat which offers a great free package. Till now I've used Heroku to host some apps and its great but it doesn't provide any free database (well it does but you'll have to enter your credit card details which in India few people have). Also the filesystem of heroku is such that it refreshes every 24hrs deleting every thing except the repo. So heroku is great for testing but hosting something with somesort of a database is not possible unless you pay.


In comes ***Openshift*** to the rescue. It provides a 1GB filesystem which doesnot refresh and a free database which is quiet generous. So right now I'm trying to host a django app on Openshift and I'll document the steps that I take here.


#Steps:

### 1.Create a python app using the web interface. ###
Or using the command line:
{% highlight bash %}
    rhc app-create mypythonapp python-2.7
{% endhighlight %}


### 2.Clone the source code. ###
Can get git url form your apps [web console](https://openshift.redhat.com/app/console/applications).
{% highlight bash %}
    git clone <your apps git repo url.>
{% endhighlight %}


### 3.Locally create a django app. ###
Now you can modify the source, but its better to start from scratch. So ceate a new django app:
{% highlight bash %}
    django-admin startproject mydjangoapp
{% endhighlight %}


### 4.Install gunicorn and set it up: ###
Follow the instructions on the [gunicorn website](http://gunicorn.org/) to setup gunicorn:
{% highlight bash %}
    pip install gunicorn
{% endhighlight %}


### 5.Copy reqired files from cloned source. ###
Copy the following from the cloned source of your Openshift app to the root of the django app: `.openshift`, `setup.py`, `requirements.txt`


### 6.Generate the requirements file.###
{% highlight bash %}
    pip freeze > requirements.txt
{% endhighlight %}


### 7.Ceate the app.py file ###
OpenShift runs this app.py file as the entry point into the app. We are using gunicorn so we need to start gunicorn server from here. To do this follow the instructions in the following links:

+ [Getting Started with Python 2.6, 2.7, and 3.3)](https://developers.openshift.com/en/python-getting-started.html#step4)
+ [Gunicorn Custom Application](http://gunicorn-docs.readthedocs.org/en/latest/custom.html) 

The final app.py file will look something like this
{% highlight python %}
#!/usr/bin/python
from __future__ import unicode_literals
import os
import sys
import multiprocessing
import gunicorn.app.base
from mydjangoapp import wsgi
from gunicorn.six import iteritems

# hack to make sure we can load wsgi.py as a module in this class
sys.path.insert(0, os.path.dirname(__file__))

virtenv = os.environ['OPENSHIFT_PYTHON_DIR'] + '/virtenv/'
virtualenv = os.path.join(virtenv, 'bin/activate_this.py')
try:
    execfile(virtualenv, dict(__file__=virtualenv))
except IOError:
    pass

# Get the environment information we need to start the server
ip = os.environ['OPENSHIFT_PYTHON_IP']
port = int(os.environ['OPENSHIFT_PYTHON_PORT'])
host_name = os.environ['OPENSHIFT_GEAR_DNS']


def number_of_workers():
    return (multiprocessing.cpu_count() * 2) + 1


class StandaloneApplication(gunicorn.app.base.BaseApplication):

    def __init__(self, app, options=None):
        self.options = options or {}
        self.application = app
        super(StandaloneApplication, self).__init__()

    def load_config(self):
        config = dict([(key, value) for key, value in iteritems(self.options)
                       if key in self.cfg.settings and value is not None])
        for key, value in iteritems(config):
            self.cfg.set(key.lower(), value)

    def load(self):
        return self.application


if __name__ == '__main__':
    options = {
        'bind': '%s:%s' % (ip, port),
        'workers': number_of_workers(),
    }
    StandaloneApplication(wsgi.application, options).run()

{% endhighlight %}


### 8.Commit all the changes and push to Openshift ###
Commit all files in `mydjangoapp` to a git repo.
{% highlight bash %}
    git init

git add *

git commit -m'initial commit'
{% endhighlight %}

Then push this repo to Openshift
{% highlight bash %}
    git remote add openshift <git url of mypythonapp>

git push openshift
{% endhighlight %}


### 9.Adding environment variables if necessary ###
Follow the instructions [here](https://help.openshift.com/hc/en-us/articles/202399310-How-to-create-and-use-environment-variables-on-the-server-){: target blank}



And that should do the job.