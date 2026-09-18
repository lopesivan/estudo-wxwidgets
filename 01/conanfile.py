from conan import ConanFile
from conan.tools.cmake import cmake_layout

class WxWidgetsDemo(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("wxwidgets/3.2.8")

    def layout(self):
        cmake_layout(self)
