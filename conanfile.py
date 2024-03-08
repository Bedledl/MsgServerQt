from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake

class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("qt/6.6.2")
        self.requires("libconfig/1.7.2")


    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")
        self.test_requires("gtest/1.14.0")
        self.tool_requires("make/4.3")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
