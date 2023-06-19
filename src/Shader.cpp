#include "Shader.h"
#include <iostream>
#include <stdexcept>

Shader::Shader(std::string_view filepath) {
  std::ifstream file(filepath.data());
  if (!file.is_open())
    throw std::runtime_error("File does not exist");

  std::string line;
  std::stringstream ss[2];
  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
  ShaderType type = ShaderType::NONE;

  while (std::getline(file, line)) {
    if (line.find("shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    } else {
      ss[(int)type] << line << '\n';
    }
  }

  const std::string vert_str = ss[0].str();
  const char *vert_src = vert_str.c_str();

  const std::string frag_str = ss[1].str();
  const char *frag_src = frag_str.c_str();

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vert_src, nullptr);
  glCompileShader(vertex_shader);
  GLint compile_ok;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    GLsizei log_length = 0;
    GLchar message[1024];
    glGetShaderInfoLog(vertex_shader, 1024, &log_length, message);
    std::cerr << "Error in vertex shader: " << message << std::endl;
    return;
  }

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &frag_src, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    GLsizei log_length = 0;
    GLchar message[1024];
    glGetShaderInfoLog(fragment_shader, 1024, &log_length, message);
    std::cerr << "Error in fragment shader: " << message << std::endl;
    return;
  }

  this->m_ID = glCreateProgram();

  glAttachShader(m_ID, vertex_shader);
  glAttachShader(m_ID, fragment_shader);
  glLinkProgram(m_ID);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

Shader::~Shader() { glDeleteProgram(m_ID); }

void Shader::bind() const { glUseProgram(m_ID); }

void Shader::unbind() const { glUseProgram(0); }

GLuint Shader::get_location(const std::string &name) const {
  if (not m_location_cache.contains(name))
    m_location_cache[name] = glGetUniformLocation(m_ID, name.c_str());
  return m_location_cache[name];
}

void Shader::set_uniform4f(const std::string &name, GLfloat v0, GLfloat v1,
                           GLfloat v2, GLfloat v3) const {
  glUniform4f(get_location(name), v0, v1, v2, v3);
}

void Shader::set_uniform_vec4f(const std::string &name, glm::vec4 v) const {
  glUniform4fv(get_location(name), 1, glm::value_ptr(v));
}

void Shader::set_uniform_mat4f(const std::string &name, glm::mat4 m) const {
  glUniformMatrix4fv(get_location(name), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::set_uniform_3i(const std::string &name, GLint v[3]) const {
  glUniform3i(get_location(name), v[0], v[1], v[2]);
}
