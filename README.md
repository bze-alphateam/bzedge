
<img height="204" src="doc/imgs/bzedge-logo.png">

### Checkout latest releases here: https://github.com/bze-alphateam/bzedge/releases


# BZEdge
**Keep running wallet to strengthen the BZEdge network. Backup your wallet in many locations & keep your coins wallet offline. Copy your BitcoinZ walet into BZEdge folder.**

### Ports:
- RPC port: 1980
- P2P port: 1990

Install
-----------------
### Linux

Get dependencies
Ubuntu/Debian:
```{r, engine='bash'}
sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python \
      zlib1g-dev wget bsdmainutils automake
```

Get dependencies
Free/OpenBSD:
```{r, engine='bash'}
sudo pkg install \
      gmake pkgconf flock automake lang/gcc autoconf libtool ncurses git python 
```


Install

```{r, engine='bash'}
# Clone BZEdge Repository
git clone https://github.com/bze-alphateam/bzedge

# Build: At the moment building works only without tests. Make sure to disable them.
cd bzedge/

Ubuntu/Debian:
CONFIGURE_FLAGS="--disable-tests --disable-bench" ./zcutil/build.sh -j$(nproc)

BSD:
MAKE=gmake CONFIGURE_FLAGS="--disable-tests --disable-bench" ./zcutil/build.sh -j 2

# fetch key
./zcutil/fetch-params.sh

# Run
./src/bzedged

# Test getting information about the network
cd src/
./bzedge-cli getmininginfo

# Test creating new transparent address
./bzedge-cli getnewaddress

# Test creating new private address
./bzedge-cli z_getnewaddress

# Test checking transparent balance
./bzedge-cli getbalance

# Test checking total balance 
./bzedge-cli z_gettotalbalance

# Check all available wallet commands
./bzedge-cli help

# Get more info about a single wallet command
./bzedge-cli help "The-command-you-want-to-learn-more-about"
./bzedge-cli help "getbalance"
```

## config file

```
daemon=1
rpcuser=INSERT_YOUR_USER_HERE
rpcpassword=INSERT_A_SECURE_PASSWORD_HERE
addnode=144.91.119.59
addnode=144.91.121.65
addnode=167.86.99.150
```
You can add more nodes from this list https://blocks.getbze.com/network  

## config file with insight features
```
server=1
txindex=1
addressindex=1
timestampindex=1
spentindex=1
zmqpubrawtx=tcp://127.0.0.1:28332
zmqpubhashblock=tcp://127.0.0.1:28332
rpcallowip=127.0.0.1
rpcuser=USER
rpcpassword=STRONG_PASSWORD_DO_NOT_FORGET_TO_CHANGE_THIS
uacomment=bitcore
showmetrics=0
externalip=127.0.0.1:1990
rpcbind=127.0.0.1:1980
rpcport=1980
bind=127.0.0.1:1990
experimentalfeatures=1
insightexplorer=1
```

## Masternode setup
### Requirements
1. A VPS
2. A control wallet ([ModernWallet](https://snowgem.org/resources/modern-wallet) or bzedged/bzedge-cli) used just for setup and to hold MN collateral\
**YOUR FUNDS ARE ALWAYS KEPT ON YOUR CONTROL WALLET! And that wallet is on your PC! You should not keep your funds on the VPS**

### VPS System Requirements
MN Collateral: 250,000 BZE\
Min. System Req.: 1 CPU / 1 GB RAM / 2GB Swap / 20 GB HDD\
Rec. System Req.:2 CPU / 1 GB RAM / 2GB Swap / 20 GB HDD\
OS: Ubuntu 16/18 or Debian

### Installation
###### Acquire a VPS on your favourite hosting provider.
We recommend:\
1. [Vultr](https://www.vultr.com/?ref=8352425-4F) - using this link will give you 50$ to test Vultr
2. [DigitalOcean](https://m.do.co/c/9d298399bf13) - using this link will give you 100$ for 60 days to test DigitalOcean
3. [Contabo](https://contabo.com/?show=vps) - using this link you gain nothing nor do we. They are the cheapest around but their services are not as reliable as previous providers.\
But there are plenty of reliable cloud providers out there, pick one. 

###### VPS Setup
Connect to your VPS using a ssh client like [putty](https://www.putty.org/) or the terminal and the root credentials provided by your service provider.\
Use the following commands explained below:
```
#creates a new user. Make sure you use a strong password that you can remember!
sudo adduser [YOUR_USERNAME]

#adds user to sudoers. This will ensure that your user has the rights to install the software needed
sudo usermod -aG sudo [YOUR_USERNAME]

#login as the new created user. The setup script must be used as a non-root user
su [YOUR_USERNAME]

#navigate to your new user home folder
cd ~

#download setup script
wget https://raw.githubusercontent.com/bze-alphateam/BZE-Scripts/master/setup-masternode.sh
#make the script executable
sudo chmod +x setup-masternode.sh

#run the script
./setup-masternode.sh
```
Follow the instructions on the screen, the script has some questions for you. :)

At this moment the script will take about 10 minutes to do plenty of stuff like download BZE, install dependencies, setup firewall and so on.\
If you're curious what it does have a look at [this repo](https://github.com/bze-alphateam/BZE-Scripts#setup-masternodesh).

After the script finished it will provide you useful information on the screen. Make sure you copy them to a text editor for later usage.
Among this info you will find your masternode.conf line and it should look something like this:
```
your_alias ${WANIP}:${PORT} ${masternodeprivkey} TxID Output_Index
```
*${WANIP}*, *${PORT}* and *${masternodeprivkey}* will be generated by the script.
*your_alias*, *TxID* and *Output_Index* are the ones we will have to change. You'll find out more soon.

###### Control wallet
Using [Tent Desktop Wallet](https://github.com/TENTOfficial/TENT-Core/releases/tag/v1.1.4) you have an easy and beautiful way of interacting with your MN.
Go ahead and install it if you haven't already.\
After the control wallet is synced and ready to be used create a *new BZE address* and send exactly 250,000 BZE to it.
The resulting transaction will be your MN *TxID* - copy it and keep it safe in a text editor.
Wait for the transaction to reach 15 confirmations.\
Access "Masternodes" section in ModernWallet and use the details provided by the script to start your MN. 

### MacOS
In case you encounter 'missing readelf' issue install "binutils"
```{r, engine='bash'}
brew install binutils
```
If the issue persists create a symlink for "readelf" (it may be named "x86_64-elf-readelf") in /usr/local/bin from the original file located in /usr/local/Cellar
Example: (it may be different depending on MacOS version:
```{r, engine='bash'}
/usr/local/Cellar/x86_64-elf-binutils/2.36.1/bin/x86_64-elf-readelf /usr/local/bin/x86_64-apple-darwin19.6.0-readelf
```

Security Warnings
-----------------

**BZEdge is experimental and a work-in-progress.** Use at your own risk

