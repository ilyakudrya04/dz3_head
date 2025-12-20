#include <iostream>
#include <map>
#include <string>
#include "tiny_obj_loader.h"
#include "Camera.h"
#include "Rasterizer.h"
#include "Shader.h"
#include "Texture.h"

using namespace Math;

int main()
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

   
    bool ret = tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &warn,
        &err,
        "models/sponza.obj",  
        "models/",            
        true                  
    );

    if (!ret) {
        std::cerr << ".obj load error: " << err << std::endl;
        return 1;
    }

    
    Texture texture;
    if (!texture.load("models/head_diffuse.png")) {  
        std::cerr << "Texture not found! Using placeholder.\n";
        
    }

   
    Camera cam(Vec3(0, 5, 5), Vec3(0, 0, 0));  
   

   
    Mat4 V = cam.getView();
    Mat4 P = cam.getProjection(3.14f / 3, float(WIDTH) / HEIGHT);

    Framebuffer fb;
    fb.clear();  

    
    std::map<std::string, Texture> textureCache;  

    
    for (const auto& mat : materials) {
        if (!mat.diffuse_texname.empty()) {
            Texture tex;
            std::string texPath = "models/" + mat.diffuse_texname;
            if (tex.load(texPath.c_str())) {
                textureCache[mat.diffuse_texname] = std::move(tex);
            }
        }
    }

   
    for (const auto& s : shapes) {  
        for (size_t i = 0; i < s.mesh.indices.size(); i += 3) {
            Vec3 clip[3];
            Vec3 world[3];
            Vec3 normalV[3];
            Vec2 uv[3];

            for (int k = 0; k < 3; k++) {
                tinyobj::index_t idx = s.mesh.indices[i + k];

                int vid = idx.vertex_index;
                Vec3 v(
                    attrib.vertices[3 * vid + 0],
                    attrib.vertices[3 * vid + 1],
                    attrib.vertices[3 * vid + 2]
                );
                world[k] = v;

                int nid = idx.normal_index;
                Vec3 n(0, 0, 1);
                if (nid >= 0) {
                    n = normalize(Vec3(
                        attrib.normals[3 * nid + 0],
                        attrib.normals[3 * nid + 1],
                        attrib.normals[3 * nid + 2]
                    ));
                }
                normalV[k] = n;

                int tid = idx.texcoord_index;
                if (tid >= 0) {
                    uv[k].x = attrib.texcoords[2 * tid + 0];
                    uv[k].y = attrib.texcoords[2 * tid + 1];
                }
                else {
                    uv[k] = Vec2(0, 0);
                }

                Vec4 p = { v.x, v.y, v.z, 1 };
                p = mul(V, p);
                p = mul(P, p);

                
                if (p.w != 0) {
                    p.x /= p.w;
                    p.y /= p.w;
                    p.z /= p.w;
                }

                clip[k].x = (p.x + 1) * 0.5f * WIDTH;
                clip[k].y = (1 - p.y) * 0.5f * HEIGHT;
                clip[k].z = p.z;
            }

            
            int minX = std::max(0, (int)std::floor(std::min({ clip[0].x, clip[1].x, clip[2].x })));
            int maxX = std::min(WIDTH - 1, (int)std::ceil(std::max({ clip[0].x, clip[1].x, clip[2].x })));
            int minY = std::max(0, (int)std::floor(std::min({ clip[0].y, clip[1].y, clip[2].y })));
            int maxY = std::min(HEIGHT - 1, (int)std::ceil(std::max({ clip[0].y, clip[1].y, clip[2].y })));

            for (int y = minY; y <= maxY; y++) {
                for (int x = minX; x <= maxX; x++) {
                    Vec3 bc = bary(x + 0.5f, y + 0.5f,
                        Vec3(clip[0].x, clip[0].y, 0),
                        Vec3(clip[1].x, clip[1].y, 0),
                        Vec3(clip[2].x, clip[2].y, 0));

                    if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;

                    float z = clip[0].z * bc.x + clip[1].z * bc.y + clip[2].z * bc.z;

                   
                    Vec3 N = normalize(
                        normalV[0] * bc.x +
                        normalV[1] * bc.y +
                        normalV[2] * bc.z
                    );

                    Vec3 Pworld =
                        world[0] * bc.x +
                        world[1] * bc.y +
                        world[2] * bc.z;

                    Vec2 uvP =
                        uv[0] * bc.x +
                        uv[1] * bc.y +
                        uv[2] * bc.z;

                    
                    unsigned char* texel = nullptr;

                    
                    int mat_id = s.mesh.material_ids[i / 3];
                    if (mat_id >= 0 && mat_id < materials.size()) {
                        const auto& mat = materials[mat_id];
                        auto it = textureCache.find(mat.diffuse_texname);
                        if (it != textureCache.end()) {
                            texel = it->second.sample(uvP.x, uvP.y);
                        }
                    }

                    
                    if (!texel) {
                        texel = texture.sample(uvP.x, uvP.y);
                    }

                    Color c = phongTexture(N, Pworld, texel);
                    fb.setPixel(x, y, c, z);
                }
            }
        }
    }

    fb.savePPM("Image.ppm");
    std::cout << "Saved image.ppm\n";

    



























    texture.free();
    for (auto& pair : textureCache) {
        pair.second.free();
    }

    return 0;
}
