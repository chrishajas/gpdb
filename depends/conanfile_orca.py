from conans import ConanFile, CMake

class OrcaConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   with open('../ORCA_VERSION', 'r') as version_file:
     orca_version=version_file.read().strip()
   requires = "orca/v%s@gpdb/stable" % orca_version

   def imports(self):
      self.copy("*", dst="build/include", src="include") # From include to build/include
      self.copy("*", dst="build/lib", src="lib") # From lib to build/lib
