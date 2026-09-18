# Gallery

:::{toctree}
:maxdepth: 1
:hidden:
example_segmentation
example_cell_tracking
example_cell_signal
:::

```{raw} html
<style>
.gallery-grid {
    display: flex;
    flex-wrap: wrap;
    gap: 20px;
    justify-content: space-around;
}

.gallery-item {
    flex-basis: calc(100% / 2 - 40px);
    margin-bottom: 20px;
    text-align: center;
}

.gallery-item img {
    width: 100%;
    height: auto;
    border-radius: 10px;
    transition: transform 0.3s ease;
}

.gallery-item:hover img {
    transform: scale(1.1);
}

.gallery-title {
    margin-top: 10px;
    font-size: 1.2em;
    font-weight: bold;
}
</style>

<div class="gallery-grid">

<div class="gallery-item">
  <a href="example_segmentation.html">
    <img src="../_static/thumbnail/example_segmentation_thumbnail.gif" alt="Image segmentation">
    <div class="gallery-title">Image segmentation</div>
  </a>
</div>

<div class="gallery-item">
  <a href="example_cell_tracking.html">
    <img src="../_static/thumbnail/example_cell_tracking.gif" alt="Image cell tracking">
    <div class="gallery-title">Image cell tracking</div>
  </a>
</div>

<div class="gallery-item">
  <a href="example_cell_signal.html">
    <img src="../_static/thumbnail/example_cell_signal.gif" alt="Image cell signal quantification">
    <div class="gallery-title">Image cell signal quantification</div>
  </a>
</div>

</div>
