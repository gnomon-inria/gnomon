# Glossary

::::{admonition} Algorithm
:name: algorithm-definition
:class: definition

An **Algorithm** is an operation that transforms a [**Form**](form-definition) (or more) into another or other **Forms**.
Some **Algorithms** serve as a **source** (no input **Form**) or a **sink** (no output **Form**).

:::{include} diagrams/algorithm_diagram.md
:::

It is implemented by [**Plugins**](plugin-definition), and can be controlled through [**Parameters**](parameter-definition).
::::

:::{admonition} Form
:name: form-definition
:class: definition

A **Form** is a temporal sequence representing the evolution of a biological object, using a given computational representation. It consists of a time-indexed set of instances of an archetypal data structure (e.g. Image, Mesh, Tree...).

The different types of Forms that Gnomon manipulates are listed [here](forms_index)
:::

:::{admonition} Parameter
:name: parameter-definition
:class: definition

A **Parameter** is a typed variable proceeding from an atomic type (boolean, number, character string) or from a more complex one (range, color, 2D curve) whose value can be manipulated by the user through a graphical editor.
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

:::{admonition} Project
:name: project-definition
:class: definition

A **Project** corresponds to a local directory in your file system that gathers the pipelines, custom plugins, data and other resources files constituting a digital experiment.

The Gnomon application manages the Project in order to keep track of the various files required to replay the computations.
:::

::::{admonition} Reader
:name: reader-definition
:class: definition

A **Reader** is a specific type of [**Algorithm**](algorithm-definition) that is used to load [**Forms**](form-definition) from data files. It has no input Form and no Parameters apart from a mandatory **path**, and returns a single type of output Form.

:::{include} diagrams/reader_diagram.md
:::

Reader [**Plugins**](plugin-definition) declare the list of file extensions they support.
::::

:::{admonition} View
:name: view-definition
:class: definition

A **View** is an object dedicated to the materialization of [**Forms**](form-definition)
in the GUI through a third-party rendering framework.
It relies on [**Visualizations**](visualization-definition)
to control the way a particular Form is rendered in a given View.
:::

::::{admonition} Visualization
:name: visualization-definition
:class: definition

A **Visualization** is an operation that defines how a [**Form**](form-definition) is rendered in a [**View**](view-definition). 
It is implemented by specific [**Plugins**](plugin-definition) relying on the View's rendering framework, and can be controlled through [**Parameters**](parameter-definition).

:::{include} diagrams/visualization_diagram.md
:::
::::

:::{admonition} Workspace
:name: workspace-definition
:class: definition

A **Workspace** is the graphical counterpart of an [**Algorithm**](algorithm-definition),
where the user is able to set [**Forms**](form-definition)
as inputs through an input [**View**](view-definition),
tune the values of the **Parameters** and retrieve **Forms** as outputs in an output **View**.

:::{include} diagrams/workspace_diagram.md
:::

Available Workspaces are listed [**here**](available-workspace-list).
:::

::::{admonition} Writer
:name: writer-definition
:class: definition

A **Writer** is a specific type of [**Algorithm**](algorithm-definition) that is used to save [**Forms**](form-definition) as local files. It has no output Form and no Parameters apart from a mandatory **path**, and takes a single type of Form as input.

:::{include} diagrams/writer_diagram.md
:::

Writer [**Plugins**](plugin-definition) declare the list of file extensions corresponding to the formats they are able to write.
::::