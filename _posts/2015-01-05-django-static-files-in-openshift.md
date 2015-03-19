---
layout: post
title: Django static files in Openshift
categories: [web-development, Django, Openshift]
tags: [Django, Openshift]
published: True

---

To serve static files for django use [whitenoise](https://warehouse.python.org/project/whitenoise/).

This will require you to add an environment variable `DJANGO_SETTINGS_MODULE="jhinuk.settings"` to your Openshift app. To do this follow the instructions [here](https://help.openshift.com/hc/en-us/articles/202399310-How-to-create-and-use-environment-variables-on-the-server-)