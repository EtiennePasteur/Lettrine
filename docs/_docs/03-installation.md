---
title: "Structure"
permalink: /docs/installation/
excerpt: "Comprendre comment installer Lettrine"
last_modified_at: 2017-05-16T18:14:43-04:00
---

## Installation
### Windows
(À venir.)  

### MacOs
(À venir.)

### Unix
#### Ubuntu / Debian
Cloner le projet
```
git clone --recursive https://github.com/EtiennePasteur/Lettrine
```
Installer les dépendances
```
sudo apt-get install libboost-all-dev
sudo apt-get install libopencv-dev libcv-dev
sudo apt-get install cmake 
```
Compilation
```
mkdir build
cd build
cmake .. && make
```
Executer le programme
```
./Lettrine ../ImagesTests/
```
Lancer la demo
```
cd ../demo/
./demo.py ../ImagesTests/1072056536
```
Dans votre navigateur naviguer vers [http://localhost:8080](http://localhost:8080)