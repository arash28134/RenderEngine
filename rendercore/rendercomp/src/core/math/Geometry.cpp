/*
 * This file is part of the RenderEngine distribution (https://github.com/NadirRoGue/RenderEngine).
 * Copyright (c) 2021 Nadir Román Guerrero.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <rendercomp/core/math/Geometry.h>

namespace rendercomp
{

float Cotangent(const Vec3f& pivot, const Vec3f& a, const Vec3f& b)
{
    const Vec3f pa = glm::normalize(a - pivot);
    const Vec3f pb = glm::normalize(b - pivot);

    const float sinA = glm::length(glm::cross(pa, pb));
    const float cosA = glm::dot(pa, pb);

    return (cosA / sinA);
}

Vec3f VoronoiTriangleAreas(const Vec3f& A, const Vec3f& B, const Vec3f& C)
{
    float aArea, bArea, cArea;

    if (glm::dot(B - A, C - A) < 0.f)
    {
        const float triangleArea = glm::length(glm::cross(B - A, C - A)) * 0.5f;
        aArea = 0.5f * triangleArea;
        bArea = cArea = 0.25f * triangleArea;
    }
    else if (glm::dot(A - B, C - B) < 0)
    {
        const float triangleArea = glm::length(glm::cross(A - B, C - B)) * 0.5f;
        bArea = 0.5f * triangleArea;
        aArea = cArea = 0.25f * triangleArea;
    }
    else if (glm::dot(A - C, B - C) < 0)
    {
        const float triangleArea = glm::length(glm::cross(A - C, B - C)) * 0.5f;
        cArea = 0.5f * triangleArea;
        aArea = bArea = 0.25f * triangleArea;
    }
    else
    {
        const float AtoB = glm::length(B - A);
        const float AtoC = glm::length(C - A);
        const float BtoC = glm::length(C - B);

        float ctngA = Cotangent(A, B, C);
        float ctngB = Cotangent(B, A, C);
        float ctngC = Cotangent(C, A, B);

        aArea = 0.125f * ((AtoB*AtoB*ctngC) + (AtoC*AtoC*ctngB));
        bArea = 0.125f * ((AtoB*AtoB*ctngC) + (BtoC*BtoC*ctngA));
        cArea = 0.125f * ((AtoC*AtoC*ctngB) + (BtoC*BtoC*ctngA));
    }

    return Vec3f(aArea, bArea, cArea);
}


Vector<Vec3f> ComputeSmoothNormals(const Vector<Vec3ui>& indices, const Vector<Vec3f>& vertices)
{
    Vector<Vec3f> perfaceNormals (vertices.size(), Vec3f(0.f, 0.f, 0.f));
    Vector<float> voronoiArea (vertices.size(), 0.f);

    for (size_t i = 0; i < indices.size(); ++i)
    {
        const Vec3ui& face = indices[i];

        if(face.x >= vertices.size() || face.y >= vertices.size() || face.z >= vertices.size())
            throw std::runtime_error("ComputeSmoothNormals(): Face index out of vertex bounds");

        const Vec3f& A = vertices[face.x];
        const Vec3f& B = vertices[face.y];
        const Vec3f& C = vertices[face.z];

        const Vec3f faceNormal = glm::normalize(glm::cross(B - A, C - A));

        const Vec3f faceAreas = VoronoiTriangleAreas(A, B, C);

        perfaceNormals[face.x] += faceNormal * faceAreas.x;
        perfaceNormals[face.y] += faceNormal * faceAreas.y;
        perfaceNormals[face.z] += faceNormal * faceAreas.z;

        voronoiArea[face.x] += faceAreas.x;
        voronoiArea[face.y] += faceAreas.y;
        voronoiArea[face.z] += faceAreas.z;
    }

    Vector<Vec3f> normals (vertices.size(), Vec3f(0.f, 0.f, 0.f));
    for (size_t i = 0; i < vertices.size(); ++i)
        normals[i] = glm::normalize(perfaceNormals[i] / voronoiArea[i]);

    return normals;
}
}
