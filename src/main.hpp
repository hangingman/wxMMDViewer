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

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/snglinst.h>
#include "wxmmdviewer.hpp"

/*
 * wxAppを継承したwxMainを宣言
 */
class wxMain: public wxApp {

public:
  virtual bool OnInit();
  virtual int OnExit();
  bool render_loop_on;
  void OnIdle(wxIdleEvent& evt);
  void ActivateRenderLoop(bool on);

private:
  virtual int FilterEvent(wxEvent& event);
  wxSingleInstanceChecker* m_checker;
  MMDViewer* wxMMDViewer;
};

DECLARE_APP(wxMain)

#endif /** MAIN_HPP_ */
