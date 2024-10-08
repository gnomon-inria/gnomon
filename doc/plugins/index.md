# Plugins

Plugins are the meat of Gnomon. This is where the different Forms[{fas}`book-open;sd-text-primary fa-2xs`](form-definition), Visualization[{fas}`book-open;sd-text-primary fa-2xs`](visualization-definition) and Algorithms[{fas}`book-open;sd-text-primary fa-2xs`](algorithm-definition) are actually implemented.

::::{grid} 2
:::{grid-item-card}
:class-header: sd-bg-light sd-font-weight-bold sd-text-center
:link: packages/index.html
Plugin Packages
^^^
Get information about the publicly available packages of Gnomon Plugins.
:::

:::{grid-item-card}
:class-header: sd-bg-light sd-font-weight-bold sd-text-center
:link: howto/new_plugin_package.html
How to
^^^
Learn how to create your own Plugins and gather them into a shareable package.
:::
::::

:::{toctree}
:maxdepth: 1
:hidden:

{% for package, package_info in packages.items() -%}
packages/index
howto/new_plugin_package
{% endfor %}
:::