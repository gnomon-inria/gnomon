# Developing Plugins for Gnomon

This guide is addressed to developers who want to extend Gnomon with their own implementations of Forms, Algorithms and Visualization, supplied as Plugins. Here you will find guidelines to learn:
* [How to define a new Plugin for an Algorithm (in Python)](new_algorithm_plugin)
* [How to gather Python Plugins in a shareable package](new_plugin_package)

:::{toctree}
:maxdepth: 1
:hidden:

{% for package, package_info in packages.items() -%}
new_algorithm_plugin
new_plugin_package
{% endfor %}
:::