from conans import ConanFile


class CppGUI(ConanFile):
    build_requires = "gpcbin2c/0.1.1@jpgygax68/testing"
    generators = "virtualbuildenv"

    #def build_requirements(self):
    #    # useful for example for conditional build_requires
    #    if self.settings.os == "Windows":
    #        self.build_requires("ToolWin/0.1@user/stable")