
**This package could control servo motor base Ethercat COE, SOEM and Xenomai.**


    | :---------------: |:---------------------------------------------:|
    | ethercat manager  | Receive and send ethercat frames              |
    | elmo control      | Send and receive status data based on CIA402  |
    | robot control     | Motor control user layer                      |
    
    

# 1.Resource

## 1.1.linux-4.9.38

**download:** [https://mirrors.edge.kernel.org/pub/linux/kernel/v4.x/linux-4.9.38.tar.gz](https://mirrors.edge.kernel.org/pub/linux/kernel/v4.x/linux-4.9.38.tar.gz)

## 1.2.Xenomai3.0.5

**download:** [https://gitlab.denx.de/Xenomai/xenomai/-/archive/stable/v3.0.x/xenomai-stable-v3.0.x.tar.bz2](https://gitlab.denx.de/Xenomai/xenomai/-/archive/stable/v3.0.x/xenomai-stable-v3.0.x.tar.bz2)

## 1.3.SOEM

**download:** [https://github.com/OpenEtherCATsociety/SOEM/archive/master.zip](https://github.com/OpenEtherCATsociety/SOEM/archive/master.zip)

# 2. Install

## 2.1 linux and xenomai install

### 2.1.1 compile tool install

> sudo apt-get install libncurses5-dev libssl-dev   
> sudo apt-get install build-essential openssl  
> sudo apt-get install libidn11-dev libidn11    
> sudo apt-get install bc   
> sudo apt-get install gawk 
> sudo apt-get install libstdc++6   
> sudo apt-get install lib32stdc++6 
> sudo apt-get install zlibc minizip    

### 2.1.2 linux kernel compile and  install
**Refer:**[https://rtt-lwr.readthedocs.io/en/latest/rtpc/xenomai3.html](https://rtt-lwr.readthedocs.io/en/latest/rtpc/xenomai3.html)


### 2.1.3 test linux version
* uname -a
**terminator show:**
> Linux tan-pc 4.9.38-xenomai-3.0.5 #1 SMP Thu May 23 22:01:51 CST 2019 x86_64 x86_64 x86_64 GNU/Linux  
**success**

### 2.1.4 test Xenomai latency
> cd /usr/xenomai/bin   
> sudo ./latency      


## 2.2 SOEM install
**Refer:**[https://github.com/OpenEtherCATsociety/SOEM/blob/master/README.md](https://github.com/OpenEtherCATsociety/SOEM/blob/master/README.md)


# 3.Elmo control

## 3.1 Install
 > cd   
 > git clone https://github.com/Jiongyu/module_robot.git    
 > cd module_robot  
 > mkdir build && cd build  
 > cmake .. 
 > make 

## 3.2 Motor control test

> cd ./../bin/  
> l 

**cia 402 control mode test**
> sudo ./csp_test   
> sudo ./csv_test   
> sudo ./cst_test   
> sudo ./pp_test    
> sudo ./pv_test    
> sudo ./pt_test    

**motor control ui**
> sudo ./ui_motor_control   