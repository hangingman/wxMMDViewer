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
 
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <GL/glut.h>
#include <clsPMDFile.hpp>
 
class BasicGLPane : public wxGLCanvas
{
     wxGLContext* m_context;
 
public:
     BasicGLPane(wxFrame* parent, int* args);
     virtual ~BasicGLPane();

     void SetPMDFile(const clsPMDFile& pmdFile);
     clsPMDFile& GetPMDFile();

private: 
     void Resized(wxSizeEvent& evt);
 
     int GetWidth();
     int GetHeight();
 
     void Render(wxPaintEvent& evt);
     void Prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
     void SetCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);

     // events
     void MouseMoved(wxMouseEvent& event);
     void MouseDown(wxMouseEvent& event);
     void MouseWheelMoved(wxMouseEvent& event);
     void MouseReleased(wxMouseEvent& event);
     void RightClick(wxMouseEvent& event);
     void MouseLeftWindow(wxMouseEvent& event);
     void KeyPressed(wxKeyEvent& event);
     void KeyReleased(wxKeyEvent& event);
 
     // true: PMDファイル投入済、false: PMDファイルなし
     bool usePMDFile;
     // PMDファイルの情報
     clsPMDFile m_pmdFile;

     DECLARE_EVENT_TABLE()
};

#endif /** BASICGLPANE_HPP_ */
