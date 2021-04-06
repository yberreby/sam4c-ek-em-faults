# Workflow pour second coeur

Compilez le projet.

```
$ make
```


Lancer JLinkGDBServer pour le coeur 0 :

```
$ make server
```

Dans un nouveau terminal, ouvrir une session GDB (qui va servir à reprogrammer la carte et à inspecter l'état du coeur 0) :

```
$ cd core0
$ make debug_flash
```

Une fois dans cette session, **exécutez `continue`**, puis faites Ctrl-C.

Dans un nouveau terminal, lancer JLinkGDBServer pour le coeur 1 :

```
$ make server_coproc
```

Dans un nouveau terminal, ouvrez une session GDB pour le coeur 1 :

```
$ cd core1
$ make debug_sram
```

