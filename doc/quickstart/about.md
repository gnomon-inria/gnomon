# Why Gnomon?

The goal of Gnomon is to provide an **integrated environment** for studying morphogenesis in biology. Analyzing and simulating the kind of complex 3D+t structures that represent a developing living system often implies **combining several computational tools**, that may operate on different data structures or proceed from independent software libraries.

Assembling these computational steps into a **reusable pipeline** that can easily be shared on a different system, or replayed to **reproduce scientific results** constitutes a major challenge in the community. With Gnomon, we propose an original solution that relies on three founding principles:

## Interoperate & extend

In order to manipulate indifferently computational tools coming from different sources, Gnomon has to be **agnostic of their actual implementations**. This means that every operation in the platform is performed on *abstract* objects, whose actual functionalities are provided by **plugins** that conform with an *interface* (or **API**) specifying the necessary "handles" for Gnomon to interact with the component.

::::{card}
:class-card: sidebar sd-bg-light sd-px-0 sd-pb-0 sd-mt-2

:::{include} ../user_guide/diagrams/plugin_diagram.md
:::
::::

We pushed this otherwise rather common plugin logic relatively far, since not only the **computational steps** but also the **data structures** representing the system, as well as **the way they are rendered** in 2D or 3D, are provided as plugins. This allows to interoperate tools coming from software libraries that may use very different representations of the same kind of structure, in a way that is transparent for the user.

The list of available plugins is not hard-coded, instead they are **dynamically discovered** at runtime. This implies that developers can **implement their own plugins** for algorithms, data structures or rendering, to use them in Gnomon and **interoperate them seamlessly** with the rest of the plugins. Gnomon also provides tools to guide the development of plugins within the platform, and make it as easy as possible to extend the platform with new methods.

## Reproduce computations

## Conceive intuitively


:::{admonition} About us
:class: note

The Gnomon project is lead since 2019 by the [**Mosaic** Inria project-team](https://team.inria.fr/mosaic), directed by [Christophe Godin](https://team.inria.fr/mosaic/welcome/team-members/christophe-godin/), with the support of [Inria](https://inria.fr/en) (through the Experimentation & Development Services of Sophia-Antipolis, Rennes and Lyon) and [INRAE](https://www.inrae.fr/en).
:::