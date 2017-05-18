---
title: "Structure du projet"
permalink: /docs/structure/
excerpt: "Comprendre comment est architecturé Lettrine"
last_modified_at: 2017-05-18T18:14:43-04:00
---

## Code
### Les fonctions
#### #extractPics
```cpp
void extractPics(std::string const &path, std::string const &destPath) {
    cv::Mat image = cv::imread(path, 1);
    std::vector<t_pos_contour> contoursPos;
    prepareImage(image);
    findPics(contoursPos, image);
    createJpeg(path, destPath, contoursPos);
}
```
La fonction `extractPics` est le point d'entrée du programme, elle prend en paramètre une image et un chemin vers lequel stocker les images extraites.  
Elle commence par charger l'image en mémoire puis appelle les fonctions de traitement qui suivent.
{: style="text-align: justify;"}

#### #prepareImage
```cpp
static void prepareImage(cv::Mat &image) {
    cvtColor(image, image, CV_RGB2GRAY);
    threshold(image, image, 75.0, 255.0, cv::THRESH_BINARY_INV);
    removeSmallElem(800, image);
}
```
La fonction `prepareImage` transforme une image JPEG 3 canaux (RGB) en image un canal (nuances de gris), puis elle applique un seuil afin de binariser l'image et de faire ressortir les contrastes. Enfin elle termine par retirer les petits éléments tels que le texte ou les poussières.
{: style="text-align: justify;"}

#### #findPics
```cpp
static void findPics(std::vector<t_pos_contour> &contoursPos, cv::Mat &image) {
    std::vector<std::vector<cv::Point>> contours;
    findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    contours.resize(contours.size());
    for (size_t k = 0; k < contours.size(); k++)
        approxPolyDP(cv::Mat(contours[k]), contours[k], 3, true);

    bool remove = false;
    t_pos_contour valueContour;
    double seuilX = image.size().width * 0.07;
    double seuilY = image.size().height * 0.07;

    //Iterate through all elem found
    for (std::vector<std::vector<cv::Point>>::iterator itV = contours.begin(); itV != contours.end();) {
        valueContour.max = {0, 0};
        valueContour.min = {image.size().width, image.size().height};

        //Iterate through each points of the contour
        for (std::vector<cv::Point>::iterator itP = itV->begin(); itP != itV->end(); itP++) {
            //Remove elem if it's contours is to close from border
            if (itP->x < 10 || itP->x > image.size().width - 10 || itP->y < 10 || itP->y > image.size().height - 10) {
                itV = contours.erase(itV);
                remove = true;
                break;
            }
            valueContour.max.x = std::max(valueContour.max.x, itP->x);
            valueContour.max.y = std::max(valueContour.max.y, itP->y);
            valueContour.min.x = std::min(valueContour.min.x, itP->x);
            valueContour.min.y = std::min(valueContour.min.y, itP->y);
        }
        if (!remove) {
            ++itV;
        } else {
            remove = false;
            continue;
        }
        //Check if elem is not to small
        if (valueContour.max.x - valueContour.min.x > seuilX && valueContour.max.y - valueContour.min.y > seuilY)
            contoursPos.push_back(valueContour);
    }
}
```
La fonction `findPics` se charge de récupérer le contour de nos images afin de pouvoir en déterminer les extrémités. Afin de rendre l'algorithme plus précis et ainsi éviter les erreurs de traitement, des filtres sont appliqués afin de supprimer les éléments trop proches du bord ; ceux-ci résultants essentiellement de la reliure du livre.
{: style="text-align: justify;"}

### Optimisations
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