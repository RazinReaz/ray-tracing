#define _USE_MATH_DEFINES
#include <windows.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <GL/glut.h>
#include <chrono>
#include <memory>

#include "classes/bitmap_image.hpp"
#include "classes/inicpp.h"

#include "classes/vector3f.h"
#include "classes/color.h"

#include "classes/random.h"
#include "classes/rtutils.h"
#include "classes/config.h"
#include "classes/camera.h"

#include "classes/ray.h"
#include "classes/material.h"
#include "classes/shape3d.h"


#include "classes/point_light.h"
#include "classes/spotlight.h"

Camera camera;
bitmap_image image;

int near_, far_, fovY, aspect_ratio, image_width, image_height;
int antialiasing_samples, recursions;
double pixels_sample_scale;
double viewport_height, viewport_width, dw, dh;

bool gamma_correction_enabled = false;

std::vector<shape3d *> objects;
std::vector<light *> lights;

vector3f x_axis = {1, 0, 0};
vector3f y_axis = {0, 1, 0};
vector3f z_axis = {0, 0, 1};

namespace utils
{
    std::vector<bool> printed(11, false);
    int count = 0;
    void progress_report(int i, int j, int image_width, int image_height)
    {
        double completed = (i * image_width + j) * 100.0 / (image_width * image_height);
        // print progress once only when completed is a multiple of 10
        if ((int)completed % 10 == 0 && !printed[(int)std::round(completed / 10)])
        {
            printed[(int)std::round(completed / 10)] = true;
            std::cout << "Rendering " << (int)completed << "% completed" << std::endl;
        }
        return;
    }


    ray shoot_ray_at(int i, int j, Camera& camera, vector3f upper_left)
    {
        vector3f pixel_position = upper_left + camera.get_right() * dw * (j + random_double()) - camera.get_up() * dh * (i+ random_double());
        vector3f ray_direction = pixel_position - camera.get_position();
        return ray(pixel_position, ray_direction);
    }

    color trace_ray(ray r, std::vector<shape3d *> objects, std::vector<light *> lights, int recursions)
    {
        color integrated = {0, 0, 0};
        color attenuation = {1, 1, 1};
        ray current_ray = r;
        while (recursions--) {
            current_ray.reset_hit();
            for (auto object : objects) 
                object->calculate_hit_distance(current_ray);

            if (!current_ray.hit_info.hit)
            {
                integrated += sky_color * attenuation;
                break;
            }
            
            vector3f point = current_ray.hit_info.point;
            vector3f normal = current_ray.hit_info.normal;
            std::shared_ptr<material> mat = current_ray.hit_info.mat;

            attenuation *= mat->albedo;

            current_ray = mat->scatter(current_ray, point, normal);
        }
        return integrated;
    }
    
    color per_pixel(int& i, int& j, Camera camera, vector3f upper_left, int& antialiasing_samples, std::vector<shape3d *> objects, std::vector<light *> lights, int& recursions) {
        double red = 0, green = 0, blue = 0;
        double pixels_sample_scale = 1.0 / antialiasing_samples;

        for (int sample = 0; sample < antialiasing_samples; sample++)
        {
            ray r = shoot_ray_at(i, j, camera, upper_left);
            color c = trace_ray(r, objects, lights, recursions);
            red += c.r;
            green += c.g;
            blue += c.b;
        }
        // average the color
        red *= pixels_sample_scale;
        green *= pixels_sample_scale;
        blue *= pixels_sample_scale;

        if (gamma_correction_enabled)
        {
            red = gamma_correction(red);
            green = gamma_correction(green);
            blue = gamma_correction(blue);
        }
        return color(red, green, blue);
    }

    void capture() {
        int red = 0, green = 0, blue = 0;

        image.clear();
        printed = std::vector<bool>(11, false);
        std::cout << "Rendering started" << std::endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        vector3f upper_left = camera.get_position() + camera.get_direction() * near_ + camera.get_up() * viewport_height / 2 - camera.get_right() * viewport_width / 2;
        for (int i = 0; i < image_height; i++)
        {
            for (int j = 0; j < image_width; j++)
            {
                color c = per_pixel(i, j, camera, upper_left, antialiasing_samples, objects, lights, recursions);
                double red = c.r * 255;
                double green = c.g * 255;
                double blue = c.b * 255;

                image.set_pixel(j, i, red, green, blue);
                progress_report(i, j, image_width, image_height);
            }
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        std::cout << "Rendering 100% completed" << std::endl;
        std::cout << "Time taken = " << elapsed_time << "[ms]" << std::endl;
        std::string image_name = "out-" + std::to_string(count++) + ".bmp";
        image.save_image("output-images/" + image_name);
        std::cout << "image saved: "+ image_name +"\n"<< std::endl;
    }

    
}

void initGL()
{
    glClearColor(sky_color.r, sky_color.g, sky_color.b, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}


void draw_line(vector3f start, vector3f end, color c = color(0, 1, 0))
{
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
}


void draw_axes()
{
    glLineWidth(3);
    draw_line({0, 0, 0}, x_axis * 500, {1, 0, 0});
    draw_line({0, 0, 0}, y_axis * 500, {0, 1, 0});
    draw_line({0, 0, 0}, z_axis * 500, {0, 0, 1});
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
    glLoadIdentity();           // Reset the model-view matrix
    camera.look();

    for (auto object : objects)
    {
        object->show();
    }
    for (auto light : lights)
    {
        light->show();
    }

    draw_axes();
    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix
    gluPerspective(fovY, aspect_ratio, near_, far_);
}

void keyboardListener(unsigned char key, int xx, int yy)
{
    double camera_rotation_rate = 0.05;
    double object_rotation_rate = 1.0;
    double bonus_mark_rotation_rate = 0.4;
    switch (key)
    {
    case '0':
    {
        utils::capture();
        break;
    }
    case '1':
        camera.rotate_horizontal(camera_rotation_rate);
        break;
    case '2':
        camera.rotate_horizontal(-camera_rotation_rate);
        break;
    case '3':
        camera.rotate_vertical(camera_rotation_rate);
        break;
    case '4':
        camera.rotate_vertical(-camera_rotation_rate);
        break;
    case '5':
        camera.tilt(camera_rotation_rate);
        break;
    case '6':
        camera.tilt(-camera_rotation_rate);
        break;
    case 'a':
        camera.revolve(object_rotation_rate, {0, 1, 0});
        break;
    case 'd':
        camera.revolve(-object_rotation_rate, {0, 1, 0});
        break;
    case 'w':
        camera.move_with_same_target({0, 1, 0}, bonus_mark_rotation_rate);
        break;
    case 's':
        camera.move_with_same_target({0, 1, 0}, -bonus_mark_rotation_rate);
        break;
    case 'f':
        fast_disregard = !fast_disregard;
        std::cout << "Fast disregard is " << (fast_disregard ? "on" : "off") << std::endl;
        break;
    case 'g':
        gamma_correction_enabled = !gamma_correction_enabled;
        std::cout << "Gamma correction is " << (gamma_correction_enabled ? "on" : "off") << std::endl;
        break;
    case 'c':
        camera.print();
        break;
    case 'q':
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y)
{
    double rate = 5.0;
    switch (key)
    {
    case GLUT_KEY_UP:
        camera.move(camera.get_direction(), rate);
        break;
    case GLUT_KEY_DOWN:
        camera.move(camera.get_direction(), -rate);
        break;
    case GLUT_KEY_RIGHT:
        camera.move(camera.get_right(), rate);
        break;
    case GLUT_KEY_LEFT:
        camera.move(camera.get_right(), -rate);
        break;
    case GLUT_KEY_PAGE_UP:
        camera.move(camera.get_up(), rate);
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera.move(camera.get_up(), -rate);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    std::string scene_file = "scenes/scene1.ini";
    ini::IniFile ini(scene_file);

    vector3f camera_position, camera_target;
    set_general_config(ini, recursions, antialiasing_samples, pixels_sample_scale);
    set_viewport_config(ini, near_, far_, fovY, aspect_ratio, viewport_width, viewport_height, image_width, image_height, dw, dh);
    set_camera_config(ini, camera_position, camera_target);
    camera = Camera(camera_position, camera_target);

    shape3d *board = create_plane(ini);
    objects.push_back(board);

    int number_of_objects = ini["scene"]["number_of_objects"].as<int>();
    int number_of_lights = ini["scene"]["number_of_lights"].as<int>();

    for (int i = 0; i < number_of_objects; i++)
    {
        std::string object_name = "object" + std::to_string(i);
        shape3d *object = create_object(ini, object_name);
        if (object != nullptr)
            objects.push_back(object);
    }

    for (int i = 0; i < number_of_lights; i++)
    {
        std::string light_name = "light" + std::to_string(i);
        light *l = create_light(ini, light_name);
        if (l != nullptr)
            lights.push_back(l);
    }

    
    
    image.setwidth_height(image_width, image_height);

    


    glutInit(&argc, argv);                                    // Initialize GLUT
    glutInitWindowSize(600, 600);             // Set the window's initial width & height
    glutInitWindowPosition(750, 100);                           // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color
    glutCreateWindow("Raytracing");                           // Create a window with the given title
    glutDisplayFunc(display);                                 // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);                                 // Register callback handler for window re-shape

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    initGL();       // Our own OpenGL initialization
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}
