Title : Visualisateur de Netlist (C++ et Qt 5)
Date : December 2022
Author : Kavish RAGHUBAR et Weiyi GONG

Install QT5 :

`sudo apt update`

`sudo apt install qtbase5-dev`


Use an alternative version of GCC :

`sudo apt install gcc-9 g++-9`

`sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9`


Then generate 'tme810' app :

`cd ~/Desktop/Visualisateur_de_Netlist/build`

`cmake ../src`

`make install`


Run with :

`./tme810`

Basically the code/app will parse the XML files (electronic circuit), set up the components based on the hierarchy of classes. Then display the netlist accordingly.

Screenshots : 
**AND2**
![](https://hackmd.io/_uploads/S18C-1-I2.png)

**OR2**
![](https://hackmd.io/_uploads/SySWzy-Lh.png)

**XOR2**
![](https://hackmd.io/_uploads/S1l7zJZ83.png)

**PMOS**
![](https://hackmd.io/_uploads/rJ7HzJWUh.png)

**HALFADDER**
![](https://hackmd.io/_uploads/SJ98GJZUn.png)

On pourra charger le fulladder en faisant :

Shortcut : CTRL+O
On écrit : fulladder
Ensuite on click sur "Ok"

**FULLADDER**
![](https://hackmd.io/_uploads/Hy4OGkW82.png)
