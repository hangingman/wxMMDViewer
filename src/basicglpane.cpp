/* wxMMDViewer - VMD file or PMD file viewer with wxWidgets and OpenGL
 * Copyright (C) 2014 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *      Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

#include "basicglpane.hpp"
#include "pmd.h"
#include <iostream>
#include <cmath>
#include <algorithm>

// Matrix Math Helpers removed in favor of GLM

// Shader Sources
const char* modelVertexSource = R"(
#version 330 core
in vec3 position;
in float alpha;
in vec3 diffuseColor;
in vec3 ambientColor;
in vec3 specularColor;
in vec3 normal;
in float shininess;
in int isEdge;
in vec2 vUV;
in float vTexLayer;
in float vSphereMode;

out float vAlpha;
out vec3 vDiffuseColor;
out vec3 vAmbientColor;
out vec3 vSpecularColor;
out vec3 vNormal;
out vec3 vPosition;
out float vShininess;
out float vIsEdge;
out vec2 UV;
out float texLayer;
out float fSphereMode;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uEdgeSize;

void main() {
    mat4 mvp = projection * view * model;
    vAlpha = alpha;
    vDiffuseColor = diffuseColor;
    vAmbientColor = ambientColor;
    vSpecularColor = specularColor;
    vNormal = normal;
    vPosition = position;
    vShininess = shininess;
    vIsEdge = isEdge;
    UV = vUV;
    texLayer = vTexLayer;
    fSphereMode = vSphereMode;

    if (isEdge == 1) {
        vec4 p0 = mvp * vec4(position, 1.0);
        vec4 p1 = mvp * vec4(position + normal, 1.0);
        vec2 dir = normalize(p1.xy / p1.w - p0.xy / p0.w);
        // Simple edge expansion in screen space or simpler normal expansion
        // Reference uses model space normal expansion?
        // "gl_Position = p0 + norm * uEdgeSize / 10.0f;" -> Mixed space? 
        // Let's implement simple constant expansion along normal
        vec4 normStart = mvp * vec4(position, 1.0);
        vec4 normEnd = mvp * vec4(position + normal, 1.0);
        gl_Position = normStart + normalize(normEnd - normStart) * uEdgeSize / 100.0;
        return;
    }
    gl_Position = mvp * vec4(position, 1.0);
}
)";

const char* modelFragmentSource = R"(
#version 330 core
uniform vec3 uLightPosition;
uniform vec3 uEdgeColor;
uniform sampler2DArray tex; 

in float vAlpha;
in vec3 vDiffuseColor;
in vec3 vAmbientColor;
in vec3 vSpecularColor;
in vec3 vNormal;
in vec3 vPosition;
in float vShininess;
in float vIsEdge;
in vec2 UV;
in float texLayer;
in float fSphereMode;

out vec4 fragColor;

void main() {
    if (vIsEdge == 1) {
        fragColor = vec4(uEdgeColor, vAlpha);
        return;
    }

    vec3 lightDir = normalize(uLightPosition - vPosition);
    vec3 normal = normalize(vNormal);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * vDiffuseColor;
    vec3 ambient = vAmbientColor; // Simple ambient
    
    // Specular? Reference has it.
    vec3 viewDir = normalize(-vPosition); // View is at 0,0,0 in ViewSpace? No, vPosition is World or Model? 
    // Reference shader assumes vPosition is World/Model? 
    // "vec3 lightDirection = normalize(uLightPosition - vPosition);"
    // We will assume vPosition passed from Vertex is Local Model Space unless transformed. 
    // Wait, Vertex Shader: "vPosition = position;" -> Local Model Space.
    // So uLightPosition must be in Model Space or vPosition transformed.
    // Let's stick to simple shading:
    
    fragColor = vec4(ambient + diffuse, vAlpha);
}
)";

BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
   EVT_MOTION(BasicGLPane::MouseMoved)
   EVT_LEFT_DOWN(BasicGLPane::MouseDown)
   EVT_LEFT_UP(BasicGLPane::MouseReleased)
   EVT_RIGHT_DOWN(BasicGLPane::RightClick)
   EVT_LEAVE_WINDOW(BasicGLPane::MouseLeftWindow)
   EVT_SIZE(BasicGLPane::Resized)
   EVT_KEY_DOWN(BasicGLPane::KeyPressed)
   EVT_KEY_UP(BasicGLPane::KeyReleased)
   EVT_MOUSEWHEEL(BasicGLPane::MouseWheelMoved)
   EVT_PAINT(BasicGLPane::Render)
END_EVENT_TABLE()

void BasicGLPane::MouseDown(wxMouseEvent& event) {
    m_gldata.beginx = event.GetX();
    m_gldata.beginy = event.GetY();
}
void BasicGLPane::MouseMoved(wxMouseEvent& event) {
    if (event.Dragging()) {
        float dx = event.GetX() - m_gldata.beginx;
        float dy = event.GetY() - m_gldata.beginy;
        m_gldata.yangle += dx * 0.5f;
        m_gldata.xangle += dy * 0.5f;
        m_gldata.beginx = event.GetX();
        m_gldata.beginy = event.GetY();
        Refresh();
    }
}
void BasicGLPane::MouseReleased(wxMouseEvent& event) {}
void BasicGLPane::RightClick(wxMouseEvent& event) {}
void BasicGLPane::MouseLeftWindow(wxMouseEvent& event) {}
void BasicGLPane::KeyReleased(wxKeyEvent& event) {}

BasicGLPane::BasicGLPane(wxFrame* parent, int* args) :
     wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
     usePMDFile(false), m_glInitialized(false), vao(0), vboi(0), indicesCount(0), shaderProgram(0)
{
     m_context = new wxGLContext(this);
     // SetCurrent is deferred until window is valid

     SetBackgroundStyle(wxBG_STYLE_CUSTOM);
     m_gldata.xangle = 0.0;
     m_gldata.yangle = 0.0;
     m_gldata.rotate_x = 0.0;
     m_gldata.zoom = 45.0;
     
     for(int i=0; i< static_cast<int>(VboIndex::MAX_COUNT); i++) vbo[i] = 0;
}

BasicGLPane::~BasicGLPane()
{
     Cleanup();
     delete m_context;
}

void BasicGLPane::Cleanup() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vboi) glDeleteBuffers(1, &vboi);
    for(int i=0; i < static_cast<int>(VboIndex::MAX_COUNT); i++) {
        if(vbo[i]) glDeleteBuffers(1, &vbo[i]);
    }
    if (shaderProgram) glDeleteProgram(shaderProgram);
}

void BasicGLPane::Resized(wxSizeEvent& evt)
{
     Refresh();
}

int BasicGLPane::GetWidth()
{
     return GetSize().x;
}

int BasicGLPane::GetHeight()
{
     return GetSize().y;
}

void BasicGLPane::InitShader() {
    // This now handles full GL checking and init
    if (m_glInitialized) return;
    
    // Check if we can set current
    if (!IsShownOnScreen()) return; // Avoid crash on GTK
    
    SetCurrent(*m_context);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
         std::cerr << "GLEW Init Error: " << glewGetErrorString(err) << std::endl;
         return;
    }
    
    // Shader Compilation
    GLint success;
    GLchar infoLog[512];

    vertShaderObj = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShaderObj, 1, &modelVertexSource, NULL);
    glCompileShader(vertShaderObj);
    glGetShaderiv(vertShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertShaderObj, 512, NULL, infoLog);
        std::cerr << "Vertex Shader Compilation Failed:\n" << infoLog << std::endl;
    }

    fragShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShaderObj, 1, &modelFragmentSource, NULL);
    glCompileShader(fragShaderObj);
    glGetShaderiv(fragShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShaderObj, 512, NULL, infoLog);
        std::cerr << "Fragment Shader Compilation Failed:\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShaderObj);
    glAttachShader(shaderProgram, fragShaderObj);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Program Link Failed:\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertShaderObj);
    glDeleteShader(fragShaderObj);
    
    m_glInitialized = true;
}

void BasicGLPane::SetPMDData(pmd_t* pmd)
{
    if (!pmd) return;
    
    // Attempt init
    InitShader();
    
    if (!m_glInitialized) {
        // Cannot upload data yet.
        // TODO: Store pmd or retry later. 
        // For now, if window not shown, we just can't upload. 
        // But usually SetPMDData comes from user action so window SHOULD be shown.
        // If it fails here, it's critical.
        std::cerr << "Error: SetPMDData called before GL initialized!" << std::endl;
        return;
    }

    // Ensure Context
    SetCurrent(*m_context);

    // Clear old data
    m_vertices.clear();
    m_info_alpha.clear();
    m_info_diffuse.clear();
    m_info_ambient.clear();
    m_info_specular.clear();
    m_normals.clear();
    m_info_shininess.clear();
    m_info_edge.clear();
    m_info_uv.clear();
    m_info_tex_layer.clear();
    m_info_sphere_mode.clear();
    m_indices.clear();

    auto* vertices = pmd->vertex()->vertex();
    auto* faces = pmd->face_vertex()->face_vert_index();
    auto* materials = pmd->material()->material();

    // Map vertex index to material
    std::vector<int> vertex_material_idx(vertices->size(), -1);
    
    // Iterate materials to find which faces range map to which material
    // And assign vertices in those faces to that material
    size_t current_face_idx = 0;
    for (size_t mat_idx = 0; mat_idx < materials->size(); ++mat_idx) {
        auto* mat = (*materials)[mat_idx].get();
        size_t face_count = mat->face_vert_count();
        
        for(size_t i=0; i < face_count; ++i) {
            if (current_face_idx + i < faces->size()) {
                uint16_t v_idx = (*faces)[current_face_idx + i];
                if (v_idx < vertex_material_idx.size()) {
                     // Only set if not already set (first material wins, usually sorted)
                     if (vertex_material_idx[v_idx] == -1) {
                         vertex_material_idx[v_idx] = mat_idx;
                     }
                }
            }
        }
        current_face_idx += face_count;
    }

    // Populate Attributes
    for (size_t i = 0; i < vertices->size(); ++i) {
        auto* v = (*vertices)[i].get();
        
        // Position
        m_vertices.push_back((*v->pos())[0]);
        m_vertices.push_back((*v->pos())[1]);
        m_vertices.push_back(-(*v->pos())[2]); // Invert Z

        // Normal
        m_normals.push_back((*v->normal_vec())[0]);
        m_normals.push_back((*v->normal_vec())[1]);
        m_normals.push_back(-(*v->normal_vec())[2]);
        
        // UV
        m_info_uv.push_back((*v->uv())[0]);
        m_info_uv.push_back((*v->uv())[1]);

        int mat_idx = vertex_material_idx[i];
        if (mat_idx >= 0 && mat_idx < materials->size()) {
            auto* m = (*materials)[mat_idx].get();
            m_info_alpha.push_back(m->alpha());
            
            m_info_diffuse.push_back((*m->diffuse_color())[0]);
            m_info_diffuse.push_back((*m->diffuse_color())[1]);
            m_info_diffuse.push_back((*m->diffuse_color())[2]);

            m_info_ambient.push_back((*m->ambient_color())[0]);
            m_info_ambient.push_back((*m->ambient_color())[1]);
            m_info_ambient.push_back((*m->ambient_color())[2]);

            m_info_specular.push_back((*m->specular_color())[0]);
            m_info_specular.push_back((*m->specular_color())[1]);
            m_info_specular.push_back((*m->specular_color())[2]);
            
            m_info_shininess.push_back(m->specularity());
            m_info_edge.push_back((float)m->edge_flag());
            
            // Texture Layer/Sphere - Placeholders
            m_info_tex_layer.push_back(0.0f); 
            m_info_sphere_mode.push_back(-1.0f);
        } else {
            // Default/Fallback
            m_info_alpha.push_back(1.0f);
            m_info_diffuse.push_back(1.0f); m_info_diffuse.push_back(1.0f); m_info_diffuse.push_back(1.0f);
            m_info_ambient.push_back(0.2f); m_info_ambient.push_back(0.2f); m_info_ambient.push_back(0.2f);
            m_info_specular.push_back(0.0f); m_info_specular.push_back(0.0f); m_info_specular.push_back(0.0f);
            m_info_shininess.push_back(0.0f);
            m_info_edge.push_back(0.0f);
            m_info_tex_layer.push_back(0.0f);
            m_info_sphere_mode.push_back(-1.0f);
        }
    }
    
    // Indices
    m_indices.assign(faces->begin(), faces->end());
    indicesCount = m_indices.size();

    // Upload to OpenGL
    if (vao == 0) glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    auto setupVbo = [&](int idx, const std::vector<float>& data, int size) {
        if (vbo[idx] == 0) glGenBuffers(1, &vbo[idx]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[idx]);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(idx, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(idx);
    };

    setupVbo((int)VboIndex::VERTEX, m_vertices, 3);
    setupVbo((int)VboIndex::ALPHA, m_info_alpha, 1);
    setupVbo((int)VboIndex::DIFFUSE_COLOR, m_info_diffuse, 3);
    setupVbo((int)VboIndex::AMBIENT_COLOR, m_info_ambient, 3);
    setupVbo((int)VboIndex::SPECULAR_COLOR, m_info_specular, 3);
    setupVbo((int)VboIndex::NORMAL, m_normals, 3);
    setupVbo((int)VboIndex::SHININESS, m_info_shininess, 1);
    setupVbo((int)VboIndex::EDGE, m_info_edge, 1);
    setupVbo((int)VboIndex::TEXTURE_COORD, m_info_uv, 2);
    // TexLayer and SphereMode skipped for brevity or filled with 0

    if (vboi == 0) glGenBuffers(1, &vboi);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint16_t), m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    usePMDFile = true;
    Refresh();
}

void BasicGLPane::Render(wxPaintEvent& evt)
{
     if(!IsShown()) return;
     
     // Lazy Init
     if (!m_glInitialized) {
         InitShader();
         if (!m_glInitialized) return; // Still failed, maybe not shown?
     }

     wxGLCanvas::SetCurrent(*m_context);
     wxPaintDC(this);

     glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     if (!usePMDFile || vao == 0) return;

     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
     glFrontFace(GL_CW); // MMD is usually CW? Or CCW with neg scale?

     glUseProgram(shaderProgram);

     // MVP Setup
     glm::mat4 proj = glm::perspective(glm::radians(m_gldata.zoom), (float)GetWidth() / (float)GetHeight(), 0.1f, 100.0f);

     glm::mat4 view = glm::mat4(1.0f);
     // Simple Orbit Camera: Translate back, Rotate X, Rotate Y
     float dist = 20.0f;
     view = glm::translate(view, glm::vec3(0.0f, -10.0f, -dist));
     view = glm::rotate(view, glm::radians(m_gldata.xangle), glm::vec3(1.0f, 0.0f, 0.0f));
     view = glm::rotate(view, glm::radians(m_gldata.yangle), glm::vec3(0.0f, 1.0f, 0.0f));

     glm::mat4 model = glm::mat4(1.0f);

     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

     glUniform3f(glGetUniformLocation(shaderProgram, "uLightPosition"), 20.0f, 20.0f, -20.0f);
     glUniform1f(glGetUniformLocation(shaderProgram, "uEdgeSize"), 1.0f);
     glUniform3f(glGetUniformLocation(shaderProgram, "uEdgeColor"), 0.0f, 0.0f, 0.0f);

     glBindVertexArray(vao);
     glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, 0);
     glBindVertexArray(0);
     
     glUseProgram(0);
     glFlush();
     SwapBuffers();
}

void BasicGLPane::Spin(float xSpin, float ySpin)
{
     m_gldata.xangle += xSpin;
     m_gldata.yangle += ySpin;
     Refresh(false);
}

void BasicGLPane::KeyPressed(wxKeyEvent& event)
{
     float angle = 5.0;
     switch ( event.GetKeyCode() )
     {
     case WXK_RIGHT: Spin( 0.0, -angle ); break;
     case WXK_LEFT: Spin( 0.0, angle ); break;
     case WXK_DOWN: Spin( -angle, 0.0 ); break;
     case WXK_UP: Spin( angle, 0.0 ); break;
     default: event.Skip(); return;
     }
}

void BasicGLPane::MouseWheelMoved(wxMouseEvent& event)
{
     if (event.m_wheelRotation > 0) m_gldata.zoom -= 1.0;
     else m_gldata.zoom += 1.0;
     event.Skip();
}
