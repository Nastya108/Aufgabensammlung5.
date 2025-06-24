#define CATCH_CONFIG_RUNNER
#define _USE_MATH_DEFINES

#include <catch.hpp>
#include <cmath>
#include <iostream>

#include "Sphere.hpp"
#include "Box.hpp"

// ------------------ SPHERE TESTS ------------------

TEST_CASE("Sphere Surface Area", "[Sphere][Area]") {
    SECTION("Standard radius") {
        Sphere s(glm::vec3(0, 0, 0), 2.0);
        REQUIRE(s.area() == Approx(4 * M_PI * 4.0).margin(1e-6));
    }

    SECTION("Very small radius") {
        Sphere s(glm::vec3(1, 1, 1), 0.0001);
        REQUIRE(s.area() == Approx(4 * M_PI * 0.00000001).margin(1e-10));
    }

    SECTION("Very large radius") {
        Sphere s(glm::vec3(0, 0, 0), 10000.0);
        REQUIRE(s.area() == Approx(4 * M_PI * 100000000.0).margin(1e-3));
    }
}

TEST_CASE("Sphere Volume", "[Sphere][Volume]") {
    SECTION("Standard radius") {
        Sphere s(glm::vec3(0, 0, 0), 2.0);
        REQUIRE(s.volume() == Approx((4.0 / 3.0) * M_PI * 8.0).margin(1e-6));
    }

    SECTION("Minimal positive radius") {
        Sphere s(glm::vec3(0, 0, 0), 0.0001);
        REQUIRE(s.volume() == Approx((4.0 / 3.0) * M_PI * 1e-12).margin(1e-10));
    }

    SECTION("Huge radius value") {
        Sphere s(glm::vec3(0, 0, 0), 10000.0);
        REQUIRE(s.volume() == Approx((4.0 / 3.0) * M_PI * 1e12).margin(1e3));
    }
}

// ------------------ BOX TESTS ------------------

TEST_CASE("Box Surface Area", "[Box][Area]") {
    SECTION("Unit cube") {
        Box b(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        REQUIRE(b.area() == Approx(6.0).margin(1e-6));
    }

    SECTION("Non-uniform dimensions") {
        Box b(glm::vec3(0, 0, 0), glm::vec3(2, 3, 4));
        REQUIRE(b.area() == Approx(2 * (2 * 3 + 3 * 4 + 4 * 2)).margin(1e-6));
    }

    SECTION("Flat box") {
        Box b(glm::vec3(0, 0, 0), glm::vec3(5, 5, 0));
        REQUIRE(b.area() == Approx(2 * (25 + 0 + 0)).margin(1e-6));
    }
}

TEST_CASE("Box Volume", "[Box][Volume]") {
    SECTION("Unit cube") {
        Box b(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
        REQUIRE(b.volume() == Approx(1.0).margin(1e-6));
    }

    SECTION("Non-uniform size") {
        Box b(glm::vec3(1, 2, 3), glm::vec3(4, 6, 7));
        REQUIRE(b.volume() == Approx(3 * 4 * 4).margin(1e-6));
    }

    SECTION("Zero volume (flat)") {
        Box b(glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));
        REQUIRE(b.volume() == Approx(0.0).margin(1e-6));
    }
}

int main(int argc, char* argv[])
{
    return Catch::Session().run(argc, argv);
}
