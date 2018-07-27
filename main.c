#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wren.h"
#include "BearLibTerminal.h"

char* wren_load_module(WrenVM*, const char*);
void wren_print(WrenVM*, const char*);
void wren_error(WrenVM*, WrenErrorType, const char*, int, const char*);
WrenForeignMethodFn wren_ffi(WrenVM*, const char*, const char*, bool, const char*);

// Foreign method API:
void wren_terminal_open(WrenVM*);
void wren_terminal_close(WrenVM*);
void wren_terminal_refresh(WrenVM*);
void wren_terminal_clear(WrenVM*);
void wren_terminal_clear_area(WrenVM*);
void wren_terminal_set(WrenVM*);
void wren_terminal_put(WrenVM*);
void wren_terminal_pick(WrenVM*);
void wren_terminal_print(WrenVM*);
void wren_terminal_print_ext(WrenVM*);
void wren_terminal_color(WrenVM*);
void wren_terminal_bkcolor(WrenVM*);
void wren_terminal_layer(WrenVM*);
void wren_terminal_state(WrenVM*);
void wren_terminal_delay(WrenVM*);

int main(void)
{
    // Configure the Wren VM instance. We add functions to get 'System.print' functionality,
    // and to load modules.
    WrenConfiguration config; 
    wrenInitConfiguration(&config);
    config.writeFn = wren_print;
    config.errorFn = wren_error;
    config.loadModuleFn = wren_load_module;
    config.bindForeignMethodFn = wren_ffi;

    WrenVM* vm = wrenNewVM(&config);    
    // Load the Wren code:
    char* code = wren_load_module(vm, "main");
    WrenInterpretResult result = wrenInterpret(vm, code);

    // Set up the call handles:
    WrenHandle* method_entry = wrenMakeCallHandle(vm, "main()");
    WrenHandle* method_input = wrenMakeCallHandle(vm, "input(_)");
    //@@TODO: create(), update(), pause(), etc. 

    // Get the entry point and run the Wren code there:    
    wrenEnsureSlots(vm, 8);
    wrenGetVariable(vm, "main", "App", 0); //  put the static roguelike object into 0
    WrenHandle* main_obj = wrenGetSlotHandle(vm, 0); // store the handle

    wrenSetSlotHandle(vm, 0, main_obj);
    result = wrenCall(vm, method_entry);
  
    // Wait for input until user closes the window
    int cur = 0;
    while (!(cur == TK_CLOSE || cur == TK_ESCAPE)) {
        cur = terminal_read();

        wrenSetSlotHandle(vm, 0, main_obj);
        wrenSetSlotDouble(vm, 1, cur);

        result = wrenCall(vm, method_input);
    }
  
    terminal_close();
    wrenFreeVM(vm);
    return 0;
}

// Load a module from a file.
char* wren_load_module(WrenVM* vm, const char* name)
{
    // Get the proper qualified name
    char *qualified_name = calloc(strlen(name) + 11, 1);
    sprintf(qualified_name, "wren/%s.wren", name);
    // printf("Attempting to load %s\n", qualified_name);

    char* string = NULL;

    // Open and load the file.
    FILE *f = fopen(qualified_name, "rb");
    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  // same as rewind(f);

        string = malloc(fsize + 1);
        fread(string, fsize, 1, f);
        fclose(f);

        // Make the loaded data into a proper string
        string[fsize] = 0;
    }

    free(qualified_name);
    return string;
}

// Provides an implementation of System.print() inside Wren.
void wren_print(WrenVM* vm, const char* str)
{
    printf(str, "%s");
}

// Stylishly print a Wren error.
void wren_error(WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message)
{
    switch (type)
    {
        case WREN_ERROR_COMPILE:
            printf("Wren compilation error: Module '%s', line %d: %s\n", module, line, message);
            break;

        case WREN_ERROR_RUNTIME:
            printf("Wren error: %s\n", message);
            break; 

        case WREN_ERROR_STACK_TRACE:
            printf("\tModule '%s', line %d: %s\n", module, line, message);
            break;
    }
}

WrenForeignMethodFn wren_ffi(
    WrenVM* vm, 
    const char* module, 
    const char* class, 
    bool is_static, 
    const char* signature)
{
    // printf("%s %s %s\n", module, class, signature);
    if (strcmp(module, "blt") == 0 && strcmp(class, "Terminal") == 0)
    {
        if (strcmp(signature, "open()") == 0)
            return wren_terminal_open;
        if (strcmp(signature, "close()") == 0)
            return wren_terminal_close;
        if (strcmp(signature, "refresh()") == 0)
            return wren_terminal_refresh;
        if (strcmp(signature, "clear()") == 0)
            return wren_terminal_clear;
        if (strcmp(signature, "clear_area(_,_,_,_)") == 0)
            return wren_terminal_clear_area;
        if (strcmp(signature, "set(_)") == 0)
            return wren_terminal_set;
        if (strcmp(signature, "put(_,_,_)") == 0)
            return wren_terminal_put;
        if (strcmp(signature, "pick(_,_,_)") == 0)
            return wren_terminal_put;
        if (strcmp(signature, "print(_,_,_)") == 0)
            return wren_terminal_print;
        if (strcmp(signature, "print_ext(_,_,_,_,_,_)") == 0)
            return wren_terminal_print;
        if (strcmp(signature, "color(_)") == 0)
            return wren_terminal_color;
        if (strcmp(signature, "bkcolor(_)") == 0)
            return wren_terminal_bkcolor;
        if (strcmp(signature, "layer(_)") == 0)
            return wren_terminal_layer;
        if (strcmp(signature, "state(_)") == 0)
            return wren_terminal_state;
        if (strcmp(signature, "delay(_)") == 0)
            return wren_terminal_delay;
    }
    return NULL;
}

void wren_terminal_open(WrenVM* vm)
{
    terminal_open();
}

void wren_terminal_close(WrenVM* vm)
{
    terminal_close();
}

void wren_terminal_refresh(WrenVM* vm)
{
    terminal_refresh();
}

void wren_terminal_clear(WrenVM* vm)
{
    terminal_clear();
}

void wren_terminal_clear_area(WrenVM* vm)
{
    int x = (int) wrenGetSlotDouble(vm, 1);
    int y = (int) wrenGetSlotDouble(vm, 2);
    int w = (int) wrenGetSlotDouble(vm, 3);
    int h = (int) wrenGetSlotDouble(vm, 4);
    terminal_clear_area(x, y, w, h);
}

void wren_terminal_set(WrenVM* vm)
{
    const char* s = wrenGetSlotString(vm, 1);
    terminal_set(s);
}

void wren_terminal_put(WrenVM* vm)
{
    int x = (int) wrenGetSlotDouble(vm, 1);
    int y = (int) wrenGetSlotDouble(vm, 2);
    int s = (int) wrenGetSlotDouble(vm, 3);
    terminal_put(x, y, s);
}

void wren_terminal_pick(WrenVM* vm)
{
    int x = (int) wrenGetSlotDouble(vm, 1);
    int y = (int) wrenGetSlotDouble(vm, 2);
    int i = (int) wrenGetSlotDouble(vm, 3);
    terminal_pick(x, y, i);
}

void wren_terminal_print(WrenVM* vm)
{
    int x = (int) wrenGetSlotDouble(vm, 1);
    int y = (int) wrenGetSlotDouble(vm, 2);
    const char* s = wrenGetSlotString(vm, 3);
    terminal_print(x, y, s);
}

void wren_terminal_print_ext(WrenVM* vm)
{
    int x = (int) wrenGetSlotDouble(vm, 1);
    int y = (int) wrenGetSlotDouble(vm, 2);
    int w = (int) wrenGetSlotDouble(vm, 3);
    int h = (int) wrenGetSlotDouble(vm, 4);
    int a = (int) wrenGetSlotDouble(vm, 5);
    const char* s = wrenGetSlotString(vm, 6);
    terminal_print_ext(x, y, w, h, a, s);
}

void wren_terminal_color(WrenVM* vm)
{
    int c = (color_t) wrenGetSlotDouble(vm, 1);
    terminal_color(c);
}

void wren_terminal_bkcolor(WrenVM* vm)
{
    int c = (color_t) wrenGetSlotDouble(vm, 1);
    terminal_bkcolor(c);
}

void wren_terminal_layer(WrenVM* vm)
{
    int l = (int) wrenGetSlotDouble(vm, 1);
    terminal_layer(l);
}

void wren_terminal_state(WrenVM* vm)
{
    int s = (int) wrenGetSlotDouble(vm, 1);
    terminal_state(s);
}

void wren_terminal_delay(WrenVM* vm)
{
    int period = (int) wrenGetSlotDouble(vm, 1);
    terminal_delay(period);
}
