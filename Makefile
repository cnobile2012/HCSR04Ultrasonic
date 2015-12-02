#
# Makefile for the HCSR04Ultrasonic library.
#
# $Author$
# $Date$
# $Revision$
#

# Local directory locations.
PACKAGE_PREFIX	= $(shell pwd)
PACKAGE_DIR	= $(shell echo $${PWD\#\#*/})
TODAY		= $(shell date +"%Y-%m-%d_%H%M")

# Current version
MAJORVERSION    = 1
MINORVERSION    = 1
PATCHLEVEL      = 2
VERSION         = ${MAJORVERSION}.${MINORVERSION}.${PATCHLEVEL}
DISTNAME        = ${PACKAGE_DIR}-$(VERSION)

#--------------------------------------------------------------
all	: clobber
	(cd ..; tar -czvf $(DISTNAME).tar.gz --exclude .git $(PACKAGE_DIR))
	(cd ..; zip -r $(DISTNAME).zip $(PACKAGE_DIR)/* --exclude \*/.git\*)

#--------------------------------------------------------------
clean	:
	$(shell $(PACKAGE_PREFIX)/cleanDirs.sh clean)

clobber	: clean
	@(cd ..; rm -rf $(DISTNAME).tar.gz $(DISTNAME).zip)
