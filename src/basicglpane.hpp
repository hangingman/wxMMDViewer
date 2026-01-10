/* wxMMDViewer - VMD file or PMD file viewer with wxWidgets and OpenGL
 * Copyright (C) 2014 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *      Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

#ifndef BASICGLPANE_HPP_
#define BASICGLPANE_HPP_

#include <GL/glew.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <GL/glut.h>
class pmd_t;
#include <vector>

// OpenGL view data
struct GLData
{
     bool initialized;           // have OpenGL been initialized?
     float beginx, beginy;       // position of mouse
     float quat[4];              // orientation of object
     float zoom;                 // field of view in degrees
     float xangle;               // model's angle X
     float yangle;               // model's angle Y
     float rotate_x;             // camera's x
};

enum class VboIndex {
    VERTEX = 0,
    ALPHA = 1,
    DIFFUSE_COLOR = 2,
    AMBIENT_COLOR = 3,
    SPECULAR_COLOR = 4,
    NORMAL = 5,
    SHININESS = 6,
    EDGE = 7,
    TEXTURE_COORD = 8,
    TEXTURE_LAYER = 9,
    SPHERE_MODE = 10,
    MAX_COUNT = 11
};

class BasicGLPane : public wxGLCanvas
{
     wxGLContext* m_context;

public:
     BasicGLPane(wxFrame* parent, int* args);
     virtual ~BasicGLPane();

     void SetPMDData(pmd_t* pmd);
     // events
     void KeyPressed(wxKeyEvent& event);
     void KeyReleased(wxKeyEvent& event);

private:
     void Resized(wxSizeEvent& evt);
     int GetWidth();
     int GetHeight();
     void Render(wxPaintEvent& evt);
     void InitShader();
     void Cleanup();

     // events
     void MouseMoved(wxMouseEvent& event);
     void MouseDown(wxMouseEvent& event);
     void MouseWheelMoved(wxMouseEvent& event);
     void MouseReleased(wxMouseEvent& event);
     void RightClick(wxMouseEvent& event);
     void MouseLeftWindow(wxMouseEvent& event);
     void Spin(float xSpin, float ySpin);

     // true: PMDファイル投入済、false: PMDファイルなし
     bool usePMDFile;

     // OpenGL Initialization state
     bool m_glInitialized;

     // VBO/VAO handles
     GLuint vao;
     GLuint vbo[static_cast<int>(VboIndex::MAX_COUNT)];
     GLuint vboi; // Index buffer
     GLsizei indicesCount;

     // Shader handles
     GLuint vertShaderObj;
     GLuint fragShaderObj;
     GLuint shaderProgram;
     
     // Attribute locations
     GLint attribVertex;
     GLint attribTexcoord;

     // PMD Attributes (Expanded for per-vertex)
     std::vector<float> m_vertices;
     std::vector<float> m_info_alpha;
     std::vector<float> m_info_diffuse;
     std::vector<float> m_info_ambient;
     std::vector<float> m_info_specular;
     std::vector<float> m_normals;
     std::vector<float> m_info_shininess;
     std::vector<float> m_info_edge;
     std::vector<float> m_info_uv;
     std::vector<float> m_info_tex_layer;
     std::vector<float> m_info_sphere_mode;
     std::vector<uint16_t> m_indices;

     // モデルの描画状態
     GLData      m_gldata;

     DECLARE_EVENT_TABLE()
};

#endif /** BASICGLPANE_HPP_ */
