# How to share a python plugin written in Python Algorithm Workspace

Once you have written a python plugin from Python Algorithm Workspace you might want to share it with other peoples.

Here is how to do it in a simple case:

## Sharing a python file

### user 1
  1. [gnomon] write python algo
  2. [gnomon] save as file
  3. share the file with colleague (email, ... )

### user 2
  1. save the received plugin as file
  2. [gnomon] load plugin


## Sharing a pipeline file
You might also want to share the plugin with other processing steps such as in a **pipeline**.

### user 1:
  1. [gnomon] create a pipeline with custom python algo
  2. [gnomon] save the pipeline
  3. share the pipeline file *.json

### user2:
  1. save the received pipeline on disk
  2. [gnomon] load the pipeline


Of course, if your code requires specific imports, don't forget to share the way to install needed imports with your colleague.
