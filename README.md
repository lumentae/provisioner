# Provisioner

**Provisioner** is a C++ command-line tool for setting up and managing **Minecraft servers**

It makes creating and configuring servers fast and easy!

## Features

- Quickly create new Minecraft servers
- Currently only supports Fabric, Neoforge and Modrinth mods
- Manage mods and configuration with ease

## Example Usage
```bash
# Create a new server
provisioner new myserver

# Add a mod
provisioner add mymod

# Updates mods
provisioner update

# Compile the server
provisioner compile

# Include a file or directory
provisioner include myfolder/myfile.json
