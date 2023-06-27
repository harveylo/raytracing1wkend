# Simple Ray Tracing Renderer
This project is based on the [book series](https://raytracing.github.io/).

All code has been copied and/or modified from the book. I have arranged it in a way that makes it more readable as a complete project.
# Multi-threads support
Use command line parameter `-b` to set number of active threads.
# Command line arguments
- `-s`: select scene to render, default value: 0.
  - 0: random spheres scene
  - 1: two spheres with checker texture
  - 2: two spheres with Perlin texture
  - 3: Earth
  - 4: a simple light scene
  - 5: a Cornell box
  - 6: a Cornell box with two volume boxs
  - 7: final scene
- `-p`: samples per pixel
- `-w`: width of the output image
- `-b`: number of active threades, better be set to match the number physical core
- `-r`: aspect ratio of the output image