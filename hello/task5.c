#include <stdio.h>
#include <X11/Xlib.h>

char *key_name[] = {"first", "second", "third", "fourth", "fifth"};
int main () {
	Display * display;
	XEvent xevent;
	Window window;
	
	if ((display = XOpenDisplay(NULL)) == NULL) {
		return -1;
	}
	window = DefaultRootWindow(display);
	XAllowEvents(display, AsyncBoth, CurrentTime);

	XGrabPointer(display, window, 1, PointerMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
	while(1) {
		XNextEvent(display, &xevent);
		switch(xevent.type) {
			case MotionNotify:
				printf("Mouse move: [%d, %d]\n", xevent.xmotion.x_root, xevent.xmotion.y_root);
				break;
		}
	}

	return 0;
}
