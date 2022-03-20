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

#include "basicglpane.hpp"

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

// some useful events to use
void BasicGLPane::MouseDown(wxMouseEvent& event) {}
void BasicGLPane::MouseMoved(wxMouseEvent& event) {}
void BasicGLPane::MouseReleased(wxMouseEvent& event) {}
void BasicGLPane::RightClick(wxMouseEvent& event) {}
void BasicGLPane::MouseLeftWindow(wxMouseEvent& event) {}
void BasicGLPane::KeyReleased(wxKeyEvent& event) {}

void idle(void)
{
     glutPostRedisplay();
}

BasicGLPane::BasicGLPane(wxFrame* parent, int* args) :
     wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
     m_context = new wxGLContext(this);

     // To avoid flashing on MSW
     SetBackgroundStyle(wxBG_STYLE_CUSTOM);
     // set glData
     m_gldata.xangle = 30.0;
     m_gldata.yangle = 30.0;
     m_gldata.rotate_x = 0.5;
}

BasicGLPane::~BasicGLPane()
{
     delete m_context;
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

void BasicGLPane::Render( wxPaintEvent& evt )
{
     if(!IsShown()) return;

     wxGLCanvas::SetCurrent(*m_context);
     wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

     static GLfloat rot = 0.0;
     glClear(GL_COLOR_BUFFER_BIT);

     //glLoadIdentity();

     glPushMatrix();
     glOrtho(-7.0f, 7.0f, -11.6f, 11.6f, 5.0f, 20.0f);
     glScalef(m_gldata.rotate_x, m_gldata.rotate_x, 1.0f );
     glRotatef(m_gldata.xangle, 1.0f, 0.0f, 0.0f);
     glRotatef(m_gldata.yangle, 0.0f, 1.0f, 0.0f);

     glClearColor (0.0, 0.0, 0.4, 0.0);
     glFrontFace(GL_CW);
     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
     //glMatrixMode(GL_PROJECTION);
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);

     /** draw 3d model here */

       /**
     if (usePMDFile)
     {
	  GLfloat vert[m_pmdFile.m_vertexs.size() * 3];
	  GLfloat norm[m_pmdFile.m_vertexs.size() * 3];
	  GLfloat colors[m_pmdFile.m_vertexs.size() * 2];
	  GLushort indices[m_pmdFile.m_indexs.size()];

	  int index = 0;

          for (auto it = m_pmdFile.m_vertexs.begin(); it != m_pmdFile.m_vertexs.end(); ++it, index=index+3)
          {
               vert[index]   = it->x;
               vert[index+1] = it->y;
               vert[index+2] = - (it->z);

               norm[index]   = it->nx;
               norm[index+1] = it->ny;
               norm[index+2] = it->nz;
          }

	  index = 0;
	  for (auto it = m_pmdFile.m_vertexs.begin(); it != m_pmdFile.m_vertexs.end(); ++it, index=index+2)
	  {
	       colors[index] = it->tx;
	       colors[index+1] = it->ty;
	  }

          index = 0;
          for (auto it = m_pmdFile.m_indexs.begin(); it != m_pmdFile.m_indexs.end(); ++it, ++index)
          {
               indices[index] = *it;
          }

	  glEnableClientState(GL_NORMAL_ARRAY);
	  glEnableClientState(GL_COLOR_ARRAY);
	  glEnableClientState(GL_VERTEX_ARRAY);

	  glNormalPointer(GL_FLOAT, 0, norm);
	  glColorPointer(3, GL_FLOAT, 0, colors);
	  glVertexPointer(3, GL_FLOAT, 0, vert);

	  glDrawElements(GL_TRIANGLES, m_pmdFile.m_vertexs.size(), GL_UNSIGNED_SHORT, indices);

	  glDisableClientState(GL_VERTEX_ARRAY);
	  glDisableClientState(GL_COLOR_ARRAY);
	  glDisableClientState(GL_NORMAL_ARRAY);

	  glPopMatrix();

	  glFlush();
	  SwapBuffers();
     }
     else
     {
	  GLfloat vertices2[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
				  1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right)
				  1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top)
				  -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left)
				  -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom)
				  1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 }; // v4,v7,v6,v5 (back)

	  // normal array
	  GLfloat normals2[]  = { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
				  1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
				  0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
				  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
				  0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
				  0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)

	  // color array
	  GLfloat colors2[]   = { 1, 1, 1,   1, 1, 0,   1, 0, 0,   1, 0, 1,   // v0,v1,v2,v3 (front)
				  1, 1, 1,   1, 0, 1,   0, 0, 1,   0, 1, 1,   // v0,v3,v4,v5 (right)
				  1, 1, 1,   0, 1, 1,   0, 1, 0,   1, 1, 0,   // v0,v5,v6,v1 (top)
				  1, 1, 0,   0, 1, 0,   0, 0, 0,   1, 0, 0,   // v1,v6,v7,v2 (left)
				  0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,   // v7,v4,v3,v2 (bottom)
				  0, 0, 1,   0, 0, 0,   0, 1, 0,   0, 1, 1 }; // v4,v7,v6,v5 (back)

	  // index array of vertex array for glDrawElements() & glDrawRangeElement()
	  GLubyte indices[]  = { 0, 1, 2,   2, 3, 0,      // front
				 4, 5, 6,   6, 7, 4,      // right
				 8, 9,10,  10,11, 8,      // top
				 12,13,14,  14,15,12,      // left
				 16,17,18,  18,19,16,      // bottom
				 20,21,22,  22,23,20 };    // back

	  // enable and specify pointers to vertex arrays
	  glEnableClientState(GL_NORMAL_ARRAY);
	  glEnableClientState(GL_COLOR_ARRAY);
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glNormalPointer(GL_FLOAT, 0, normals2);
	  glColorPointer(3, GL_FLOAT, 0, colors2);
	  glVertexPointer(3, GL_FLOAT, 0, vertices2);

	  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	  glDisableClientState(GL_VERTEX_ARRAY);
	  glDisableClientState(GL_COLOR_ARRAY);
	  glDisableClientState(GL_NORMAL_ARRAY);
	  glPopMatrix();

	  glFlush();
	  SwapBuffers();
	  }*/
}

//void BasicGLPane::SetPMDFile(const clsPMDFile& pmdFile)
//{
//     m_pmdFile = pmdFile;
//     usePMDFile = true;
//
//     for (auto it = m_pmdFile.m_vertexs.begin(); it != m_pmdFile.m_vertexs.end(); ++it)
//     {/**
// 	  wxLogMessage(wxT("x:%f, y:%f, z:%f, nx:%f, ny:%f, nz:%f, tx:%f, ty:%f"),
// 		       it->x, it->y, it->z, it->nx, it->ny, it->nz, it->tx, it->ty);
//      */}
//
//     // vector<int>::iterator it = array.begin();
//     for (PMD_INDEX_CHUNK::iterator it = m_pmdFile.m_indexs.begin(); it != m_pmdFile.m_indexs.end(); ++it)
//     {
// 	  //int* p = it;
// 	  short unsigned int w = *it;
// 	  //wxLogMessage(wxT("index:%hu"), &w);
//     }
//
//     Refresh();
//}
//
//clsPMDFile& BasicGLPane::GetPMDFile()
//{
//     return m_pmdFile;
//}

void BasicGLPane::Spin(float xSpin, float ySpin)
{
     m_gldata.xangle += xSpin;
     m_gldata.yangle += ySpin;

     Refresh(false);
}

void BasicGLPane::KeyPressed(wxKeyEvent& event)
{
     float angle = 5.0;
     //wxLogDebug(wxT("Key pressed."));

     switch ( event.GetKeyCode() )
     {
     case WXK_RIGHT:
	  Spin( 0.0, -angle );
	  break;

     case WXK_LEFT:
	  Spin( 0.0, angle );
	  break;

     case WXK_DOWN:
	  Spin( -angle, 0.0 );
	  break;

     case WXK_UP:
	  Spin( angle, 0.0 );
	  break;

     default:
	  event.Skip();
	  return;
     }
}

void BasicGLPane::MouseWheelMoved(wxMouseEvent& event)
{
     if (event.m_wheelRotation > 0) {
	  // +
	  m_gldata.rotate_x -= .05;
     } else {
	  // -
	  m_gldata.rotate_x += .05;
     }

     event.Skip();
}
