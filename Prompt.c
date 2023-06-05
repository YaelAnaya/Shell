//
// Created by yaelao on 5/30/23.
//

#include "Prompt.h"

/**
 * Esta funcion obtiene el valor de un comando de entrada.
 * @param key El comando de entrada.
 * @return El valor del comando de entrada.
 * */
enum Option value_of(char *key) {
    int i;
    for (i = 0; i < sizeof(lookup_table) / sizeof(Pair); i++) {
        if (strcmp(key, lookup_table[i].key) == 0) {
            return lookup_table[i].value;
        }
    }
    return -1;
}


void init_shell(void) {
    process_queue = create_queue();
    init_memory();
}

/**
 * Esta funcion se encarga de hacer un split de los argumentos
 * que introduce el usuario.
 * @param args: String con los argumentos pasados.
 * @return un arreglo de strings con los argumentos separados.
 * */

char **split_args(char *args) {
    char **args_array = malloc(100 * sizeof(char *));
    char *arg = strtok(args, " ");
    int i = 0;
    while (arg != NULL) {
        arg = strtok(NULL, " ");
        args_array[i++] = arg;
    }
    args_array[i] = NULL;
    return args_array;
}

/**
 * Esta función se encarga de verificar que la cantidad de argumentos pasados
 * sea la correcta.
 *
 * @param args: los argumentos pasados por el usuario.
 * @param num_of_args: la cantidad de argumentos que se espera recibir.
 * @return 0 si la cantidad de argumentos es correcta, -1 en caso contrario.
 * */
bool verify_num_of_args(char **args, int num_of_args) {

    int i = 0;
    while (args[i] != NULL) {
        i++;
    }

    if (i < num_of_args) {
        printf("Error: missing arguments.\n");
        return false;
    }

    if (args[num_of_args] != NULL) {
        printf("Error: too many arguments.\n");
        return false;
    }

    return true;
}

/**
 * Esta funcion se encarga de hacer un split de los argumentos
 * que introduce el usuario.
 * @param args: String con los argumentos pasados.
 * @return un arreglo de strings con los argumentos separados.
 * */

char **split_args_execvp(char *args) {
    char **args_array = malloc(100 * sizeof(char *));
    char *arg = strtok(args, " ");
    int i = 0;
    args_array[i++] = arg;
    while (arg != NULL) {
        arg = strtok(NULL, " ");
        if (arg != NULL && strcmp(arg, "|") == 0)
            break;
        /**
         * Si el usuario introduce un argumento con espacios, se
         * debe de tomar en cuenta que el argumento se encuentra
         * entre comillas simples.
         * */
        if (arg != NULL && strcmp(arg, "\'") == 0) {
            args_array[i++] = " ";
            arg = strtok(NULL, "\' ");
        }
        args_array[i++] = arg;
    }
    args_array[i] = NULL;
    return args_array;
}

/**
 * Esta funcion se encarga de ejecutar un comando simple.
 * @param args: Arreglo de strings con
 * los argumentos del comando.
 * */
void exec_single_command(char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            printf("Error executing command\n");
        }

    } else {
        wait(NULL);
    }
}

/**
 * En caso de que el usuario introduzca un pipe,
 * se ejecuta esta funcion.
 * @param first_section: Arreglo de strings con los argumentos
 * del primer comando.
 * @param second_section: Arreglo de strings con los argumentos
 * del segundo comando.
 * */
void exec_pipe(char **first_section, char **second_section) {
    pid_t first_section_pid, second_section_pid;
    int pipe_descriptor[2];
    pipe(pipe_descriptor);
    /**
     * Se crea un nodo hijo para poder ejecutar
     * la primera seccion del pipe.
     * */
    first_section_pid = fork();

    if (first_section_pid == 0) {
        close(pipe_descriptor[READ_END]);

        /*Se redirecciona la parte de escritura
         * del pipe a la salida estandar*/
        dup2(pipe_descriptor[WRITE_END], STDOUT_FILENO);
        close(pipe_descriptor[WRITE_END]);
        execvp(first_section[0], first_section);

    } else {
        wait(NULL);
        /**
         * Se crea un segundo nodo hijo para poder ejecutar
         * la segunda seccion del pipe.
         * */
        second_section_pid = fork();
        if (second_section_pid == 0) {
            close(pipe_descriptor[WRITE_END]);
            /*
             * Se redirecciona la parte de lectura del pipe
             * a la entrada estandar .
             * */
            dup2(pipe_descriptor[READ_END], STDIN_FILENO);
            close(pipe_descriptor[READ_END]);
            execvp(second_section[0], second_section);

        } else {
            close(pipe_descriptor[READ_END]);
            close(pipe_descriptor[WRITE_END]);
            wait(NULL);
        }
    }
}
/**
 * Esta funcion se encarga de leer la entrada del usuario.
 * @param input: String donde se guardara la entrada del usuario.
 * @return un booleano que indica si el usuario introdujo un pipe.
 * */
bool read_user_input(char *input) {
    bool has_pipe = false;
    char char_input;
    printf("narco_barbie_69:~$ ");
    while ((char_input = (char) getchar()) != '\n') {
        if (char_input == '|') has_pipe = true;

        strncat(input, &char_input, 1);
    }
    strncat(input, "\0", 1);

    return has_pipe;
}

/**
 * Esta funcion se encarga de hacer un split
 * de los canales  de la pipe.
 * @param input: String con los canales de la pipe.
 * @return un arreglo de strings con los canales separados.
 * */
char **split_channels(char *input) {
    char **channels = malloc(100 * sizeof(char *));
    int i = 0;
    char *channel;
    while ((channel = strsep(&input, "|")) != NULL) {
        channels[i++] = channel;
    }
    return channels;
}

void bash_commands(bool has_pipe, char *input) {
    if (has_pipe) {
        char **channels = split_channels(input);
        char **first_section = split_args_execvp(channels[0]);
        char **second_section = split_args_execvp(channels[1]);

        exec_pipe(first_section, second_section);
        free(channels);
        free(first_section);
        free(second_section);
    } else {
        char **args = split_args_execvp(input);
        exec_single_command(args);
        free(args);
    }
}

/**
 * Esta funcion se encarga de mostrar el prompt al usuario
 * y de ejecutar los comandos introducidos por el usuario.
 * */
void show_prompt() {
    char *input = malloc(100 * sizeof(char));
    bool has_pipe = read_user_input(input);
    bool is_exit = strcmp(input, "exit") == 0;

    if (is_exit) {
        printf("bye\n");
        free(input);
        exit(0);
    }

    if (input == NULL) {
        free(input);
        return;
    }
    /* Copia de la entrada estandar, y con esta copia
     * se obtiene el nombre del comando. */
    char *cpy_one = malloc(100 * sizeof(char));
    char *cpy_two = malloc(100 * sizeof(char));
    strcpy(cpy_one, input);
    strcpy(cpy_two, input);

    char *command_name = strtok(cpy_one, " ");
    char **args = split_args(cpy_two);

    switch (value_of(command_name)) {
        case ALLOC:
            if (!verify_num_of_args(args, 2))
                break;
            else {
                int pid = atoi(args[0]);
                Process *process = get_process(process_queue, pid);
                if (process == NULL) {
                    printf("Process not found\n");
                    break;
                }
                assign_memory(process, args[1]);
            }
            break;

        case FREE:
            if (!verify_num_of_args(args, 1))
                break;
            else {
                int pid = atoi(args[0]);
                Process *process = get_process(process_queue, pid);
                if (process == NULL) {
                    printf("Process not found\n");
                    break;
                }

                free_memory(pid);
            }
            break;

        case COMPACT:
            if (!verify_num_of_args(args, 0))
                break;
            else {
                compact_memory();
            }
            break;
        case STATE:
            if (!verify_num_of_args(args, 0))
                break;
            else {
                report_memory();
            }
            break;
        case MKPS:
            if (!verify_num_of_args(args, 3))
                break;
            else {
                int pid = atoi(args[0]);
                int burst = atoi(args[1]);
                int size = atoi(args[2]);
                Process *process = make_process(pid, burst, size);
                if (contains(process_queue, process, compare_process)) {
                    printf("Process already exist\n");
                    break;
                }
                enqueue(process_queue, process, sizeof(Process));
            }
            break;
        case LSP:
            if (!verify_num_of_args(args, 0))
                break;
            else {
                print_queue(process_queue, print_process);
            }
            break;
        case KILL:
            if (!verify_num_of_args(args, 1))
                break;
            else {
                int pid = atoi(args[0]);
                Process *process = get_process(process_queue, pid);
                if (process == NULL) {
                    printf("Process not found\n");
                    break;
                }
                free_memory(pid);
                kill_process(process_queue, pid);
            }
            break;
        case RR:
            if (!verify_num_of_args(args, 1))
                break;
            else {
                int quantum = atoi(args[0]);
                if (quantum <= 0) {
                    printf("Invalid quantum\n");
                    break;
                }
                round_robin(process_queue, quantum);
            }
            break;
        case FCFS:
            if (!verify_num_of_args(args, 0))
                break;
            else {
                first_come_first_served(process_queue);
            }
            break;
        case SJF:
            if (!verify_num_of_args(args, 0))
                break;
            else {
                shortest_job_first(process_queue);
            }
            break;

        default:
            bash_commands(has_pipe, input);
            break;
    }

}