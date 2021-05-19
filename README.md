# ido-horaires

<img src="https://fabmstic.imag.fr/images/articles/idohoraires1.jpg" width="300"> <img src="https://fabmstic.imag.fr/images/articles/idohoraires2.jpg" width="500">

Création d'un système permettant d'afficher les horraires du FabMSTIC et les fermetures exceptionnelles sur la porte du FabMSTIC. Le pilotage de l'affichage se fait grâce à un minitel par communication LORA.

## Matériels utilisés
- un Minitel 1 bistandard
- un kit https://www.idosens.com/ reprogrammé

## Software
- Utilisation du middleware RIOT-OS. Il faut utiliser le fork https://github.com/CampusIoT/RIOT qui inclut les cartes Idosens. 

## Fonctionnement

- A l'état initial, la base Idosens affiche les horaires d'ouverture du FabMSTIC sur l'écran E-Paper. Elle est en mode veille.
- L'appui sur le bouton mode réveille la base et elle passe en mode écoute LORA.
- La télécommande Idosens est connectée en liaison série a un minitel qui permet d'envoyer les commandes lora.

### Commandes disponibles

- default : force l'affichage par défaut (horaire d'ouverture)
- settime : change l'horaire du la base Idosens
- setopentime : défini une date de réveil rtc et affiche la date de réouverture du FabMSTIC
- idle : passe la base idosens en mode veille.

## Organisation du dépot

- IdoCommon : dossier contenant les fichiers communs à la base idosens et à la télécommande
- IdoDoor : programme de la base Idosens qui permet l'affichage sur l'écran E-paper
- IdoTel : programme de la télécommande qui est connectée au minitel et qui envoie les commandes par LORA.
