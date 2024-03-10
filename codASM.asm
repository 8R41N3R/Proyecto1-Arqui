section .data
    archivo_config db 'configuracion.txt', 0
    archivo_datos db 'datos.txt', 0

section .text
    extern main
    global iniciar

iniciar:
    ; Llama a la función main en C
    push archivo_datos
    push archivo_config
    call main
    add esp, 8  ; limpia la pila

    ; Termina el programa
    mov eax, 1
    xor ebx, ebx
    int 0x80

