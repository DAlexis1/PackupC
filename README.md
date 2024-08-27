# Packup
***

To install PackupC  :
```bash
git clone https://github.com/DAlexis1/PackupC
git submodule init
git submodule update
```

To build it :
```bash
#from the PackupC root
mkdir build
cmake . -B build
cd build
make
```

To run it :
```bash
PackupC [--force] Action
```

The following actions are supported :
backup
restore
uninstall
newconf
