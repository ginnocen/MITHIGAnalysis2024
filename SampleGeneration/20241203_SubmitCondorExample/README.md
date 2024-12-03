# Run the C++ Condor Demo

First, make sure your VOMS proxy is valid and in your user directory. You can 
add the following lines to `~/.bashrc` to do this automatically:
```bash
alias proxy='voms-proxy-init -rfc -voms cms; cp /tmp/x509up_u'$(id -u)' ~/'
proxy
```

Initiate VOMS:
```bash
voms-proxy-init -voms cms -valid 192:00

# If it outputs to /tmp/x509up_uXXXXXX, manually move it:
cp /tmp/x509up_uXXXXXX ~/
```

Now navigate to the directory containing `cpptest.cc`, `cpptest.sh` and `runcpptest.sh`. You 
should be able to just run the shell script and everything will work:
```bash
bash runcpptest.sh
```

Note that this shell script automatically makes the condor config based on your user settings.
