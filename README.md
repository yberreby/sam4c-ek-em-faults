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

