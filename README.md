# Lettrine
[![Build Status](https://travis-ci.org/EtiennePasteur/Lettrine.svg?branch=master)](https://travis-ci.org/EtiennePasteur/Lettrine) [![GitHub license](https://img.shields.io/badge/license-MIT-lightgrey.svg)](https://raw.githubusercontent.com/EtiennePasteur/Lettrine/master/LICENSE)
## Project description
Le Bodmer Lab de l'Université de Genève vient d'achever la numérisation d'une importante collection de livres de voyage de la Renaissance, publiés aux XVIe-XVIIe siècles et richement illustrés par des gravures et de cartes. A cet ensemble s'ajoutent plus de 200 éditions du Faust de Goethe, souvent illustrées. Ces deux sets de données sont mis à disposition des participants de Geneva Open Libraries (#GEOPENGLAM) pendant le #OGH17, et en anticipant leur publication sur le site du Bodmer Lab, attendue au printemps 2018.

Ce projet #GEOPENGLAM se propose d'identifier et d'extraire automatiquement les illustrations des ces quelque 50'000 pages. Un processus d'indexation et de “tagging” est par la suite envisagé. Au-delà de ce processus d'analyse quantitative des données, notre ambition est de créer une interface pouvant répertorier les illustrations et permettre aux chercheurs, par la suite de faire des classements et des analyses qualitatives (quelles gravures sont répétées dans plusieurs volumes, quelles similitudes peut-on déceler à travers la collection, etc).

## Comment tester le projet sur Debian 8 ?
1. Cloner le projet
```
git clone https://github.com/EtiennePasteur/Lettrine
```
2. Installer les dépendances
```
sudo apt-get install libboost-all-dev
sudo apt-get install libopencv-dev
git submodule update --init
```
3. Build
```
mkdir build
cd build
cmake .. && make
```
4. Tester
```
build/Lettrine -d ImagesTests/img/1072056536
```


