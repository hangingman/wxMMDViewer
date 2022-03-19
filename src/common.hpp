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
 *  Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

#ifdef HAVE_CONFIG_H
   #include "config.h"
#endif

#define XSTR(x) #x
#define STR(x)  XSTR(x)

// ファイルの区切り文字(FileSeparator)
#ifdef __WXMSW__
  #define wxFS wxT("\\")
#else
  #define wxFS wxT("/")
#endif

// 実行ファイルの拡張子
#ifdef __WXMSW__
  #define wxExt wxT(".exe")
#else
  #define wxExt wxT("")
#endif

/** wxMMDViewerの作業用隠しフォルダ名 */
#define WXMMD_DIR wxT(".wxmmd")

#endif /** COMMON_HPP_ */
