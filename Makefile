#
# Makefile for the HCSR04Ultrasonic library.
#
# $Author: $
# $Date: $
# $Revision: $
#

# Local directory locations.
PACKAGE_PREFIX	= $(shell pwd)
PACKAGE_DIR	= $(shell echo $${PWD\#\#*/})
TODAY		= $(shell date +"%Y-%m-%d")

# Current version
MAJORVERSION    = 1
MINORVERSION    = 0
PATCHLEVEL      = 0
VERSION         = ${MAJORVERSION}.${MINORVERSION}.${PATCHLEVEL}

DISTNAME        = ${PACKAGE_DIR}-$(VERSION)
SVN_PATH	= /exports/nas-storage/cnobile/repos/svnroot/arduino-svn/HCSR04Ultrasonic

#--------------------------------------------------------------
all	: clean
	(cd ..; tar -czvf $(DISTNAME).tar.gz --exclude .svn $(PACKAGE_DIR))
	(cd ..; zip -r $(DISTNAME).zip $(PACKAGE_DIR)/* --exclude \*/.svn\*)

svn-tag :
	svn copy svn+ssh://foundation${SVN_PATH}/trunk \
          svn+ssh://foundation${SVN_PATH}/tags/tag-${VERSION}-${TODAY} \
          -m "Tag--release $(VERSION)."

#--------------------------------------------------------------
clean	:
	$(shell $(PACKAGE_PREFIX)/cleanDirs.sh clean)
