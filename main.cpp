#include "utility.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"
#include "material.h"
// #include "hittable.h"
#include "bvh.h"
#include "box.h"
#include "mesh.h"

#include <iostream>

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_data data;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, data))
        return background;

    ray scattered;
    color attenuation;
    color emitted = data.material_pointer->emitted();

    if (!data.material_pointer->scatter(r, data, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

hittable_list cornell_box() {
    hittable_list objects;
    hittable_list world;
    mesh m;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));


    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    // objects.add(make_shared<triangle>(point3(120, 20, 200),point3(400, 200, 100), point3(400, 22, 300), green));

    //adding manually triangles - resulted in weird lighting i think because of normals
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-51.320016, -46.565577, 70.381863),point3(54.657696, -6.065080, 82.045443), point3(-82.744186, 50.397899, 19.213408), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-82.744186, 50.397899, 19.213408),point3(54.657696, -6.065080, 82.045443), point3(23.233526, 90.898396, 30.876989), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-82.744186, 50.397899, 19.213408),point3(23.233526, 90.898396, 30.876989), point3(-54.657696, 6.065080, -82.045443), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-54.657696, 6.065080, -82.045443),point3(23.233526, 90.898396, 30.876989), point3(51.320016, 46.565577, -70.381863), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-54.657696, 6.065080, -82.045443),point3(51.320016, 46.565577, -70.381863), point3(-23.233526, -90.898396, -30.876989), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-23.233526, -90.898396, -30.876989),point3(51.320016, 46.565577, -70.381863), point3(82.744186, -50.397899, -19.213408), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-23.233526, -90.898396, -30.876989),point3(82.744186, -50.397899, -19.213408), point3(-51.320016, -46.565577, 70.381863), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-51.320016, -46.565577, 70.381863),point3( 82.744186, -50.397899, -19.213408), point3(54.657696, -6.065080, 82.045443), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(54.657696, -6.065080, 82.045443),point3(82.744186, -50.397899, -19.213408), point3( 23.233526, 90.898396, 30.876989), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3( 23.233526, 90.898396, 30.876989),point3(82.744186, -50.397899 ,-19.213408), point3(51.320016 ,46.565577 ,-70.381863), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-23.233526, -90.898396, -30.876989),point3(-51.320016, -46.565577, 70.381863), point3(-54.657696, 6.065080 ,-82.045443), red), vec3(320,170,370)));
    // objects.add(make_shared<translate>(make_shared<triangle>(point3(-54.657696, 6.065080, -82.045443),point3(-51.320016, -46.565577, 70.381863), point3(-82.744186 ,50.397899, 19.213408), red), vec3(320,170,370)));

    // adding triangles from obj file -- good result and fast
    auto tri = m.loadget(red);
    for (int i = 0; i < tri.size(); i++) {
        objects.add(make_shared<translate>(tri[i], vec3(320,170,370)));
    }

    //adding mesh to hittable list -- good result but slow... probably a bug here
    // objects.add(make_shared<translate>(make_shared<mesh>(red), vec3(320,170,370)));

    world.add(make_shared<bvh_node>(objects, 0.0, 1.0));

    return world;
}

hittable_list simple_light() {
    hittable_list objects;
    auto ground_material = make_shared<lambertian>(color(0.9, 0.5, 0.5));
    objects.add(make_shared<sphere>(point3(-1000,0,-1000), 1000, ground_material));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, white));
    objects.add(make_shared<triangle>(point3(-3.14912, 3.14912, 0),point3(3.14912, -3.14912, 2.0), point3(3.14912, 3.14912, 1.0), green));
    objects.add(make_shared<sphere>(point3(-1,3,0), 3, white));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    objects.add(make_shared<xy_rect>(2, 3, 1, 3, -2, difflight));
    // objects.add(make_shared<xz_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));
    hittable_list objects;

    auto blue = make_shared<lambertian>(color(.35, .55, .95));
    auto green = make_shared<lambertian>(color(.30, .45, .23));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto terracotta= make_shared<lambertian>(color(.75, .35, .25));
    auto terracottalight= make_shared<lambertian>(color(.88, .44, .35));

    auto metalic = make_shared<metal>(color(.12, .45, .15), 0.3);
    // boxes1.add(make_shared<box>(point3(-200,-200,-200), point3(200,200,200), blue));
    // boxes1.add(make_shared<triangle>(point3(120, 20, 200),point3(400, 200, 100), point3(400, 22, 300), green));


    // boxes1.add(make_shared<yz_rect>(0, 555, 0, 555, 555, white));
    // boxes1.add(make_shared<yz_rect>(0, 555, 0, 555, 0, white));
    // boxes1.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    boxes1.add(make_shared<box>(point3(200, -500, 200), point3(400, 0 , 400), terracottalight));
    // boxes1.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    // boxes1.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto glass = make_shared<dielectric>(1.5);
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    // objects.add(make_shared<sphere>(point3(400, 150, 170), 70, white));
    // objects.add(make_shared<sphere>(point3(200, 300, 400), 70, red));

    boxes1.add(make_shared<translate>(make_shared<mesh>(terracotta), vec3(300, 150, 145)));
    boxes1.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));
    // objects.add(make_shared<xy_rect>(300, 400, 150, 300, 150, light));
    objects.add(make_shared<bvh_node>(boxes1, 0, 1));



    return objects;
}

int main() {

    // Image
    // const auto aspect_ratio = 16.0 / 9.0;
    // const auto aspect_ratio = 3.0 / 2.0;
    const auto aspect_ratio = 1.0;
    const int image_width = 200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10000;
    const int max_depth = 50;
    color background(0,0,0);

    //cornell_box
    auto world = cornell_box();
    auto lookfrom = point3(278, 278, -800);
    auto lookat = point3(278, 278, 0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    auto vfov = 40.0;

    //simple_scene
    // auto world = simple_light();
    // auto lookfrom = point3(26,3,6);
    // auto lookat = point3(0,2,0);
    // auto vfov = 20.0;
    // vec3 vup(0,1,0);
    // auto dist_to_focus = 10.0;
    // auto aperture = 0.1;

    //final_scene
    // auto world = final_scene();
    // auto dist_to_focus = 10.0;
    // auto aperture = 0.1;
    // vec3 vup(0,1,0);
    // background = color(.88, .44, .35);
    // auto lookfrom = point3(320, 140, -450);
    // auto lookat = point3(320, 130, 0);
    // auto vfov = 40.0;


    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

    // Render

    //This line is important for ppm images I think
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::cerr << "\rScanlines remaining: " << image_height << ' ' <<j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
