# Glossary

:::::{admonition} Algorithm
:name: algorithm-definition
:class: definition

An **Algorithm** is an operation that transforms a [**Form**](form-definition) (or more) into another or other **Forms**.
Some **Algorithms** serve as a **source** (no input **Form**) or a **sink** (no output **Form**).

::::{grid}
:padding: 1

:::{grid-item}
:columns: 3
:margin: auto
:class: sd-text-center sd-badge sd-text-success
<br/>

{fas}`images; fa-xl`

Input Form(s)
:::

:::{grid-item}
:columns: 1
:margin: auto
:class: sd-text-center sd-badge sd-text-success

{material-outlined}`east;3em;sd-text-success`

:::

:::{grid-item-card}
:columns: 3
:margin: auto
:class-body: sd-outline-warning sd-text-center sd-badge sd-text-warning
{fas}`gear; fa-xl`

Algorithm
:::

:::{grid-item}
:columns: 1
:margin: auto
:class: sd-text-center sd-badge sd-text-success

{material-outlined}`east;3em;sd-text-success`

:::

:::{grid-item}
:columns: 3
:margin: auto
:class: sd-text-center sd-badge sd-text-success
<br/>

{fas}`images; fa-xl`

Output Form(s)
:::


:::{grid-item}
:columns: 1
:margin: auto
:::

::::

It is implemented by [**Plugins**](plugin-definition).
:::::

:::{admonition} Form
:name: form-definition
:class: definition

A **Form** is a temporal sequence representing the evolution of a biological object, using a given computational representation. It consists of a time-indexed set of instances of an archetypal data structure (e.g. Image, Mesh, Tree...).
:::

:::{admonition} Parameter
:name: parameter-definition
:class: definition

A **Parameter** is a typed variable proceeding from an atomic type (boolean, number, character string) or from a more complex one (range, color, 2D curve) whose value the user can manipulate through a graphical editor.
:::

:::{admonition} Plugin
:name: plugin-definition
:class: definition

A **Plugin** is an inherited object conforming with the API of one type of
[**Algorithm**](algorithm-definition),
[**Form**](form-definition) or 
[**Visualization**](visualization-definition),
and generally relying on a third-party software library providing the functionality.

Available **Plugin** packages are listed [**here**](../plugins/index).
:::

:::{admonition} View
:name: view-definition
:class: definition

A **View** is an object dedicated to the materialization of [**Forms**](form-definition)
in the GUI through a third-party rendering framework.
It relies on [**Visualizations**](visualization-definition)
to control the way a particular Form is rendered in a given View.
:::

:::{admonition} Visualization
:name: visualization-definition
:class: definition

A **Visualization** is an operation that defines how a [**Form**](form-definition) is rendered in a [**View**](view-definition). 
It is implemented by specific [**Plugins**](plugin-definition) relying on the View's rendering framework.
:::

:::{admonition} Workspace
:name: workspace-definition
:class: definition

A **Workspace** is the graphical counterpart of an [**Algorithm**](algorithm-definition),
where the user is able to set [**Forms**](form-definition)
as inputs through an input [**View**](view-definition),
tune the values of the **Parameters** and retrieve **Forms** as outputs in an output **View**.

Available workspaces are listed [**here**](workspaces).
:::
