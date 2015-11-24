# Mini-framework to study possible parameterizations of the QCD-multijet background

## Usage

First, compile the code with `scram b`.

The central executable is called `FitBackgroundModel` and the available options
are obtained with `FitBackgroundModel -h`. An example call would look like this:
```
FitBackgroundModel -b bukin
```

Use the flags `-l` to see the parameters of the chosen model and `-m` to modify
the parameter settings from the command line.


## Memory analysis with `valgrind`

```
valgrind --tool=memcheck --track-origins=yes \
 --suppressions=$CMSSW_RELEASE_BASE/src/Utilities/ReleaseScripts/data/cms-valgrind-memcheck.supp \
 --suppressions=$ROOTSYS/etc/valgrind-root.supp \
 FitBackgroundModel -b bukin
```

NB: Compile with the following flags in `Analysis/BackgroundModel/BuildFile.xml`
    and `Analysis/BackgroundModel/bin/BuildFile.xml`:
```
<flags CXXFLAGS="-g -O0" />
```
This is also useful for using gdb.
