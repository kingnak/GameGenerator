TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += \
    gg_core \
	GGEditor_src \
	GGEditor_app \
    GGEditor_test

GGEditor_app.depends = GGEditor_src
GGEditor_test.depends = GGEditor_src
