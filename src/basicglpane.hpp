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

#ifndef BASICGLPANE_HPP_
#define BASICGLPANE_HPP_
 
#include "wx/wx.h"
#include "wx/glcanvas.h"

#ifdef __WXMAC__
   #include <GL/glut.h>
#endif
 
class BasicGLPane : public wxGLCanvas
{
     wxGLContext*	m_context;
 
public:
     BasicGLPane(wxFrame* parent, int* args);
     virtual ~BasicGLPane();
 
     void resized(wxSizeEvent& evt);
 
     int getWidth();
     int getHeight();
 
     void render(wxPaintEvent& evt);
     void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
     void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
 
     // events
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
 
     DECLARE_EVENT_TABLE()
};

#endif /** BASICGLPANE_HPP_ */
