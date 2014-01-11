# subdirectories
SUBDIRS = libwxnkf libvmdconv src

# chain with ChangeLog
dist-hook: ChangeLog

# work when configure.ac is updated
ChangeLog: configure.ac
	git log --stat --name-only --date=short --abbrev-commit > ChangeLog

# OSX application bundle用の設定
ICONFILE		= wxmac.icns
ICONPATH		= $(top_builddir)/src/rc/$(ICONFILE)
RCPATH			= $(top_builddir)/src/rc
PROGVER			= $(VERSION)
BUNDLE			= $(top_builddir)/$(PACKAGE_NAME).app
MACICON			= $(BUNDLE)/Contents/$(ICONFILE)
MACPKGINFO		= $(BUNDLE)/Contents/PkgInfo
MACINFOPLIST		= $(BUNDLE)/Contents/Info.plist
# OSX Info.plist用の設定
COMPANY			= ????
BUNDLEPACKAGE		= APPL
BUNDLESIGNATURE		= ???
EXECUTABLE		= $(top_builddir)/src/.libs/$(PACKAGE_NAME)

# OSX向けのバイナリ(テスト用)
osx-make-bin : remove_distdir $(BUNDLE) $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME) $(MACICON) $(MACPKGINFO) $(MACINFOPLIST) wxmac-dist-shared-lib

# OSX向けのバイナリ作成
osx-make-hook : remove_distdir $(BUNDLE) $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME) $(MACICON) $(MACPKGINFO) $(MACINFOPLIST) check-dylibbundler wxmac-dist-shared-lib package-osx-intel64-dmg

# Bundleその他の生成方法を書く
#  This builds the bundle's directory structure.
$(BUNDLE) :
	@echo "==== Building bundle directory structure ===="
	mkdir -p $(BUNDLE)/Contents
	mkdir -p $(BUNDLE)/Contents/MacOS
	mkdir -p $(BUNDLE)/Contents/Resources

#  This builds the executable right inside the bundle.
$(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME):
	@echo "==== Copying executable ===="
	cp -f $(EXECUTABLE) $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME)
	cp -rf $(RCPATH) $(BUNDLE)/Contents/MacOS

#  This copies the icon file into the bundle.
$(MACICON):
	@echo "==== Copying icon file into bundle ===="
	cp -f $(ICONPATH) $(BUNDLE)/Contents/Resources/$(ICONFILE)

#  This creates the Contents/PkgInfo file.
$(MACPKGINFO):
	@echo "==== Creating PkgInfo ===="
	touch $(MACPKGINFO)
	@echo -n "$(BUNDLEPACKAGE)$(BUNDLESIGNATURE)" > $(MACPKGINFO)

#  This creates the Contents/Info.plist file.
$(MACINFOPLIST):
	@echo "==== Creating Info.plist ===="
	touch $(MACINFOPLIST)
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >> $(MACINFOPLIST)
	@echo "<!DOCTYPE plist PUBLIC " >> $(MACINFOPLIST)
	@echo "\"-//Apple Computer//DTD PLIST 1.0//EN\" " >> $(MACINFOPLIST)
	@echo "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" >> $(MACINFOPLIST)
	@echo "<plist version=\"1.0\">" >> $(MACINFOPLIST)
	@echo "<dict>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleDevelopmentRegion</key>" >> $(MACINFOPLIST)
	@echo "   <string>Japanese</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleExecutable</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PACKAGE_NAME)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleIconFile</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(ICONFILE)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleName</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PACKAGE_NAME)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleIdentifier</key>" >> $(MACINFOPLIST)
	@echo "   <string>com.$(COMPANY).$(PACKAGE_NAME)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleInfoDictionaryVersion</key>" >> $(MACINFOPLIST)
	@echo "   <string>6.0</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundlePackageType</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(BUNDLEPACKAGE)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleSignature</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(BUNDLESIGNATURE)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleVersion</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PROGVER)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleShortVersionString</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PROGVER)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleGetInfoString</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PACKAGE_NAME), Version $(PROGVER), $(COMPANY)</string>" >> $(MACINFOPLIST)
	@echo "</dict>" >> $(MACINFOPLIST)
	@echo "</plist>" >> $(MACINFOPLIST)

# OSX向けにライブラリをBUNDLEに設定する
WXLIBPOSTFIX = *wx*@WX_VERSION_MAJOR@.@WX_VERSION_MINOR@.@WX_VERSION_MICRO@.dylib
WXLIBDIR     = `@WX_CONFIG_PATH@ --prefix`/lib
BINDIR	     = $(BUNDLE)/Contents/MacOS
LIBDEFDIR    = /usr/local/lib
MACEXECUTABLE   = $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME)
DYLIBBUNDLER = dylibbundler

wxmac-dist-shared-lib:
	@echo "Copying and Patching wxMMDViewer subdirectory libraries to " $(BINDIR)/lib " ..."
	mkdir $(BINDIR)/lib
	@for dylibs in `find ./ -name \*.dylib -print | sort -u `; do                \
	   if [[ $$dylibs != /usr/lib/* ]]; then                                    \
	      echo "Copying " $$dylibs " to " $(BINDIR)/lib;                        \
	      cp -p $$dylibs $(BINDIR)/lib;                                         \
	      echo "Patching " $$dylibs " to " $(BINDIR)/lib;                       \
	      install_name_tool -change $$dylibs                                    \
	                        @executable_path/lib/`basename $$dylibs`            \
	                        $(MACEXECUTABLE);                                   \
	   fi                                                                       \
	done
	@echo "=====================================================================";
	@echo "End of wxMMDViewer's subdirectories patching...                      ";
	@echo "=====================================================================";
	@for dylibs in `otool -L $(MACEXECUTABLE) | grep -o /.\*.dylib | sort -u `; do \
	   if [[ $$dylibs != /usr/lib/* ]]; then                                    \
	      echo "Copying " $$dylibs " to " $(BINDIR)/lib;                        \
	      cp -p $$dylibs $(BINDIR)/lib;                                         \
	      echo "Patching " $$dylibs " to " $(BINDIR)/lib;                       \
	      install_name_tool -change $$dylibs                                    \
	                        @executable_path/lib/`basename $$dylibs`            \
	                        $(MACEXECUTABLE);                                   \
	   fi                                                                       \
	done
	@echo "=====================================================================";
	@echo "End of wxMMDViewer's dependency patching...                          ";
	@echo "=====================================================================";
	@for deplibs in `otool -L $(BINDIR)/lib/*.dylib | grep -o /.\*dylib | sort -u`; do   \
	   if [[ $$deplibs = /usr/local/lib/* ]] || [[ $$deplibs = /opt/local/lib/* ]]; then \
	      if [[ ! -e $(BINDIR)/lib/`basename $$deplibs` ]]; then \
	         echo "Copying " $$deplibs " to " $(BINDIR)/lib;     \
	         cp -p $$deplibs $(BINDIR)/lib;                      \
	      fi;                 \
	   fi;                    \
	done
	@for deplibs in `otool -L $(BINDIR)/lib/*.dylib | grep -o /.\*dylib | sort -u`; do   \
	   if [[ $$deplibs = /usr/local/lib/* ]] || [[ $$deplibs = /opt/local/lib/* ]]; then \
	      if [[ ! -e $(BINDIR)/lib/`basename $$deplibs` ]]; then \
	         echo "Copying " $$deplibs " to " $(BINDIR)/lib;     \
	         cp -p $$deplibs $(BINDIR)/lib;                      \
	      fi;                 \
	   fi;                    \
	done
	@echo "=====================================================================";
	@echo "End of wxMMDViewer's dependency libraries copying...                 ";
	@echo "=====================================================================";
	@for file in `ls $(BINDIR)/lib/*.dylib` ; do \
	   otool -L $$file | egrep -v /\(usr/lib\|System\) | awk {'print $$1'} | while read line; \
	   do                 \
	      if [[ $$line != *: ]]; then \
	         echo "Patching " $$line " to @executable_path/lib/" `basename $$line` ;              \
	         install_name_tool -change $${line} @executable_path/lib/`basename $$line` $${file};  \
	      fi                          \
	   done                           \
	done

# OSX向けのインストーラ作成
package-osx-intel64-dmg:
	@echo "!!! Start make disk image file !!!";
	hdiutil create -ov -srcfolder $(BUNDLE) -fs HFS+ -format UDZO \
               -imagekey zlib-level=9 -volname "wxMMDViewer.app" $(PACKAGE)-osx64-$(VERSION).dmg;
	@echo "!!! Finish make disk image file !!!";

# インストーラ作成のための準備
prepare-package: package-clean
	@echo 'create working directory...'
	mkdir $(top_builddir)/extras
	mkdir $(top_builddir)/extras/package

remove_distdir :
# パッケージを削除
	if test -d "$(distdir)"; then \
	find "$(distdir)" -type d ! -perm -200 -exec chmod u+w {} ';' \
	&& rm -rf "$(distdir)" \
	|| { sleep 5 && rm -rf "$(distdir)"; }; \
	else :; fi
	rm -rf $(distdir).tar.gz
	rm -rf $(distdir).zip
	rm -rf $(BUNDLE)

clean-hook: remove_distdir
