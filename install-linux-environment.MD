Set USB ASP driver settings
```bash
sudo vi /etc/udev/rules.d/10-usbasp.rules
```

в нем записать строчку
```text
SUBSYSTEM=="usb", ATTR{idVendor}=="16c0", ATTR{idProduct}=="05dc", GROUP="adm", MODE="0666", SYMLINK+="usbasp" # USBasp
```
и после
```bash
sudo /etc/init.d/udev restart
```