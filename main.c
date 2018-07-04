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
void wren_terminal_print(WrenVM*);

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
    WrenHandle* entry_point = wrenMakeCallHandle(vm, "main()");
    //@@TODO: create(), update(), pause(), etc. 

    // Get the entry point and run the Wren code there:    
    wrenEnsureSlots(vm, 4);
    wrenGetVariable(vm, "main", "Roguelike", 0); //  put the static roguelike object into 0
    WrenHandle* main_obj = wrenGetSlotHandle(vm, 0); // store the handle

    wrenSetSlotHandle(vm, 0, main_obj);
    result = wrenCall(vm, entry_point);
  
    // Wait for input until user closes the window
    int cur = 0;
    while (!(cur == TK_CLOSE || cur == TK_ESCAPE)) {
        cur = terminal_read();
        // printf("keypress: %d\n", cur);
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

    char* string = "";

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
            printf("Wren compilation error: Module %s, line %d: %s\n", module, line, message);
            break;

        case WREN_ERROR_RUNTIME:
            printf("Wren error: %s\n", message);
            break; 

        case WREN_ERROR_STACK_TRACE:
            printf("\tModule %s, line %d: %s\n", module, line, message);
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
    if (strcmp(module, "blt") == 0 && strcmp(class, "BearLibTerminal") == 0)
    {
        if (strcmp(signature, "open()") == 0)
            return wren_terminal_open;
        if (strcmp(signature, "close()") == 0)
            return wren_terminal_close;
        if (strcmp(signature, "refresh()") == 0)
            return wren_terminal_refresh;
        if (strcmp(signature, "print(_,_,_)") == 0)
            return wren_terminal_print;
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

void wren_terminal_print(WrenVM* vm)
{
    int x = (int) wrenGetSlotDouble(vm, 1);
    int y = (int) wrenGetSlotDouble(vm, 2);
    const char* s = wrenGetSlotString(vm, 3);
    terminal_print(x, y, s);
}
