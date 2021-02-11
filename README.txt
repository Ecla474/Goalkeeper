Le jeu de foot

Vous etes le gardien, et le but du jeu est d'arrêter toutes les balles qui vont vers le but.

Il se peut que certaines balles ne rentreront pas dans les cages mais ceci est volontaire de ma part il suffira juste de modifier les valeurs de frand situé a la ligne
94 et 95 dans la fonction initBallon (ligne 89 à 96).

Puisque qu'on est dans une représentation réel, j'ai mis en place un système de gravitation vu en TP pour que la balle puisse redescendre et atterrir dans les cages.

Le gardien est modélisé comme une barre de pong qu'on a vu dans le TP de Collision afin qu'il puisse arreter les balles

J'ai mis un terrain avec une cage de foot, j'ai inclus une image de gardien en PNG afin de ne pas voir le fond de l'image qui gachera le coté esthétique.

Un système de points est mis en place pour rendre le jeu plus amusant. On gagne un point quand on arrete une balle et on en perd lorsque cette balle aterri dans les 
cages (procédure collision ligne 125 à 163)