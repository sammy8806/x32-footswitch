TEMPLATE=subdirs
SUBDIRS = app \
    OSC

app.depends = OSC
app.file = app/hsb_footcontroller.pro

# CONFIG += ordered
