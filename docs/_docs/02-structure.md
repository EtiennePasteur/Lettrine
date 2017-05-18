---
title: "Structure"
permalink: /docs/structure/
excerpt: "Comprendre comment est architecturé Lettrine"
last_modified_at: 2017-05-16T18:14:43-04:00
---

## La structure
Lettrine est *multithreadé*. Cette méthode permet de profiter de l'architecture multi-coeur des processeurs modernes
en parallélisant les calculs. Un processeur à 8 coeurs permet le traitement de 8 images à la fois, résultant en un traitement effectif 8 fois plus
rapide que sur une architecture non threadée. Cette méthode est particulièrement efficace sur les processeurs Intel possédant
  la technologie [HyperThreading](http://www.intel.fr/content/www/fr/fr/architecture-and-technology/hyper-threading/hyper-threading-technology.html).  
{: style="text-align: justify;"}


**Changement de contexte** (*aka **context switch***)  
Lorsque le nombre de thread logiciels dépasse le nombre de threads physiques (nombre de coeurs CPU disponibles),
le  noyau du système d'exploitation a recours à des mécanismes pour éviter de bloquer les calculs sur un seul programme.  
Ces mécanismes sont appelés *context switch*. Lors d'un *context switch*, l'état du programme en cours d'exécution est sauvegardé
pour pouvoir être récupéré plus tard. Le programme est ensuite mit en pause et laisse sa place à un autre programme.
Des milliers de *context switches* s'effectuent chaque seconde afin d'assurer la fluidité du système. Malheureusement ces opérations
ont un coût temporel. Sauvegarder et restaurer des états prend un temps qui est essentiellement perdu pour les calculs.
{: style="text-align: justify;"}

Pour éviter les *context switch* inutiles, il existe autant de threads que de coeurs sur la machine hôte.
Chaque image est traitée par un seul et unique coeur CPU. Un coeur dédie donc toute sa puissance de
calcul à une seule image à la fois et n'a pas à jongler entre plusieurs. En pratique, il reste encore des context switch, étant donné
qu'OpenCV est aussi threadé, mais il s'agit ici de réduire les coûts.
Chaque thread pioche la prochaine page à analyser dans une queue protégée à l'aide d'un mutex.
Aucun thread n'est au repos avant la fin totale de l'analyse.
{: style="text-align: justify;"}