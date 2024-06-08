# Vcpkg Integration Tool

This tool is designed to automate the process of setting up Vcpkg and integrating it with Visual Studio on a Windows system. It checks for the presence of Git and Vcpkg, installs them if necessary, and then integrates Vcpkg with Visual Studio. Additionally, it provides a command-line interface for installing and removing libraries using Vcpkg.

## Prerequisites

- **Windows Operating System**: This tool is specifically designed for Windows.
- **Visual Studio**: Make sure Visual Studio is installed on your system.
- **Internet Connection**: An active internet connection is required for downloading Git and Vcpkg.

## Usage

### Installation

1. Clone or download the repository containing this tool.
2. Compile the program using a C++ compiler.
3. Run the compiled executable.

### Commands

The tool provides the following commands:

- `install libraryname`: Installs the specified library using Vcpkg.
- `delete libraryname`: Deletes the specified library using Vcpkg.
- `exit`: Exits the program.

## Example

```sh
install zlib
delete openssl
exit
Important Note
Administrator Privileges: Ensure that the tool is run with administrator privileges to allow installation of Git and Vcpkg.
Troubleshooting
If you encounter any issues during installation or integration, refer to the error messages displayed by the tool. Ensure that you have the necessary permissions and a stable internet connection.
Contributing
Contributions to this tool are welcome! If you encounter any bugs or have suggestions for improvement, please open an issue or submit a pull request on GitHub.

License
This tool is licensed under the MIT License.
