# mikaboo

Gruppo **lso17az05**

## Compilazione
Per compilare il kernel della fase 1, basta eseguire il comando `make`, e il file `kernel` verrà creato utilizzando i tool di crosscompiling `arm-none-eabi-gcc` e `arm-none-eabi-ld`. 
In caso si utilizzi binari con nomi diversi, si può sovrascrivere passando il nuovo nome come variabile del comando make: `make CC=new-gcc LD=new-ld`.
In caso uarm sia stato installato in una direcory diversa da quella di default (`/usr/local`), si può sovrascrivere passando una variabile al comando `make BASEDIR=/path/to/dir`.

## Esecuzione
Per eseguire questo kernel è necessario utilizzare l'ultima versione di uARM, perché il comando `make` salta il passaggio (non più richiesto) da formato elf a formato uarm. Se si utilizza la vecchia versione, bisogna eseguire il comando `elf2uarm -k kernel` per generare i file richiesti.

### Testing automatico
Tra i comandi make è disponibile `make check` che esegue in automatico uARM, e manda in stdout l'output del kernel. Questo progetto è accompagnato da una versione modificata del BIOS, perché la versione attuale non presenta i caratteri di linefeed `\n` alla fine delle stringhe di halt di sistema. Il sorgente, [BIOS.s](https://raw.githubusercontent.com/MartelliEnrico/uARM/master/facilities/BIOS.s), è preso dalla fork di uARM, [MartelliEnrico/uARM](https://github.com/MartelliEnrico/uARM).
