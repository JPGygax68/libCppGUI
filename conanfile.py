from conans import ConanFile, CMake


class CppGUI(ConanFile):
    requires = "gpcbin2c/0.1.1@jpgygax68/testing", "boost/1.62.0@lasote/vcpkg"
    settings = "os", "compiler", "build_type", "arch"

    generators = "cmake", "virtualenv"

    #def build_requirements(self):
    #    # useful for example for conditional build_requires
    #    if self.settings.os == "Windows":
    #        self.build_requires("ToolWin/0.1@user/stable")
    
    #def configure(self):
    #    cmake = CMake(self)
    #    cmake.configure()