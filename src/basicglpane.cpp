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
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

#include "basicglpane.hpp"

BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
   EVT_MOTION(BasicGLPane::mouseMoved)
   EVT_LEFT_DOWN(BasicGLPane::mouseDown)
   EVT_LEFT_UP(BasicGLPane::mouseReleased)
   EVT_RIGHT_DOWN(BasicGLPane::rightClick)
   EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
   EVT_SIZE(BasicGLPane::resized)
   EVT_KEY_DOWN(BasicGLPane::keyPressed)
   EVT_KEY_UP(BasicGLPane::keyReleased)
   EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
   EVT_PAINT(BasicGLPane::render)
END_EVENT_TABLE() 
 
// some useful events to use
void BasicGLPane::mouseMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseDown(wxMouseEvent& event) {}
void BasicGLPane::mouseWheelMoved(wxMouseEvent& event) {}
void BasicGLPane::mouseReleased(wxMouseEvent& event) {}
void BasicGLPane::rightClick(wxMouseEvent& event) {}
void BasicGLPane::mouseLeftWindow(wxMouseEvent& event) {}
void BasicGLPane::keyPressed(wxKeyEvent& event) {}
void BasicGLPane::keyReleased(wxKeyEvent& event) {}
 
// Vertices and faces of a simple cube to demonstrate 3D render
// source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
GLfloat v[8][3];
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
     {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
     {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
 
 
 
BasicGLPane::BasicGLPane(wxFrame* parent, int* args) :
     wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
     m_context = new wxGLContext(this);
     // prepare a simple cube to demonstrate 3D render
     // source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
     v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
     v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
     v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
     v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
     v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
     v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;    
 
     // To avoid flashing on MSW
     SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}
 
BasicGLPane::~BasicGLPane()
{
     delete m_context;
}
 
void BasicGLPane::resized(wxSizeEvent& evt)
{
     Refresh();
}
 
/** Inits the OpenGL viewport for drawing in 3D. */
void BasicGLPane::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
 
     glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
     glClearDepth(1.0f);	           // Depth Buffer Setup
     glEnable(GL_DEPTH_TEST);              // Enables Depth Testing
     glDepthFunc(GL_LEQUAL);               // The Type Of Depth Testing To Do
     glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 
     glEnable(GL_COLOR_MATERIAL);
 
     glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
 
     float ratio_w_h = (float)(bottomrigth_x-topleft_x)/(float)(bottomrigth_y-topleft_y);
     gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
 
}
 
int BasicGLPane::getWidth()
{
     return GetSize().x;
}
 
int BasicGLPane::getHeight()
{
     return GetSize().y;
} 
 
void BasicGLPane::render( wxPaintEvent& evt )
{
     if(!IsShown()) return;
 
     wxGLCanvas::SetCurrent(*m_context);
     wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
 
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     if (usePMDFile)
     {
	  // ------------- draw some 3D ----------------
	  prepare3DViewport(0,0,getWidth(), getHeight());
	  glLoadIdentity();
 
	  glColor4f(0,0,1,1);
	  glTranslatef(0,0,-5);
	  glRotatef(50.0f, 0.0f, 1.0f, 0.0f);
 
	  /** 頂点情報を描画する */
	  PMD_VERTEX_CHUNK pmdVertexChunk = m_pmdFile.GetVertexChunk();
	  if ( !pmdVertexChunk.empty() )
	  {
	       glColor4f(1, 0, 0, 1);

	       std::vector<PMD_VERTEX_RECORD>::const_iterator it = pmdVertexChunk.begin();
	       GLubyte indices[pmdVertexChunk.size()];
	       GLfloat vert[pmdVertexChunk.size()][6];
	       int index = 0;

	       for (auto it = pmdVertexChunk.begin(); it != pmdVertexChunk.end(); ++it, ++index)
	       {
		    indices[index] = index;

		    vert[index][0] = it->x;
		    vert[index][1] = it->y;
		    vert[index][2] = it->z;
		    vert[index][3] = it->nx;
		    vert[index][4] = it->ny;
		    vert[index][5] = it->nz;

		    wxLogMessage(wxT("x:%f, y:%f, z:%f, nx:%f, ny:%f, nz:%f, tx:%f, ty:%f"), 
			         it->x, it->y, it->z, it->nx, it->ny, it->nz, it->tx, it->ty);
	       }

	       // 頂点情報の数
	       glClear(GL_COLOR_BUFFER_BIT);
	       glEnableClientState(GL_VERTEX_ARRAY);
	       // 3次元, float型, オフセットなし, 配列へのポインタ
	       glVertexPointer(3 , GL_FLOAT , 0 , vert);
	       // 線を引く, 配列のサイズ, float型, 
	       glDrawElements(GL_LINE_LOOP , 3 , GL_UNSIGNED_BYTE , indices);
	  }
     }
     else
     {
	  // red square
	  glColor4f(1, 0, 0, 1);
	  glBegin(GL_QUADS);
	  glVertex3f(getWidth()/8, getHeight()/3, 0);
	  glVertex3f(getWidth()*3/8, getHeight()/3, 0);
	  glVertex3f(getWidth()*3/8, getHeight()*2/3, 0);
	  glVertex3f(getWidth()/8, getHeight()*2/3, 0);
	  glEnd();
 
	  // ------------- draw some 3D ----------------
	  prepare3DViewport(0,0,getWidth(), getHeight());
	  glLoadIdentity();
 
	  glColor4f(0,0,1,1);
	  glTranslatef(0,0,-5);
	  glRotatef(50.0f, 0.0f, 1.0f, 0.0f);
 
	  glColor4f(1, 0, 0, 1);
	  for (int i = 0; i < 6; i++)
	  {
	       glBegin(GL_LINE_STRIP);
	       glVertex3fv(&v[faces[i][0]][0]);
	       glVertex3fv(&v[faces[i][1]][0]);
	       glVertex3fv(&v[faces[i][2]][0]);
	       glVertex3fv(&v[faces[i][3]][0]);
	       glVertex3fv(&v[faces[i][0]][0]);
	       glEnd();
	  }
     }
 
     glFlush();
     SwapBuffers();
}

void BasicGLPane::SetPMDFile(const clsPMDFile& pmdFile)
{
     m_pmdFile = pmdFile;
     usePMDFile = true;

     Refresh();
}

clsPMDFile& BasicGLPane::GetPMDFile()
{
     return m_pmdFile;
}

