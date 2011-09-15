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

# Current version
MAJORVERSION    = 1
MINORVERSION    = 0
PATCHLEVEL      = 0
VERSION         = ${MAJORVERSION}.${MINORVERSION}.${PATCHLEVEL}

DISTNAME        = ${PACKAGE_DIR}-$(VERSION)

#--------------------------------------------------------------
all	: clean
	(cd ..; tar -czvf $(DISTNAME).tar.gz --exclude .svn $(PACKAGE_DIR))
	(cd ..; zip -r $(DISTNAME).zip $(PACKAGE_DIR) --exclude /\*.svn* /.svn/)

#--------------------------------------------------------------
clean	:
	$(shell $(PACKAGE_PREFIX)/cleanDirs.sh clean)
