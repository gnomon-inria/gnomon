# Python Algorithm Workspace

The Python Algorithm Workspace allows to develop a new Python plugin class directly within the application, and to integrate it to the current pipeline.

:::::{hint}
::::{grid}
:gutter: 1
:margin: 0

:::{grid-item} 
:columns: auto
:child-align: center
You can find a detailed tutorial for the Python Algorithm Workspace in the
:::

:::{grid-item}
:columns: auto  
:child-align: center
```{button-link} ../example/image_python_plugin.html
:color: primary
:outline:
Image Python Plugin Scenario
```
:::
::::
:::::

Through the Python editor, it is possible to write a plugin class that uses any library installed in the current environment. However the data structures used by the plugin must correspond to an existing form data plugin. There are buttons in the right menu that allow to simply configure the input and output forms of the plugin, and their associated data plugins.

Once written, or loaded from disk, the plugin can be executed by swiping the right menu towards the right, making appear a standard workspace display.