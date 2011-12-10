VPATH=life:animation:pig:orderManager:buttonManager:button:splash

CC=g++
CXXFLAGS=-fPIC -I/opt/local/include/
LDFLAGS=-lgear2d -shared

all: libclipLife.so libbutton.so libbuttonManager.so liborderManager.so libtutorial.so libsplash.so libpig.so libdrink.so libnoSheet.so

libclipLife.so: clipLife.cc
	cd life && $(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(<F)

libnoSheet.so: noSheet.cc
	cd animation && $(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(<F)

libdrink.so: drink.cc
	cd animation && $(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(<F)

libpig.so: pig.cc
	cd pig && $(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(<F)

liborderManager.so: orderManager.cc
	cd orderManager && $(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(<F)

libbutton.so: button.cc
	cd button && $(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(<F)
	
libbuttonManager.so: buttonManager.cc
	cd buttonManager && $(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(<F)

libtutorial.so: tutorial.cc
	cd splash && $(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(<F)

libsplash.so: splash.cc
	cd splash && $(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $(<F)
