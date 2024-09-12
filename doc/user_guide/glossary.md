# Glossary

:::{admonition} Form
:name: form-definition
:class: definition

A **Form** is a temporal sequence representing the evolution of a biological object, using a given computational representation. It consists of a time-indexed set of instances of an archetypal data structure (e.g. Image, Mesh, Tree...).
:::

:::{admonition} Plugin
:name: plugin-definition
:class: definition

A **Plugin** is an inherited object conforming with the API of one type of **Algorithm**, **Form** or **Visualization**, and generally relying on a third-party software library providing the functionality.
:::

:::{admonition} View
:name: view-definition
:class: definition

A **View** is an object dedicated to the materialization of **Forms** in the GUI through a third-party rendering framework. It relies on **Visualizations** to control the way a particular Form is rendered in a given View.
:::

:::{admonition} Workspace
:name: workspace-definition
:class: definition

A **Workspace** is the graphical counterpart of an **Algorithm**, where the user is able to set **Forms** as inputs through an input **View**, tune the values of the **Parameters** and retrieve **Forms** as outputs in an output **View**.
:::
