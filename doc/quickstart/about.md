# Why Gnomon?

The goal of Gnomon is to provide an **integrated environment** for studying morphogenesis in biology. Analyzing and simulating the kind of complex 3D+t structures that represent a developing living system often implies **combining several computational tools**, that may operate on different data structures or proceed from independent software libraries.

Assembling these computational steps into a **reusable pipeline** that can easily be shared on a different system, or replayed to **reproduce scientific results** constitutes a major challenge in the community. With Gnomon, we propose an original solution that relies on three founding principles:

## Interoperate & extend

In order to manipulate indifferently computational tools coming from different sources, Gnomon has to be **agnostic of their actual implementations**. This means that every operation in the platform is performed on *abstract* objects, whose actual functionalities are provided by **plugins** that conform with an *interface* (or **API**) specifying the necessary "handles" for Gnomon to interact with the component.

::::{card}
:class-card: sidebar sd-bg-light sd-px-0 sd-pb-0 sd-mt-2

:::{include} diagrams/plugin_diagram.md
:::
::::

We pushed this otherwise rather common plugin logic relatively far, since not only the **computational steps** but also the **data structures** representing the system, as well as **the way they are rendered** in 2D or 3D, are provided as plugins. This allows to interoperate tools coming from software libraries that may use very different representations of the same kind of structure, in a way that is transparent for the user.

The list of available plugins is not hard-coded, instead they are **dynamically discovered** at runtime. This implies that developers can **implement their own plugins** for algorithms, data structures or rendering, to use them in Gnomon and **interoperate them seamlessly** with the rest of the plugins. Gnomon also provides tools to guide the development of plugins within the platform, and make it as easy as possible to extend the platform with new methods.

## Reproduce computations

Guaranteeing the reproducibility of the results in a fundamental issue in scientific computing. Gnomon tackles it by keeping track of all the choices made by the user while processing its data: the **plugins that have been used**, with their versions and dependencies, and all **the values of their parameters**. It is then possible to trace back any produced output to its source data, through the process that computed it.

Gnomon formalizes this notion in the concept of **pipeline**, a directed graph of computational steps that represent the dependency links between the processes required to compute a given data. Pipelines can be replayed to **reproduce the computations** that have been performed by the user, but they can also be applied to different source data, in order to **automatize a processing** made manually on one sample data to a whole dataset.

## Design intuitively

Conceiving a user interface to efficiently design computational pipelines is not an obvious task. On the one hand, interfaces that let the user explicitly create boxes and connect them require to know in advance the expected outcome, and all the steps to reach it. On the other hand, interfaces that allow to apply computational steps one by one and explore the result generally alter the central data structure, making it impossible to design pipelines with convoluted dependency links.

::::{card}
:class-card: sidebar sd-bg-light sd-px-0 sd-pb-0 sd-mt-2

:::{include} diagrams/workspace_diagram.md
:::
::::

Gnomon proposes an original interface paradigm centered on one processing step, and the **transformation of data** from inputs to outputs. It materializes into the concept of **workspace**, the visual counterpart of one box in the pipeline graph. In a workspace, the user chooses a plugin and configures it through graphical editors, before running it and visually exploring the results. The data is materialized in **input and output views** that allow to interact simultaneously with the data before and after the transformation.

By moving data through several of these workspaces, the user, while focusing on the transformation steps, is **implicitly designing** the dependency graph between all the data it progressively generates. We believe that this dynamic way of interacting with computational steps constitutes an original user interface that allows an **intuitive an efficient conception of reproducible pipelines**.

:::{admonition} About us
:class: note

The Gnomon project is lead since 2019 by the [**Mosaic** Inria project-team](https://team.inria.fr/mosaic), directed by [Christophe Godin](https://team.inria.fr/mosaic/welcome/team-members/christophe-godin/), with the support of [Inria](https://inria.fr/en) (through the Experimentation & Development Services of Sophia-Antipolis, Rennes and Lyon) and [INRAE](https://www.inrae.fr/en).
:::