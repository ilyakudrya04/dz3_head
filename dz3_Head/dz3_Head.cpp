#include <iostream>
#include "tiny_obj_loader.h"
#include "Camera.h"
#include "Rasterizer.h"
#include "Shader.h"
#include "Texture.h"

using namespace Math;

int main()
{

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes; //объекты моделей с сеткой треугольников
    std::vector<tinyobj::material_t> materials; //цвета и материалы моделей
    std::string warn, err;

    bool ret = tinyobj::LoadObj
    (
        &attrib, &shapes, &materials,
        &warn, &err,
        "models/african_head.obj",
        "models",
        true
    );
    if (!ret)
    {
        std::cerr << ".obj load error: " << err << std::endl;
        return 1;
    }
    

    Texture texture;
    if (!texture.load("models/head_diffuse.png")) {
        std::cerr << "Texture not found!\n";
        return 1;
    }
    Camera cam(Vec3(0, 0, 3), Vec3(0, 0, 0));

    Mat4 V = cam.getView();
    Mat4 P = cam.getProjection(3.14f / 2, float(WIDTH) / HEIGHT);

    Framebuffer fb;

    for (tinyobj::shape_t& s : shapes)
    {
        //проходим по всем треугольникам
        for (int i = 0; i < s.mesh.indices.size(); i += 3)
        {

            Vec3 clip[3]; //координаты вершин на экране
            Vec3 world[3]; //координаты вершин в мире
            Vec3 normalV[3]; //нормали вершин
            Vec2 uv[3]; //координаты для текстуры

            for (int k = 0; k < 3; k++) //проходим по всем вершинам треугольника
            {
                tinyobj::index_t idx = s.mesh.indices[i + k];

                //координаты вершины
                int vid = idx.vertex_index;
                Vec3 v
                (
                    attrib.vertices[3 * vid + 0], //x
                    attrib.vertices[3 * vid + 1], //y
                    attrib.vertices[3 * vid + 2]  //z
                );
                world[k] = v;

                //нормали
                int nid = idx.normal_index;
                Vec3 n(0, 0, 1); //сама нормаль
                if (nid >= 0)
                {
                    n = normalize(Vec3
                    (
                        attrib.normals[3 * nid + 0],
                        attrib.normals[3 * nid + 1],
                        attrib.normals[3 * nid + 2]
                    ));
                }
                normalV[k] = n;

                //UV
                int tid = idx.texcoord_index;
                if (tid >= 0)
                {
                    uv[k].x = attrib.texcoords[2 * tid + 0]; //координата u
                    uv[k].y = attrib.texcoords[2 * tid + 1]; //координата v
                }
                else  //иначе (0,0)
                {
                    uv[k] = Vec2(0, 0);
                }

                Vec4 p = { v.x, v.y, v.z, 1 };

                p = mul(V, p); //перевод в координаты камеры
                p = mul(P, p); //перспектива

                p.x /= p.w;
                p.y /= p.w;
                p.z /= p.w;

                clip[k].x = (p.x + 1) * 0.5f * WIDTH;
                clip[k].y = (1 - p.y) * 0.5f * HEIGHT;
                clip[k].z = p.z;
            }

            //вычисление границ треугольника
            int minX = std::max(0, (int)std::floor(std::min({ clip[0].x, clip[1].x, clip[2].x })));
            int maxX = std::min(WIDTH - 1, (int)std::ceil(std::max({ clip[0].x, clip[1].x, clip[2].x })));

            int minY = std::max(0, (int)std::floor(std::min({ clip[0].y, clip[1].y, clip[2].y })));
            int maxY = std::min(HEIGHT - 1, (int)std::ceil(std::max({ clip[0].y, clip[1].y, clip[2].y })));

            //проверка на принадлежность пикселя треугольнику
            for (int y = minY; y <= maxY; y++)
            {
                for (int x = minX; x <= maxX; x++)
                {
                    //вычисление барицентрических координат
                    Vec3 bc = bary(x + 0.5f, y + 0.5f, clip[0], clip[1], clip[2]);

                    if (bc.x < 0 || bc.y < 0 || bc.z < 0)
                    {
                        continue;
                    }

                    float z = clip[0].z * bc.x + clip[1].z * bc.y + clip[2].z * bc.z;


                    //интерполяция нормалей
                    Vec3 N = normalize
                    (
                        normalV[0] * bc.x +
                        normalV[1] * bc.y +
                        normalV[2] * bc.z
                    );

                    //интерполяция позиций 
                    Vec3 Pworld =
                        world[0] * bc.x +
                        world[1] * bc.y +
                        world[2] * bc.z;

                    //интерполяция координат текстур
                    Vec2 uvP =
                        uv[0] * bc.x +
                        uv[1] * bc.y +
                        uv[2] * bc.z;


                    //получение цвета пикселя из текстуры
                    unsigned char* texel = texture.sample(uvP.x, uvP.y);

                    //свет
                    Color c = phongTexture(N, Pworld, texel);

                    //fb.clear();
                    fb.setPixel(x, y, c, z);
                }
            }
        }
    }


    fb.savePPM("Image.ppm");
    std::cout << "Saved image.ppm\n";

    texture.free();
}