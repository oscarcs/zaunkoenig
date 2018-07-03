#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wren.h"
#include "BearLibTerminal.h"

char* wren_load_module(WrenVM*, const char*);
void wren_print(WrenVM*, const char*);
void wren_error(WrenVM*, WrenErrorType, const char*, int, const char*);

int main(void)
{
    // Configure the Wren VM instance. We add functions to get 'System.print' functionality,
    // and to load modules.
    WrenConfiguration config; 
    wrenInitConfiguration(&config);
    config.writeFn = wren_print;
    config.errorFn = wren_error;
    config.loadModuleFn = wren_load_module;

    WrenVM* vm = wrenNewVM(&config);    
    // Load the Wren code:
    char* code = wren_load_module(vm, "test");
    WrenInterpretResult result = wrenInterpret(vm, code);

    // Set up the call handles:
    WrenHandle* entry_point = wrenMakeCallHandle(vm, "test()");
    //@@TODO: create(), update(), pause(), etc. 

    // Get the entry point and run the Wren code there:    
    wrenEnsureSlots(vm, 4);
    wrenGetVariable(vm, "main", "Roguelike", 0); //  put the static roguelike object into 0
    WrenHandle* main_obj = wrenGetSlotHandle(vm, 0); // store the handle

    wrenSetSlotHandle(vm, 0, main_obj);
    result = wrenCall(vm, entry_point);

    // Open the terminal
    terminal_open();
    terminal_print(1, 1, "Hello, world!");
    terminal_refresh();
  
    // Wait for input until user closes the window
    int cur = 0;
    while (!(cur == TK_CLOSE || cur == TK_ESCAPE)) {
        cur = terminal_read();
        printf("keypress: %d\n", cur);
    }
  
    // Close BearLibTerminal and the Wren VM.
    terminal_close();
    wrenFreeVM(vm);

    return 0;
}

// Load a module from a file.
char* wren_load_module(WrenVM* vm, const char* name)
{
    // Concatenate the file suffix:
    char *qualified_name = malloc(strlen(name) + 1);
    strcpy(qualified_name, name);
    strcat(qualified_name, ".wren");

    // Open and load the file.
    FILE *f = fopen(qualified_name, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  // same as rewind(f);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    // Make the loaded data into a proper string
    string[fsize] = 0;

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