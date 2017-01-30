# mikaboo

Gruppo **lso17az05**

## Compilazione
Per compilare il kernel della fase 1, basta eseguire il comando `make`, e il file `kernel` verrà creato utilizzando i tool di crosscompiling `arm-none-eabi-gcc` e `arm-none-eabi-ld`. 
In caso si utilizzi binari con nomi diversi, si può sovrascrivere passando il nuovo nome come variabile del comando make: `make CC=new-gcc LD=new-ld`.
In caso uarm sia stato installato in una direcory diversa da quella di default (`/usr/local`), si può sovrascrivere passando una variabile al comando `make BASEDIR=/path/to/dir`.

## Esecuzione
Per eseguire questo kernel è necessario utilizzare l'ultima versione di uARM, perché il comando `make` salta il passaggio (non più richiesto) da formato elf a formato uarm. Se si utilizza la vecchia versione, bisogna eseguire il comando `elf2uarm -k kernel` per generare i file richiesti.

## Test automatico
Tra i comandi make è disponibile `make check` che esegue in automatico uARM, e manda in stdout l'output del kernel. Per usufruire di questa utility è necessario modificare il file BIOS, perché la versione attuale non presenta i linefeed `\n` alla fine delle stringhe di halt di sistema. È disponibile una versione aggiornata del [https://raw.githubusercontent.com/MartelliEnrico/uARM/master/facilities/BIOS](BIOS) e del sorgente [https://raw.githubusercontent.com/MartelliEnrico/uARM/master/facilities/BIOS.s](BIOS.s) nella fork [https://github.com/MartelliEnrico/uARM](MartelliEnrico/uARM).