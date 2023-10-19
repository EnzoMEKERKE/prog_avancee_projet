# Projet de programation système
MEKERKE Enzo, ROUTIER Fabien, SABI Rayane, VEYSSEYRE Léo

Ce projet consiste à résoudre l'un des problèmes de coordination présent dans le livre "The little book of semaphores". Ainsi, nous avons choisi le problème du chaptre 5.2, "The barber shop problem". De ce fait, nous avons adapté ce problème à une situation de la vie réelle tout en gardant le même principe.

Imaginez donc un supermarché avec plusieurs caisses et une caisse prioritaire. La caisse prioritaire est gérée par un caissier unique. Voici les règles du système :

- Le caissier ne peut servir qu'une personne à la fois.
- Si le caissier est libre et qu'un client de la caisse prioritaire arrive, il est servi immédiatement.
- Si le caissier est occupé, les clients de la caisse prioritaire attendent dans la zone d'attente.
- La zone d'attente a un nombre limité de places (défini par NB_CHAISES, ici 5). Si elle est pleine, les clients doivent partir ou choisir une autre caisse.
- Si la caisse prioritaire n'a pas de clients, le caissier peut se reposer.

