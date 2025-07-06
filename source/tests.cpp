#define CATCH_CONFIG_RUNNER
#define _USE_MATH_DEFINES

#include <catch.hpp>
#include <cmath>
#include <iostream>
#include <glm/gtx/intersect.hpp>
#include <glm/vec3.hpp>

#include "Box.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Box.hpp"

// ------------------ SPHERE TESTS ------------------

TEST_CASE("Sphere Surface Area", "[Sphere][Area]") {
    SECTION("Standard radius") {
        Sphere s("Standard", glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), 2.0);
        REQUIRE(s.area() == Approx(4 * M_PI * 4.0).margin(1e-6));
    }

    SECTION("Very small radius") {
        Sphere s("Tiny", glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), 0.0001);
        REQUIRE(s.area() == Approx(4 * M_PI * 0.00000001).margin(1e-10));
    }

    SECTION("Very large radius") {
        Sphere s("Huge", glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), 10000.0);
        REQUIRE(s.area() == Approx(4 * M_PI * 100000000.0).margin(1e-3));
    }
}

TEST_CASE("Sphere Volume", "[Sphere][Volume]") {
    SECTION("Standard radius") {
        Sphere s("Standard", glm::vec3(1, 0.5, 0), glm::vec3(0, 0, 0), 2.0);
        REQUIRE(s.volume() == Approx((4.0 / 3.0) * M_PI * 8.0).margin(1e-6));
    }

    SECTION("Minimal positive radius") {
        Sphere s("TinyVol", glm::vec3(0.3, 0.3, 0.3), glm::vec3(0, 0, 0), 0.0001);
        REQUIRE(s.volume() == Approx((4.0 / 3.0) * M_PI * 1e-12).margin(1e-10));
    }

    SECTION("Huge radius value") {
        Sphere s("HugeVol", glm::vec3(0.1, 0.1, 0.9), glm::vec3(0, 0, 0), 10000.0);
        REQUIRE(s.volume() == Approx((4.0 / 3.0) * M_PI * 1e12).margin(1e3));
    }
}

// ------------------ BOX TESTS ------------------

TEST_CASE("Box Surface Area", "[Box][Area]") {
    SECTION("Unit cube") {
        Box b("UnitBox", glm::vec3(1, 1, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        REQUIRE(b.area() == Approx(6.0).margin(1e-6));
    }

    SECTION("Non-uniform dimensions") {
        Box b("NonUniformBox", glm::vec3(0.5, 0.2, 0.8), glm::vec3(0, 0, 0), glm::vec3(2, 3, 4));
        REQUIRE(b.area() == Approx(2 * (2 * 3 + 3 * 4 + 4 * 2)).margin(1e-6));
    }

    SECTION("Flat box") {
        Box b("FlatBox", glm::vec3(0.2, 0.2, 0.2), glm::vec3(0, 0, 0), glm::vec3(5, 5, 0));
        REQUIRE(b.area() == Approx(2 * (25 + 0 + 0)).margin(1e-6));
    }
}

TEST_CASE("Box Volume", "[Box][Volume]") {
    SECTION("Unit cube") {
        Box b("UnitVol", glm::vec3(0, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        REQUIRE(b.volume() == Approx(1.0).margin(1e-6));
    }

    SECTION("Non-uniform size") {
        Box b("NonUniformVol", glm::vec3(0.7, 0.3, 0.5), glm::vec3(1, 2, 3), glm::vec3(4, 6, 7));
        REQUIRE(b.volume() == Approx(3 * 4 * 4).margin(1e-6));
    }

    SECTION("Zero volume (flat)") {
        Box b("FlatVol", glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
        REQUIRE(b.volume() == Approx(0.0).margin(1e-6));

        std::cout << b;
    }
}

// ------------------ RAY HITS BOX TESTS ------------------

void testRayHitsBox() {
    Box box("UniqueBox", glm::vec3(0, 1, 0), glm::vec3(2, 2, 2), glm::vec3(4, 4, 4));

    Ray ray;
    ray.origin = glm::vec3(1, 3, 3);
    ray.direction = glm::normalize(glm::vec3(1, 0, 0)); 
    HitPoint hp = box.intersect(ray);
    if (hp.hit) {
        std::cout << "Hit detected at distance: " << hp.distance << "\n";
        std::cout << "Hit position: (" << hp.position.x << ", " << hp.position.y << ", " << hp.position.z << ")\n";
    }
    else {
        std::cout << "No hit detected (expected hit)\n";
    }
}

void testRayMissesBox() {
    Box box("UniqueBox", glm::vec3(0, 1, 0), glm::vec3(2, 2, 2), glm::vec3(4, 4, 4));

    Ray ray;
    ray.origin = glm::vec3(1, 5, 3);
    ray.direction = glm::normalize(glm::vec3(1, 0, 0)); 

    HitPoint hp = box.intersect(ray);
    if (!hp.hit) {
        std::cout << "Correctly detected no hit\n";
    }
    else {
        std::cout << "Unexpected hit detected\n";
    }
}

void testRayStartsInsideBox() {
    Box box("UniqueBox", glm::vec3(0, 1, 0), glm::vec3(2, 2, 2), glm::vec3(4, 4, 4));

    Ray ray;
    ray.origin = glm::vec3(3, 3, 3);
    ray.direction = glm::normalize(glm::vec3(0, 1, 0)); 

    HitPoint hp = box.intersect(ray);
    if (hp.hit && hp.distance >= 0) {
        std::cout << "Hit from inside detected at distance: " << hp.distance << "\n";
    }
    else {
        std::cout << "No hit detected (expected hit from inside)\n";
    }
}


TEST_CASE("intersect_ray_sphere", "[intersect]")
{
    // Ray
    glm::vec3 ray_origin{ 0.0f, 0.0f, 0.0f };
    // ray direction has to be normalized !
    // you can use:
    // v = glm::normalize(some_vector)
    glm::vec3 ray_direction{ 0.0f, 0.0f, 1.0f };
    // Sphere
    glm::vec3 sphere_center{ 0.0f ,0.0f, 5.0f };
    float sphere_radius{ 1.0f };
    float distance = 0.0f;
    auto result = glm::intersectRaySphere(
        ray_origin, ray_direction,
        sphere_center,
        sphere_radius * sphere_radius, // squared radius !!!
        distance);
    REQUIRE(distance == Approx(4.0f));
}


int main(int argc, char* argv[])
{
    return Catch::Session().run(argc, argv);
}
