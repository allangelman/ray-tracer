# ⚡️ Ray Tracer with OBJ mesh loading

![image 8h 10m 5s](https://github.com/allangelman/ray-tracer/assets/45411265/44a65569-072e-4dcb-86cd-ca76458338bf)

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

Additionally you will need to load the triangles from the mesh and add them to your hittable_list object. For example:

```
hittable_list objects;
hittable_list world;
mesh m;

auto tri = m.get_triangles(glass);
for (int i = 0; i < tri.size(); i++) {
    objects.add(tri[i]);
}

world.add(make_shared<bvh_node>(objects, 0.0, 1.0));
```

# Renders

This image shows some of the basic features of the ray tracer, including metallic/glass/reflective materials, depth of field, lights, and rendering spheres, AABB (axis-aligned bounding boxes), and planes.

800 x 533 pixels, 6000 samples, 5 max depth, 8 hr 10 min 5 sec on 2020 Macbook
![image 8h 10m 5s](https://github.com/allangelman/ray-tracer/assets/45411265/44a65569-072e-4dcb-86cd-ca76458338bf)

This image shows a 3D model of a samovar inside a classic Cornell box. I created this 3D model for [one of my art projects](https://vimeo.com/492851519?embedded=true&source=video_title&owner=94929753), and decided to use it to create this render. This model contains about 15,700 triangles.

600 x 400 pixels, 6000 samples, 5 max depth, 16 hr 22 min 46 sec on 2020 Macbook
![image1d 16h 22m 46s ](https://github.com/allangelman/ray-tracer/assets/45411265/9c4a1203-0616-4330-a46b-a4caa43d9ca2)

This image shows a low-poly Stanford bunny with a glass material. This model contains 1000 triangles.

600 x 400 pixels, 6000 samples, 5 max depth, 69 hr 17 min 50 sec on 2020 Macbook
![image2d 21h 17m 50s](https://github.com/allangelman/ray-tracer/assets/45411265/8ddb90d6-3c60-4c9b-8271-742b3fb62ab9)

# Implementation

## Camera, Ray, and Hittable

I initially started implementing the Ray Tracing in One Weekend book. This involved setting up camera, ray, and hittable classes. Hittable is an abstract class that other classes like sphere, aabb, and aarect all implement. Each hittable class implements these methods:

- hit
- bounding_box

The hit method is responsible for filling out the information in the hit_data data structure, which includes:

```
struct hit_data {
    point3 hit_point;
    vec3 hit_normal;
    shared_ptr<material> material_pointer;
    double t;
    bool front_face;
};
```

The bounding_box method needs to be implemented for the bounding volume hierarchy acceleration.

The camera class takes in these parameters:

- lookfrom
- lookat
- vup
- vfov
- aspect_ratio
- aperture
- focus_dist

Ray is defined by a point as the origin and a vector for the direction. So Ray takes these parameters:

- orig
- dir

## Bounding Volume Hierarchy (BVH)

This is an acceleration data structure that I referenced the Ray Tracing: The Next Week book to implement. The general idea behind this is to creating bounding boxes around sub sections of the scene. If a ray doesn't intersect the bounding box, we know it won't intersect anything inside the box. For this, we use Axis Aligned Bounding Boxes. The aabb class is defined by the maximum and minimum point of the box.

From there, we need to implement the bounding box method for all hittables. For the sphere, for example, the bounding box is fairly simple -- it is just a aabb constructed from the minimum and maximum points of the sphere (it's smallest and largest point in all 3 components)

```
bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}
```

The last step is making the bvh_node class. This is the class responsible for the splitting the list of objects into "left" and "right" sub lists, and recursing until all the objects have been hit tested.

## Mesh loading

For mesh loading, I ended up referencing a computer graphics course I took in college. I first had to figure out how to make a triangle hittable class, since a mesh is just a collection of triangles. Figuring out the correct triangle hit method was the challenging, but I eventually went with the Barycentric method, since this also allows to include interpolation between the three triangle points in the normal calculation. The other method I attempted to use was the [Moller-Trumbone method](https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm). It was actually faster than the Barycentric method when I tested, but I went with Barycentric in the end because it allowed for interpolation.

As you can see, the center of the samovar is triangulated when using the Moller-Trumbone method

![triangles](https://github.com/allangelman/ray-tracer/assets/45411265/c391856d-f4c2-4caf-a8a0-47f3abc3735f)

The next step was making a mesh class which was simply a list of triangles. This is where I loaded in my OBJ. This was simply a matter of parsing every line of the OBJ, which actually has a fairly simple file format. Each line is prefixed with a v, vn, or f for vertex, vertex normal, and face. Each face line includes nine numbers in the format: a/b/c d/e/f g/h/i. This defines a face with three vertices with indices a, d, g and normals c, f, i.

# Next Steps

For next steps, I would like to explore more acceleration data structures. My glass bunny took a while to render, and I am curious how to expedite that.

# Bloopers

This render happened early on when I was calculating the intersection of the sphere and I made a mistake in the roots calculation. Very funky!

![calculating roots incorrectly I think](https://github.com/allangelman/ray-tracer/assets/45411265/73011bea-f74b-4251-9535-fbe5241fde88)

This was a great milestone, when I finally got a triangle to render!

![cornel](https://github.com/allangelman/ray-tracer/assets/45411265/f1f334b9-29dc-4bb8-948e-d7f8f334afd7)

This was a silly mistake. I was trying to render a cube, but I left it quaded and forgot to triangulate the mesh before exporting it as an OBJ. My implementation expects the OBJs to include triangles, not quads, so the render came out like this, only displaying half of the triangles of the cube.

![didn'ttriangulate](https://github.com/allangelman/ray-tracer/assets/45411265/7174e225-663d-4037-a542-31472aff7d6a)

This was a cool render I made when deciding what material to render the Stanford bunny in. Looking at it again, I would like to render this at a bigger size and with more samples at some point, because this metallic bunny looks pretty cool!

![interpattempt](https://github.com/allangelman/ray-tracer/assets/45411265/41e7ddd2-1d0e-46d3-a53a-34209f8ac8ed)

This was also a milestone. I believe the Suzanne model was the first more complicated mesh I rendered with my ray tracer! There were still some bugs at this point, where the mesh only rendered if there was a plane behind it, hence the pink background.

![susanbig](https://github.com/allangelman/ray-tracer/assets/45411265/c06dcdbc-b28e-412e-9e70-59e9395c501e)
