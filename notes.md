## Workflow pour second coeur

Compilez le projet.

```
$ make
```

Lancez JLinkGDBServer pour le coeur 0 :

```
$ make server
```

Dans un nouveau terminal, ouvrez une session GDB (qui va servir à reprogrammer la carte et à inspecter l'état du coeur 0) :

```
$ cd core0
$ make debug_flash
```

Une fois dans cette session, **exécutez `continue`**, puis faites Ctrl-C.

Dans un nouveau terminal, lancez JLinkGDBServer pour le coeur 1 :

```
$ make server_coproc
```

Note : il est **nécessaire** d'avoir exécuté le programme du coeur 0 avant de lancer le serveur pour le coeur 1, car c'est le coeur 0 qui démarre le coeur 1.

Dans un nouveau terminal, ouvrez une session GDB pour le coeur 1 :

```
$ cd core1
$ make debug_sram
```


## Utilisation de `run_test_seq`

Lors d'une exception ou de la fin d'une séquence de test, la procédure `run_test_seq` stocke l'état de certains registres dans la RAM, ce qui permet d'inspecter leur valeur depuis un debugger attaché au coeur 0.

| addresse   | registre |
|------------|----------|
| 0x20160000 | PC       |
| 0x20160004 | SP       |
| 0x20160008 | LR       |
| 0x2016000c | R0       |
| 0x20160010 | R1       |
| 0x20160014 | R2       |
| 0x20160018 | R3       |
| 0x2016001c | R4       |
| 0x20160020 | R5       |
| 0x20160024 | R6       |
| 0x20160028 | R7       |
| 0x2016002c | R8       |
| 0x20160030 | R9       |
| 0x20160034 | XPSR     |



## PIO

```
#define PIOA_OUTPUT_BASE 0x400E0E00UL
#define PIOB_OUTPUT_BASE 0x400e1000UL
#define PIOC_OUTPUT_BASE 0x4800C000UL
```

