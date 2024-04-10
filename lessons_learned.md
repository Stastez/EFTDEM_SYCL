# Lessons learned
- When using two-dimensional buffers / accessors in SYCL, the first size parameter is the height by default, the second
    is the width (think the indexing of matrices)