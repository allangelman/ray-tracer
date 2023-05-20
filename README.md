# ⚡️ Ray Tracer with OBJ mesh loading

# Overview

For this personal project, I decided to reference the [Ray Tracing in One Weekend series](https://raytracing.github.io/) to create my own ray tracer! I heavily referenced the first book, [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html), partially referenced the second book [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html), and then added my own extensions for OBJ mesh loading.

# How to use

To create the build file run this command after cd'ing into the project. This will save a new build file called `raytracer` at the `./build` directory:

```
g++ -std=c++11 -I /Eigen main.cpp -o ./build/raytracer
```

To create a render, run this command. This will create a new image called `image.ppm` saved in the `./build` directory. Note: if you already have an image with the same name in that directory, this will overwrite that image. So be sure to rename your render before running a new render, or change the command below to use a different image name.

```
./build/raytracer > ./build/image.ppm
```

To create a render that includes a OBJ mesh, run this command. This will load in an OBJ named `mesh.obj` that is saved in the `./mesh` directory

```
./build/raytracer < ./mesh/mesh.obj > ./build/image.ppm
```

# Renders

# How this works

## Mesh loading

## BVH

## Barycentric

# Next Steps
