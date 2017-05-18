---
title: "Comment ça marche"
permalink: /docs/comment-ca-marche/
excerpt: "Comprendre comment fonctionne Lettrine"
last_modified_at: 2017-05-18T10:14:43-04:00
---

## Les différentes étapes
<figure style="max-width: 350px" class="align-center">
  <img src="{{ site.url }}{{ site.baseurl }}/assets/images/Etapes_extraction_images_0.jpg" alt="">
  <figcaption>Image originale</figcaption>
</figure> 
L'algorithme d'extraction d'images se décompose en deux étapes. Dans un premier temps, nous préparons l'image au travers de divers procédés. Puis, dans un second temps, l'algorithme va extraire les images du contenu numeriser.
{: style="text-align: justify;"}  


### Première étape
![image-center]({{ site.url }}{{ site.baseurl }}/assets/images/Etapes_extraction_images_1.jpg)  

Dans un premier temps, nous préparons l'image à être traitée pour l'extraction. Ce procédé se déroule en 3 étapes :
- Une image JPEG est composer de 3 canaux (rouge, bleu et vert), le premier traitement appliquer a l'image la transforme en une image sur un seul canal (Nuance de gris).
- Ensuite, un seuil est appliqué sur notre image afin de la binariser (Noir ou Blanc / 0 ou 1) cela va nous permettre de tracer beaucoup plus facilement des contours autour de nos différents objets.
- Enfin, grâce à un algorithme de détection par région ([article Wikipedia](https://en.wikipedia.org/wiki/Blob_detection)), nous supprimons le texte ainsi que les petits détails (poussière, tache sur le papier, etc.), ceci afin de faciliter le détourage de nos futures images et de ne pas extraire du texte.
{: style="text-align: justify;"}  

### Deuxième étape
![image-center]({{ site.url }}{{ site.baseurl }}/assets/images/Etapes_extraction_images_2.jpg)  

Âpres avoir binariser notre image et y avoir retiré les petits éléments, la deuxième étape ce concentre sur l'extraction des images a proprement parlé.
- On commence par récupérer les contours de nos images en récupérant les coordonnées de chaque pixel blanc situer a coté d'un pixel noir.
- Ensuite, afin d'améliorer la précision de notre algorithme, nous décidons d'ignorer les contours possédant des pixels situés sur les extrémités de la page, ceci afin d'éviter le plus possible les fausses détections (notamment causer par la reliure du livre).
- Enfin, pour chaque contour nous recherchons les coordonner minimal en X et en Y, puis les coordonner maximal parmi tous les points qui le compose. Il ne reste donc plus qu'à extraire la région située entre ces deux points sur l'image originale.
{: style="text-align: justify;"}