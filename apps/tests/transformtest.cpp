#include <rendercomp/core/math/Transform.h>

#include <iostream>

int main(int, char**)
{
    using namespace rendercomp;

    Transform transform;

    std::cout << "Forward: " << transform.forward() << std::endl;
    std::cout << "Up: " << transform.up() << std::endl;
    std::cout << "Right: " << transform.right() << std::endl;
    std::cout << std::endl;

    transform.rotateY(90.f);

    std::cout << "Forward: " << transform.forward() << std::endl;
    std::cout << "Up: " << transform.up() << std::endl;
    std::cout << "Right: " << transform.right() << std::endl;
    std::cout << std::endl;

    //transform.rotateX(90.f);
    transform.translate({100.f, 0.f, 0.f});

    Vec4f zVector (0.f, 0.f, 1.f, 0.f);
    std::cout << (transform.toMatrix() * zVector) << std::endl;

    Vec4f pos (0.f, 0.f, 0.f, 1.f);
    std::cout << (transform.toMatrix() * pos) << std::endl;

    std::cout << "Translate to x = 100, then rotate 90 around Y\n" <<
        (glm::toMat4(Quaterion(Vec3f(0.f, glm::radians(90.f), 0.f))) * glm::translate(Vec3f(100.f, 0.f, 0.f)) * pos) << std::endl;

    std::cout << "Rotate 90 around Y, then translate to x = 100\n" <<
        (glm::translate(Vec3f(100.f, 0.f, 0.f)) * glm::toMat4(Quaterion(Vec3f(0.f, glm::radians(90.f), 0.f))) * pos) << std::endl;

    Vec3f maxX (100.f, 0.f, 0.f);
    Vec3f maxY (0.f, 100.f, 0.f);

    std::cout << "MIN " << glm::min(maxX, maxY) << std::endl;
    std::cout << "MAX " << glm::max(maxX, maxY) << std::endl;


    return 0;
}
