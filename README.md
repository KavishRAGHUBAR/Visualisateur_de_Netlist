Title : Visualisateur de Netlist (C++ et Qt 5)

Date : December 2022

Author : Kavish RAGHUBAR et Weiyi GONG

This code/app will parse the XML files (electronic circuit), set up the components based on the hierarchy of classes. Then display the netlist accordingly.

Install QT5 :

`sudo apt update`

`sudo apt install qtbase5-dev`


Use an alternative version of gcc (skip if you already have an up-to-date version of gcc) :

`sudo apt install gcc-9 g++-9`

`sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9`


Then generate 'tme810' app :

`cd ~/Desktop/Visualisateur_de_Netlist/build`

`cmake ../src`

`make install`


Run with :

`./tme810`


Screenshots : 
**AND2**
![](https://github.com/KavishRAGHUBAR/Visualisateur_de_Netlist/blob/main/Screenshots/AND2.png)

**OR2**
![](https://github.com/KavishRAGHUBAR/Visualisateur_de_Netlist/blob/main/Screenshots/OR2.png)

**XOR2**
![](https://github.com/KavishRAGHUBAR/Visualisateur_de_Netlist/blob/main/Screenshots/XOR2.png)

**PMOS**
![](https://github.com/KavishRAGHUBAR/Visualisateur_de_Netlist/blob/main/Screenshots/PMOS.png)

**HALFADDER**
![](https://github.com/KavishRAGHUBAR/Visualisateur_de_Netlist/blob/main/Screenshots/HALFADDER.png)

We can load the 'fulladder' by doing :

Shortcut : CTRL+O

Type : fulladder

Then click on "Ok"

**FULLADDER**
![](https://github.com/KavishRAGHUBAR/Visualisateur_de_Netlist/blob/main/Screenshots/FULLADDER.png)

