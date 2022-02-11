# TODO

- CMake (or make) build system
- Argument parsing
- Allow selecting root path for aircfrafts, scripts, etc.
- Flightgear socket


# Start FlightGear

```
$ fgfs --fdm=null --native-fdm=socket,in,60,,5550,udp --fg-aircraft=/Users/jakub/Rockets/JFP/assets/aircraft/sabre_light --aircraft=sabre_light --disable-sound --timeofday=noon --airport=EDDF --enable-terrasync
# WORKS! (looks like only in /Users/jakub/Rockets/JFP/assets/aircraft directory)
```

```
fgfs --fdm=null --generic=socket,in,60,localhost,5550,tcp,jsbsim_JFP --fg-aircraft=/Users/jakub/Rockets/JFP/assets/aircraft/sabre_light --aircraft=sabre_light --disable-sound --timeofday=noon --airport=EDDF --enable-terrasync
```

# Start jsbsim

```
JSBSim scripts/rocket_flight.xml data_output/socket.xml --nice --realtime
```

# Start JFP

```
./JFP <script-file> <output-format-file>
```

# Compile JFP with included JSBSim

```
$ cmake .. -DBUILD_DOCS=OFF
$ make -j
```