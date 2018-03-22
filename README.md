# Iris

## Quick start

### Build dependencies

#### Particle - cli

Install particle command line interface: `https://github.com/spark/particle-cli`

### Build

The compilation is processed on the Particle Cloud.

```script
particle compile target_harware project_files
```

Example: in case of a photon

```script
particle compile photon $(find . -iname '*.h' -o -iname '*.cpp' -o -iname '*.ino' )
```

The command create a photon_firmware_*.bin file. Example:

```script
photon_firmware_1457783257040.bin
```

### Flash firmware to a device

You need to specify a target device. The available devices can be requested
by the following command:

```script
particle list
particle flash device_id photon_firmware_1457783257040.bin
```

### Dependencies

Particle-Neopixel: https://github.com/technobly/Particle-NeoPixel