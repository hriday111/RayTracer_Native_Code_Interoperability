#include "rtweekend.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include <vector>
#include <cstring>
#include <memory>
#include <cstdint>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "external/stb_image_write.h"


typedef void* SceneHandle;
typedef void* MaterialHandle;

typedef void(*RenderCallback)(int samples, uint8_t* buffer);

struct CameraConfig {
    double aspect_ratio;
    int image_width;
    int samples_per_pixel;
    int max_depth;

    double vfov;
    double lookfrom_x, lookfrom_y, lookfrom_z;
    double lookat_x, lookat_y, lookat_z;
    double vup_x, vup_y, vup_z;

    double defocus_angle;
    double focus_dist;
};

extern "C"{

    MaterialHandle CreateLambertianMaterial(double r, double g, double b)
    {

        return new shared_ptr<material>(std::make_shared<lambertian>(new color(r,g,b)));

    }
    MaterialHandle CreateDielectricMaterial(double refraction_index) 
    {
        return new shared_ptr<material>(std::make_shared<dielectric>(refraction_index));
    }

    void DestroyMaterial(MaterialHandle handle)
    {
        auto ptr = static_cast<shared_ptr<material>*>(handle);
        delete ptr;
    }

    SceneHandle CreateScene()
    {
        return new hittable_list();
    }

    void DestroyScene(SceneHandle handle)
    {
        auto scene = static_cast<hittable_list*>(handle);
        delete scene;
    }

    void AddSphere(SceneHandle s_handle, double center_x, double center_y, double center_z, double radius, MaterialHandle m_handle)
    {
        auto scene = static_cast<hittable_list*>(s_handle);
        auto ptr = static_cast<shared_ptr<material>*>(m_handle);
        
        point3 center(center_x, center_y, center_z);

        scene->add(std::make_shared<sphere>(center, radius, *ptr));
    }
    void ClearScene(SceneHandle handle)
    {
        auto scene = static_cast<hittable_list*>(handle); 
        scene->clear();
    }

    void RenderScene(SceneHandle s_handle, const CameraConfig* config, uint8_t* output_buffer, RenderCallback callback)
    {
        auto scene = static_cast<hittable_list*>(s_handle); 
        camera cam;
        cam.aspect_ratio = config->aspect_ratio;
        cam.image_width = config->image_width;
        cam.samples_per_pixel = config->samples_per_pixel;
        cam.vfov = config->vfov;
        cam.lookfrom = point3(config->lookfrom_x, config->lookfrom_y, config->lookfrom_z);
        cam.lookat = point3(config->lookat_x, config->lookat_y, config->lookat_z);
        cam.vup = vec3(config->vup_x, config->vup_y, config->vup_z);

        cam.defocus_angle = config->defocus_angle;
        cam.focus_dist = config->focus_dist;

        cam.render(*scene, output_buffer, callback);
    }
    int SavePng(const char* filename, const uint8_t* buffer, int width, int height) {
    
    return stbi_write_png(filename, width, height, 4, buffer, width * 4);
}
}

