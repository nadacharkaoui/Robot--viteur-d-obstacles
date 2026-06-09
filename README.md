# Robot--viteur-d-obstacles

Description du projet:
Ce projet consiste à concevoir et réaliser un robot mobile autonome capable de détecter et d’éviter les obstacles à l’aide de capteurs.
Ce projet a été réalisé de manière autonome dans le cadre de l’apprentissage personnel des systèmes embarqués et de la robotique mobile.

Objectifs:
Comprendre le fonctionnement d’un système embarqué simple
Intégrer des capteurs de distance avec un microcontrôleur
Programmer un comportement autonome de navigation
Développer un projet matériel complet de A à Z

Matériel utilisé:
Arduino UNO 
Shield V5
Capteur ultrason HC-SR04
Driver moteur L298N
2 Moteurs DC + 2 roues
Châssis de robot
4 piles AA rechargeables
Fils de connexion

Principe de fonctionnement:
Le capteur ultrason mesure la distance devant le robot
Si aucun obstacle n’est détecté → le robot avance
Si un obstacle est détecté à une distance critique :
le robot s’arrête
il recule légèrement
il change de direction (gauche ou droite)
puis reprend son déplacement
