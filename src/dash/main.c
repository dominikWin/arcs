#include "common.h"
#include <stdio.h>
#include <ncurses.h>
#include "screen.h"
#include "io.h"
#include "js.h"
#include "interface.h"

int main() {
#ifdef DEBUG
	fprintf(stdout, "Started dashboard\n");
#endif
	screen_init();

	char *js = found_js();
	if (js != NULL) {
		js_connect(js, js_update);
	}

	struct properties props;
	props.js = (js == NULL) ? "Not found" : js;
	redraw(props);


	while(1) {
		props.jsstat=get_js_status();
		redraw(props);
	}

#ifdef DEBUG
	fprintf(stdout, "Exiting\n");
#endif
	screen_close();
	return 0;
}
