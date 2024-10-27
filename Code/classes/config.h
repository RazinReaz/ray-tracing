#include "inicpp.h"
#include "vector3f.h"
#include "interfaces.h"
#include "sphere.h"
#include "checkerboard.h"
#include "point_light.h"
#include "spotlight.h"
#include "material.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::vector<double> double_vector_from_string(const std::string &str, const char delimiter)
{
    std::vector<double> result;
    std::stringstream ss(str);
    while (ss.good())
    {
        std::string substr;
        getline(ss, substr, delimiter);
        result.push_back(std::stof(substr));
    }
    return result;
}

std::shared_ptr<material> create_material(ini::IniFile ini, std::string material_name)
{
    std::string material_type = ini[material_name]["type"].as<std::string>();
    if (material_type == "metal")
    {
        std::vector<double> albedo = double_vector_from_string(ini[material_name]["albedo"].as<std::string>(), ',');
        color c = {albedo[0], albedo[1], albedo[2]};
        double fuzziness = ini[material_name]["fuzziness"].as<double>();
        return std::make_shared<metal>(c, fuzziness);
    }
    else if (material_type == "lambertian")
    {
        std::vector<double> albedo = double_vector_from_string(ini[material_name]["albedo"].as<std::string>(), ',');
        color c = {albedo[0], albedo[1], albedo[2]};
        return std::make_shared<lambertian>(c);
    }
    else
    {
        std::cout << "Invalid material type" << std::endl;
        return nullptr;
    }
}

void set_general_config(ini::IniFile ini, int &recursions, int &antialiasing_samples, double &pixels_sample_scale)
{
    recursions = ini["general"]["recursions"].as<int>();
    antialiasing_samples = ini["general"]["antialiasing_samples"].as<int>();
    pixels_sample_scale = 1.0 / antialiasing_samples;
    return;
}

void set_viewport_config(ini::IniFile ini, int &near_, int &far_, int &fovY, int &aspect_ratio, double &vp_width, double &vp_height, int &img_width, int &img_height, double &dw, double &dh)
{
    near_ = ini["viewport"]["near"].as<int>();
    far_ = ini["viewport"]["far"].as<int>();
    fovY = ini["viewport"]["fov"].as<int>();
    aspect_ratio = ini["viewport"]["aspectratio"].as<int>();
    img_width = ini["viewport"]["image_width"].as<int>();
    img_height = img_width / aspect_ratio;
    vp_height = 2 * near_ * tan(fovY * M_PI / 360);
    vp_width = vp_height * aspect_ratio;
    dw = vp_width / img_width;
    dh = vp_height / img_height;
    return;
}

void set_camera_config(ini::IniFile ini, vector3f &position, vector3f &target)
{
    std::vector<double> camera_position = double_vector_from_string(ini["camera"]["position"].as<std::string>(), ',');
    std::vector<double> camera_target = double_vector_from_string(ini["camera"]["target"].as<std::string>(), ',');
    position = {camera_position[0], camera_position[1], camera_position[2]};
    target = {camera_target[0], camera_target[1], camera_target[2]};
    return;
}

shape3d *create_plane(ini::IniFile ini)
{
    std::string type = ini["plane"]["type"].as<std::string>();
    if (type == "checkerboard") {
        int checkerboard_size = ini["plane"]["size"].as<int>();
        std::shared_ptr<material> mat = create_material(ini, ini["plane"]["material"].as<std::string>());
        return new checkerboard(checkerboard_size, mat);

    } else if (type == "sphere") {
        double radius = ini["plane"]["radius"].as<double>();
        std::shared_ptr<material> mat = create_material(ini, ini["plane"]["material"].as<std::string>());
        return new sphere({0, 0, -radius}, radius, mat);

    } else {
        std::cout << "Invalid plane type" << std::endl;
        return nullptr;
    }
}

shape3d *create_object(ini::IniFile ini, std::string object_name)
{
    std::string object_type = ini[object_name]["type"].as<std::string>();
    if (object_type == "sphere")
    {
        std::vector<double> center = double_vector_from_string(ini[object_name]["center"].as<std::string>(), ',');
        vector3f center_ = {center[0], center[1], center[2]};
        double radius = ini[object_name]["radius"].as<double>();
        std::shared_ptr<material> mat = create_material(ini, ini[object_name]["material"].as<std::string>());
        return new sphere(center, radius, mat);
    }
    else
    {
        std::cout << "Invalid object type" << std::endl;
        return nullptr;
    }
}

light *create_light(ini::IniFile ini, std::string light_name)
{
    std::string light_type = ini[light_name]["type"].as<std::string>();
    if (light_type == "point")
    {
        std::vector<double> position = double_vector_from_string(ini[light_name]["position"].as<std::string>(), ',');
        double falloff = ini[light_name]["falloff"].as<double>();
        return new point_light({position[0], position[1], position[2]}, falloff);
    }
    else if (light_type == "spot" || light_type == "spotlight")
    {
        std::vector<double> position = double_vector_from_string(ini[light_name]["position"].as<std::string>(), ',');
        std::vector<double> direction = double_vector_from_string(ini[light_name]["direction"].as<std::string>(), ',');
        double falloff = ini[light_name]["falloff"].as<double>();
        double cutoff_angle = ini[light_name]["cutoff_angle"].as<double>();
        return new spotlight({position[0], position[1], position[2]}, falloff, cutoff_angle, {direction[0], direction[1], direction[2]});
    }
    else
    {
        std::cout << "Invalid light type" << std::endl;
        return nullptr;
    }
}

