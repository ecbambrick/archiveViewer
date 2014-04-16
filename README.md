Archive Viewer v0.1b
================================================================================
Application for viewing images from directories and archives.

Building
--------------------------------------------------------------------------------
Requires Qt 5.2 to build.

Documentation for building with qmake can be found [here](https://qt-project.org/doc/qmake-running.html). Alternatively, open ArchiveViewer.pro with Qt Creator.

To open zip archives with 7-Zip:

* On Windows 7z.exe must be included in the same directory as where Archive Viewer is built. 
* On Linux, 7-Zip must be installed.
* A link to download 7-Zip can be found under Third Party Libraries

Third Party Libaries
--------------------------------------------------------------------------------
Archive Viewer uses the following libraries for reading from zip archives:

* [7-zip](http://www.7-zip.org/) - 7-Zip is licensed under the GNU LGPL license.
* [QuaZip](http://quazip.sourceforge.net/) - QuaZip is licensed under the GNU LGPL license.
* [zlib](http://www.zlib.net/) - For libz licensing, see the zlib README file.
