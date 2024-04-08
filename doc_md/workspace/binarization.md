# Binarization Workspace

The Binarization Workspace allows to compute a [BinaryImage form](../form/binary_image) using the intensity signal of an [Image form](../form/image) passed as input.

The form that is computed is a binary mask, an image-like data containing only 0 and 1 values, and will be rendered in the right output view. Some plugins, typically using iterative algorithms, might benefit from an initialization, which can be provided as an option BinaryImage in the left view.