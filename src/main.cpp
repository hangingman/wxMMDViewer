/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012 Hiroyuki Nagata
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

#include <wx/wx.h>

/** ライブラリのリンクテスト用 */
#include "common.h"
#include "MMD_IO.h"
#include "clsCSVFile.h"
#include "clsVMDFile.h"
#include "VMDConverter.h"

/*
 * wxAppを継承したwxMainを宣言
 */
class wxMain: public wxApp {

  wxLocale m_Locale;

public:
  wxMain() : m_Locale(wxLANGUAGE_DEFAULT){}
  virtual bool OnInit();
  virtual int OnExit();
};

IMPLEMENT_APP(wxMain)

/**
 * wxMainの実装
 */
bool wxMain::OnInit() {

    if (!wxApp::OnInit())
	 return false;
     
    return true;
}

/**
 * 終了後の後始末
 */
int wxMain::OnExit() {
     return 0;
}
