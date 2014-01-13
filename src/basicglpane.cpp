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
void BasicGLPane::MouseMoved(wxMouseEvent& event) {}
void BasicGLPane::MouseDown(wxMouseEvent& event) {}
void BasicGLPane::MouseWheelMoved(wxMouseEvent& event) {}
void BasicGLPane::MouseReleased(wxMouseEvent& event) {}
void BasicGLPane::RightClick(wxMouseEvent& event) {}
void BasicGLPane::MouseLeftWindow(wxMouseEvent& event) {}
void BasicGLPane::KeyPressed(wxKeyEvent& event) {}
void BasicGLPane::KeyReleased(wxKeyEvent& event) {}
 
// Vertices and faces of a simple cube to demonstrate 3D render
// source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
GLfloat v[8][3];
GLint faces[6][4] = { /* Vertex indices for the 6 faces of a cube. */
     {0, 1, 2, 3}, 
     {3, 2, 6, 7}, 
     {7, 6, 5, 4},
     {4, 5, 1, 0}, 
     {5, 6, 2, 1}, 
     {7, 4, 0, 3} 
};
 
void initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 20, 1}; // positional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
} 

void idle(void)
{
     glutPostRedisplay();
}

void reshape(int w, int h)
{
     glViewport (0, 0, (GLsizei) w, (GLsizei) h);
     glMatrixMode (GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D (-2.0, 2.0, -2.0, 2.0);
     glMatrixMode(GL_MODELVIEW);
}
 
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
     // initialize lights
     initLights();
     // set idle function
     //glutIdleFunc(idle);
     // set reshape function
     //glutReshapeFunc(reshape);
}
 
BasicGLPane::~BasicGLPane()
{
     delete m_context;
}
 
void BasicGLPane::Resized(wxSizeEvent& evt)
{
     Refresh();
}
 
/** Inits the OpenGL viewport for drawing in 3D. */
void BasicGLPane::Prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
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

#if 1
     /** テスト用 */
     

     static GLfloat rot = 0.0;
     glClear(GL_COLOR_BUFFER_BIT);
     glPushMatrix();
     glRotatef(rot, 1.0, 1.0, 1.0);
     rot += 4.0;

     glClearColor (0.0, 0.0, 0.4, 0.0);
     glFrontFace(GL_CW);
     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);

     glBegin (GL_TRIANGLE_FAN);
     glColor3f(0.0, 0.0, 1.0);  glVertex3f(0.0, 0.0, 1.0);
     glColor3f(1.0, 0.0, 0.0);  glVertex3f(1.0, 0.0, 0.0);
     glColor3f(0.0, 1.0, 0.0);  glVertex3f(0.0, 1.0, 0.0);
     glColor3f(0.0, 1.0, 1.0);  glVertex3f(-1.0, 0.0, 0.0);
     glColor3f(1.0, 0.0, 1.0);  glVertex3f(0.0, -1.0, 0.0);
     glColor3f(0.0, 1.0, 1.0);  glVertex3f(1.0, 0.0, 0.0);
     glEnd();

     glBegin (GL_TRIANGLE_FAN);
     glColor3f(1.0, 1.0, 0.0);  glVertex3f(0.0, 0.0, -1.0);
     glColor3f(1.0, 0.0, 0.0);  glVertex3f(1.0, 0.0, 0.0);
     glColor3f(1.0, 0.0, 1.0);  glVertex3f(0.0, -1.0, 0.0);
     glColor3f(0.0, 1.0, 1.0);  glVertex3f(-1.0, 0.0, 0.0);
     glColor3f(0.0, 1.0, 0.0);  glVertex3f(0.0, 1.0, 0.0);
     glColor3f(1.0, 0.0, 0.0);  glVertex3f(1.0, 0.0, 0.0);
     glEnd();

     glPopMatrix();
     
/**
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

     // 描画準備
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     Prepare3DViewport(0,0,GetWidth(), GetHeight());
     glLoadIdentity();
     
     glColor4f(0,0,1,1);
     glRotatef(50.0f, 0.0f, 1.0f, 0.0f);

     // enable and specify pointers to vertex arrays
     glEnableClientState(GL_NORMAL_ARRAY);
     glEnableClientState(GL_COLOR_ARRAY);
     glEnableClientState(GL_VERTEX_ARRAY);
     glNormalPointer(GL_FLOAT, 0, normals2);
     glColorPointer(3, GL_FLOAT, 0, colors2);
     glVertexPointer(3, GL_FLOAT, 0, vertices2);

     glPushMatrix();
     glTranslatef(-2, -2, 0);                // move to bottom-left corner
     glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
     glPopMatrix();

     glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
     glDisableClientState(GL_COLOR_ARRAY);
     glDisableClientState(GL_NORMAL_ARRAY);
*/

#else
     if (usePMDFile)
     {
	  /** 頂点情報を描画する */
	  PMD_VERTEX_CHUNK pmdVertexChunk = m_pmdFile.GetVertexChunk();
	  if ( !pmdVertexChunk.empty() )
	  {

	       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	       prepare3DViewport(0,0,getWidth(), getHeight());
	       glLoadIdentity();
 
	       glColor4f(0,0,1,1);
	       glTranslatef(0,0,-5);
	       glRotatef(50.0f, 0.0f, 1.0f, 0.0f);

	       glColor4f(1, 0, 0, 1);

	       GLubyte indices[pmdVertexChunk.size()];
	       GLfloat vert[pmdVertexChunk.size()][3]; // 頂点データ
	       GLfloat norm[pmdVertexChunk.size()][3]; // 法線データ
	       GLfloat texc[pmdVertexChunk.size()][2]; // テクスチャ座標・UV座標
	       
	       int index = 0;

	       for (auto it = pmdVertexChunk.begin(); it != pmdVertexChunk.end(); ++it, ++index)
	       {
		    indices[index] = index;

		    vert[index][0] = it->x;
		    vert[index][1] = it->y;
		    vert[index][2] = it->z;
		    norm[index][0] = it->nx;
		    norm[index][1] = it->ny;
		    norm[index][2] = it->nz;
		    texc[index][0] = it->tx;
		    texc[index][1] = it->ty;

		    wxLogMessage(wxT("x:%f, y:%f, z:%f, nx:%f, ny:%f, nz:%f, tx:%f, ty:%f"), 
			         it->x, it->y, it->z, it->nx, it->ny, it->nz, it->tx, it->ty);
	       }

	       glEnableClientState(GL_VERTEX_ARRAY);
	       glVertexPointer(3 , GL_FLOAT , 0 , vert);
	       glDrawElements(GL_LINE_LOOP , 3 , GL_UNSIGNED_BYTE , indices);

               /* 三角形の数 */
	       //const int nf = sizeof indices / sizeof indices[0];
	       //const int nf = 0;

               /* 頂点データ，法線データ，テクスチャ座標の配列を有効にする */
	       //glEnableClientState(GL_VERTEX_ARRAY);
	       //glEnableClientState(GL_NORMAL_ARRAY);
	       //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

               /* 頂点データ，法線データ，テクスチャ座標の場所を指定する */
	       //glVertexPointer(3, GL_FLOAT, 0, vert);
	       //glNormalPointer(GL_FLOAT, 0, norm);
	       //glTexCoordPointer(2, GL_FLOAT, 0, texc);
  
	       /* 頂点のインデックスの場所を指定して図形を描画する */
	       //glEnable(GL_TEXTURE_2D);
	       //glColor4f(1, 0, 0, 1);
	       //glDrawElements(GL_TRIANGLES, nf * 3, GL_UNSIGNED_INT, indices);
	       //glDisable(GL_TEXTURE_2D);
  
	       /* 頂点データ，法線データ，テクスチャ座標の配列を無効にする */
	       //glDisableClientState(GL_VERTEX_ARRAY);
	       //glDisableClientState(GL_NORMAL_ARRAY);
	       //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	  }
     }
     else
     {

	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  // red square
	  glColor4f(1, 0, 0, 1);
	  glBegin(GL_QUADS);
	  glVertex3f(GetWidth()/8, GetHeight()/3, 0);
	  glVertex3f(GetWidth()*3/8, GetHeight()/3, 0);
	  glVertex3f(GetWidth()*3/8, GetHeight()*2/3, 0);
	  glVertex3f(GetWidth()/8, GetHeight()*2/3, 0);
	  glEnd();
 
	  // ------------- draw some 3D ----------------
	  Prepare3DViewport(0,0,GetWidth(), GetHeight());
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

#endif
 
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

void BasicGLPane::SetCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}
