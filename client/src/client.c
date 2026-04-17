#include "client.h"
#include <readline/readline.h>

int main(void)
{
    //---------------------------------------------------PARTE 2-------------------------------------------------------------//

    int conexion;
    char* ip;
    char* puerto;
    char* valor;

    t_log* logger;
    t_config* config;

    /* ---------------- LOGGING ---------------- */

    logger = iniciar_logger();

    log_info(logger, "Hola! Soy un log");

    // Usando el logger creado previamente
    // Escribi: "Hola! Soy un log"


    // ---------------- ARCHIVOS DE CONFIGURACION ---------------- //

    config = iniciar_config();

    // Usando el config creado previamente, leemos los valores del config y los 
    // dejamos en las variables 'ip', 'puerto' y 'valor'

    // Loggeamos el valor de config

    // Leemos los valores del config
    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
    valor = config_get_string_value(config, "CLAVE");

    // Loggeamos el valor de config
    log_info(logger, "El valor de la clave es: %s", valor);
    printf("Intentando conectar a %s en el puerto %s...\n", ip, puerto);
  // Creamos una conexión hacia el servidor
  if (conexion == -1) {
        printf("ERROR FATAL: No se pudo conectar. Terminando programa.\n");
        log_error(logger, "Fallo de conexion");
        return EXIT_FAILURE;
    } else {
        printf("CONECTADO EXITOSAMENTE!\n");
    }
    // ---------------- LEER DE CONSOLA ---------------- //

    leer_consola(logger);

    //---------------------------------------------------PARTE 3-------------------------------------------------------------//

    // ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

  
    // Enviamos al servidor el valor de CLAVE como mensaje
    enviar_mensaje(valor, conexion);
    // Armamos y enviamos el paquete
    paquete(conexion);

    terminar_programa(conexion, logger, config);

   //---------------------------------------------------PARTE 5-------------------------------------------------------------/
    // Proximamente
}

t_log* iniciar_logger(void)
{
    t_log* nuevo_logger = log_create("tp0.log", "CLIENTE", 1, LOG_LEVEL_INFO);

    return nuevo_logger;
}
t_config* iniciar_config(void)
{
    t_config* nuevo_config = config_create("cliente.config");

    if (nuevo_config == NULL) {
        abort(); // Si falla, terminamos la ejecución
    }

    return nuevo_config;
}


void leer_consola(t_log* logger)
{
    char* leido;   
    leido = readline("> ");

    // Iteramos mientras el string devuelto no esté vacío
    while (leido != NULL && strcmp(leido, "") != 0) {
        log_info(logger, "%s", leido); // Logueamos lo que ingresó el usuario
        free(leido);                   // ¡Fundamental liberar la memoria!
        leido = readline("> ");        // Volvemos a pedir otra línea
    }

    // Al salir del while porque vino vacío, también hay que liberar esa última línea
    free(leido);
}
void paquete(int conexion)
{
    // Ahora toca lo divertido!
    char* leido;
    t_paquete* paquete = crear_paquete(); //lo creo

    // Leemos y esta vez agregamos las lineas al paquete
    //leo primer linea
    leido = readline("> ");

    while(leido !=NULL && strcmp(leido, "") !=0) {
// Agregamos el texto al paquete. 
        // Usamos strlen(leido) + 1 para incluir el caracter de fin de cadena '\0'
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        
        // ¡Importante! Liberamos la memoria de la línea leída
        free(leido);
        
        // Volvemos a pedir otra línea
        leido = readline("> ");    
    }
    // ¡No te olvides de liberar las líneas y el paquete antes de regresar!
    free(leido);
    enviar_paquete(paquete,conexion);
    eliminar_paquete(paquete);
    
}
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
    // Destruimos el log y el config
    log_destroy(logger);
    config_destroy(config);

    // (La conexion se va a liberar a partir de la Etapa 3 usando liberar_conexion(conexion))
    liberar_conexion(conexion);
}