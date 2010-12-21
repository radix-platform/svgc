

############################################################
# Don't strip svgc-config script when `make install-strip' #
############################################################
INSTALL_SCRIPT = ${INSTALL}


bin_SCRIPTS = svgc-config


svgc-config: make-svgc-config head tail
	./make-svgc-config

clean-am:
	rm -f svgc-config

