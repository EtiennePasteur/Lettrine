---
title: "Fonctionnement"
permalink: /docs/fonctionnement/
excerpt: "Comprendre le fonctionnement de l'algorithme de Lettrine"
last_modified_at: 2017-05-18T10:14:43-04:00
---

## Les différentes étapes
<figure style="max-width: 350px" class="align-center">
  <img src="https://github.com/EtiennePasteur/Lettrine/raw/master/docs/assets/images/Etapes_extraction_images_0.jpg" alt="">
  <figcaption>Image originale</figcaption>
</figure> 
L'algorithme d'extraction d'images se décompose en deux étapes. Dans un premier temps, nous préparons l'image au travers de divers procédés. Puis, dans un second temps, l'algorithme va extraire les images du contenu numerisé.
{: style="text-align: justify;"}  

### Première étape
![image-center](https://github.com/EtiennePasteur/Lettrine/raw/master/docs/assets/images/Etapes_extraction_images_1.jpg)  

Dans un premier temps, nous préparons l'image à être traitée pour l'extraction. Ce procédé se déroule en 3 étapes :
- Une image JPEG est composée de 3 canaux (rouge, bleu et vert), le premier traitement appliqué à l'image la transforme en une image sur un seul canal (Nuances de gris).
- Ensuite, un seuil est appliqué sur notre image afin de la binariser (noir ou blanc / 0 ou 1) cela va nous permettre de tracer beaucoup plus facilement les contours de nos différents objets.
- Enfin, grâce à un algorithme de détection par région ([article Wikipedia](https://en.wikipedia.org/wiki/Blob_detection)), nous supprimons le texte ainsi que les petits détails (poussière, tache sur le papier, etc.), ceci afin de faciliter le détourage de nos futures images et de ne pas extraire du texte.
{: style="text-align: justify;"}  

### Deuxième étape
![image-center](https://github.com/EtiennePasteur/Lettrine/raw/master/docs/assets/images/Etapes_extraction_images_2.jpg)  

Apres avoir binarisé notre image et y avoir retiré les petits éléments, la deuxième étape se concentre sur l'extraction des images a proprement parler.
- On commence par récupérer les contours de nos images en récupérant les coordonnées de chaque pixel blanc situé à coté d'un pixel noir.
- Ensuite, afin d'améliorer la précision de notre algorithme, nous décidons d'ignorer les contours possédant des pixels situés sur les extrémités de la page, ceci afin d'éviter le plus possible les fausses détections (notamment causées par la reliure du livre).
- Enfin, pour chaque contour nous recherchons les coordonnées minimales en X et en Y, puis les coordonnées maximales parmis tous les points qui le composent. Il ne reste donc plus qu'à extraire la région située entre ces deux points sur l'image originale.
{: style="text-align: justify;"}