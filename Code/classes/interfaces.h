#pragma once




// color shape3d::get_diffuse_and_specular_color(vector3f point, ray &r, std::vector<light *> lights, std::vector<shape3d *> objects)
// {
//     // assumes that the point is always on the shape
//     double lambert = 0,
//            phong = 0;
//     vector3f N = r.hit_info.normal;

//     for (auto light : lights)
//     {
//         if (!light->is_visible_from(point, N, objects))
//         {
//             continue;
//         }
//         vector3f to_source = light->position - point;
//         double distance = to_source.length();
//         to_source = to_source / distance;
//         double scaling_factor = exp(-1 * distance * distance * light->falloff); //! optimizable? 

//         vector3f R = r.reflect(point, N).direction; //! optimizable?
//         lambert += scaling_factor * std::max(0.0, N.dot(to_source)); // diffuse component
//         phong += pow(R.dot(to_source), shininess) * scaling_factor; // specular component
//     }
    
//     color object_color = get_color_at(point);

//     double diffuse_component = this->diffuse * lambert;
//     double specular_component = this->specular * phong;
    
//     return object_color * (this->ambient + diffuse_component + specular_component);
// };

// color shape3d::get_reflection_color(vector3f point, ray &r, std::vector<light *> lights, std::vector<shape3d *> objects, int recursions)
// {

//     ray current_ray = r;
//     double offset = 0.1, multiplier = 1.0;
//     color reflected = {0, 0, 0};
//     vector3f final_point = point;
//     while (--recursions)
//     {
//         vector3f normal = current_ray.hit_info.normal;
//         current_ray = current_ray.reflect(final_point, normal);
//         current_ray.origin = current_ray.origin + current_ray.direction * offset;
//         for (auto object : objects)
//         {
//             object->calculate_hit_distance(current_ray);
//         }
//         if (!current_ray.hit_info.hit)
//         {
//             reflected += sky_color * multiplier;
//             break;
//         }
//         final_point = current_ray.origin + current_ray.direction * current_ray.hit_info.distance;
//         reflected += current_ray.hit_info.object->get_diffuse_and_specular_color(final_point, current_ray, lights, objects) * multiplier;
//         multiplier *= current_ray.hit_info.object->reflection;
//     }
//     // vector3f final_point = current_ray.origin + current_ray.direction *current_ray.hit_info.distance;
//     // color reflected_ = current_ray.hit_info.object->get_diffuse_and_specular_color(final_point, current_ray, lights, objects );
//     return reflected * this->reflection;
// }

