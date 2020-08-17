# Makefile
#
# Install data files or gererate resources including icons, configure files
# and etc.

GLIB_COMPILE_RESOURCES = glib-compile-resources

RES = $(BUILD_DIR)/resources.c

GLADES = $(wildcard ui/*.glade)
ICONS = $(wildcard icons/*.*)
THEMES = $(wildcard themes/*.css)

default: config icons themes misc plugins

.PHONY: resources
resources: $(RES)

$(RES): srain.gresource.xml $(GLADES) $(ICONS)
	$(GLIB_COMPILE_RESOURCES) "$<" --target="$@" --sourcedir=. --generate-source

.PHONY: config
config: builtin.cfg
	$(INSTALL) -Dm644 "$<" "$(DESTDIR)$(PACKAGE_CONFIG_DIR)/$(PACKAGE)/$<"

.PHONY: icons
icons: icons/128x128/srain.png icons/128x128/srain-red.png
	$(INSTALL) -Dm644 "icons/128x128/srain.png" "$(DESTDIR)$(PACKAGE_DATA_DIR)/icons/hicolor/128x128/apps/$(PACKAGE_APPID).png"
	$(INSTALL) -Dm644 "icons/128x128/srain-red.png" "$(DESTDIR)$(PACKAGE_DATA_DIR)/icons/hicolor/128x128/apps/$(PACKAGE_APPID)-red.png"

.PHONY: themes
themes: $(THEMES)
	for i in $^; do \
		$(INSTALL) -Dm644 "$$i" "$(DESTDIR)$(PACKAGE_DATA_DIR)/$(PACKAGE)/$$i"; \
	done

.PHONY: misc
misc: srain.desktop srain.metainfo.xml
	$(INSTALL) -Dm644 "srain.desktop" "$(DESTDIR)$(PACKAGE_DATA_DIR)/applications/$(PACKAGE_APPID).desktop"
	$(INSTALL) -Dm644 "srain.metainfo.xml" "$(DESTDIR)$(PACKAGE_DATA_DIR)/metainfo/$(PACKAGE_APPID).metainfo.xml"

.PHONY: plugins
plugins:
	# TODO
