# Cuda3DRaytracing
basis of ray tracing:  2 luminosity sources , physic with spheres and textures in real time ,visual studio 2019 and cuda are needed.Frames are generated pixel by pixel.

Projet Ray tracing C++ et Cuda:
2 sources de lumière , textures de mur et planche.
4 sphères avec physique , rebonds ,collision  tout en temps réel 
,possibilité de changer la position des sphères et d'une source lumineuse.

Envoi de vecteurs sur toute la surface de l'écran (plan 2D) depuis la caméra ou point de vue.
Calcul du trajet ,des rebonds , des objets, leur couleurs et de l'éloignement de la source lumineuse puis affichage sur l'écran pixel par pixel(Plan 3D derrière le plan 2D).
Les calculs sont effectués pour chaques vecteurs envoyés , ils ont été parallélisés sur gpu avec CUDA, pour avoir du temps réel.

![image](https://user-images.githubusercontent.com/56161715/131219431-20711954-b658-4041-bb5e-3c6fe838d331.png)

 
 Voici des screenschots

![image](https://user-images.githubusercontent.com/56161715/131216679-b8eb0f9c-3590-4fa5-b0f5-e7d0a9b47782.png)

![image](https://user-images.githubusercontent.com/56161715/131216704-57fdb39e-da52-4357-a798-baa1a3d7b8a1.png)

![image](https://user-images.githubusercontent.com/56161715/131217185-3d329b8b-ae35-4ee8-b4b5-9edc1e6c2372.png)

![image](https://user-images.githubusercontent.com/56161715/131218808-8df05011-0b03-4ba6-ba93-572e48f897d1.png)
