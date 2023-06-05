#include <stdio.h>
#include "Queue.h"
#include "List.h"
#include "Process.h"
#include "Memory.h"
#include "Prompt.h"

int main() {

    init_shell();

    while(true)
       show_prompt();

    return 0;
}
