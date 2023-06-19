#pragma once
#include <GL/glew.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

class Shader {
public:
  Shader(std::string_view filepath);
  ~Shader();

  void bind() const;
  void unbind() const;

  void set_uniform4f(const std::string &name, GLfloat v0, GLfloat v1,
                     GLfloat v2, GLfloat v3) const;
  void set_uniform_vec4f(const std::string &name, glm::vec4 v) const;
  void set_uniform_mat4f(const std::string &name, glm::mat4 m) const;
  void set_uniform_3i(const std::string &name, GLint v[3]) const;

private:
  GLuint get_location(const std::string &name) const;
  mutable std::unordered_map<std::string, GLint> m_location_cache;
  GLuint m_ID;
};