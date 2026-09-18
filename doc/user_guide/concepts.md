# Concepts

During a session in the Gnomon application, the user will perform actions that impact three major components that represent complementary point of views on the processing that is being performed:

:::::{grid}
:padding: 0
:gutter: 0

:::{grid-item-card}
:columns: 3
:margin: auto
:link: concepts/world.html
{fas}`layer-group; fa1x`&nbsp; **World**
^^^

<small>The pool of Forms that are available to all of the application.</small>
:::

:::{grid-item}
:columns: 1
:margin: auto
:class: sd-text-center
<br/>

{fas}`right-left; fa-2x`
:::

:::{grid-item-card}
:columns: 3
:margin: auto
:link: concepts/workspace.html
{fas}`table-columns; fa1x`&nbsp; **Workspaces**
^^^

<small>The representation of the performed computational steps.</small>
:::

:::{grid-item}
:columns: 1
:margin: auto
:class: sd-text-center
<br/>

{fas}`right-left; fa-2x`
:::

:::{grid-item-card}
:columns: 3
:margin: auto
:link: concepts/pipeline.html
{fas}`diagram-project; fa1x`&nbsp; **Pipeline**
^^^

<small>The dependency graph linking those different steps.</small>
:::

:::::

:::{toctree}
:maxdepth: 1
:hidden:

concepts/workspace
concepts/world
concepts/pipeline
:::