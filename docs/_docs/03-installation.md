---
title: "Installation"
permalink: /docs/installation/
excerpt: "Processus d'installation de Lettrine"
last_modified_at: 2017-05-16T18:14:43-04:00
---

### Unix / MacOs
1. Cloner le projet
```
git clone --recursive https://github.com/EtiennePasteur/Lettrine
```
2. Installer les dépendances
```
Ubuntu / Debian
sudo apt-get install libboost-all-dev
sudo apt-get install libopencv-dev libcv-dev
sudo apt-get install cmake 
```
```
ArchLinux
yaourt -Sy boost opencv cmake
```
```
MacOs
brew tap homebrew/science
brew install opencv
brew install cmake
brew install 
```

3. Compilation
```
mkdir build
cd build
cmake .. && make
```
4. Executer le programme
```
./Lettrine ../ImagesTests/
```
5. Lancer la demo
```
cd ../demo/
./demo.py ../ImagesTests/1072056536
```
Dans votre navigateur naviguer vers [http://localhost:8080](http://localhost:8080)  

### Windows
(À venir.)  
